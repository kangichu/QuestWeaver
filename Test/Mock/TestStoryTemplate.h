//
// Created by michael on 05.02.16.
//

#pragma once

#include <Story/StoryTemplate.h>

namespace weave {
    class TestStoryTemplate : public StoryTemplate {
    public:
        bool ReturnInvalidIDs = false;

        TestStoryTemplate(std::set<std::string> requiredTypes, std::vector<RawStoryLine> rawLines,
                          std::vector<WorldModelAction> actions)
                : StoryTemplate(rawLines), requiredTypes(requiredTypes), actions(actions) {
        }

        std::set<std::string> GetRequiredEntities() const override {
            return requiredTypes;
        }

        StoryTemplateResult CreateStory(const EntityMap &entities, const WeaverGraph &graph,
                                        const WorldModel &worldModel) const override {
            std::vector<StoryLine> lines;

            for (auto rawLine : rawLines) {
                std::vector<NuggetOption> options;
                for (auto nuggetKey : rawLine.nuggets) {
                    std::vector<ID> ids;
                    if (!entities.empty() && !requiredTypes.empty()) {
                        std::string someType = *requiredTypes.begin();
                        if (ReturnInvalidIDs) {
                            ids.push_back(133337);
                        } else {
                            ids.push_back(entities.find(someType)->second.at(0)->GetId());
                        }
                    }
                    options.push_back(NuggetOption(nuggetKey, ids));
                }
                lines.push_back(StoryLine(rawLine.prePart, options, rawLine.postPart));
            }

            StoryTemplateResult result;
            result.lines = move(lines);
            result.worldActions = actions;
            return result;
        }

    private:
        std::set<std::string> requiredTypes;
        std::vector<WorldModelAction> actions;
    };
}
