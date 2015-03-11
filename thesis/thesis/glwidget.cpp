#include "glwidget.h"

#include <iostream>
#include <exception>

#include <QMessageBox>

#include "utils.h"

GLWidget::GLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
	setFocusPolicy(Qt::StrongFocus);
	core_engine_ = new Core();
}

GLWidget::~GLWidget()
{
	//core_engine_->delete();
	delete core_engine_;
}

/// <summary>
/// Initializes the OpenGl contex.
/// </summary>
void GLWidget::initializeGL()
{
	try
	{
		core_engine_->initialize();
	}
	catch(non_critical_exception e){
		std::cout << "Error: \n" << e.what() << std::endl;
		QMessageBox msgBox;
		msgBox.setStandardButtons(QMessageBox::Close);
		msgBox.setWindowTitle(tr("Error"));
		msgBox.setText(tr(e.what()));
		msgBox.setModal(true);
		//msgBox.open(this, SLOT(msgBoxClosed(QAbstractButton*)));
		msgBox.exec();
		
	}
	catch (critical_exception e){
		std::cout << "Critical error: \n" << e.what() << std::endl;
		QMessageBox msgBox;
		msgBox.setStandardButtons(QMessageBox::Close);
		msgBox.setWindowTitle(tr("Critical error"));
		msgBox.setText(tr(e.what()));
		msgBox.setModal(true);
		//msgBox.open(this, SLOT(msgBoxClosed(QAbstractButton*)));
		msgBox.exec();
		exit(EXIT_FAILURE);
	}
	std::cout << "Initialization successfull" << std::endl;
}

/// <summary>
/// Resizes the OpenGl contex.
/// </summary>
/// <param name="w">window width.</param>
/// <param name="h">window height.</param>
void GLWidget::resizeGL(int w, int h)
{
	core_engine_->resize(w, h);
	//glViewport(0, 0, w, h);
	//cam.UpdateProjection(glm::radians(fov), float(w) / float(h));
	//scene.ResizeBuffers(w, h);
}

/// <summary>
/// Renders the scene.
/// </summary>
void GLWidget::paintGL()
{
	std::cout << "FBO" << this->defaultFramebufferObject() << std::endl;
	glClear(GL_COLOR_BUFFER_BIT);
	std::cout << "Paint GL" << std::endl;
	core_engine_->render();
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