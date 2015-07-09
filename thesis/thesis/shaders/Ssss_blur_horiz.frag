#version 330
precision highp float;
layout(location=0) out vec4 vFColor;


uniform sampler2D color_texture;
uniform sampler2D lineal_depth_texture;

uniform float correction = 10;//0.0125*800
uniform float sssStrength = 15.75;
uniform vec2 pixel_size;
uniform vec4 gaussian;
const vec2 dir = vec2(1, 0);

smooth in vec2 vUV;

const float w[7] = float[](0.006,   0.061,   0.242, 0.382, 0.242,  0.061, 0.006 );
const float o[7] = float[](-1.0, -0.6667, -0.3333, 0, 0.3333, 0.6667,   1.0 );

///////////////AUX COLOR TRANSFORMS///////////////
vec3 rgb2xyz(vec3 rgb) {
    mat3 m = mat3(	0.5141364, 0.3238786,  0.16036376,
					0.265068,  0.67023428, 0.06409157,
					0.0241188, 0.1228178,  0.84442666 );
    return /*1/0.1228178*/(m * rgb);
}

float labFordwardFunction(float t)
{
	const float val = (6/29)*(6/29)*(6/29);
	if(t > val) return pow(t, 1/3);
	return 1/3*(29/6)*(29/6)*t + 4/29;
}

vec3 xyz2lab(vec3 xyz)
{
	const float xn = 0.95047;
	const float yn = 1;
	const float zn = 1.08883;
	vec3 lab;
	lab.x = 116*labFordwardFunction(xyz.y/yn) - 16;
	lab.y = 500*(labFordwardFunction(xyz.x/xn) - labFordwardFunction(xyz.y/yn));
	lab.z = 200*(labFordwardFunction(xyz.y/yn) - labFordwardFunction(xyz.z/zn));
	return lab;
}
//////////////////////////////////////////////////

//#define ORIGINAL_FILTER
//#define SIMPLE_COL_DIST_FILTER
#define SIMPLE_BILATERAL_FILTER
vec4 BlurSSSSPas(float sssStrength, float gauss_size, vec2 pixel_size, vec2 dir, float correction, vec2 vUV, sampler2D color_texture, sampler2D depth_texture)
{
    vec2 step = sssStrength * gauss_size * pixel_size * dir;
    vec3 colorM = texture2D(color_texture, vUV).rgb;
    float depthM = texture2D(depth_texture, vUV).r;
    vec2 finalStep = /*colorM.a */ step / depthM;
	finalStep/= 3;
	finalStep = step;


	vec3 colorBlurred = vec3(0);//colorM;
    //colorBlurred.rgb *= 0.382;
	float weigths = 0;
    for (int i = 0; i < 7; i++)
    {
        vec2 offset = vUV + o[i] * finalStep;
        vec3 colorS = texture2D(color_texture, offset).rgb;
        float depth = texture2D(depth_texture, offset).r;
        float s = min(correction * abs(depthM - depth), 1.0);
        vec3 color = mix(colorS, colorM, s);

		float weight;
		#ifdef ORIGINAL_FILTER
			weight = w[i];
		#endif

		#ifdef SIMPLE_COL_DIST_FILTER
			//weight = w[i]*exp(-distance(xyz2lab(rgb2xyz(colorM)), xyz2lab(rgb2xyz(colorS))));
			weight = w[i]*exp(-distance(colorM, colorS));
		#endif 
		
		#ifdef SIMPLE_BILATERAL_FILTER
			//weight = w[i]*exp(-10*distance(xyz2lab(rgb2xyz(colorM)), xyz2lab(rgb2xyz(colorS))))*exp(-abs(offset));
			weight = w[i]*exp(-10*distance(colorM, colorS))*exp(-abs(offset));
		#endif

        colorBlurred += weight * color;
		weigths += weight;
    }
    return vec4(colorBlurred/vec3(weigths), 1);
}

void main(void)
{
        float gauss_size = sqrt(gaussian.x);
        vFColor = BlurSSSSPas(sssStrength, gauss_size, pixel_size, dir, correction, vUV, color_texture, lineal_depth_texture);
 }