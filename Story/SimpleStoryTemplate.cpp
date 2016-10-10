//
// Created by michael on 16.02.16.
//

#include <Story/SimpleStoryTemplate.h>

using namespace weave;
using namespace std;

StoryTemplateResult SimpleStoryTemplate::CreateStory(const EntityMap &requiredEntities, const WeaverGraph &,
                                                     const WorldModel &worldModel,
                                                     shared_ptr<RandomStream> randomStream) const {
    auto entities = getValidEntities(requiredEntities, worldModel, randomStream);
    if (entities.empty()) {
        Logger::Fatal("Invalid template call, not all required entities were found!");
    }

    TokenToEntityMap tokenEntityMap;
    StoryTemplateResult result;
    for (uint64_t i = 1; i <= GetRequiredEntities().size(); i++) {
        auto entity = entities[0];
        tokenEntityMap[to_string(i)] = {entity->GetId()};

        if (conditions.count(StoryCondition::OncePerEntity)) {
            MetaData markedData;
            markedData.SetValue(key, 1);
            result.worldActions.emplace_back(WorldActionType::UPDATE, entity, markedData);
        }
    }

    result.tokenMap = createTokenMapping(tokenEntityMap);
    result.rawText = rawText;
    return result;
}

bool SimpleStoryTemplate::IsValid(const EntityMap &requiredEntities, const WeaverGraph &,
                                  const WorldModel &worldModel) const {
    return getValidEntities(requiredEntities, worldModel, make_shared<RandomStream>()).size() > 0;
}

vector<shared_ptr<WorldEntity>> SimpleStoryTemplate::getValidEntities(const EntityMap &entityMap,
                                                                      const WorldModel &worldModel,
                                                                      shared_ptr<RandomStream> randomStream) const {
    vector<shared_ptr<WorldEntity>> result;
    for (string type : GetRequiredEntities()) {
        vector<shared_ptr<WorldEntity>> validEntities;
        auto mapIter = entityMap.find(type);
        if (mapIter == entityMap.end()) {
            return validEntities;
        }

        for (shared_ptr<WorldEntity> entity : mapIter->second) {
            ID id = entity->GetId();
            MetaData metaData = worldModel.GetMetaData(id);

            // check the "once per entity" condition
            if (conditions.count(StoryCondition::OncePerEntity) && metaData.HasValue(key)) {
                Logger::Debug("          Entity " + entity->ToString() +
                              " cannot be used, because the story was already created for it once.");
                continue;
            }

            // check the "without property" condition
            auto iter = conditions.find(StoryCondition::WithoutProperty);
            if (iter != conditions.end() && !iter->second.empty() && metaData.HasValue(iter->second[0])) {
                Logger::Debug("          Entity " + entity->ToString() +
                                      " cannot be used, because it has property " + iter->second[0]);
                continue;
            }
            validEntities.push_back(entity);
        }
        if (validEntities.empty()) {
            return validEntities;
        }
        result.push_back(validEntities[randomStream->GetRandomIndex(validEntities.size())]);
    }
    return result;
}
