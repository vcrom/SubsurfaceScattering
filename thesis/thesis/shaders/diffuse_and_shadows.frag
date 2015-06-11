#version 330 core
//precision highp float;

layout(location=0) out vec4 vFragColor;

//uniforms
uniform sampler2DShadow shadow_map;
uniform vec3 eye_light_position = vec3(0,0,0);

//inputs from the vertex shader
smooth in vec3 vEyeSpaceNormal;
smooth in vec3 vEyeSpacePosition;
smooth in vec4 vShadowCoords;

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

void main() {

    //get light position in eye space
    vec4 vEyeSpaceLightPosition = vec4(eye_light_position,1);

    //get the light vector
    vec3 L = vEyeSpaceLightPosition.xyz-vEyeSpacePosition;
    L = normalize(L);

    //get diffuse component
    vec3 N = normalize(vEyeSpaceNormal);
    float diffuse = /*max(0.0f, */dot(N, L);//);

    diffuse = 1;

    if(vShadowCoords.w>1) {
        //In case of PCF, we take a number of shadow map samples and then
        //average their contributions. The average value is then used to
        //darken/lighten the shading. For this case, we have a separate
        //function textureProjOffset that accepts an offset from the given
        //shadow coordinate and returns the shadow comparison result.

        float sum = 0;
        float shadow = 1;

        //using 3x3 neighborhood
        #ifdef PCF_STRATIFIED_3x3

		float rad = 2.0f;
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

//        if(diffuse > shadow) diffuse = shadow;
//        else diffuse = mix(diffuse, diffuse*shadow, 0.5);

        //diffuse = mix(diffuse, diffuse*shadow, 0.5);

        //diffuse = min(diffuse, shadow);
        diffuse = shadow;
    }

    vFragColor = vec4(vec3(diffuse), 1.0f);

    //vFragColor = vec4(0.0f, 0.0f, 0.0f, 1-shadow_intensity);
    //vFragColor = vec4(1, 0, 0, 1);
    //vFragColor = vec4(0.5, 0.5, 0.5, 1);
}
