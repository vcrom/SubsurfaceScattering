#pragma once
#include "renderableobject.h"
#include "glslshader.h"
#include "texture2d.h"

#include <iostream>
#include <memory>


class CSkybox:public RenderableObject
{
public: 
	CSkybox(std::string sky_textures_dir_path);
	virtual ~CSkybox(void);

	unsigned int getTotalVertices();
	unsigned int getTotalIndices();
	GLenum getPrimitiveType(); 
	void fillVertexBuffer( GLfloat* pBuffer);
    void fillIndexBuffer( GLuint* pBuffer);
    void fillNormalBuffer(GLfloat* pBuffer);

	unsigned int sizeOfVertexElement();
	unsigned int vertexNumberOfComponents();

    void render(const GLfloat* MVP);
private:
	GlslShader shader;
	std::shared_ptr<Texture2D> _sky_texture;
};

