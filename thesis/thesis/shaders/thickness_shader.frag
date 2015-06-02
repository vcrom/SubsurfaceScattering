#version 330

//precision highp float;
layout(location = 0) out highp float thickness;

//smooth in vec3 eye_vNormal;
smooth in highp float lineal_depth;

void main()
{
    highp float aux = lineal_depth;
    thickness = gl_FrontFacing ? aux : -aux;
}
