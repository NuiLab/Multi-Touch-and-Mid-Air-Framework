/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

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

class Ui_TamVisualizationClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TamVisualizationClass)
    {
        if (TamVisualizationClass->objectName().isEmpty())
            TamVisualizationClass->setObjectName(QStringLiteral("TamVisualizationClass"));
        TamVisualizationClass->resize(600, 400);
        menuBar = new QMenuBar(TamVisualizationClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        TamVisualizationClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TamVisualizationClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        TamVisualizationClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(TamVisualizationClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        TamVisualizationClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(TamVisualizationClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        TamVisualizationClass->setStatusBar(statusBar);

        retranslateUi(TamVisualizationClass);

        QMetaObject::connectSlotsByName(TamVisualizationClass);
    } // setupUi

    void retranslateUi(QMainWindow *TamVisualizationClass)
    {
        TamVisualizationClass->setWindowTitle(QApplication::translate("TamVisualizationClass", "TamVisualization", 0));
    } // retranslateUi

};

namespace Ui {
    class TamVisualizationClass: public Ui_TamVisualizationClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
