/********************************************************************************
** Form generated from reading UI file 'gyroscopevisualizer.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GYROSCOPEVISUALIZER_H
#define UI_GYROSCOPEVISUALIZER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GyroscopeVisualizerClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GyroscopeVisualizerClass)
    {
        if (GyroscopeVisualizerClass->objectName().isEmpty())
            GyroscopeVisualizerClass->setObjectName(QStringLiteral("GyroscopeVisualizerClass"));
        GyroscopeVisualizerClass->resize(600, 400);
        menuBar = new QMenuBar(GyroscopeVisualizerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        GyroscopeVisualizerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(GyroscopeVisualizerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        GyroscopeVisualizerClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(GyroscopeVisualizerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        GyroscopeVisualizerClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(GyroscopeVisualizerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        GyroscopeVisualizerClass->setStatusBar(statusBar);

        retranslateUi(GyroscopeVisualizerClass);

        QMetaObject::connectSlotsByName(GyroscopeVisualizerClass);
    } // setupUi

    void retranslateUi(QMainWindow *GyroscopeVisualizerClass)
    {
        GyroscopeVisualizerClass->setWindowTitle(QApplication::translate("GyroscopeVisualizerClass", "GyroscopeVisualizer", 0));
    } // retranslateUi

};

namespace Ui {
    class GyroscopeVisualizerClass: public Ui_GyroscopeVisualizerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GYROSCOPEVISUALIZER_H
