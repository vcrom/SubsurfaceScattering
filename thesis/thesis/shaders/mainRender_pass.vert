#version 330
  
layout(location = 0) in vec3 vVertex;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec4 vColor;


//uniforms
uniform mat4 curr_WorldViewProjM;
uniform mat4 prev_WorldViewProjM;
uniform mat4 worldInverseTransposeM; //Normal matrix
uniform mat4 worldM;
uniform mat4 viewM;
uniform vec3 m_camera_pos;
uniform float z_far;

uniform float ambient;

smooth out vec4 mesh_color;
smooth out float linear_depth;
smooth out vec3 worldPosition;
smooth out vec3 worldNormal;
smooth out vec3 prev_vPosition;
smooth out vec3 curr_vPosition;
smooth out vec3 view_vector;

void main()
{  
	//position
    gl_Position = curr_WorldViewProjM*vec4(vVertex.xyz,1);
	curr_vPosition = vec4(curr_WorldViewProjM*vec4(vVertex.xyz,1)).xyw;
	prev_vPosition = vec4(prev_WorldViewProjM*vec4(vVertex.xyz,1)).xyw;
	curr_vPosition.xy /= 2.0;
    prev_vPosition.xy /= 2.0;
	// Texture coordinates have a top-to-bottom y axis, so flip this axis:
    curr_vPosition.y = -curr_vPosition.y;
    prev_vPosition.y = -prev_vPosition.y;

	//shading variables
	vec4 world_pos = worldM*vec4(vVertex, 1);
	worldPosition = world_pos.xyz;
	view_vector = m_camera_pos - worldPosition;
	worldNormal = vec4(worldInverseTransposeM*vec4(vNormal, 1)).xyz;

	//color
	mesh_color = vColor;

	//lineal depth
	vec4 view_pos = viewM * world_pos;
	//linear depth 0..1
	linear_depth = -(view_pos.z / view_pos.w) / z_far;
}
