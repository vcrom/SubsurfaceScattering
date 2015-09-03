#include "mainwindow.h"
#include <qfiledialog.h>
#include <qmessagebox.h>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	QList<QMenu*> menus = ui.menuBar->findChildren<QMenu*>();
	QMenu* file_menu = menus[0];
	QAction* load_kernel_action = file_menu->addAction("Load Kernel");
	connect(load_kernel_action, SIGNAL(triggered()), this, SLOT(on_actionLoad_Kernel_tiggered()));

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
	QString filename = QFileDialog::getOpenFileName(this, tr("Load Cam"), "./", tr("Cam Files (*.cam);;All files (*)"));
	if (!filename.isNull())
		ui.widget->loadCam(filename);
}

void MainWindow::on_actionLoad_Kernel_tiggered()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Load pre-computed Kernel"), "./", tr("Bynary Kernel (*.bn);;All files (*)"));
	if (!filename.isNull())
		ui.widget->loadKernel(filename);
}

void MainWindow::on_actionLoadEnv_triggered()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open enviroment dir"), "./", QFileDialog::ShowDirsOnly);
	if (!dir.isNull())
	{
		ui.widget->loadEnviroment(dir);
	}
}