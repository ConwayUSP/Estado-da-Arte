#version 430 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    unsigned int lightVAO;
    glGenVertexArrays(1, & lightVAO);
    glBindVertexArray(lightVAO);

    // Precisamos apenas fazer o binding com o VBO, os dados do VBO do contêiner
    // já contêm os dados.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Definindo o atributo do vértice
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void * ) 0);
    glEnableVertexAttribArray(0);
}
