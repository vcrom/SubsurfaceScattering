#version 330
precision highp float;



//const float w[6] = float[](0.006,   0.061,   0.242,  0.242,  0.061, 0.006 );

uniform int ssss_n_samples = 25;
uniform vec4 kernel[100];
#define saturate(a) clamp(a, 0.0, 1.0)

layout(location = 0) out vec4 vFragColor;

uniform sampler2D color_texture;
uniform sampler2D lineal_depth_texture;
uniform float sssWidth;
uniform float cam_fovy;

uniform float kernel_range = 3.0f;
uniform float correction = 6000;
smooth in vec2 vUV;

uniform sampler2D cross_bilateral_factor;
uniform sampler2D curvature_texture;

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

///Str factor
//#define STREGTH_CURVATURE

#ifdef STREGTH_CURVATURE
	#define SSSS_STREGTH_SOURCE 0.3+1.5*abs(texture(curvature_texture, vUV).r)
	//#define SSSS_STREGTH_SOURCE 1+1.5*abs(texture(curvature_texture, vUV).r)
	//#define SSSS_STREGTH_SOURCE min(abs(texture(curvature_texture, vUV).r), 1)
#else
	#define SSSS_STREGTH_SOURCE 1.0 
	//#define SSSS_STREGTH_SOURCE (colorM.a)
#endif

vec4 SSSSBlurPS(vec2 texcoord, sampler2D colorTex, sampler2D depthTex,  float sssWidth,  vec2 dir, float fovy, bool follow_surf)
{
	// Fetch color of current pixel:
    vec4 colorM = texture(colorTex, texcoord).rgba;
	#ifdef CROSS_BILATERAL_FILTER
		float I_p = rgb2gray(colorM.rgb)*texture(cross_bilateral_factor, texcoord).r;
	#endif

	if (SSSS_STREGTH_SOURCE == 0.0) discard;

	// Fetch linear depth of current pixel:
    float depthM = texture(depthTex, texcoord).r;

	// Calculate the sssWidth scale (1.0 for a unit plane sitting on the
    // projection window):
	float distanceToProjectionWindow = 1.0 / tan(0.5 * fovy);//fovy in rads
    float scale = distanceToProjectionWindow / depthM;

    // Calculate the final step to fetch the surrounding pixels:
    vec2 finalStep = sssWidth * scale * dir * 0.7;
    finalStep *= SSSS_STREGTH_SOURCE; // Modulate it using the alpha channel.
    finalStep *= 1.0 / kernel_range;//(3.0*sssWidth);//3.0; // Divide by 3 as the kernels range from -3 to 3.

	// Accumulate the center sample:
    vec4 colorBlurred = vec4(0);//colorM;
    //colorBlurred.rgb *= kernel[0].rgb;
	vec3 weigths = vec3(0);//kernel[0].rgb;

	for (int i = 0; i < ssss_n_samples; i++) {
		// Fetch color and depth for current sample:
		vec2 despl = kernel[i].a * finalStep;
		vec2 offset = texcoord + despl;
		vec4 colorS = texture2D(color_texture, offset).rgba;
		if(colorS.rgb == vec3(0)) continue;
		//vec4 color = colorS;

		if(follow_surf)
		{
			// If the difference in depth is huge, we lerp color back to "colorM":
			float depth = texture(depthTex, offset).r;
			float s = 0;
			//#ifndef CROSS_BILATERAL_FILTER
				s = saturate(correction * distanceToProjectionWindow * sssWidth * abs(depthM - depth));
			//#endif
			colorS.rgb = mix(colorS.rgb, colorM.rgb, s);
		}

		vec3 weight;
		#ifdef ORIGINAL_FILTER
			weight = kernel[i].rgb;
		#endif

		#ifdef SIMPLE_COL_DIST_FILTER
			//weight = kernel[i].rgb*exp(-distance(rgb2luv(colorM), rgb2luv(colorS)));
			weight = kernel[i].rgb*exp(-0.1*distance(rgb2lab(colorM.rgb), rgb2lab(colorS.rgb)));
			//weight = kernel[i].rgb*exp(-distance(colorM.rgb, colorS.rgb));
		#endif 
		
		#ifdef SIMPLE_BILATERAL_FILTER
			weight = kernel[i].rgb*exp(-/*0.025*/ 0.09*distance(rgb2lab(colorM.rgb), rgb2lab(colorS.rgb)))*exp(-abs(length(despl)));
			//weight = kernel[i].rgb*exp(-10*distance(colorM, colorS))*exp(-abs(length(offset)));
		#endif

		#ifdef CROSS_BILATERAL_FILTER
			weight = kernel[i].rgb*exp(-10*distance(I_p, rgb2gray(colorS.rgb)*texture(cross_bilateral_factor, offset).r))*exp(-abs(length(despl)));
			//weight = kernel[i].rgb*exp(-10*distance(colorM, colorS))*exp(-abs(length(offset)));
		#endif

		#ifdef BILATERAL_ON_CURV
			weight = kernel[i].rgb*exp(-5*distance(texture(curvature_texture, vUV).r, texture(curvature_texture, offset).r))*exp(-abs(length(despl)));
		#endif

		// Accumulate:
		colorBlurred.rgb += weight * colorS.rgb;
		weigths += weight;
	}
	colorBlurred.rgb /= weigths;
	//if(colorBlurred.rgb == vec3(0) || weigths == 0) colorBlurred.rgb = texture(color_texture, texcoord).rgb;
	return colorBlurred;

}

void main()
{
    vFragColor = SSSSBlurPS(vUV, color_texture, lineal_depth_texture, sssWidth,  vec2(1, 0), cam_fovy, true);
	//vFragColor = vec4(vec3(texture(curvature_texture, vUV).r), 1);
	//vFragColor = vec4(vec3(texture(cross_bilateral_factor, vUV).r), 1);
	
	//vFragColor = vec4(texture(lineal_depth_texture, vUV).r);
	//vFragColor = vec4(1, 0, 0, 1);
}
