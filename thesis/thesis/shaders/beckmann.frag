#version 330 core

uniform sampler2D color_texture;

smooth in vec2 vUV;

layout(location=0) out vec4 vFColor;

void main(void)
{

	vec4 color = texture(color_texture, vUV).rgba;
	vFColor = color;

}
http://http.developer.nvidia.com/GPUGems3/gpugems3_ch14.html