#ifndef CORE_H
#define CORE_H

#include <GL/glew.h>
#include <GL/gl.h>

#include "targetcamera.h"

class Core
{
public:
	Core();
	~Core();

	void initialize();
	void render();

private:
	void initializeGL();
	void glewInitialization();

	TargetCamera cam_;
	
};

#endif // CORE_H
