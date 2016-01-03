#version 330

layout(location = 0) out vec4 vFragColor;

//uniforms
uniform sampler2D shadow_map;
uniform vec3 light_position = vec3(0,0,0);

uniform float scale = 4.0;

uniform mat4 BP_L;
uniform mat4 V_L;
uniform float z_far;

uniform float translucency = 1; /*0..1*/
uniform float sssWidth = 0.3; /**/

//
//smooth in vec4 vShadowCoords;
smooth in vec3 vWorldSpacePosition;
smooth in vec3 vWorldSpaceNormal;
//smooth in float vLight_lineal_depth;


vec3 transmittance(float translucency, float sss_width, vec3 world_position, vec3 world_normal, vec3 light_vector, sampler2D shadow_map, mat4 light_V, mat4 ligh_BP ,float light_far_plane)
{
	float scale = 8.25 * (1.0 - translucency) / sss_width;
	scale = 4;
	vec4 shrinked_pos = vec4(world_position - 0.005 * world_normal, 1.0);
	vec4 shadow_pos = light_V*shrinked_pos;

	float d2 = shadow_pos.z/shadow_pos.w;

	shadow_pos = ligh_BP * shadow_pos;
	float d1 = texture(shadow_map, shadow_pos.xy/shadow_pos.w).r;
	d1 *= -light_far_plane;

	float d = scale * abs(d1 - d2);
	float dd = -d * d;
    vec3 profile =	vec3(0.233, 0.455, 0.649) * exp(dd / 0.0064) +
                    vec3(0.1,   0.336, 0.344) * exp(dd / 0.0484) +
                    vec3(0.118, 0.198, 0.0)   * exp(dd / 0.187)  +
                    vec3(0.113, 0.007, 0.007) * exp(dd / 0.567)  +
                    vec3(0.358, 0.004, 0.0)   * exp(dd / 1.99)   +
                    vec3(0.078, 0.0,   0.0)   * exp(dd / 7.41);

	return profile * clamp(0.3 + dot(light_vector, -world_normal), 0.0, 1.0);
}


void main()
{
	vFragColor = vec4(transmittance(1, 1, vWorldSpacePosition, vWorldSpaceNormal, light_position - vWorldSpacePosition, shadow_map, V_L, BP_L, z_far), 1);
}
