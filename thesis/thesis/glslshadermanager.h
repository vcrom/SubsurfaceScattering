#ifndef GLSLSHADERMANAGER_H
#define GLSLSHADERMANAGER_H

#include <vector>
#include <memory>

#include "glslshader.h"

class GlslShaderManager
{
	using uint = unsigned int;
public:
	enum Shaders : uint {	
		TEXTURE_TO_SCREEN, PASS_THROUGH, SHADOWS_AND_DIFFUSE, LINEAL_DEEPTH, TRANSLUCENCY_SHADER, THICKNESS_SHADER, MAIN_RENDER_SHADER, 
		SSSS_HORIZONTAL_BLUR, SSSS_VERTICAL_BLUR,
		SEPARABLE_SSSS_HORIZONTAL_BLUR, SEPARABLE_SSSS_VERTICAL_BLUR,
		TONE_MAP,
		GAUSSIAN_SSSS_HORIZONTAL_BLUR, GAUSSIAN_SSSS_VERTICAL_BLUR,
		IMAGE_BLUR_SHADER_HORI, IMAGE_BLUR_SHADER_VERT,
		Count 
	};

	static std::shared_ptr<GlslShaderManager> instance();
	//static GlslShaderManager& instance();
	~GlslShaderManager();

	void initializeShaders();
	void deleteShaders();
	void reloadShaders();
	std::shared_ptr<GlslShader> getShader(Shaders shader);

protected:
	GlslShaderManager();
	std::vector<GlslShader> _shaders;

private:
	void initTextureToScreenShader();
	void initPassThroughShader();
	void initShadowsAndDiffuseShader();
	void initLinealDeepthShader();
	void initTranslucecyhader();
	void initMainRenderShader(); 
	void initSSSSHori();
	void initSSSSVert();
	void initSeparableSSSSHori();
	void initSeparableSSSSVert();
	void initToneMap();
	void initSSSSGaussianHori();
	void initSSSSGaussianVert();
	void initImageBlurHori();
	void initImageBlurVert();

};

#endif // GLSLSHADERMANAGER_H

//class FileSystem
//{
//public:
//	static FileSystem& instance();
//
//	virtual ~FileSystem() {}
//	virtual char* readFile(char* path) = 0;
//	virtual void  writeFile(char* path, char* contents) = 0;
//
//protected:
//	FileSystem() {}
//};
//The clever part is how the instance is created :
//
//FileSystem& FileSystem::instance()
//{
//#if PLATFORM == PLAYSTATION3
//	static FileSystem *instance = new PS3FileSystem();
//#elif PLATFORM == WII
//	static FileSystem *instance = new WiiFileSystem();
//#endif
//
//	return *instance;
//}