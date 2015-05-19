#version 330
  
layout(location = 0) in vec3 vVertex;
layout(location = 1) in vec3 vNormal;

uniform mat4 MVP;  

smooth out vec3 Normal;
void main()
{  
    gl_Position = MVP*vec4(vVertex.xyz,1);
    Normal = vNormal;
	//
}
