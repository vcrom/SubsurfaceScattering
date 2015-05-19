#version 330

layout(location = 0) out vec4 vFragColor;

smooth in vec3 Normal;
void main()
{
    float col = (dot(Normal, vec3(0, 1, 0)) + 1)/2;
    vFragColor = vec4(col,col, col, 1);
	//vFragColor = vec4(1,0,0, 1);
}
