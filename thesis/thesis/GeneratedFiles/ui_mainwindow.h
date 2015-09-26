/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "glwidget.h"

QT_BEGIN_NAMESPACE

class Ui_mainwindowClass
{
public:
    QAction *actionSaveCam;
    QAction *actionLoadCam;
    QAction *actionLoadEnv;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    GLWidget *widget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QFrame *line;
    QPushButton *pushButton;
    QFrame *line_2;
    QRadioButton *radioButton;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *comboBox;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_10;
    QSpinBox *spinBox;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBox;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QDoubleSpinBox *doubleSpinBox_2;
    QFrame *line_3;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_8;
    QComboBox *comboBox_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QDoubleSpinBox *doubleSpinBox_3;
    QFrame *line_5;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QDoubleSpinBox *doubleSpinBox_4;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_7;
    QDoubleSpinBox *doubleSpinBox_5;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_11;
    QDoubleSpinBox *doubleSpinBox_7;
    QHBoxLayout *horizontalLayout_17;
    QLabel *label_18;
    QDoubleSpinBox *doubleSpinBox_13;
    QFrame *line_4;
    QLabel *label_14;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_9;
    QDoubleSpinBox *doubleSpinBox_6;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_12;
    QDoubleSpinBox *doubleSpinBox_9;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_13;
    QDoubleSpinBox *doubleSpinBox_10;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_15;
    QDoubleSpinBox *doubleSpinBox_11;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_16;
    QDoubleSpinBox *doubleSpinBox_8;
    QHBoxLayout *horizontalLayout_16;
    QLabel *label_17;
    QDoubleSpinBox *doubleSpinBox_12;
    QSpacerItem *verticalSpacer;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuCammera;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *mainwindowClass)
    {
        if (mainwindowClass->objectName().isEmpty())
            mainwindowClass->setObjectName(QStringLiteral("mainwindowClass"));
        mainwindowClass->resize(1008, 777);
        actionSaveCam = new QAction(mainwindowClass);
        actionSaveCam->setObjectName(QStringLiteral("actionSaveCam"));
        actionLoadCam = new QAction(mainwindowClass);
        actionLoadCam->setObjectName(QStringLiteral("actionLoadCam"));
        actionLoadEnv = new QAction(mainwindowClass);
        actionLoadEnv->setObjectName(QStringLiteral("actionLoadEnv"));
        centralWidget = new QWidget(mainwindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        widget = new GLWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(widget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setLayoutDirection(Qt::LeftToRight);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout->addWidget(pushButton);

        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_2);

        radioButton = new QRadioButton(centralWidget);
        radioButton->setObjectName(QStringLiteral("radioButton"));
        radioButton->setChecked(true);

        verticalLayout->addWidget(radioButton);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        comboBox = new QComboBox(centralWidget);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        horizontalLayout_2->addWidget(comboBox);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_10->addWidget(label_10);

        spinBox = new QSpinBox(centralWidget);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setMaximum(100);
        spinBox->setValue(20);

        horizontalLayout_10->addWidget(spinBox);


        verticalLayout->addLayout(horizontalLayout_10);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_3->addWidget(label_3);

        doubleSpinBox = new QDoubleSpinBox(centralWidget);
        doubleSpinBox->setObjectName(QStringLiteral("doubleSpinBox"));
        doubleSpinBox->setDecimals(4);
        doubleSpinBox->setMaximum(1);
        doubleSpinBox->setSingleStep(0.0001);
        doubleSpinBox->setValue(0.95);

        horizontalLayout_3->addWidget(doubleSpinBox);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_4->addWidget(label_4);

        doubleSpinBox_2 = new QDoubleSpinBox(centralWidget);
        doubleSpinBox_2->setObjectName(QStringLiteral("doubleSpinBox_2"));
        doubleSpinBox_2->setDecimals(4);
        doubleSpinBox_2->setMaximum(2);
        doubleSpinBox_2->setSingleStep(0.0001);
        doubleSpinBox_2->setValue(1);

        horizontalLayout_4->addWidget(doubleSpinBox_2);


        verticalLayout->addLayout(horizontalLayout_4);

        line_3 = new QFrame(centralWidget);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_3);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_8->addWidget(label_8);

        comboBox_2 = new QComboBox(centralWidget);
        comboBox_2->setObjectName(QStringLiteral("comboBox_2"));

        horizontalLayout_8->addWidget(comboBox_2);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_5->addWidget(label_5);

        doubleSpinBox_3 = new QDoubleSpinBox(centralWidget);
        doubleSpinBox_3->setObjectName(QStringLiteral("doubleSpinBox_3"));
        doubleSpinBox_3->setMaximum(5);
        doubleSpinBox_3->setSingleStep(0.01);
        doubleSpinBox_3->setValue(2);

        horizontalLayout_5->addWidget(doubleSpinBox_3);


        verticalLayout->addLayout(horizontalLayout_5);

        line_5 = new QFrame(centralWidget);
        line_5->setObjectName(QStringLiteral("line_5"));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_6->addWidget(label_6);

        doubleSpinBox_4 = new QDoubleSpinBox(centralWidget);
        doubleSpinBox_4->setObjectName(QStringLiteral("doubleSpinBox_4"));
        doubleSpinBox_4->setMaximum(1);
        doubleSpinBox_4->setSingleStep(0.01);
        doubleSpinBox_4->setValue(1);

        horizontalLayout_6->addWidget(doubleSpinBox_4);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_7->addWidget(label_7);

        doubleSpinBox_5 = new QDoubleSpinBox(centralWidget);
        doubleSpinBox_5->setObjectName(QStringLiteral("doubleSpinBox_5"));
        doubleSpinBox_5->setMaximum(4);
        doubleSpinBox_5->setSingleStep(0.01);
        doubleSpinBox_5->setValue(1);

        horizontalLayout_7->addWidget(doubleSpinBox_5);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        label_11 = new QLabel(centralWidget);
        label_11->setObjectName(QStringLiteral("label_11"));

        horizontalLayout_11->addWidget(label_11);

        doubleSpinBox_7 = new QDoubleSpinBox(centralWidget);
        doubleSpinBox_7->setObjectName(QStringLiteral("doubleSpinBox_7"));
        doubleSpinBox_7->setMaximum(1);
        doubleSpinBox_7->setSingleStep(0.01);
        doubleSpinBox_7->setValue(0.2);

        horizontalLayout_11->addWidget(doubleSpinBox_7);


        verticalLayout->addLayout(horizontalLayout_11);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setObjectName(QStringLiteral("horizontalLayout_17"));
        label_18 = new QLabel(centralWidget);
        label_18->setObjectName(QStringLiteral("label_18"));

        horizontalLayout_17->addWidget(label_18);

        doubleSpinBox_13 = new QDoubleSpinBox(centralWidget);
        doubleSpinBox_13->setObjectName(QStringLiteral("doubleSpinBox_13"));
        doubleSpinBox_13->setMaximum(4);
        doubleSpinBox_13->setSingleStep(0.01);
        doubleSpinBox_13->setValue(0.5);

        horizontalLayout_17->addWidget(doubleSpinBox_13);


        verticalLayout->addLayout(horizontalLayout_17);

        line_4 = new QFrame(centralWidget);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_4);

        label_14 = new QLabel(centralWidget);
        label_14->setObjectName(QStringLiteral("label_14"));

        verticalLayout->addWidget(label_14);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_9->addWidget(label_9);

        doubleSpinBox_6 = new QDoubleSpinBox(centralWidget);
        doubleSpinBox_6->setObjectName(QStringLiteral("doubleSpinBox_6"));
        doubleSpinBox_6->setMaximum(1);
        doubleSpinBox_6->setSingleStep(0.01);
        doubleSpinBox_6->setValue(0.48);

        horizontalLayout_9->addWidget(doubleSpinBox_6);


        verticalLayout->addLayout(horizontalLayout_9);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        label_12 = new QLabel(centralWidget);
        label_12->setObjectName(QStringLiteral("label_12"));

        horizontalLayout_12->addWidget(label_12);

        doubleSpinBox_9 = new QDoubleSpinBox(centralWidget);
        doubleSpinBox_9->setObjectName(QStringLiteral("doubleSpinBox_9"));
        doubleSpinBox_9->setMaximum(1);
        doubleSpinBox_9->setSingleStep(0.01);
        doubleSpinBox_9->setValue(0.4);

        horizontalLayout_12->addWidget(doubleSpinBox_9);


        verticalLayout->addLayout(horizontalLayout_12);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        label_13 = new QLabel(centralWidget);
        label_13->setObjectName(QStringLiteral("label_13"));

        horizontalLayout_13->addWidget(label_13);

        doubleSpinBox_10 = new QDoubleSpinBox(centralWidget);
        doubleSpinBox_10->setObjectName(QStringLiteral("doubleSpinBox_10"));
        doubleSpinBox_10->setMaximum(1);
        doubleSpinBox_10->setSingleStep(0.01);
        doubleSpinBox_10->setValue(0.28);

        horizontalLayout_13->addWidget(doubleSpinBox_10);


        verticalLayout->addLayout(horizontalLayout_13);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        label_15 = new QLabel(centralWidget);
        label_15->setObjectName(QStringLiteral("label_15"));

        horizontalLayout_14->addWidget(label_15);

        doubleSpinBox_11 = new QDoubleSpinBox(centralWidget);
        doubleSpinBox_11->setObjectName(QStringLiteral("doubleSpinBox_11"));
        doubleSpinBox_11->setMaximum(1);
        doubleSpinBox_11->setSingleStep(0.01);
        doubleSpinBox_11->setValue(1);

        horizontalLayout_14->addWidget(doubleSpinBox_11);


        verticalLayout->addLayout(horizontalLayout_14);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        label_16 = new QLabel(centralWidget);
        label_16->setObjectName(QStringLiteral("label_16"));

        horizontalLayout_15->addWidget(label_16);

        doubleSpinBox_8 = new QDoubleSpinBox(centralWidget);
        doubleSpinBox_8->setObjectName(QStringLiteral("doubleSpinBox_8"));
        doubleSpinBox_8->setMaximum(1);
        doubleSpinBox_8->setSingleStep(0.01);
        doubleSpinBox_8->setValue(0.37);

        horizontalLayout_15->addWidget(doubleSpinBox_8);


        verticalLayout->addLayout(horizontalLayout_15);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setSpacing(6);
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        label_17 = new QLabel(centralWidget);
        label_17->setObjectName(QStringLiteral("label_17"));

        horizontalLayout_16->addWidget(label_17);

        doubleSpinBox_12 = new QDoubleSpinBox(centralWidget);
        doubleSpinBox_12->setObjectName(QStringLiteral("doubleSpinBox_12"));
        doubleSpinBox_12->setMaximum(1);
        doubleSpinBox_12->setSingleStep(0.01);
        doubleSpinBox_12->setValue(0.3);

        horizontalLayout_16->addWidget(doubleSpinBox_12);


        verticalLayout->addLayout(horizontalLayout_16);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);

        mainwindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(mainwindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1008, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuCammera = new QMenu(menuFile);
        menuCammera->setObjectName(QStringLiteral("menuCammera"));
        mainwindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(mainwindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainwindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(mainwindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        mainwindowClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(menuCammera->menuAction());
        menuFile->addAction(actionLoadEnv);
        menuCammera->addAction(actionSaveCam);
        menuCammera->addAction(actionLoadCam);

        retranslateUi(mainwindowClass);
        QObject::connect(radioButton, SIGNAL(clicked()), widget, SLOT(toggleSSS()));
        QObject::connect(pushButton, SIGNAL(pressed()), widget, SLOT(reloadShaders()));
        QObject::connect(doubleSpinBox, SIGNAL(valueChanged(double)), widget, SLOT(setTranslucency(double)));
        QObject::connect(doubleSpinBox_2, SIGNAL(valueChanged(double)), widget, SLOT(setSSWidth(double)));
        QObject::connect(doubleSpinBox_3, SIGNAL(valueChanged(double)), widget, SLOT(setExposure(double)));
        QObject::connect(doubleSpinBox_4, SIGNAL(valueChanged(double)), widget, SLOT(setAmbientInt(double)));
        QObject::connect(doubleSpinBox_5, SIGNAL(valueChanged(double)), widget, SLOT(setSpeculartInt(double)));
        QObject::connect(comboBox, SIGNAL(currentIndexChanged(int)), widget, SLOT(selectSSSMethod(int)));
        QObject::connect(comboBox_2, SIGNAL(currentIndexChanged(int)), widget, SLOT(selectToneMappingMethod(int)));
        QObject::connect(doubleSpinBox_6, SIGNAL(valueChanged(double)), widget, SLOT(setSSSRedStr(double)));
        QObject::connect(doubleSpinBox_9, SIGNAL(valueChanged(double)), widget, SLOT(setSSSGreenStr(double)));
        QObject::connect(doubleSpinBox_10, SIGNAL(valueChanged(double)), widget, SLOT(setSSSBlueStr(double)));
        QObject::connect(spinBox, SIGNAL(valueChanged(int)), widget, SLOT(setSSSNumSamples(int)));
        QObject::connect(doubleSpinBox_7, SIGNAL(valueChanged(double)), widget, SLOT(setGlossines(double)));
        QObject::connect(doubleSpinBox_11, SIGNAL(valueChanged(double)), widget, SLOT(setSSSRedFalloff(double)));
        QObject::connect(doubleSpinBox_8, SIGNAL(valueChanged(double)), widget, SLOT(setSSSGreenFalloff(double)));
        QObject::connect(doubleSpinBox_12, SIGNAL(valueChanged(double)), widget, SLOT(setSSSBlueFalloff(double)));
        QObject::connect(doubleSpinBox_13, SIGNAL(valueChanged(double)), widget, SLOT(setBumpInt(double)));

        QMetaObject::connectSlotsByName(mainwindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *mainwindowClass)
    {
        mainwindowClass->setWindowTitle(QApplication::translate("mainwindowClass", "mainwindow", 0));
        actionSaveCam->setText(QApplication::translate("mainwindowClass", "Save", 0));
        actionLoadCam->setText(QApplication::translate("mainwindowClass", "Load", 0));
        actionLoadEnv->setText(QApplication::translate("mainwindowClass", "Load Env", 0));
        label->setText(QApplication::translate("mainwindowClass", "Control Panel", 0));
        pushButton->setText(QApplication::translate("mainwindowClass", "Reload Shaders", 0));
        radioButton->setText(QApplication::translate("mainwindowClass", "SSS active", 0));
        label_2->setText(QApplication::translate("mainwindowClass", "SSS Method", 0));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("mainwindowClass", "Separable Artistic", 0)
         << QApplication::translate("mainwindowClass", "Separable Pre Integrated", 0)
         << QApplication::translate("mainwindowClass", "Samples Gaussian", 0)
         << QApplication::translate("mainwindowClass", "Perceptual", 0)
        );
        label_10->setText(QApplication::translate("mainwindowClass", "#Samples", 0));
        label_3->setText(QApplication::translate("mainwindowClass", "translucency", 0));
        label_4->setText(QApplication::translate("mainwindowClass", "sssStrength", 0));
        label_8->setText(QApplication::translate("mainwindowClass", "ToneMap Method", 0));
        comboBox_2->clear();
        comboBox_2->insertItems(0, QStringList()
         << QApplication::translate("mainwindowClass", "Gamma corr", 0)
         << QApplication::translate("mainwindowClass", "Linear", 0)
         << QApplication::translate("mainwindowClass", "Exponential", 0)
         << QApplication::translate("mainwindowClass", "Exponential hsv", 0)
         << QApplication::translate("mainwindowClass", "Reinhard", 0)
         << QApplication::translate("mainwindowClass", "Filmic", 0)
        );
        label_5->setText(QApplication::translate("mainwindowClass", "exposure", 0));
        label_6->setText(QApplication::translate("mainwindowClass", "diffuseInt", 0));
        label_7->setText(QApplication::translate("mainwindowClass", "specularInt", 0));
        label_11->setText(QApplication::translate("mainwindowClass", "glossiness", 0));
        label_18->setText(QApplication::translate("mainwindowClass", "bump str", 0));
        label_14->setText(QApplication::translate("mainwindowClass", "Artistic method", 0));
        label_9->setText(QApplication::translate("mainwindowClass", "R str", 0));
        label_12->setText(QApplication::translate("mainwindowClass", "G str", 0));
        label_13->setText(QApplication::translate("mainwindowClass", "B str", 0));
        label_15->setText(QApplication::translate("mainwindowClass", "R falloff", 0));
        label_16->setText(QApplication::translate("mainwindowClass", "G falloff", 0));
        label_17->setText(QApplication::translate("mainwindowClass", "B falloff", 0));
        menuFile->setTitle(QApplication::translate("mainwindowClass", "File", 0));
        menuCammera->setTitle(QApplication::translate("mainwindowClass", "Camera", 0));
    } // retranslateUi

};

namespace Ui {
    class mainwindowClass: public Ui_mainwindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
