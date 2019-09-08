//
// Created by aurailus on 13/08/19.
//

#include "Font.h"

Font::Font(TextureAtlas& atlas, std::shared_ptr<AtlasRef> tex) :
    fontTex(std::move(tex)),
    atlasSize(atlas.getAtlasSize()) {

    getCharWidths(atlas);
}

uint Font::getCharWidth(char c) {
    uint index = static_cast<uint>(c) - 32;
    if (index >= AMOUNT_CHARS) {
        std::cout << Log::err << "Invalid char index!" << std::endl;
        return 0;
    }
    return charWidths[index];
}

void Font::getCharWidths(TextureAtlas &atlas) {
    const unsigned char* data = atlas.getAtlasData();

    charWidths[0] = 2;

    for (unsigned int i = 1; i < AMOUNT_CHARS; i++) {
        glm::vec2 charPos = {i % 18 * CHAR_WIDTH, std::floor(i / 18) * CHAR_HEIGHT};

        uint xBase = static_cast<uint>(fontTex->pos.x) + static_cast<uint>(charPos.x);
        uint yBase = static_cast<uint>(fontTex->pos.y) + static_cast<uint>(charPos.y);

        unsigned short width = 0;

        for (uint j = 0; j < CHAR_WIDTH; j++) {
            bool empty = true;
            for (uint k = 0; k < CHAR_HEIGHT; k++) {
                uint xx = xBase + j;
                uint yy = yBase + k;

                uint offset = yy * static_cast<uint>(atlasSize.x) * 4 + xx * 4 + 3;

                if (data[offset] != 0) {
                    empty = false;
                    break;
                }
            }
            if (!empty) width = static_cast<unsigned short>(j);
        }

        charWidths[i] = width;
    }
}

glm::vec4 Font::getCharUVs(char c) {
    uint index = static_cast<uint>(c) - 32;
    if (index >= AMOUNT_CHARS) {
        std::cout << Log::err << "Invalid char index!" << std::endl;
        return {};
    }

    glm::vec4 uv;

    glm::vec2 charPos = {((index % 18) * CHAR_WIDTH),
                         (std::floor(index / 18) * CHAR_HEIGHT)};

    uv.x = fontTex->uv.x + (charPos.x) / atlasSize.x;
    uv.y = fontTex->uv.y + (charPos.y) / atlasSize.y;
    uv.z = fontTex->uv.x + (charPos.x + getCharWidth(c) + 1) / atlasSize.x;
    uv.w = fontTex->uv.y + (charPos.y + CHAR_HEIGHT) / atlasSize.y;

    return uv;
}