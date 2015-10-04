#version 330
//precision highp float;
layout(location=0) out vec4 vFColor;
layout(location=1) out vec4 vBlur;

uniform sampler2D color_texture;
uniform sampler2D lineal_depth_texture;
uniform sampler2D pinpong_texture;

uniform float cam_fovy;
uniform float correction = 800;
uniform float sssWidth = 15.75;
uniform vec2 pixel_size;
uniform vec4 gaussian;

smooth in vec2 vUV;


uniform int ssss_n_samples = 25;
uniform vec2 kernel[100];

uniform sampler2D cross_bilateral_factor;
uniform sampler2D curvature_texture;

#define saturate(a) clamp(a, 0.0, 1.0)
///////////////AUX COLOR TRANSFORMS///////////////
vec3 rgb2lab(in vec3 rgb){
    // threshold
    float T = 0.008856;

    float X = rgb.r * 0.412453 + rgb.g * 0.357580 + rgb.b * 0.180423;
    float Y = rgb.r * 0.212671 + rgb.g * 0.715160 + rgb.b * 0.072169;
    float Z = rgb.r * 0.019334 + rgb.g * 0.119193 + rgb.b * 0.950227;

    // Normalize for white point
    X = X / 0.950456; Y = Y; Z = Z / 1.088754;
    bool XT, YT, ZT;
    XT = false; YT=false; ZT=false;
    if(X > T) XT = true; if(Y > T) YT = true; if(Z > T) ZT = true;

    float Y3 = pow(Y,1.0/3.0);
    float fX, fY, fZ;
    if(XT){ fX = pow(X, 1.0/3.0);} else{ fX = 7.787 * X + 16.0/116.0; }
    if(YT){ fY = Y3; } else{ fY = 7.787 * Y + 16.0/116.0 ; }
    if(ZT){ fZ = pow(Z,1.0/3.0); } else{ fZ = 7.787 * Z + 16.0/116.0; }

    float L; if(YT){ L = (116.0 * Y3) - 16.0; }else { L = 903.3 * Y; }
    float a = 500.0 * ( fX - fY );
    float b = 200.0 * ( fY - fZ );

    return vec3(L,a,b);
}

///FIXME
vec3 rgb2luv(vec3 rgb)
{
    // threshold
    float T = 0.008856;

    float X = rgb.r * 0.412453 + rgb.g * 0.357580 + rgb.b * 0.180423;
    float Y = rgb.r * 0.212671 + rgb.g * 0.715160 + rgb.b * 0.072169;
    float Z = rgb.r * 0.019334 + rgb.g * 0.119193 + rgb.b * 0.950227;


	float var_U = ( 4 * X ) / ( X + ( 15 * Y ) + ( 3 * Z ) );
	float var_V = ( 9 * Y ) / ( X + ( 15 * Y ) + ( 3 * Z ) );

	float var_Y = Y / 100;
	if ( var_Y > T ) var_Y = pow(var_Y,1.0/3.0);
	else var_Y = ( 7.787 * var_Y ) + ( 16 / 116.0 );

	float ref_X =  95.047;     
	float ref_Y = 100.000;
	float ref_Z = 108.883;

	float ref_U = ( 4 * ref_X ) / ( ref_X + ( 15 * ref_Y ) + ( 3 * ref_Z ) );
	float ref_V = ( 9 * ref_Y ) / ( ref_X + ( 15 * ref_Y ) + ( 3 * ref_Z ) );

	float L = ( 116 * var_Y ) - 16;
	float u = 13 * L * ( var_U - ref_U );
	float v = 13 * L * ( var_V - ref_V );
	return vec3(L, u, v);
}

float rgb2gray(vec3 rgb)
{
	return (rgb.r + rgb.g + rgb.b)/3.0;
}
//////////////////////////////////////////////////

//Filtering
#define ORIGINAL_FILTER
//#define SIMPLE_COL_DIST_FILTER
//#define SIMPLE_BILATERAL_FILTER
//#define CROSS_BILATERAL_FILTER
//#define BILATERAL_ON_CURV

///Strengh factor
#define STREGTH_CURVATURE

#ifdef STREGTH_CURVATURE
	#define SSSS_STREGTH_SOURCE 1.0+texture(curvature_texture, vUV).r
#else
	#define SSSS_STREGTH_SOURCE 1.0 
	//#define SSSS_STREGTH_SOURCE (colorM.a)
#endif

