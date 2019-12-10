//
// Created by aurailus on 10/06/19.
//

#include "ServerDefs.h"

ServerDefs::ServerDefs(const std::string& subgame, const std::string& execPath) {
    size_t exec = execPath.find_last_of('/');
    std::string gamePath = execPath.substr(0, exec + 1);
    subgamePath += "subgames/" + subgame + "/";

    if (subgame.empty()) {
        std::cout << Log::err << "No subgame specified." << Log::endl;
        exit(1);
    }
    else if (!cf_file_exists(subgamePath.data())) {
        std::cout << Log::err << "Subgame '" << subgame << "' does not exist." << Log::endl;
        exit(1);
    }
}

void ServerDefs::init(ServerWorld &world) {
    luaApi.init(*this, world, subgamePath);
}

ServerDefinitionAtlas& ServerDefs::defs() {
    return blockAtlas;
}

ServerLuaParser& ServerDefs::lua() {
    return luaApi;
}

AssetStorage& ServerDefs::assets() {
    return assetStorage;
}

ServerBiomeAtlas& ServerDefs::gen() {
    return biomes;
}

void ServerDefs::update(double delta) {
    this->delta += delta;
    while (this->delta > 0.05f) {
        luaApi.update(this->delta);
        this->delta -= 0.05f;
    }
}
