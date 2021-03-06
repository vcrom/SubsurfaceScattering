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
	//void* foo = wglGetCurrentContext();

	//QSurfaceFormat newFormat = this->format();
	////newFormat.setSampleBuffers(true);
	//newFormat.setSamples(16);
	//this->setFormat(newFormat);
	_previous_path = "./";

}

GLWidget::~GLWidget()
{
	//core_engine_->delete();
	delete core_engine_;
	core_engine_ = nullptr;
}

/// <summary>
/// Initializes the OpenGl contex.
/// </summary>
void GLWidget::initializeGL()
{
	try
	{
		core_engine_->setDefaultFBO(this->defaultFramebufferObject());
		core_engine_->initialize();
		core_engine_->setGlossines(0.3);
		core_engine_->setSpeculartInt(1);
		core_engine_->setAmbientInt(1);

		//selectSSSMethod(1);
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
	core_engine_->setDefaultFBO(this->defaultFramebufferObject());
	//glViewport(0, 0, w, h);
	//cam.UpdateProjection(glm::radians(fov), float(w) / float(h));
	//scene.ResizeBuffers(w, h);
}

/// <summary>
/// Renders the scene.
/// </summary>
void GLWidget::paintGL()
{
	//core_engine_->setDefaultFBO(this->defaultFramebufferObject());
	//std::cout << "QT FBO" << this->defaultFramebufferObject() << std::endl;
	std::cout << "QGL Paint GL" << std::endl;
	core_engine_->onRender(); 
}

#include <QFileDialog>
/// <summary>
/// QT Key the pressed event handling.
/// </summary>
/// <param name="event">QT key event.</param>
void GLWidget::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_W)
	{
		core_engine_->moveLight(glm::vec3(0, 0, -1));
		update();
	}
	else if (event->key() == Qt::Key_S)
	{
		core_engine_->moveLight(glm::vec3(0, 0, 1));
		update();
	}
	else if (event->key() == Qt::Key_A)
	{
		core_engine_->moveLight(glm::vec3(-1, 0, 0));
		update();
	}
	else if (event->key() == Qt::Key_D)
	{
		core_engine_->moveLight(glm::vec3(1, 0, 0));
		update();
	}
	else if (event->key() == Qt::Key_E)
	{
		core_engine_->moveLight(glm::vec3(0, 1, 0));
		update();
	}
	else if (event->key() == Qt::Key_Q)
	{
		core_engine_->moveLight(glm::vec3(0, -1, 0));
		update();
	}
	else if (event->key() == Qt::Key_M)
	{
		QString filename = QFileDialog::getOpenFileName(this, tr("Load Mesh"), _previous_path, tr("PLY Files (*.ply);;All files (*)"));
		if (!filename.isNull())
		{
			makeCurrent();
			core_engine_->loadMesh(filename.toStdString());
			_previous_path = filename.left(filename.lastIndexOf("/"));
		}
		update();
	}
	else if (event->key() == Qt::Key_N)
	{
		QString filename = QFileDialog::getOpenFileName(this, tr("Load Diffuse tex"), _previous_path, tr("Image Files (*.png *.jpg *.bmp)"));
		if (!filename.isNull())
		{
			makeCurrent();
			core_engine_->loadMeshDiffuseTexture(filename.toStdString());
			_previous_path = filename.left(filename.lastIndexOf("/"));
		}
		update();
	}
	else if (event->key() == Qt::Key_B)
	{
		QString filename = QFileDialog::getOpenFileName(this, tr("Load AO tex"), _previous_path, tr("Image Files (*.png *.jpg *.bmp)"));
		if (!filename.isNull())
		{
			makeCurrent();
			core_engine_->loadMeshAOTexture(filename.toStdString());
			_previous_path = filename.left(filename.lastIndexOf("/"));
		}
		update();
	}
	else if (event->key() == Qt::Key_V)
	{
		QString filename = QFileDialog::getOpenFileName(this, tr("Load Normals tex"), _previous_path, tr("Image Files (*.png *.jpg *.bmp)"));
		if (!filename.isNull())
		{
			makeCurrent();
			core_engine_->loadMeshNormalsTexture(filename.toStdString());
			_previous_path = filename.left(filename.lastIndexOf("/"));
		}
		update();
	}
	else if (event->key() == Qt::Key_L)
	{
		core_engine_->toggleControlBool(0);
		update();
	}
	else if (event->key() == Qt::Key_J)
	{
		core_engine_->toggleControlBool(1);
		update();
	}

	else if (event->key() == Qt::Key_K)
	{
		core_engine_->toggleControlBool(2);
		update();
	}
	else std::cout << "key press event" << std::endl;
}

