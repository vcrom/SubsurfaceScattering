#version 330

layout(location = 0) in vec3 vVertex;
layout(location = 1) in vec3 vNormal;

uniform mat4 MVP;
uniform mat4 MV;
uniform float z_near = 0;
uniform float z_far;

smooth out float linear_depth;
void main()
{
    vec4 viewPos = MV * vec4(vVertex, 1);
    viewPos.xyz/=viewPos.w;
    linear_depth = -viewPos.z/z_far;

    gl_Position = MVP*vec4(vVertex.xyz,1);

	float Fcoef = 2.0 / log2(z_far + 1.0).
	gl_Position.z = log2(max(1e-6, 1.0 + gl_Position.w)) * Fcoef - 1.0;
}
