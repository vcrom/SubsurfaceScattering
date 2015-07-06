#version 330
precision highp float;
layout(location = 0) in vec3 vVertex;
//layout(location = 1) in vec3 vNormal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
//uniform float z_near = 0;
uniform float z_far;
uniform vec3 light_position = vec3(0, 0, 0);

smooth out highp float linear_depth;

void main()
{
	vec4 world_pos = M*vec4(vVertex, 1);
	vec3 L = normalize(light_position - world_pos.xyz);
	//world_pos.xyz -= L*0.025;

	vec4 view_pos = V * world_pos;
	linear_depth = -(view_pos.z / view_pos.w) / z_far;
	gl_Position = P*view_pos;
}
