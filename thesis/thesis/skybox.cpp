#include "skybox.h"
#include "texture.h"
#include "textureloader.h"

//glm
#define GLM_FORCE_PURE
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

CSkybox::CSkybox(std::string sky_textures_dir_path)
{ 
	shader.loadFromFile(GL_VERTEX_SHADER, "shaders/skybox.vert");
	shader.loadFromFile(GL_FRAGMENT_SHADER, "shaders/skybox.frag");
	//compile and link shader
	shader.createAndLinkProgram();
	shader.use();
		shader.addAttribute("vVertex"); 
		shader.addUniform("MVP");
		shader.addUniform("cubeMap");
		glUniform1i(shader("cubeMap"),0);
	shader.unUse(); 
	//setup the parent's fields
	initialize();
    //Init texture
	_sky_texture = TextureLoader::createCubeMap(sky_textures_dir_path);
}
 

CSkybox::~CSkybox(void)
{
} 

//there are 8 vertices in a skybox
unsigned int CSkybox::getTotalVertices() {
	return 8;
}

unsigned int CSkybox::getTotalIndices() {
	//6 faces with 2 triangles each with 3 vertices
	return 6*2*3;
}

GLenum CSkybox::getPrimitiveType() {
	return GL_TRIANGLES;
}

 void CSkybox::fillVertexBuffer(GLfloat* pBuffer) {
	glm::vec3* vertices = (glm::vec3*)(pBuffer); 
	vertices[0]=glm::vec3(-0.5f,-0.5f,-0.5f);
	vertices[1]=glm::vec3( 0.5f,-0.5f,-0.5f);
	vertices[2]=glm::vec3( 0.5f, 0.5f,-0.5f);
	vertices[3]=glm::vec3(-0.5f, 0.5f,-0.5f);
	vertices[4]=glm::vec3(-0.5f,-0.5f, 0.5f);
	vertices[5]=glm::vec3( 0.5f,-0.5f, 0.5f);
	vertices[6]=glm::vec3( 0.5f, 0.5f, 0.5f);
	vertices[7]=glm::vec3(-0.5f, 0.5f, 0.5f); 
}

void CSkybox::fillIndexBuffer(GLuint* pBuffer) {
	 
	//fill indices array
	GLuint* id=pBuffer; 

	//bottom face
	*id++ = 0; 	*id++ = 4; 	*id++ = 5;
	*id++ = 5; 	*id++ = 1; 	*id++ = 0; 
	
	//top face
	*id++ = 3; 	*id++ = 6; 	*id++ = 7;
	*id++ = 3; 	*id++ = 2; 	*id++ = 6;

	//front face
	*id++ = 7; 	*id++ = 6; 	*id++ = 4;
	*id++ = 6; 	*id++ = 5; 	*id++ = 4;

	//back face
	*id++ = 2; 	*id++ = 3; 	*id++ = 1;
	*id++ = 3; 	*id++ = 0; 	*id++ = 1;

	//left face 
	*id++ = 3; 	*id++ = 7; 	*id++ = 0;
	*id++ = 7; 	*id++ = 4; 	*id++ = 0;

	//right face 
	*id++ = 6; 	*id++ = 2; 	*id++ = 5;
	*id++ = 2; 	*id++ = 1; 	*id++ = 5;
}

void CSkybox::fillNormalBuffer(GLfloat* pBuffer)
{
    glm::vec3* normal = (glm::vec3*)(pBuffer);
    normal[0]=glm::vec3(0.5f, 0.5f, 0.5f);
    normal[1]=glm::vec3(-0.5f, 0.5f, 0.5f);
    normal[2]=glm::vec3(-0.5f, -0.5f, 0.5f);
    normal[3]=glm::vec3(0.5f, -0.5f, 0.5f);
    normal[4]=glm::vec3(0.5f, 0.5f, -0.5f);
    normal[5]=glm::vec3(-0.5f, 0.5f, -0.5f);
    normal[6]=glm::vec3(-0.5f, -0.5f, -0.5f);
    normal[7]=glm::vec3(0.5f, -0.5f, -0.5f);
}


void CSkybox::render(const GLfloat* MVP) {
    shader.use();
        glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, MVP);
        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _sky_texture->getTextureID());
        RenderableObject::render();
    shader.unUse();
}

unsigned int CSkybox::sizeOfVertexElement()
{
	return sizeof(glm::vec3);
}

unsigned int CSkybox::vertexNumberOfComponents()
{
	return 3;
}