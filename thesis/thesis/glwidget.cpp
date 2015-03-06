#include "glwidget.h"
#include <iostream>

GLWidget::GLWidget(QWidget *parent)
	: QGLWidget(parent)
{
	setFocusPolicy(Qt::StrongFocus);
}

GLWidget::~GLWidget()
{

}

/// <summary>
/// Initializes the OpenGl contex.
/// </summary>
void GLWidget::initializeGL()
{
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

	std::cout << "Initialization successfull" << std::endl;
}

/// <summary>
/// Resizes the OpenGl contex.
/// </summary>
/// <param name="w">window width.</param>
/// <param name="h">window height.</param>
void GLWidget::resizeGL(int w, int h)
{
	std::cout << "Resize(" << w << ", " << h << ")" << std::endl;
	glViewport(0, 0, w, h);
	//cam.UpdateProjection(glm::radians(fov), float(w) / float(h));
	//scene.ResizeBuffers(w, h);
}

/// <summary>
/// Renders the scene.
/// </summary>
void GLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	std::cout << "Paint GL" << std::endl;
}


/// <summary>
/// QT Key the pressed event handling.
/// </summary>
/// <param name="event">QT key event.</param>
void GLWidget::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_F)
	{
	}
	std::cout << "key press event" << std::endl;
}

/// <summary>
/// QT mouse button pressed event handling.
/// </summary>
/// <param name="event">QT mouse event.</param>
void GLWidget::mousePressEvent(QMouseEvent* event)
{
	std::cout << "mouse press event" << std::endl;
}

/// <summary>
/// QT mouse moved event handling.
/// </summary>
/// <param name="event">QT mouse event.</param>
void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
	std::cout << "mouse move event" << std::endl;
}

/// <summary>
/// QT mouse button relased event handling.
/// </summary>
/// <param name="event">QT mouse event.</param>
void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
	std::cout << "mpouse rel event" << std::endl;
}