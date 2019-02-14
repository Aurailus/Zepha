//
// Created by aurailus on 17/12/18.
//

#include "Renderer.h"

Renderer::Renderer() : Renderer(1366, 768) {};

Renderer::Renderer(GLint winWidth, GLint winHeight) {

    window = new Window(winWidth, winHeight);
    window->initialize();

    camera = new Camera();
    camera->create(glm::vec3(0, 1, 0));

    worldShader = new Shader();
    worldShader->createFromFile("../res/shader/world.vs", "../res/shader/world.fs");

    uProj = worldShader->getUniformLocation("projection");
    uModel = worldShader->getUniformLocation("model");
    uView = worldShader->getUniformLocation("view");

    guiShader = new Shader();
    guiShader->createFromFile("../res/shader/gui.vs", "../res/shader/gui.fs");

    uOrtho = guiShader->getUniformLocation("ortho");
    uGuiModel = guiShader->getUniformLocation("model");

    createMatrices();

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::update() {
    window->update();
    if (window->resized) {
        createMatrices();
        window->resized = false;
    }
}

void Renderer::begin() {
    //Clear Window
    glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::enableWorldShader() {
    worldShader->useShader();

    glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
}

void Renderer::enableGuiShader() {
    guiShader->useShader();

    glUniformMatrix4fv(uOrtho, 1, GL_FALSE, glm::value_ptr(orthographicMatrix));
}

void Renderer::draw(Entity* entity) {
    glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(entity->getModelMatrix()));
    entity->draw();
}

void Renderer::drawGui(Entity* entity) {
    glUniformMatrix4fv(uGuiModel, 1, GL_FALSE, glm::value_ptr(entity->getModelMatrix()));
    entity->draw();
}

void Renderer::end() {
    Shader::clearShader();
    window->swapBuffers();
}

Renderer::~Renderer() {
    worldShader->cleanup();
    delete worldShader;
    guiShader->cleanup();
    delete guiShader;
}

Window *Renderer::getWindow() {
    return window;
}

Camera *Renderer::getCamera() {
    return camera;
}

void Renderer::createMatrices() {
    projectionMatrix = glm::perspective(45.0f, window->getBufferWidth() / window->getBufferHeight(), 0.1f, 1000.0f);
    orthographicMatrix = glm::ortho(0.0f, window->getBufferWidth(), window->getBufferHeight(), 0.0f, 0.0f, 100.0f);
}