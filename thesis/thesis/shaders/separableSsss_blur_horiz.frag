#version 330
precision highp float;

/**
 * Here you have ready-to-use kernels for quickstarters. Three kernels are 
 * readily available, with varying quality.
 * To create new kernels take a look into SSS::calculateKernel, or simply
 * push CTRL+C in the demo to copy the customized kernel into the clipboard.
 *
 * Note: these preset kernels are not used by the demo. They are calculated on
 * the fly depending on the selected values in the interface, by directly using
 * SSS::calculateKernel.
 *
 * Quality ranges from 0 to 2, being 2 the highest quality available.
 * The quality is with respect to 1080p; for 720p Quality=0 suffices.
 */
#define SSSS_QUALITY 2


//const float w[6] = float[](0.006,   0.061,   0.242,  0.242,  0.061, 0.006 );

#if SSSS_QUALITY == 2
uniform int ssss_n_samples = 25;
uniform vec4 kernel[100];/* = vec4[](
    vec4(0.530605, 0.613514, 0.739601, 0),
    vec4(0.000973794, 1.11862e-005, 9.43437e-007, -3),
    vec4(0.00333804, 7.85443e-005, 1.2945e-005, -2.52083),
    vec4(0.00500364, 0.00020094, 5.28848e-005, -2.08333),
    vec4(0.00700976, 0.00049366, 0.000151938, -1.6875),
    vec4(0.0094389, 0.00139119, 0.000416598, -1.33333),
    vec4(0.0128496, 0.00356329, 0.00132016, -1.02083),
    vec4(0.017924, 0.00711691, 0.00347194, -0.75),
    vec4(0.0263642, 0.0119715, 0.00684598, -0.520833),
    vec4(0.0410172, 0.0199899, 0.0118481, -0.333333),
    vec4(0.0493588, 0.0367726, 0.0219485, -0.1875),
    vec4(0.0402784, 0.0657244, 0.04631, -0.0833333),
    vec4(0.0211412, 0.0459286, 0.0378196, -0.0208333),
    vec4(0.0211412, 0.0459286, 0.0378196, 0.0208333),
    vec4(0.0402784, 0.0657244, 0.04631, 0.0833333),
    vec4(0.0493588, 0.0367726, 0.0219485, 0.1875),
    vec4(0.0410172, 0.0199899, 0.0118481, 0.333333),
    vec4(0.0263642, 0.0119715, 0.00684598, 0.520833),
    vec4(0.017924, 0.00711691, 0.00347194, 0.75),
    vec4(0.0128496, 0.00356329, 0.00132016, 1.02083),
    vec4(0.0094389, 0.00139119, 0.000416598, 1.33333),
    vec4(0.00700976, 0.00049366, 0.000151938, 1.6875),
    vec4(0.00500364, 0.00020094, 5.28848e-005, 2.08333),
    vec4(0.00333804, 7.85443e-005, 1.2945e-005, 2.52083),
    vec4(0.000973794, 1.11862e-005, 9.43437e-007, 3)
);*/

#elif SSSS_QUALITY == 1
uniform int ssss_n_samples = 17;
uniform vec4 kernel[] = vec4[](
    vec4(0.536343, 0.624624, 0.748867, 0),
    vec4(0.00317394, 0.000134823, 3.77269e-005, -2),
    vec4(0.0100386, 0.000914679, 0.000275702, -1.53125),
    vec4(0.0144609, 0.00317269, 0.00106399, -1.125),
    vec4(0.0216301, 0.00794618, 0.00376991, -0.78125),
    vec4(0.0347317, 0.0151085, 0.00871983, -0.5),
    vec4(0.0571056, 0.0287432, 0.0172844, -0.28125),
    vec4(0.0582416, 0.0659959, 0.0411329, -0.125),
    vec4(0.0324462, 0.0656718, 0.0532821, -0.03125),
    vec4(0.0324462, 0.0656718, 0.0532821, 0.03125),
    vec4(0.0582416, 0.0659959, 0.0411329, 0.125),
    vec4(0.0571056, 0.0287432, 0.0172844, 0.28125),
    vec4(0.0347317, 0.0151085, 0.00871983, 0.5),
    vec4(0.0216301, 0.00794618, 0.00376991, 0.78125),
    vec4(0.0144609, 0.00317269, 0.00106399, 1.125),
    vec4(0.0100386, 0.000914679, 0.000275702, 1.53125),
    vec4(0.00317394, 0.000134823, 3.77269e-005, 2)
);

#elif SSSS_QUALITY == 0
uniform int ssss_n_samples = 11;
uniform vec4 kernel[] = vec4[](
    vec4(0.560479, 0.669086, 0.784728, 0),
    vec4(0.00471691, 0.000184771, 5.07566e-005, -2),
    vec4(0.0192831, 0.00282018, 0.00084214, -1.28),
    vec4(0.03639, 0.0130999, 0.00643685, -0.72),
    vec4(0.0821904, 0.0358608, 0.0209261, -0.32),
    vec4(0.0771802, 0.113491, 0.0793803, -0.08),
    vec4(0.0771802, 0.113491, 0.0793803, 0.08),
    vec4(0.0821904, 0.0358608, 0.0209261, 0.32),
    vec4(0.03639, 0.0130999, 0.00643685, 0.72),
    vec4(0.0192831, 0.00282018, 0.00084214, 1.28),
    vec4(0.00471691, 0.000184771, 5.07565e-005, 2)
);
#endif
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

///Stngh factor
//#define STREGTH_CURVATURE

#ifdef STREGTH_CURVATURE
	#define SSSS_STREGTH_SOURCE 0.3+1.5*texture(curvature_texture, vUV).r
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
		//vec4 color = colorS;
		if(colorS.rgb == vec3(0)) continue;

		if(follow_surf)
		{
			// If the difference in depth is huge, we lerp color back to "colorM":
			float depth = texture(depthTex, offset).r;
			float s = 0;
			//#ifndef CROSS_BILATERAL_FILTER
				//s = saturate(correction * distanceToProjectionWindow * sssWidth * abs(depthM - depth));
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
			weight = kernel[i].rgb*exp(-50*distance(I_p, rgb2gray(colorS.rgb)*texture(cross_bilateral_factor, offset).r))*exp(-abs(length(despl)));
			//weight = kernel[i].rgb*exp(-10*distance(colorM, colorS))*exp(-abs(length(offset)));
		#endif

		#ifdef BILATERAL_ON_CURV
			weight = kernel[i].rgb*exp(-5*distance(rgb2lab(colorM.rgb)*texture(curvature_texture, vUV).r, rgb2lab(colorS.rgb)*texture(curvature_texture, offset).r))*exp(-abs(length(despl)));
		#endif

		// Accumulate:
		colorBlurred.rgb += weight * colorS.rgb;
		weigths += weight;
	}
	colorBlurred.rgb /= weigths;
	return colorBlurred;

}

void main()
{
    vFragColor = SSSSBlurPS(vUV, color_texture, lineal_depth_texture, sssWidth,  vec2(0, 1), cam_fovy, true);
	
	//vFragColor = vec4(texture(lineal_depth_texture, vUV).r);
}
