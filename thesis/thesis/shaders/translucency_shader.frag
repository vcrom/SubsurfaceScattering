#version 330

layout(location = 0) out vec4 vFragColor;

//uniforms
uniform sampler2D shadow_map;
uniform vec3 eye_light_position = vec3(0,0,0);
uniform float scale = 4.0;

//
smooth in vec3 vEyeSpacePosition;
smooth in vec4 vShadowCoords;
smooth in vec3 vEyeSpaceNormal;
smooth in float vlight_lineal_depth;

float distance(vec2 shadow_coords, sampler2D shadow_map, float lineal_depth) {
    // Shrink the position to avoid artifacts on the silhouette:
    //posW = posW - 0.005 * normalW;

    // Transform to light space:
    //vec3 posL = mul(vec3(posW, 1.0), lights[i].viewproj);

    // Fetch depth from the shadow map:
    // (Depth from shadow maps is expected to be linear)
	float d1 = texture(shadow_map, shadow_coords /*/ posL.w*/).r;
    float d2 = lineal_depth;

    // Calculate the difference:
    return abs(d1 - d2);
}

// This function can be precomputed for efficiency
vec3 T(float s) {
    return  vec3(0.233, 0.455, 0.649) * exp(-s * s / 0.0064) +
            vec3(0.1,   0.336, 0.344) * exp(-s * s / 0.0484) +
            vec3(0.118, 0.198, 0.0)   * exp(-s * s / 0.187)  +
            vec3(0.113, 0.007, 0.007) * exp(-s * s / 0.567)  +
            vec3(0.358, 0.004, 0.0)   * exp(-s * s / 1.99)   +
            vec3(0.078, 0.0,   0.0)   * exp(-s * s / 7.41);
}

void main()
{
    //get the light vector
    vec3 L = eye_light_position.xyz-vEyeSpacePosition;
    L = normalize(L);

	float s = scale*distance(vShadowCoords.xy/vShadowCoords.w, shadow_map, vlight_lineal_depth);
	float E = max(0.3 + dot(-vEyeSpaceNormal, L), 0.0);

	vec3 transmitance = T(s)*E;


	vFragColor = vec4(vec3(E), 1);
	vFragColor = vec4(transmitance, 1);
	//vFragColor = vec4(T(frag_tex_coords.x*4), 1);
}
