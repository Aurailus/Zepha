//
// Created by aurailus on 16/04/19.
//

#pragma once

#include <map>
#include <cmath>
#include <memory>
#include <vector>
#include <algorithm>
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <cute_files/cute_files.h>

#include "AtlasRef.h"
#include "RawTexData.h"
#include "../../util/Log.h"
#include "../../game/graph/Texture.h"

class TextureAtlas {
public:
    TextureAtlas() = default;
    explicit TextureAtlas(unsigned int width, unsigned int height = 0);
    std::vector<std::shared_ptr<AtlasRef>> loadDirectory(const std::string& path, bool base = true, bool recursive = true);
    std::shared_ptr<AtlasRef> loadImage(const std::string& path, const std::string& name, bool base = false);

    void update();

    glm::vec4 sampleTexturePixel(const std::shared_ptr<AtlasRef>& atlasRef, glm::vec2 pixel);

    std::shared_ptr<AtlasRef> addImage(unsigned char *data, const std::string& name, bool base, int texWidth, int texHeight);
    std::shared_ptr<AtlasRef> generateCrackImage(const std::string &name, unsigned short crackLevel);

    std::shared_ptr<AtlasRef> operator[](const std::string& name);

    ~TextureAtlas();

    glm::ivec2 pixelSize {};
    glm::ivec2 tileSize {};

    Texture atlasTexture {};
    unsigned char* atlasData = nullptr;

    unsigned int textureSlotsUsed = 0;
    unsigned int maxTextureSlots = 0;
private:
    std::shared_ptr<AtlasRef> generateTexture(std::string req);

    RawTexData getBytesOfTex(const std::string& name);
    RawTexData getBytesAtPos(glm::ivec2 pos, glm::ivec2 dims);
    glm::vec2 findImageSpace(int w, int h);

    void createMissingImage();
    void updateAtlas(int tileX, int tileY, int texWidth, int texHeight, unsigned char *data);

    void deleteImage(std::shared_ptr<AtlasRef> ref);

    std::map<std::string, std::shared_ptr<AtlasRef>> textures;
    std::vector<bool> empty;
};

