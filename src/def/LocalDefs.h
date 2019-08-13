//
// Created by aurailus on 18/04/19.
//

#ifndef ZEUS_GAMEDEFS_H
#define ZEUS_GAMEDEFS_H

#include "texture/TextureAtlas.h"
#include "LocalDefinitionAtlas.h"
#include "../api/client/LocalLuaParser.h"

class LocalDefs {
public:
    explicit LocalDefs(const std::string& tex_path);
    LocalDefs(const LocalDefs& copy);

    LocalDefinitionAtlas& defs();
    TextureAtlas& textures();
    LocalLuaParser& lua();

    void initLuaApi(LocalWorld &world, GameGui& gui);
    void update(float delta);

    ~LocalDefs() = default;
private:
    float delta = 0;

    std::string tex_path;

    TextureAtlas textureAtlas;
    LocalDefinitionAtlas blockAtlas;
    LocalLuaParser luaApi;
};


#endif //ZEUS_GAMEDEFS_H
