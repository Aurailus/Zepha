//
// Created by aurailus on 17/12/18.
//

#include "GameScene.h"

GameScene::GameScene(ClientState* state) : Scene(state),
    defs("../res/tex"),
    world(defs, &playerChunkPos),
    server("127.0.0.1", 12345, defs.textures()),

    gameGui(state->renderer->getCamera()->getBufferDimensions()),
    debugGui(state->renderer->getCamera()->getBufferDimensions(),  &defs.textures().getAtlasTexture()) {

    auto wireframe = new WireframeEntity({0, 0, 0}, {1, 1, 1}, 0.01);
    entities.push_back(wireframe);

    player = Player(&world, &defs, state->renderer->getCamera(), wireframe);

    gui.push_back(&gameGui);
    gui.push_back(&debugGui);

    server.init(entities);
}


void GameScene::update() {
    defs.textures().update();
    server.update(player);

    auto window = state->renderer->getWindow();

    playerChunkPos = TransPos::roundPos(player.getPos() / glm::vec3(TransPos::CHUNK_SIZE));
    player.update(window->input, state->deltaTime, window->getDeltaX(), window->getDeltaY());

    if (state->renderer->resized) {
        debugGui.bufferResized(state->renderer->getCamera()->getBufferDimensions());
        gameGui.bufferResized(state->renderer->getCamera()->getBufferDimensions());

        state->renderer->resized = false;
    }

    while (!server.chunkPackets.empty()) {
        auto it = server.chunkPackets.begin();
        Packet* p = *it;
        server.chunkPackets.erase(it);
        world.loadChunkPacket(p);
    }

    debugGui.update(player, world, defs, state->fps, world.getMeshChunkCount(), drawCalls, server.serverSideChunkGens, server.recvPackets);
    world.update(state->deltaTime);

    if (window->input.isKeyPressed(GLFW_KEY_F1)) {
        hudVisible = !hudVisible;
        debugGui.changeVisibilityState(hudVisible ? debugVisible ? 0 : 2 : 1);
        gameGui.setVisible(hudVisible);
    }

    if (window->input.isKeyPressed(GLFW_KEY_F3)) {
        debugVisible = !debugVisible;
        debugGui.changeVisibilityState(hudVisible ? debugVisible ? 0 : 2 : 1);
    }
}

void GameScene::draw() {
    auto &renderer = *state->renderer;
    auto &camera = *renderer.getCamera();

    drawCalls = 0;

    renderer.begin();

    renderer.enableTexture(&defs.textures().getAtlasTexture());
    drawCalls = world.render(renderer);

    for (auto entity : entities) {
        entity->draw(renderer);
    }

    state->renderer->beginGUI();

    for (auto entity : gui) {
        entity->draw(renderer);
    }

    state->renderer->end();
}

void GameScene::cleanup() {
    //No pointers to clean up at this point
}