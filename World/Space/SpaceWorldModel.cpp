//
// Created by michael on 06.10.15.
//

#include <World/Space/SpaceWorldModel.h>

using namespace std;
using namespace weave;

shared_ptr<SpaceLocation> SpaceWorldModel::CreateLocation() const {
    double x = rs->GetIntInRange(param.minLocation, param.maxLocation);
    double y = rs->GetIntInRange(param.minLocation, param.maxLocation);
    double z = rs->GetIntInRange(param.minLocation, param.maxLocation);
    return make_shared<SpaceLocation>(x, y, z);
}

SpaceWorldModel::SpaceWorldModel(std::shared_ptr<RandomStream> randomStream) : WorldModel() {
    rs = randomStream;
}

ModelParameters SpaceWorldModel::GetParameters() {
    return param;
}

void SpaceWorldModel::SetParameters(ModelParameters parameters) {
    this->param = parameters;
}

const NameGenerator &SpaceWorldModel::GetNameGenerator() const {
    return nameGenerator;
}

std::shared_ptr<SpaceAgent> SpaceWorldModel::CreateAgent() const {
    return make_shared<SpaceAgent>(nameGenerator.CreateName(NameType::LIGHT_PERSON, rs));
}

std::shared_ptr<SolarSystem> SpaceWorldModel::CreateSolarSystem() const {
    return make_shared<SolarSystem>(nameGenerator.CreateName(NameType::LIGHT_THING, rs), CreateLocation());
}
