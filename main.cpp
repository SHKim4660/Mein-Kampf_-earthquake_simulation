#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/functions.h>
#include <iostream>
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <glbinding/getProcAddress.h>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Sparse>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "utils.hpp"
#include "shader.hpp"

#define WIDTH 1920
#define HEIGHT 1080

using namespace gl;
using namespace glm;
using namespace sf;

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

unsigned int indices[36] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25,26,27,28,29,30,31,32,33,34,35
};

int main()
{
    ContextSettings settings;
    settings.majorVersion = 4;
    settings.minorVersion = 3;
    settings.depthBits = 24;
    
    Window window(VideoMode(WIDTH, HEIGHT), "your face", Style::Default, settings);
    // window.setVerticalSyncEnabled(true);
    window.setActive(true);

    Clock clock;

    glbinding::initialize(nullptr);
    // glbinding::useContext(0);

    glEnable(GL_DEPTH_TEST);

    GLuint VAO, VBO, IBO;
    glCreateVertexArrays(1, &VAO);
    glCreateBuffers(1, &VBO);
    glCreateBuffers(1, &IBO);
    
    glNamedBufferStorage(VBO, sizeof(vertices), vertices, GL_DYNAMIC_STORAGE_BIT);

    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 5*sizeof(float));

    glEnableVertexArrayAttrib(VAO, 0);
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(VAO, 0, 0);

    glEnableVertexArrayAttrib(VAO, 1);
    glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 3*sizeof(float));
    glVertexArrayAttribBinding(VAO, 1, 0);
    //                   attribindex^  ^vertexbuffer index

    glNamedBufferStorage(IBO, sizeof(indices), indices, GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(VAO, IBO);

    Shader shader("vertexshader.vert", "fragmentshader.frag");
    shader.bind();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    bool running = true;
    while (running)
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                running = false;
            else if (event.type == Event::KeyPressed)
                switch (event.key.code)
                {
                case Keyboard::Escape:
                    running = false;
                    break;

                default:
                    break;
                }
        }
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.bind();

        mat4 model(1);
        mat4 view(1);
        mat4 proj(1);

        view = translate(view, vec3(0.f, 0.f, -5.f));
        proj = perspective(radians(45.f), (float)WIDTH/HEIGHT, 0.1f, 100.0f);

        // model = rotate(model, clock.getElapsedTime().asSeconds(), {0.f, 1.f, 0.f});
        model = rotate(model, clock.getElapsedTime().asSeconds(), {1.f, 0.f, 0.f});
        view = translate(view, {-3+clock.getElapsedTime().asSeconds()/2, 0.f, 0.f});

        glUniformMatrix4fv(2, 1, GL_FALSE, value_ptr(model));
        glUniformMatrix4fv(3, 1, GL_FALSE, value_ptr(view));
        glUniformMatrix4fv(4, 1, GL_FALSE, value_ptr(proj));

        // for (int i=0;i<36*3;i+=5)
        // {
        //     vertices[i] += 0.01f;
        // }

        glBindVertexArray(VAO);
        // glNamedBufferSubData(VBO, 0, sizeof(vertices), vertices);
        
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        window.display();
    }

    terminate(0);
}
