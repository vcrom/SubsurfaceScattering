#version 330

layout(location = 0) out vec4 FragColor;
layout(location = 1) out float FragLinearDepth;
layout(location = 2) out vec3 FragSpecularColor;
layout(location = 3) out float FragCBFFactor;
layout(location = 4) out float FragCurvature;

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
uniform mat4 viewInverseTransposeM; 
smooth in vec4 mesh_color;
smooth in float linear_depth;
smooth in vec3 worldPosition;
smooth in vec3 worldNormal;
smooth in vec3 worldTangent;
//smooth in vec3 viewNormal;
smooth in vec3 viewPos;
//smooth in vec3 prev_vPosition;
//smooth in vec3 curr_vPosition;
smooth in vec3 view_vector;
uniform float m_ambientcomp = 0.61;
uniform float spec_int = 0.5;
uniform float z_near = 0;

smooth in vec2 texture_coords;
uniform int texture_enabled;
uniform sampler2D diffuse_color_texture;
uniform sampler2D ao_texture;
uniform sampler2D normal_texture;
uniform float z_far;
uniform float roughness = 	pow(8192.0f, 0.5f);
///////////////Main///////////////



/////////////Translucency/////////////
uniform bool sssEnabled = true;
uniform float sssWidth;
uniform float translucency;
/////////////Translucency/////////////




///////////////Shadows///////////////
//shadow mapping mode
//#define PCF_STRATIFIED_3x3
#define PCF_STRATIFIED_4x4
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
//vShadowCoords.xy *= 2;
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

	return profile * clamp(0.3 + dot(normalize(light_vector), -normalize(world_normal)), 0.0, 1.0);
}
/////////////Translucency/////////////

//vec3 FresnelSchlick(vec3 F0, vec3 l, vec3 h)
//{
//    return F0 + (1 - F0)*pow(1 - max(0, dot(l, h)),5);
//}

//float fresnelSlickSpecular(vec3 L, vec3 H)
//{
//	vec3 cross_fres = FresnelSchlick(vec3(0.04f, 0.04f, 0.04f), L, H);
//}

//precision highp float;
float computeCBFFactor(vec3 view_normal, vec3 view_pos, float z_near)
{
	vec3 w = -normalize(view_pos);
	float cos_a_i = abs(dot(vec3(0,0,-1), -w));
	float cos_a_j = abs(dot(view_normal, w));
	//view_pos.z = 5*view_pos.z/z_far;
	float norm = length(vec3(view_pos.xy, -z_near));
	return cos_a_i*cos_a_i*cos_a_i*norm*norm/cos_a_j;
}

vec3 bumpMap(sampler2D normalTex, vec2 texcoord) {
    vec3 bump = vec3(-1.0) + vec3(2.0) * texture(normalTex, texcoord).rgb;
    return normalize(bump);
}

#extension GL_OES_standard_derivatives : enable
//http://madebyevan.com/shaders/curvature/
float screenSpaceCurvature(vec3 normal, vec3 screen_pos)
{
	vec3 dx = dFdx(normal);
	vec3 dy = dFdy(normal);
	vec3 xneg = normal - dx;
	vec3 xpos = normal + dx;
	vec3 yneg = normal - dy;
	vec3 ypos = normal + dy;
	float depth = length(screen_pos);
	float curvature = (cross(xneg, xpos).y - cross(yneg, ypos).x) * 4 / depth;
	return curvature;
}

vec3 FresnelSchlick(vec3 F0, vec3 l, vec3 h)
{
    return F0 + (1 - F0)*pow(1 - max(0, dot(l, h)),5);
}

