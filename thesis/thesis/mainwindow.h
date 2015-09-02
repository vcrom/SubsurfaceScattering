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

private slots:
	void on_actionSaveCam_triggered();
	void on_actionLoadCam_triggered();
};

#endif // MAINWINDOW_H
