#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>
#include <GL/gl.h>
//#define NDEBUG
#include <cassert>

#include "exceptions.h"

#define throw_non_critical(arg) throw non_critical_exception(arg, __FILE__, __LINE__);
#define throw_critical(arg) throw critical_exception(arg, __FILE__, __LINE__);

//#define checkCritOpenGLError() {GLenum _err_ = glGetError(); if(_err_ != GL_NO_ERROR) throw_critical("OpenGL error : " + std::to_string(_err_))};
//#define checkNonCritOpenGLError() {GLenum _err_ = glGetError(); if(_err_ != GL_NO_ERROR) throw_non_critical("OpenGL error : " + std::to_string(_err_))};
#define checkCritOpenGLError() assert(glGetError() == GL_NO_ERROR);
#define checkNonCritOpenGLError() assert(glGetError() == GL_NO_ERROR);

#ifndef GL_CHECK_ERRORS
#define GL_CHECK_ERRORS assert(glGetError()== GL_NO_ERROR);
#endif

#ifndef GL_CHECK_ERRORS_PRINT
#define GL_CHECK_ERRORS_PRINT {GLenum err; while ((err = glGetError()) != GL_NO_ERROR) {std::cerr << "OpenGL error: " << err << std::endl;} exit(err);}
#endif

#endif // UTILS_H