//#define NORMAL_MAP_CURV
#define TRANSLUCENCY_MOD_CURV
void main()
{
	//Main
	//use normal map
	vec3 N = normalize(worldNormal);
	if(texture_enabled != 0)
	{
		vec3 tg = normalize(worldTangent);
		vec3 bitg = normalize(cross(N, tg));
		mat3 tbn = transpose(mat3(tg, bitg, N));
		tbn = mat3(tg, bitg, N);

		float bumpiness = 0.5;
		vec3 tangent_normal = mix(vec3(0.0, 0.0, 1.0), bumpMap(normal_texture, texture_coords), bumpiness);
		N = normalize(tbn*tangent_normal);
	}

	vec3 view_normal = normalize(vec4(viewInverseTransposeM*vec4(normalize(worldNormal), 1)).xyz);
	vec3 aux_N = normalize(vec4(viewInverseTransposeM*vec4(N, 1)).xyz);
	#ifdef NORMAL_MAP_CURV
	float curvature = screenSpaceCurvature(aux_N, viewPos);
	#else
	float curvature = screenSpaceCurvature(view_normal, viewPos);
	#endif
	FragCurvature = saturate(curvature);//0 ... 1

	//CBF
	FragCBFFactor = computeCBFFactor(/*view_normal*/aux_N, vec3(viewPos.xy, viewPos.z), z_near);// (normalize(viewNormal)+1)/2;
	//FragCBFFactor /= max_cbf_factor;

	//vec3 N = normalize(worldNormal);
    vec3 V = normalize(view_vector);
	vec4 albedo =  vec4(pow(mesh_color.rgb, vec3(2.2)), 1.0f);;
	float occlusion = 1;
	if(texture_enabled != 0)
	{
		albedo =  vec4(pow(texture(diffuse_color_texture, texture_coords), vec4(2.2)));
		occlusion = texture(ao_texture, texture_coords).r;
	}
	//else
	//	albedo =  vec4(pow(mesh_color.rgb, vec3(2.2)), 1.0f);// mesh_color;

	//float intensity = 1.88;
	//float roughness = 0.3;

	vec4 color = vec4(0.0, 0.0, 0.0, 1.0);

	//float occlusion = 1;
	//if(texture_enabled != 0) occlusion = texture(ao_texture, texture_coords).r;

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

	float diffuse = saturate(dot(L, aux_N/*N*/)) * m_ambientcomp;

	//specular
	vec3 specular = (roughness + 2) / 8*pow((max(0, dot(N, H))), roughness) * FresnelSchlick(vec3(0.04, 0.04, 0.04), L, H)*max(0, dot(aux_N/*N*/, L))*spec_int;
	//float specular = max(0, pow(dot(N, H), 17)) * spec_int;//17
	//float specular = intensity * SpecularKSK(beckmannTex, normal, light, input.view, roughness);

	vec4 shadow_coords = lightViewProjBiasM*vec4(worldPosition + 0.005*L/*+ 0.001*N*/, 1);
	float shadow = shadowMapping(shadow_map, shadow_coords);
	// Add the diffuse and specular components:
    #ifdef SEPARATE_SPECULARS
    color.rgb += shadow * f2 * diffuse;
    FragSpecularColor += shadow * f1 * specular;
    #else
    color.rgb += shadow * (f2 * diffuse + f1 * specular);
    #endif

	// Add the transmittance component:
    if (sssEnabled && translucency != 0)
	{
	#ifdef TRANSLUCENCY_MOD_CURV
        color.rgb += f2 * transmittance(translucency, sssWidth*(1.0+FragCurvature), worldPosition, N, L, lightLinearShadowMap, lightViewM, lightProjBiasM, light_far_plane);
	#else
		color.rgb += f2 * transmittance(translucency, sssWidth, worldPosition, N, L, lightLinearShadowMap, lightViewM, lightProjBiasM, light_far_plane);
	#endif
	}
	////end for
	//// Add the ambient component:
    color.rgb += occlusion*albedo.rgb;//cubemapAmbient;
	//// Store the linear depth value:
	FragLinearDepth = linear_depth;

	//// Convert to non-homogeneous points by dividing by w:
	//vec2 curr = curr_vPosition.xy/curr_vPosition.z; // w is stored in z
    //vec2 prev = prev_vPosition.xy/prev_vPosition.z;

    // Calculate velocity in non-homogeneous projection space:
    //vec2 velocity = curr - prev;

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
	color.a = albedo.a;
	FragColor = color;
 //   float col = (dot(N, vec3(0,1,0)) + 1)/2;
	//FragColor = vec4(mesh_color*col);
	//FragColor = vec4(1,0, 1, 1);
	//FragColor = vec4(texture(ao_texture, texture_coords).r);
	//FragColor = vec4(bumpMap(normal_texture, texture_coords), 1);view_normal
	//FragColor = vec4((N+1)/2, 1);
	//FragColor = vec4((aux_N+1)/2, 1);
	//FragColor = vec4((view_normal+1)/2, 1);
	//FragColor = vec4(vec3(abs(curvature)), 1);
	//FragColor = vec4(vec3(dFdx(worldNormal)), 1);
	//FragColor = vec4(vec3(dFdy(worldNormal)), 1);
	//FragColor = vec4(FragCBFFactor);
	//if(FragCBFFactor < 0) FragColor = vec4(1, 0, 0, 1);
	//FragColor = vec4(FragSpecularColor, 1);
	//FragColor = vec4(vec3(FragCurvature), 1);
	//FragCurvature = 0.0;

}
