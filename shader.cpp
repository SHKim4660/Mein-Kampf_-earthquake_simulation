#include "shader.hpp"
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

using namespace std;
using namespace gl;

GLuint createShader(const char *content, GLenum type)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1,
                   &content, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char log[1024];
        glGetShaderInfoLog(shader, 1024, NULL, log);

        panik(log);
    }

    return shader;
}

Shader::Shader(string vfilename, string ffilename)
{
    GLuint vert =
        createShader(load_file(vfilename).c_str(), GL_VERTEX_SHADER);
    GLuint frag =
        createShader(load_file(ffilename).c_str(), GL_FRAGMENT_SHADER);

    program = glCreateProgram();

    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char log[1024];
        glGetProgramInfoLog(program, 1024, NULL, log);

        panik(log);
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
}

Shader::~Shader()
{
    glDeleteProgram(program);
}    

void Shader::bind()
{
    glUseProgram(program);
}
