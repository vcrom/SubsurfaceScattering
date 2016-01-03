#version 330 core

smooth in vec2 vUV;

layout(location=0) out float vFColor;

float PHBeckmann( float ndoth, float m )   
{   
	float alpha = acos( ndoth );   
	float ta = tan( alpha );   
	float val = 1.0/(m*m*pow(ndoth,4.0))*exp(-(ta*ta)/(m*m));   
	return val;   
}  

// Render a screen-aligned quad to precompute a 512x512 texture.   
float KSTextureCompute(vec2 tex)   
{   
	// Scale the value to fit within [0,1] – invert upon lookup.   
	return 0.5 * pow( PHBeckmann( tex.x, tex.y ), 0.1 );   
} 

void main(void)
{

	vFColor = KSTextureCompute(vUV);

}
http://http.developer.nvidia.com/GPUGems3/gpugems3_ch14.html


