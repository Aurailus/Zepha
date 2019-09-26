//
// Created by aurailus on 25/09/19.
//

#include "WorldGeometryShader.h"

WorldGeometryShader::WorldGeometryShader(glm::vec2 windowSize, float bufferScale) : Shader(),
    windowSize(windowSize),
    bufferScale(bufferScale),
    swayData(new unsigned char[16 * 4 * 16]) {

    swayNoise.SetFrequency(0.5);
    swayNoise.SetOctaveCount(2);
}

void WorldGeometryShader::postCreate() {
    swayTex.loadFromBytes(swayData, 16, 16, GL_LINEAR, GL_MIRRORED_REPEAT);

    uniforms.proj  = get("projection");
    uniforms.model = get("model");
    uniforms.view  = get("view");

    uniforms.swaySampler = get("swayTex");
    uniforms.time = get("time");

    use();
    set(uniforms.swaySampler, 1);
}

void WorldGeometryShader::windowResized(glm::vec2 windowSize) {
    this->windowSize = windowSize;
}
void WorldGeometryShader::updateSwayMap(double delta) {
    swayOffset += delta * 1.4;
    for (int i = 0; i < 16 * 16; i++) {
        swayData[i*4]   = static_cast<unsigned char>((fmax(-1, fmin(1, swayNoise.GetValue((i / 16) / 3.f, (i % 16) / 3.f, swayOffset)))       + 1) / 2.f * 255.f);
        swayData[i*4+1] = static_cast<unsigned char>((fmax(-1, fmin(1, swayNoise.GetValue((i / 16) / 3.f, (i % 16) / 3.f, swayOffset + 50)))  + 1) / 2.f * 255.f);
        swayData[i*4+2] = static_cast<unsigned char>((fmax(-1, fmin(1, swayNoise.GetValue((i / 16) / 3.f, (i % 16) / 3.f, swayOffset + 100))) + 1) / 2.f * 255.f);
    }
    swayTex.updateTexture(0, 0, 16, 16, swayData);
}

WorldGeometryShader::~WorldGeometryShader() {
    swayTex.clear();
    delete[] swayData;
}
