#version 330

layout(location = 0) out vec4 FragColor;
layout(location = 1) out float FragLinearDepth;
layout(location = 2) out float FragSpecularColor;

#define saturate(a) clamp(a, 0.0, 1.0)
#define SEPARATE_SPECULARS
/*
    float bumpiness;
    float specularIntensity;
    float specularRoughness;
    float specularFresnel;
    float translucency;
    float sssEnabled;
    float sssWidth;
    float ambient;
*/


////////////////////////////////////////
/////////////For each light/////////////

///////////////Main///////////////
uniform vec3 light_color = vec3(1, 1, 1);
uniform vec3 light_position = vec3(0, 2, 0);
///////////////Main///////////////

///////////////Shadows///////////////
uniform sampler2DShadow shadow_map;
uniform mat4 lightViewProjBiasM;
///////////////Shadows///////////////

/////////////Translucency/////////////
uniform mat4 lightViewM;
uniform mat4 lightProjBiasM;
uniform float light_far_plane;
uniform sampler2D lightLinearShadowMap;
/////////////Translucency/////////////

/////////////For each light/////////////
////////////////////////////////////////

///////////////Main///////////////
smooth in vec4 mesh_color;
smooth in float linear_depth;
smooth in vec3 worldPosition;
smooth in vec3 worldNormal;
smooth in vec3 prev_vPosition;
smooth in vec3 curr_vPosition;
smooth in vec3 view_vector;
uniform float m_ambientcomp = 1.0;
///////////////Main///////////////



/////////////Translucency/////////////
uniform bool sssEnabled = true;
uniform float sssWidth;
uniform float translucency;
/////////////Translucency/////////////




///////////////Shadows///////////////
//shadow mapping mode
#define PCF_STRATIFIED_3x3
//#define PCF_STRATIFIED_4x4
//#define PCF_RANDOM_SAMPLING
//#define SIMPLE_SHADOW_MAP

#ifdef RANDOM_SAMPLING
//pseudorandom number generator
float random(vec4 seed) {
        float dot_product = dot(seed, vec4(12.9898,78.233,45.164,94.673));
    return fract(sin(dot_product) * 43758.5453);
}
#endif

float shadowMapping(sampler2DShadow shadow_map, vec4 vShadowCoords)
{
    float shadow = 1;
    if(vShadowCoords.w>1) {
        //In case of PCF, we take a number of shadow map samples and then
        //average their contributions. The average value is then used to
        //darken/lighten the shading. For this case, we have a separate
        //function textureProjOffset that accepts an offset from the given
        //shadow coordinate and returns the shadow comparison result.

        float sum = 0;

        //using 3x3 neighborhood
        #ifdef PCF_STRATIFIED_3x3

		const float rad = 1.0f;
        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2(-rad,-rad));
        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2(-rad, 0));
        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2(-rad, rad));

        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2( 0,-rad));
        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2( 0, 0));
        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2( 0, rad));

        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2( rad,-rad));
        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2( rad, 0));
        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2( rad, rad));

		shadow = sum/9.0;


        #endif

        //using 4x4 neighborhood
        #ifdef PCF_STRATIFIED_4x4
        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2(-2,-2));
        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2(-1,-2));
        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2( 1,-2));
        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2( 2,-2));

        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2(-2,-1));
        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2(-1,-1));
        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2( 1,-1));
        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2( 2,-1));

        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2(-2, 1));
        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2(-1, 1));
        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2( 1, 1));
        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2( 2, 1));

        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2(-2, 2));
        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2(-1, 2));
        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2( 1, 2));
        sum += textureProjOffset(shadow_map, vShadowCoords, ivec2( 2, 2));

        shadow = sum/16.0;
        #endif

        #ifdef PCF_RANDOM_SAMPLING
        for(int i=0;i<16;i++) {
            float indexA = (random(vec4(gl_FragCoord.xyx, i))*0.25);
            float indexB = (random(vec4(gl_FragCoord.yxy, i))*0.25);
            sum += textureProj(shadow_map, vShadowCoords+vec4(indexA, indexB, 0, 0));
        }
        shadow = sum/16.0;
        #endif

        #ifdef SIMPLE_SHADOW_MAP
        shadow = textureProj(shadow_map, vShadowCoords);
        #endif
	}
	return shadow;
}
///////////////Shadows///////////////

