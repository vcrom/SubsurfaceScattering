#version 330

layout(location = 0) out vec4 vFragColor;

smooth in float linear_depth;
void main()
{
    vFragColor = vec4(linear_depth, 1);
}