//vec4 BlurSSSSPas(vec2 texcoord, sampler2D colorTex, sampler2D depthTex,  float sssWidth,  vec2 dir, float fovy
vec4 BlurSSSSPas(float sssWidth, float gauss_size, vec2 pixel_size, vec2 dir, float correction, vec2 vUV, sampler2D color_texture, sampler2D depth_texture, float fovy)
{
    //vec2 step = sssWidth * gauss_size * pixel_size * dir;
    //vec3 colorM = texture2D(color_texture, vUV).rgb;
    //float depthM = texture2D(depth_texture, vUV).r;
    //vec2 finalStep = /*colorM.a */ step / depthM;
	//finalStep/= 3;
	//finalStep = step;

	// Fetch linear depth of current pixel:
    float depthM = texture(depth_texture, vUV).r;

	// Calculate the sssWidth scale (1.0 for a unit plane sitting on the
    // projection window):
	float distanceToProjectionWindow = 1.0 / tan(0.5 * fovy);//fovy in rads
    float scale = distanceToProjectionWindow / depthM;

	// Calculate the final step to fetch the surrounding pixels:
	sssWidth *= 0.1;
    vec2 finalStep = gauss_size*sssWidth * scale * dir;
    finalStep *= SSSS_STREGTH_SOURCE;
    finalStep *= 1.0 / 3.0;

    vec3 colorM = texture2D(color_texture, vUV).rgb;
	#ifdef CROSS_BILATERAL_FILTER
		float I_p = rgb2gray(colorM)*texture(cross_bilateral_factor, vUV).r;
	#endif

	vec3 colorBlurred = vec3(0);//colorM;
    //colorBlurred.rgb *= 0.382;
	float weigths = 0;
    for (int i = 0; i < ssss_n_samples; i++)
    {
		vec2 despl = kernel[i].g * finalStep;
        vec2 offset = vUV + despl;
        vec3 colorS = texture2D(color_texture, offset).rgb;
        float depth = texture2D(depth_texture, offset).r;
        float s = 0;
		

		#ifndef CROSS_BILATERAL_FILTER
			s = min(correction * abs(depthM - depth), 1.0);
			//s = saturate(correction * distanceToProjectionWindow * sssWidth * abs(depthM - depth));
			colorS = mix(colorS, colorM, s);
		#endif


		float weight;
		#ifdef ORIGINAL_FILTER
			weight = kernel[i].r;
		#endif

		#ifdef SIMPLE_COL_DIST_FILTER
			//weight = w[i]*exp(-distance(rgb2luv(colorM), rgb2luv(colorS)));
			weight = kernel[i].r*exp(-0.5*distance(rgb2lab(colorM), rgb2lab(colorS)));
			//weight = w[i]*exp(-distance(colorM, colorS));
		#endif 
		
		#ifdef SIMPLE_BILATERAL_FILTER
			weight = kernel[i].r*exp(-0.5*distance(rgb2lab(colorM), rgb2lab(colorS)))*exp(-abs(length(despl)));
			//weight = w[i]*exp(-10*distance(colorM, colorS))*exp(-abs(length(offset)));
		#endif

		#ifdef CROSS_BILATERAL_FILTER
			weight = kernel[i].r*exp(-10*distance(I_p, rgb2gray(colorS)*texture(cross_bilateral_factor, offset).r))*exp(-abs(length(despl)));
			//weight = w[i]*exp(-10*distance(colorM, colorS))*exp(-abs(length(offset)));
		#endif

		#ifdef BILATERAL_ON_CURV
			weight = kernel[i].r*exp(-10*distance(texture(curvature_texture, vUV).r, texture(curvature_texture, offset).r))*exp(-abs(length(despl)));
		#endif

        colorBlurred += weight * colorS;
		weigths += weight;
    }
    return vec4(colorBlurred/vec3(weigths), 1);
}

void main(void)
{
        float gauss_size = sqrt(gaussian.x);
        vFColor = BlurSSSSPas(sssWidth, gauss_size, pixel_size, vec2(0, 1), correction, vUV, color_texture, lineal_depth_texture, cam_fovy);

		vec4 src_col = vFColor;
		vec3 dest_col = texture2D(pinpong_texture, vUV).rgb;
		vBlur = vec4(src_col.r*gaussian.g + dest_col.r*(1.0-gaussian.g),
                    src_col.g*gaussian.b + dest_col.g*(1.0-gaussian.b),
                    src_col.b*gaussian.a + dest_col.b*(1.0-gaussian.a), 1);
		//vBlur = vec4(src_col.r*gaussian.g + dest_col.r, src_col.g*gaussian.b + dest_col.g, src_col.b*gaussian.a + dest_col.b, 1);
		//vBlur = vec4(src_col.rgb*gaussian.gba, 1);

		//vBlur = texture2D(pinpong_texture, vUV).rgba;
		//vBlur = vFColor; 
		//vBlur = BlurSSSSPas;
		//vBlur = vec4(1, 0, 0, 1);
		//vBlur = vec4(vec3(texture2D(cross_bilateral_factor, vUV).r), 1);
 }