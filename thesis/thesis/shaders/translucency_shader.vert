#version 330

layout(location = 0) in vec3 vVertex;
layout(location = 1) in vec3 vNormal;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 M;
uniform mat4 N;
uniform float z_far;


//shader outputs to the fragment shader
smooth out vec3 vWorldSpacePosition;
//smooth out vec4 vShadowCoords;
smooth out vec3 vWorldSpaceNormal;
//smooth out float vlight_lineal_depth;


void main()
{
	vWorldSpaceNormal = vec4(normalize(N*vec4(vNormal, 0))).xyz;
	vWorldSpacePosition = vec4(M*vec4(vVertex, 1)).xyz;
	//vShadowCoords = S*(vec4(vVertex - 0.025*vNormal, 1));
	gl_Position = MVP*vec4(vVertex, 1);

    //vec4 viewPos = MV_L * vec4(vVertex, 1);
    //viewPos.xyz/=viewPos.w;
    //vlight_lineal_depth = -viewPos.z/z_far;
}