/// <summary>
/// QT mouse button pressed event handling.
/// </summary>
/// <param name="event">QT mouse event.</param>
void GLWidget::mousePressEvent(QMouseEvent* event)
{
	Core::Input in = Core::NOTHING;
	if (event->button() == Qt::LeftButton) in = Core::MOUSE_LEFT_BUTTON;
	else if (event->button() == Qt::RightButton)in = Core::MOUSE_RIGHT_BUTTON; 
	core_engine_->mouseclick(event->x(), event->y(), in);
	update();
}

/// <summary>
/// QT mouse moved event handling.
/// </summary>
/// <param name="event">QT mouse event.</param>
void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
	Core::Input in = Core::NOTHING;
	if (event->buttons() & Qt::LeftButton) in = Core::MOUSE_LEFT_BUTTON;
	else if (event->buttons() & Qt::RightButton)in = Core::MOUSE_RIGHT_BUTTON;
	core_engine_->mouseMoved(event->x(), event->y(), in);
	update();
}

/// <summary>
/// QT mouse button relased event handling.
/// </summary>
/// <param name="event">QT mouse event.</param>
void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
	Core::Input in = Core::NOTHING;
	if (event->buttons() & Qt::LeftButton) in = Core::MOUSE_LEFT_BUTTON;
	else if (event->buttons() & Qt::RightButton)in = Core::MOUSE_RIGHT_BUTTON;
	core_engine_->mouseReleased(event->x(), event->y(), in);
	update();
}

void GLWidget::reloadShaders()
{
	core_engine_->reloadShaders();
	update();
}

void GLWidget::toggleSSS()
{
	core_engine_->toggleControlBool(2);
	update();
}

void GLWidget::setTranslucency(double t)
{
	core_engine_->setTranslucency(t);
	update();
}

void GLWidget::setSSWidth(double w)
{
	core_engine_->setSSWidth(w);
	update();
}

void GLWidget::setExposure(double e)
{
	core_engine_->setExposure(e);
	update();
}

void GLWidget::setAmbientInt(double a)
{
	core_engine_->setAmbientInt(a);
	update();
}

void GLWidget::setSpeculartInt(double s)
{
	core_engine_->setSpeculartInt(s);
	update();
}

void GLWidget::selectSSSMethod(int i)
{
	core_engine_->setSSSMethod(i);
	update();
}

void GLWidget::selectToneMappingMethod(int i)
{
	core_engine_->setToneMappingMethod(i);
	update();
}

void GLWidget::setSSSRedStr(double s)
{
	core_engine_->setSSSRedStr(s);
	update();
}
void GLWidget::setSSSGreenStr(double s)
{
	core_engine_->setSSSGreenStr(s);
	update();
}
void GLWidget::setSSSBlueStr(double s)
{
	core_engine_->setSSSBlueStr(s);
	update();
}

void GLWidget::setSSSRedFalloff(double s)
{
	core_engine_->setSSSRedFalloff(s);
	update();
}
void GLWidget::setSSSGreenFalloff(double s)
{
	core_engine_->setSSSGreenFalloff(s);
	update();
}
void GLWidget::setSSSBlueFalloff(double s)
{
	core_engine_->setSSSBlueFalloff(s);
	update();
}

void GLWidget::setSSSNumSamples(int s)
{
	core_engine_->setSSSNumSamples(s);
	update();
}

void GLWidget::setGlossines(double g)
{
	core_engine_->setGlossines(g);
	update();
}

void GLWidget::setBumpInt(double g)
{
	core_engine_->setBumpInt(g);
	update();
}

void GLWidget::saveCam(const QString &path)
{
	std::string string_path= path.toLatin1().data();
	core_engine_->saveCamtoFile(string_path);
}

void GLWidget::loadCam(const QString &path)
{
	core_engine_->loadCamFromFile(path.toLatin1().data());
	update();
}


void GLWidget::loadKernel(const QString &path)
{
	core_engine_->loadPreComputedKernel(path.toLatin1().data());
}

void GLWidget::loadEnviroment(const QString &path)
{
	core_engine_->loadEnviroment(path.toLatin1().data());
}