//
// Created by aurailus on 25/11/18.
//

#include "Mesh.h"

Mesh::Mesh() {
    VAO = 0;
    VBO = 0;
    IBO = 0;
    indCount = 0;
}

void Mesh::create(std::vector<float>* vertices, std::vector<unsigned int>* indices) {
    this->indCount = (int)indices->size();

    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indCount * sizeof(unsigned int), &indices->front(), GL_STATIC_DRAW);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(float), &vertices->front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, nullptr);
    glEnableVertexAttribArray(0); //If dynamic amounts of attrib arrays are needed, then do these before drawing

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 5));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::draw() {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glDrawElements(GL_TRIANGLES, indCount, GL_UNSIGNED_INT, nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::cleanup() {
    if (IBO != 0) {
        glDeleteBuffers(1, &IBO);
        IBO = 0;
    }

    if (VBO != 0) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }

    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }

    indCount = 0;
}

Mesh::~Mesh() {
    cleanup();
}