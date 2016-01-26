//
// Created by michael on 18.01.16.
//

#pragma once

#include <World/WorldModel.h>
#include <Template/TemplateFactory.h>

namespace weave {

    struct WeaverConfig {
        /*
         * The random seed used to create new quests. Quest Systems initialized with the same seed create the same
         * stream of quests (given the quests are initialized and completed the same way).
         */
        uint64_t seed = 0;

        /*
         * The directories in which the templates and other files can be found in. Also allows to define a mod directory
         * which can overwrite the default directory for selected files.
         */
        Directories dirs;

        /*
         * The world model is responsible for creating all quest entities and informs about world changes.
         * MUST be initialized by the caller before creating a new QuestWeaver instance.
         */
        WorldModel *worldModel = nullptr;

        /*
         * The template factories used to create new quest templates.
         * At least one factory has to be registered before trying to create new quests.
         */
        std::vector<std::shared_ptr<TemplateFactory>> templateFactories;

        /*
         * True if used for testing and debugging.
         */
        bool debug = true;

        WeaverConfig() = default;

        explicit WeaverConfig(uint64_t seed) : seed(seed) { }
    };
}
