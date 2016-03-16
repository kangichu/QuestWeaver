//
// Created by michael on 14.08.15.
//

#include <Template/Space/ExploreRegionTemplate.h>
#include <QuestModel/Space/ExploreRegionQuest.h>

using namespace std;
using namespace weave;

ExploreRegionTemplate::ExploreRegionTemplate(string title,
                                             vector<TemplateQuestProperty> properties,
                                             vector<TemplateQuestDescription> descriptions,
                                             FormatterType formatterType)
        : QuestTemplate(title, properties, descriptions, formatterType) {
}

vector<WorldModelAction> ExploreRegionTemplate::GetPropertyCandidates(const TemplateQuestProperty &property,
                                                                      const WorldModel &worldModel) const {
    vector<WorldModelAction> actions;
    const SpaceWorldModel &spaceModel = (const SpaceWorldModel &) worldModel;
    if (property.GetName() == "location") {
        gatherLocationEntities(&actions, spaceModel);
    } else if (property.GetName() == "sponsor") {
        gatherSponsorEntities(&actions, spaceModel);
    }
    return actions;
}

void ExploreRegionTemplate::gatherSponsorEntities(vector<WorldModelAction> *actions,
                                                  const SpaceWorldModel &spaceModel) const {
    auto newEntityAction = spaceModel.CreateAgent();
    actions->push_back(newEntityAction);

    MetaData metaData;
    metaData.SetValue("relationToPlayer", 10);
    WorldModelAction metaDataAction(WorldActionType::UPDATE, newEntityAction.GetEntity(), metaData);
    actions->push_back(move(metaDataAction));

    for (auto entity : spaceModel.GetEntities()) {
        if (entity->GetType() == "agent") {
            auto entityData = spaceModel.GetMetaData(entity->GetId());
            if (entityData.GetValue("relationToPlayer") >= 10) {
                WorldModelAction modelAction(WorldActionType::KEEP, entity);
                actions->push_back(move(modelAction));
            }
        }
    }
}

void ExploreRegionTemplate::gatherLocationEntities(vector<WorldModelAction> *actions,
                                                   const SpaceWorldModel &spaceModel) const {
    auto newEntityAction = spaceModel.CreateLocation();
    actions->push_back(newEntityAction);

    MetaData metaData;
    metaData.SetValue("explored", 0);
    metaData.SetValue("explorationQuestLock", 1);  // so it does not get picked by another exploration quest
    WorldModelAction metaDataAction(WorldActionType::UPDATE, newEntityAction.GetEntity(), metaData);
    actions->push_back(move(metaDataAction));

    // search for existing locations
    for (auto entity : spaceModel.GetEntities()) {
        if (entity->GetType() == "location") {
            auto entityData = spaceModel.GetMetaData(entity->GetId());
            if (!entityData.HasValue("explorationQuestLock") && entityData.GetValue("explored") == 0) {
                WorldModelAction modelAction(WorldActionType::UPDATE, entity, metaData);
                actions->push_back(move(modelAction));
            }
        }
    }
}

shared_ptr<Quest> ExploreRegionTemplate::ToQuest(const vector<QuestPropertyValue> &questPropertyValues,
                                                 const std::string &questStory) const {
    const string &description = getBestFittingDescription(questPropertyValues);
    const string &questTitle = getTitle(questPropertyValues);

    ID location = getEntityIdFromProperty("location", questPropertyValues);
    ID sponsor = getEntityIdFromProperty("sponsor", questPropertyValues);
    return make_shared<ExploreRegionQuest>(questTitle, description, questStory, location, sponsor);
}
