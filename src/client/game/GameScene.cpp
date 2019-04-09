//
// Created by aurailus on 17/12/18.
//

#include "GameScene.h"

#include "../lua/l_register_block.h"
#include "../lua/l_register_blockmodel.h"

GameScene::GameScene(ClientState* state) :
        Scene(state),
        gui(state->renderer->getCamera()->getBufferDimensions()),
        debugGui(state->renderer->getCamera()->getBufferDimensions()) {

    server = new ServerConnection("127.0.0.1", 12345);
    server->init();

    textureAtlas = new TextureAtlas("../res/tex");
    blockAtlas = new BlockAtlas(textureAtlas);

    LuaParser p;
    p.init();

    //Register APIs here
    l_register_block(this, &p);
    l_register_blockmodel(this, &p);

    p.doFile("../res/lua/file.lua");

    //The scene requires the blockAtlas for meshing and handling inputs.
    world = new LocalWorld(blockAtlas);

    player = new Player();
    player->create(world, state->renderer->getCamera());

    gui.pushGuiObjects(guiEntities);
    debugGui.pushGuiObjects(guiEntities);
}


void GameScene::update() {
    server->update(*player, playerEntities);

    auto window = state->renderer->getWindow();

    player->update(window->getKeysArray(), state->deltaTime, window->getDeltaX(), window->getDeltaY(), window->mouseIsDown(), window->mouseIsDown());

    if (state->renderer->resized) {
        debugGui.bufferResized(state->renderer->getCamera()->getBufferDimensions());
        gui.bufferResized(state->renderer->getCamera()->getBufferDimensions());

        state->renderer->resized = false;
    }

    while (!server->chunkPackets.empty()) {
        auto it = server->chunkPackets.begin();
        Packet* p = *it;
        server->chunkPackets.erase(it);
        world->loadChunkPacket(p);
    }

    debugGui.update(player, world, blockAtlas, state->fps, (int)world->getMeshChunks()->size(), drawCalls, server->serverSideChunkGens, server->recvPackets);
    world->update();

    if (window->getKeysArray()[GLFW_KEY_F1]) {
        if (!F1Down) {
            F1Down = true;
            hudVisible = !hudVisible;
            debugGui.changeVisibilityState(hudVisible ? debugVisible ? 0 : 2 : 1);
            gui.setVisible(hudVisible);
        }
    }
    else F1Down = false;

    if (window->getKeysArray()[GLFW_KEY_F3]) {
        if (!F3Down) {
            F3Down = true;
            debugVisible = !debugVisible;
            debugGui.changeVisibilityState(hudVisible ? debugVisible ? 0 : 2 : 1);
        }
    }
    else F3Down = false;
}

void GameScene::draw() {
    auto camera = state->renderer->getCamera();

    state->renderer->begin();
    state->renderer->enableWorldShader();

    textureAtlas->getTexture()->use();

    drawCalls = 0;

    for (auto &chunkPair : *world->getMeshChunks()) {
        auto chunk = chunkPair.second;

        FrustumAABB bbox(*chunk->getPosition(), glm::vec3(16, 16, 16));

        if (camera->inFrustum(bbox) != Frustum::OUTSIDE) {

            state->renderer->draw(chunk);
            drawCalls++;

        }
    }

    Texture* prevTexture = nullptr;

    for (auto &entity : entities) {
        auto newTexture = entity->getTexture();

        if (newTexture != nullptr && newTexture != prevTexture) {
            prevTexture = newTexture;
            newTexture->use();
        }

        state->renderer->draw(entity);
    }

    prevTexture = nullptr;

    //TEMPORARY
    for (auto &entity : playerEntities) {
        auto newTexture = entity->getTexture();

        if (newTexture != nullptr && newTexture != prevTexture) {
            prevTexture = newTexture;
            newTexture->use();
        }

        state->renderer->draw(entity);
    }

    prevTexture = nullptr;

    state->renderer->enableGuiShader();

    for (auto &entity : guiEntities) {
        if (entity->isVisible()) {
            auto newTexture = entity->getTexture();

            if (newTexture != nullptr && newTexture != prevTexture) {
                prevTexture = newTexture;
                newTexture->use();
            }

            state->renderer->draw(entity);
        }
    }

    state->renderer->end();
}

void GameScene::cleanup() {
    //TODO: Clean up
}