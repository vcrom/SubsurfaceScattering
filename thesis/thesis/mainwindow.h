#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::mainwindowClass ui;

public slots:


private slots:
	void on_actionSaveCam_triggered();
	void on_actionLoadCam_triggered();
	void on_actionLoad_Kernel_tiggered();
	void on_actionLoadEnv_triggered();


};

#endif // MAINWINDOW_H
