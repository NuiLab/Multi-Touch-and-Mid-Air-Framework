/********************************************************************************
** Form generated from reading UI file 'qtleapgl.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTLEAPGL_H
#define UI_QTLEAPGL_H

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

class Ui_QtLeapGLClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtLeapGLClass)
    {
        if (QtLeapGLClass->objectName().isEmpty())
            QtLeapGLClass->setObjectName(QStringLiteral("QtLeapGLClass"));
        QtLeapGLClass->resize(600, 400);
        menuBar = new QMenuBar(QtLeapGLClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        QtLeapGLClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtLeapGLClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QtLeapGLClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(QtLeapGLClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QtLeapGLClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(QtLeapGLClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QtLeapGLClass->setStatusBar(statusBar);

        retranslateUi(QtLeapGLClass);

        QMetaObject::connectSlotsByName(QtLeapGLClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtLeapGLClass)
    {
        QtLeapGLClass->setWindowTitle(QApplication::translate("QtLeapGLClass", "QtLeapGL", 0));
    } // retranslateUi

};

namespace Ui {
    class QtLeapGLClass: public Ui_QtLeapGLClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTLEAPGL_H
