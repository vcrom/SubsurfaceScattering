/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
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
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "glwidget.h"

QT_BEGIN_NAMESPACE

class Ui_mainwindowClass
{
public:
    QAction *actionNothing;
    QAction *actionFoo;
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
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QDoubleSpinBox *doubleSpinBox_4;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_7;
    QDoubleSpinBox *doubleSpinBox_5;
    QSpacerItem *verticalSpacer;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *mainwindowClass)
    {
        if (mainwindowClass->objectName().isEmpty())
            mainwindowClass->setObjectName(QStringLiteral("mainwindowClass"));
        mainwindowClass->resize(910, 697);
        actionNothing = new QAction(mainwindowClass);
        actionNothing->setObjectName(QStringLiteral("actionNothing"));
        actionFoo = new QAction(mainwindowClass);
        actionFoo->setObjectName(QStringLiteral("actionFoo"));
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

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_3->addWidget(label_3);

        doubleSpinBox = new QDoubleSpinBox(centralWidget);
        doubleSpinBox->setObjectName(QStringLiteral("doubleSpinBox"));
        doubleSpinBox->setMaximum(1);
        doubleSpinBox->setSingleStep(0.01);
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
        doubleSpinBox_2->setMaximum(0.05);
        doubleSpinBox_2->setSingleStep(0.0001);
        doubleSpinBox_2->setValue(0.005);

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
        doubleSpinBox_4->setValue(0.66);

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
        doubleSpinBox_5->setValue(0.5);

        horizontalLayout_7->addWidget(doubleSpinBox_5);


        verticalLayout->addLayout(horizontalLayout_7);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);

        mainwindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(mainwindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 910, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        mainwindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(mainwindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainwindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(mainwindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        mainwindowClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionNothing);
        menuFile->addAction(actionFoo);

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

        QMetaObject::connectSlotsByName(mainwindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *mainwindowClass)
    {
        mainwindowClass->setWindowTitle(QApplication::translate("mainwindowClass", "mainwindow", 0));
        actionNothing->setText(QApplication::translate("mainwindowClass", "nothing", 0));
        actionFoo->setText(QApplication::translate("mainwindowClass", "foo", 0));
        label->setText(QApplication::translate("mainwindowClass", "Control Panel", 0));
        pushButton->setText(QApplication::translate("mainwindowClass", "Reload Shaders", 0));
        radioButton->setText(QApplication::translate("mainwindowClass", "SSS active", 0));
        label_2->setText(QApplication::translate("mainwindowClass", "SSS Method", 0));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("mainwindowClass", "Separable", 0)
         << QApplication::translate("mainwindowClass", "Perceptual", 0)
        );
        label_3->setText(QApplication::translate("mainwindowClass", "translucency", 0));
        label_4->setText(QApplication::translate("mainwindowClass", "sssWidth", 0));
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
        menuFile->setTitle(QApplication::translate("mainwindowClass", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class mainwindowClass: public Ui_mainwindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
