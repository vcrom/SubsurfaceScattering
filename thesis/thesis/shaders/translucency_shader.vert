#version 330

layout(location = 0) in vec3 vVertex;
layout(location = 1) in vec3 vNormal;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 MV_L;
uniform mat4 S;
uniform mat4 N;
uniform highp float z_far;


//shader outputs to the fragment shader
smooth out vec3 vEyeSpacePosition;
smooth out vec4 vShadowCoords;
smooth out vec3 vEyeSpaceNormal;
smooth out float vlight_lineal_depth;

void main()
{
	vEyeSpaceNormal = vec4(normalize(N*vec4(vNormal, 0))).xyz;
	vEyeSpacePosition = vec4(MV*vec4(vVertex, 1)).xyz;
	vShadowCoords = S*(vec4(vVertex - 0.025*vNormal, 1));
	gl_Position = MVP*vec4(vVertex, 1);

    vec4 viewPos = MV_L * vec4(vVertex, 1);
    viewPos.xyz/=viewPos.w;
    vlight_lineal_depth = -viewPos.z/z_far;
}
