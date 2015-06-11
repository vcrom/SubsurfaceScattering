#version 330

layout(location = 0) out vec4 vFragColor;

smooth in vec3 Normal;
smooth in vec4 Color;

void main()
{
    float col = (dot(Normal, vec3(0,1,0)) + 1)/2;
	vFragColor = vec4(Color*col);
	//vFragColor = vec4(1,0, 1, 1);
}
