#version 330

layout(location = 0) in vec3 vVertex;
layout(location = 1) in vec3 vNormal;

uniform mat4 MVP;
uniform mat4 MV;
//uniform mat4 N;
//uniform float z_near = 0;
uniform highp float z_far;

smooth out highp float lineal_depth;
//smooth out vec3 eye_normal;

void main()
{
    vec4 viewPos = MV * vec4(vVertex, 1);
    viewPos.xyz/=viewPos.w;
    linear_depth = -viewPos.z/zF;

    //eye_vNormal = normalize(vec4(N*vec4(vNormal,0)).xyz);

    gl_Position = MVP*vec4(vVertex.xyz,1);
}
