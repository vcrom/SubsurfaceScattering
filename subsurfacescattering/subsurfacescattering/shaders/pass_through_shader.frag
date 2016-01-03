#version 330

layout(location = 0) out vec4 vFragColor;

uniform vec3 color = vec3(1, 1, 1);

smooth in vec3 Normal;
void main()
{
    float col = (dot(Normal, vec3(0, 1, 0)) + 1)/2;
    vFragColor = vec4(vec3(1, 1, 1)*col, 1);
	//vFragColor = vec4(1,0,0, 1);
}
