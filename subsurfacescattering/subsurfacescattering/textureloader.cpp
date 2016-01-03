#include "textureloader.h"


//TextureLoader::TextureLoader()
//{
//
//}

//TextureLoader::~TextureLoader()
//{
//
//}

#include <iostream>
#include <vector>
#include "fimage.h"
#include "utils.h"

std::shared_ptr<Texture2D> TextureLoader::create2DTexture(const std::string& path)
{
	std::shared_ptr<Texture2D> tex = create2DTexture(path, GL_RGB8);
	return tex;
}

std::shared_ptr<Texture2D> TextureLoader::create2DTexture(const std::string& path, GLint internalformat)
{
	fImage image;
	image.loadImage(path);

	std::shared_ptr<Texture2D> tex = std::shared_ptr<Texture2D>(new Texture2D(GL_TEXTURE_2D));
	//tex->createTexture();
	tex->use();
	//TODO: adapt the parameters for each image automatically
	tex->loadBufferToTexture(0, internalformat, image.getWidth(), image.getHeight(), GL_BGR, GL_UNSIGNED_BYTE, image.getImageData());
	tex->generateMipMaps();// glGenerateMipmap(GL_TEXTURE_2D);
	tex->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	return tex;
}

std::shared_ptr<Texture2D> TextureLoader::createCubeMap(const std::string& folder_path)
{
	return createCubeMap(folder_path, GL_RGB8);
}

std::shared_ptr<Texture2D> TextureLoader::createCubeMap(const std::string& folder_path, GLint internalformat)
{
	const std::string texture_names[6] = {	"/i_c00.jpg",   //posx
											"/i_c01.jpg",   //negx
											"/i_c02.jpg",   //posy
											"/i_c03.jpg",   //negy
											"/i_c05.jpg",   //posz
											"/i_c04.jpg" };  //negz

	std::vector<fImage> faces(6);
	for (int i = 0; i < 6; ++i)
	{
		faces[i].loadImage(folder_path + texture_names[i]);
		if(i != 2 && i != 3) faces[i].flipDiagonal();
	}
	std::shared_ptr<Texture2D> tex = std::shared_ptr<Texture2D>(new Texture2D(GL_TEXTURE_CUBE_MAP));
	tex->use();
	tex->setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	tex->setTexParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	tex->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	tex->setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	tex->setTexParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	tex->initTexStorage(1, internalformat, faces[0].getWidth(), faces[0].getHeight());

	//Init faces
	for (unsigned int face = 0; face < 6; ++face)
		tex->cubeFaceFromBuffer(face, 0, GL_BGR, GL_UNSIGNED_BYTE, faces[face].getImageData());

	return tex;
}