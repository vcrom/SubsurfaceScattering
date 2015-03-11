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
};

#endif // GLWIDGET_H
