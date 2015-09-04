#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <GL/glew.h>
//#include <QGLWidget>
#include <QOpenGLWidget>
#include <QMouseEvent>

#include "core.h"

class GLWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	GLWidget(QWidget *parent);
	~GLWidget();

public slots:
	void reloadShaders();
	void toggleSSS();
	void setTranslucency(double t);
	void setSSWidth(double w);
	void setExposure(double e);
	void setAmbientInt(double a);
	void setSpeculartInt(double s);
	void selectSSSMethod(int i);
	void selectToneMappingMethod(int i);

	void setSSSRedStr(double s);
	void setSSSGreenStr(double s);
	void setSSSBlueStr(double s);
	void setSSSRedFalloff(double s);
	void setSSSGreenFalloff(double s);
	void setSSSBlueFalloff(double s);
	void setSSSNumSamples(int s);
	void setGlossines(double g);
	void setBumpInt(double g);

	void saveCam(const QString &path);
	void loadCam(const QString &path);
	void loadKernel(const QString &path);
	void loadEnviroment(const QString &path);

protected:
	void paintGL();
	void initializeGL();
	void resizeGL(int w, int h);

	//events
	void keyPressEvent(QKeyEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

private:
	Core *core_engine_;
	QString _previous_path;
};

#endif // GLWIDGET_H
