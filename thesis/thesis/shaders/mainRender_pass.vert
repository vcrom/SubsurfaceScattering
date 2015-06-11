#version 330
  
layout(location = 0) in vec3 vVertex;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec4 vColor;


//uniforms
uniform mat4 MVP;  

smooth out vec3 Normal;
smooth out vec4 Color;

void main()
{  
	Color = vColor;
    gl_Position = MVP*vec4(vVertex.xyz,1);
    Normal = vNormal;
}
