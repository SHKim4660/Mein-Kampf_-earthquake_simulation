#version 430 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;

layout (location = 2) uniform mat4 M;
layout (location = 3) uniform mat4 V;
layout (location = 4) uniform mat4 P;

out vec2 ftexCoord;

void main()
{
    gl_Position = P*V*M*vec4(pos, 1.0);
    ftexCoord = texCoord;
}
