#version 330
layout(location = 0) out vec4 pixelColor;

vec3 rgb2hsv(vec3 rgb) {
    float minValue = min(min(rgb.r, rgb.g), rgb.b);
    float maxValue = max(max(rgb.r, rgb.g), rgb.b);
    float d = maxValue - minValue;

    vec3 hsv = vec3(0.0);
    hsv.b = maxValue;
    if (d != 0) {
        hsv.g = d / maxValue;

        vec3 delrgb = (((vec3(maxValue) - rgb) / 6.0) + d / 2.0) / d;
        if      (maxValue == rgb.r) { hsv.r = delrgb.b - delrgb.g; }
        else if (maxValue == rgb.g) { hsv.r = 1.0 / 3.0 + delrgb.r - delrgb.b; }
        else if (maxValue == rgb.b) { hsv.r = 2.0 / 3.0 + delrgb.g - delrgb.r; }

        if (hsv.r < 0.0) { hsv.r += 1.0; }
        if (hsv.r > 1.0) { hsv.r -= 1.0; }
    }
    return hsv;
}

vec3 hsv2rgb(vec3 hsv) {
    float h = hsv.r, s = hsv.g, v = hsv.b;

    vec3 rgb = vec3(v);
    if (hsv.g != 0.0) {
        float h_i = floor(6 * h);
        float f = 6 * h - h_i;

        float p = v * (1.0 - s);
        float q = v * (1.0 - f * s);
        float t = v * (1.0 - (1.0 - f) * s);

        if      (h_i == 0) { rgb = vec3(v, t, p); }
        else if (h_i == 1) { rgb = vec3(q, v, p); }
        else if (h_i == 2) { rgb = vec3(p, v, t); }
        else if (h_i == 3) { rgb = vec3(p, q, v); }
        else if (h_i == 4) { rgb = vec3(t, p, v); }
        else               { rgb = vec3(v, p, q); }
    }
    return rgb;
}

vec3 rgb2xyz(vec3 rgb) {
    mat3 m = mat3(	0.5141364, 0.3238786,  0.16036376,
					0.265068,  0.67023428, 0.06409157,
					0.0241188, 0.1228178,  0.84442666 );
    return m * rgb;
	//vec3 xyz;
	//float xyz.x = rgb.r * 0.412453 + rgb.g * 0.357580 + rgb.b * 0.180423;
 //   float xyz.y = rgb.r * 0.212671 + rgb.g * 0.715160 + rgb.b * 0.072169;
 //   float xyz.z = rgb.r * 0.019334 + rgb.g * 0.119193 + rgb.b * 0.950227;
	//return xyz;
}


vec3 xyz2rgb(vec3 xyz) {
    mat3 m  = mat3( 2.5651, -1.1665, -0.3986,
                    -1.0217,  1.9777,  0.0439, 
                    0.0753, -0.2543,  1.1892 );
    return m * xyz;
}

vec3 xyz2Yxy(vec3 xyz) {
    float w = xyz.r + xyz.g + xyz.b;
    if (w > 0.0) {
        vec3 Yxy;
        Yxy.r = xyz.g;
        Yxy.g = xyz.r / w;
        Yxy.b = xyz.g / w;
        return Yxy;
    } else {
        return vec3(0.0);
    }
}

vec3 Yxy2xyz(vec3 Yxy) {
    vec3 xyz;
    xyz.g = Yxy.r;
    if (Yxy.b > 0.0) {
        xyz.r = Yxy.r * Yxy.g / Yxy.b;
        xyz.b = Yxy.r * (1 - Yxy.g - Yxy.b) / Yxy.b;
    } else {
        xyz.rb = vec2(0.0);
    }
    return xyz;
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

float labReverseFunction(float t)
{
	if(t > 6/29) return t*t*t;
	return 3*(6/29)*(6/29)*(t - 4/29);
}

vec3 lab2xyz(vec3 lab)
{
	const float xn = 0.95047;
	const float yn = 1;
	const float zn = 1.08883;
	vec3 xyz;
	xyz.x = xn*labReverseFunction(1/116*(lab.x + 16) + 1/500*lab.y);
	xyz.y = yn*labReverseFunction(1/116*(lab.x + 16));
	xyz.z = zn*labReverseFunction(1/116*(lab.x + 16) - 1/200*lab.z);
	return xyz;
}

vec3 FilmicTonemap(vec3 x) {
    float A = 0.15;
    float B = 0.50;
    float C = 0.10;
    float D = 0.20;
    float E = 0.02;
    float F = 0.30;
    float W = 11.2;
    return ((x*(A*x+C*B)+D*E) / (x*(A*x+B)+D*F))- E / F;
}

//#ifndef TONEMAP_OPERATOR
//	#define TONEMAP_OPERATOR 4
//#endif
#define GAMMA_CORR int(0)
#define TONEMAP_LINEAR int(1)
#define TONEMAP_EXPONENTIAL int(2)
#define TONEMAP_EXPONENTIAL_HSV int(3)
#define TONEMAP_REINHARD int(4)
#define TONEMAP_FILMIC int(5)


vec3 doToneMap(vec3 color, int option, float exposure, float burnout) {
//return color;
	if(option == GAMMA_CORR)
	{
		color = pow(color, vec3(1/2.2));
		return color;
	}
    else if(option == TONEMAP_LINEAR)
	{
		return exposure * color;
	}
	else if( option == TONEMAP_EXPONENTIAL)
	{
		color = 1.0 - exp2(-exposure * color);
		return color;
	}
	else if( option == TONEMAP_EXPONENTIAL_HSV)
	{
		color = rgb2hsv(color);
		color.b = 1.0 - exp2(-exposure * color.b);
		color = hsv2rgb(color);
		return color;
	}
	else if( option == TONEMAP_REINHARD)
	{
		color = xyz2Yxy(rgb2xyz(color));
		float L = color.r;
		L *= exposure;
		float LL = 1 + L / (burnout * burnout);
		float L_d = L * LL / (1 + L);
		color.r = L_d;
		color = xyz2rgb(Yxy2xyz(color));
		return color;
	}
	else if( option == TONEMAP_FILMIC)
	{
		color = 2.0f * FilmicTonemap(exposure * color);
		vec3 whiteScale = vec3(1.0f) / FilmicTonemap(vec3(11.2));
		color *= whiteScale;
		return color;
	}
}


uniform sampler2D color_texture;
uniform float exposure = 2;
uniform float m_burnout;
uniform int method;

smooth in vec2 vUV;

void main()
{
	vec4 color = texture(color_texture, vUV);
	color.rgb = doToneMap(color.rgb, method, exposure, m_burnout);
	pixelColor = color;
}
