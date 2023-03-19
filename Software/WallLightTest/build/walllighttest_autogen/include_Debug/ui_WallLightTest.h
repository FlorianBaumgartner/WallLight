/********************************************************************************
** Form generated from reading UI file 'WallLightTest.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WALLLIGHTTEST_H
#define UI_WALLLIGHTTEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WallLightTest
{
public:
    QAction *actionExit;
    QWidget *centralWidget;
    QGridLayout *gridLayout;

    void setupUi(QMainWindow *WallLightTest)
    {
        if (WallLightTest->objectName().isEmpty())
            WallLightTest->setObjectName(QString::fromUtf8("WallLightTest"));
        WallLightTest->resize(250, 1000);
        actionExit = new QAction(WallLightTest);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        centralWidget = new QWidget(WallLightTest);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        WallLightTest->setCentralWidget(centralWidget);

        retranslateUi(WallLightTest);

        QMetaObject::connectSlotsByName(WallLightTest);
    } // setupUi

    void retranslateUi(QMainWindow *WallLightTest)
    {
        WallLightTest->setWindowTitle(QCoreApplication::translate("WallLightTest", "WallLight Test", nullptr));
        actionExit->setText(QCoreApplication::translate("WallLightTest", "E&xit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WallLightTest: public Ui_WallLightTest {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WALLLIGHTTEST_H
