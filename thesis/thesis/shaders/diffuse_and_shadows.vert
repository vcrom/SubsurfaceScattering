#version 330 core

layout(location=0) in vec3 vVertex;
layout(location=1) in vec3 vNormal;

//uniforms
uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 M;
uniform mat4 S;
uniform mat4 N;

//shader outputs to the fragment shader
smooth out vec3 vEyeSpacePosition;
smooth out vec4 vShadowCoords;
smooth out vec3 vEyeSpaceNormal;

void main()
{
    vEyeSpaceNormal = vec4(normalize(N*vec4(vNormal,0))).xyz;
    vEyeSpacePosition = vec4(MV*vec4(vVertex,1)).xyz;
    vShadowCoords = S*(vec4(vVertex,1));
    gl_Position = MVP*vec4(vVertex,1);
}
