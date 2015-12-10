/********************************************************************************
** Form generated from reading UI file 'knvis.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KNVIS_H
#define UI_KNVIS_H

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

class Ui_KnVisClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *KnVisClass)
    {
        if (KnVisClass->objectName().isEmpty())
            KnVisClass->setObjectName(QStringLiteral("KnVisClass"));
        KnVisClass->resize(600, 400);
        menuBar = new QMenuBar(KnVisClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        KnVisClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(KnVisClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        KnVisClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(KnVisClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        KnVisClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(KnVisClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        KnVisClass->setStatusBar(statusBar);

        retranslateUi(KnVisClass);

        QMetaObject::connectSlotsByName(KnVisClass);
    } // setupUi

    void retranslateUi(QMainWindow *KnVisClass)
    {
        KnVisClass->setWindowTitle(QApplication::translate("KnVisClass", "KnVis", 0));
    } // retranslateUi

};

namespace Ui {
    class KnVisClass: public Ui_KnVisClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KNVIS_H