/////////////Translucency/////////////
vec3 transmittance(float translucency, float sss_width, vec3 world_position, vec3 world_normal, vec3 light_vector, sampler2D shadow_map, mat4 light_V, mat4 ligh_BP ,float light_far_plane)
{
	float scale = 8.25 * (1.0 - translucency) / sss_width;
	vec4 shrinked_pos = vec4(world_position - 0.005 * world_normal, 1.0);
	vec4 shadow_pos = light_V*shrinked_pos;

	highp float d2 = (shadow_pos.z/shadow_pos.w)/-light_far_plane;

	shadow_pos = ligh_BP * shadow_pos;
	highp float d1 = texture(shadow_map, shadow_pos.xy/shadow_pos.w).r;
	//return vec3(2*d1);
	d1 *= light_far_plane;
	d2 *= light_far_plane;
	//return vec3(abs(d1 - d2)*10);

	highp float dist = abs(d1 - d2)*scale;
	//float d = scale * dist;
	highp float d = dist;
	highp float dd = -d * d;
    vec3 profile =	vec3(0.233, 0.455, 0.649) * exp(dd / 0.0064) +
                    vec3(0.1,   0.336, 0.344) * exp(dd / 0.0484) +
                    vec3(0.118, 0.198, 0.0)   * exp(dd / 0.187)  +
                    vec3(0.113, 0.007, 0.007) * exp(dd / 0.567)  +
                    vec3(0.358, 0.004, 0.0)   * exp(dd / 1.99)   +
                    vec3(0.078, 0.0,   0.0)   * exp(dd / 7.41);

	//return profile * max(0.3 + dot(-world_normal, light_vector), 0.0);

	return profile * clamp(0.3 + dot(normalize(light_vector), -normalize(world_normal)), 0.0, 1.0);
}
/////////////Translucency/////////////


void main()
{

	vec3 N = normalize(worldNormal);
    vec3 V = normalize(view_vector);

	
	vec4 albedo =  vec4(pow(mesh_color.rgb, vec3(2.2)), 1.0f);// mesh_color;
	float intensity = 1.88;
	float roughness = 0.3;

	vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
	//FragSpecularColor = 0;
	//for all lights
	vec3 L = light_position - worldPosition;
	float dist = length(L);
	//L /= dist;
	L = normalize(L);
	vec3 H = normalize(L + V);

	float attenuation = 1.0; 
	float spot = 1.0; 
	vec3 f1 = light_color * attenuation * spot;
	vec3 f2 = albedo.rgb * f1;

	float diffuse = saturate(dot(L, N));
	float specular = max(0, pow(dot(N, H), 17));
	//float specular = intensity * SpecularKSK(beckmannTex, normal, light, input.view, roughness);
	vec4 shadow_coords = lightViewProjBiasM*vec4(worldPosition + 0.025*N, 1);
	float shadow = shadowMapping(shadow_map, shadow_coords);
	// Add the diffuse and specular components:
    #ifdef SEPARATE_SPECULARS
    color.rgb += shadow * f2 * diffuse;
    FragSpecularColor += shadow * 1 * specular;
    #else
    color.rgb += shadow * (f2 * diffuse + f1 * specular);
    #endif

	// Add the transmittance component:
    if (sssEnabled)
        color.rgb += f2 * transmittance(translucency, sssWidth, worldPosition, N, L, lightLinearShadowMap, lightViewM, lightProjBiasM, light_far_plane);

	////end for
	//// Add the ambient component:
    color.rgb += color.rgb + m_ambientcomp*albedo.rgb;//cubemapAmbient;
	//// Store the depth value:
	FragLinearDepth = linear_depth;

	//// Convert to non-homogeneous points by dividing by w:
	vec2 curr = curr_vPosition.xy/curr_vPosition.z; // w is stored in z
    vec2 prev = prev_vPosition.xy/prev_vPosition.z;

    // Calculate velocity in non-homogeneous projection space:
    vec2 velocity = curr - prev;

    // Compress the velocity for storing it in a 8-bit render target:
    //color.a = sqrt(5.0 * length(velocity));


	//color.rgb = transmittance(translucency, sssWidth, worldPosition, N, L, lightLinearShadowMap, lightViewM, lightProjBiasM, light_far_plane);
	//transmittance(translucency, float sss_width, vec3 world_position, vec3 world_normal, vec3 light_vector, sampler2D shadow_map, mat4 light_V, mat4 ligh_BP ,float light_far_plane)
	//color = vec4(color.a*400);
	//color = vec4(shadowMapping(shadow_map, shadow_coords));
	//color = vec4(FragSpecularColor);
	//color = vec4(pow(color.rgb, vec3(1/2.2)), 1.0f);

	//color.rgb = pow(f2*transmittance(translucency, sssWidth, worldPosition, N, L, lightLinearShadowMap, lightViewM, lightProjBiasM, light_far_plane), vec3(1/2.2));
	//color.rgb = transmittance(translucency, sssWidth, worldPosition, N, L, lightLinearShadowMap, lightViewM, lightProjBiasM, light_far_plane);
	FragColor = color;
 //   float col = (dot(N, vec3(0,1,0)) + 1)/2;
	//FragColor = vec4(mesh_color*col);
	//FragColor = vec4(1,0, 1, 1);
}
