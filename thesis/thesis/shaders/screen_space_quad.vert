#version 330 core

layout(location=0) in vec2 vVertex;
smooth out vec2 vUV;

void main()
{
    gl_Position = vec4(vVertex*2-1.0,0,1);
    vUV = vVertex;
}
