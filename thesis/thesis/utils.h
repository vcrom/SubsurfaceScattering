#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <stdexcept>
#include <string>

class my_exception : public std::runtime_error {
public:
	my_exception(const std::string &arg, const char *file, int line);
	~my_exception() throw();
	const char *what() const throw();
private:
	std::string msg;
};

#define throw_line(arg) throw my_exception(arg, __FILE__, __LINE__);
//#define checkOpenGLError() {GLenum _err_ = glGetError(); if(_err_ != GL_NO_ERROR) throw_line("OpenGL error : " + std::to_string(_err_))};
#define checkOpenGLError() assert(glGetError() == GL_NO_ERROR);
#endif // UTILS_H