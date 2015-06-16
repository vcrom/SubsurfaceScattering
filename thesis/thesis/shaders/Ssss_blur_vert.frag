#version 330
precision highp float;
layout(location=0) out vec4 vFColor;
layout(location=1) out vec4 vBlur;

uniform sampler2D color_texture;
uniform sampler2D lineal_depth_texture;
uniform sampler2D pinpong_texture;

uniform float correction = 800;
uniform float sssStrength = 15.75;
uniform vec2 pixel_size;
uniform vec4 gaussian;
const vec2 dir = vec2(0, 1);

smooth in vec2 vUV;


const float w[6] = float[](0.006,   0.061,   0.242,  0.242,  0.061, 0.006 );
const float o[6] = float[](-1.0, -0.6667, -0.3333, 0.3333, 0.6667,   1.0 );

vec4 BlurSSSSPas(float sssStrength, float gauss_size, vec2 pixel_size, vec2 dir, float correction, vec2 vUV, sampler2D color_texture, sampler2D depth_texture)
{
    vec2 step = sssStrength * gauss_size * pixel_size * dir;
    vec4 colorM = texture2D(color_texture, vUV).rgba;
    float depthM = texture2D(depth_texture, vUV).r;
    vec4 colorBlurred = colorM;
    colorBlurred.rgb *= 0.382;
    vec2 finalStep = colorM.a * step / depthM;
	finalStep/= 3;
	finalStep = step;
    for (int i = 0; i < 6; i++)
    {
        vec2 offset = vUV + o[i] * finalStep;
        vec3 color = texture2D(color_texture, offset).rgb;
        float depth = texture2D(depth_texture, offset).r;
        float s = min(0.0125 * correction * abs(depthM - depth), 1.0);
		//s = 1;
        color = mix(color, colorM.rgb, s);
        colorBlurred.rgb += w[i] * color;
    }
    return colorBlurred;
}

void main(void)
{
        float gauss_size = sqrt(gaussian.x);
        vFColor = BlurSSSSPas(sssStrength, gauss_size, pixel_size, dir, correction, vUV, color_texture, lineal_depth_texture);

		vec4 src_col = vFColor;
		vec3 dest_col = texture2D(pinpong_texture, vUV).rgb;
		vBlur = vec4(src_col.r*gaussian.g + dest_col.r*(1-gaussian.g),
                    src_col.g*gaussian.b + dest_col.g*(1-gaussian.b),
                    src_col.b*gaussian.a + dest_col.b*(1-gaussian.a), 1);

		//vBlur = texture2D(pinpong_texture, vUV).rgba;
		//vBlur = vFColor; 
		//vBlur = BlurSSSSPas;
		//vBlur = vec4(1, 0, 0, 1);
		//vBlur = vec4(vec3(texture2D(lineal_depth_texture, vUV).r), 1);
 }