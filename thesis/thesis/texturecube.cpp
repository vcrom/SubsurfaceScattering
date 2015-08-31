#include "texturecube.h"

TextureCube::TextureCube() : Texture(GL_TEXTURE_CUBE_MAP)
{
	_width = _height = 0;
}

TextureCube::~TextureCube()
{

}

GLsizei TextureCube::getWidth()
{
	return _width;
}

GLsizei TextureCube::getHeight()
{
	return _height;
}