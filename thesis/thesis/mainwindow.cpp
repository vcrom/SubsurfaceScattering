#include "mainwindow.h"
#include <qfiledialog.h>
#include <qmessagebox.h>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

MainWindow::~MainWindow()
{

}


void MainWindow::on_actionSaveCam_triggered()
{
	QString filename;
	QFileDialog file_dialog;
	file_dialog.setDefaultSuffix("cam");
	QString filter = tr("Cam Files (*.cam)");
	filename = file_dialog.getSaveFileName(this, tr("Save Cam"), "./", filter);
	if (!filename.contains(".cam"))
		filename = filename + ".cam";

	if (!filename.isNull())
		ui.widget->saveCam(filename);
}

void MainWindow::on_actionLoadCam_triggered()
{
	QString filename;

	filename = QFileDialog::getOpenFileName(this, tr("Load Cam"), "./", tr("Cam Files (*.cam);;All files (*)"));
	if (!filename.isNull())
		ui.widget->loadCam(filename);
}