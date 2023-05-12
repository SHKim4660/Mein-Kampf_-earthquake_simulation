# version 430 core

in vec2 ftexCoord;
out vec4 fragColor;

void main()
{
    fragColor = vec4(ftexCoord, 0., 1.);
}
