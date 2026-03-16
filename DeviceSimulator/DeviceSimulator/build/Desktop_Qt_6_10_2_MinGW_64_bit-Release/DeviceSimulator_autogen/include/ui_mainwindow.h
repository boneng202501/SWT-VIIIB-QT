/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLineEdit *lePort;
    QLabel *label;
    QPushButton *btnStart;
    QPushButton *btnStop;
    QGroupBox *groupBox;
    QPushButton *btnTripComplete;
    QPushButton *btnCloseComplete;
    QPushButton *btnWaveReady;
    QPushButton *btnFault;
    QPushButton *btnClearLog;
    QPushButton *btnSaveLog;
    QGroupBox *groupBox_2;
    QPlainTextEdit *plainLog;
    QMenuBar *menubar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 378);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        lePort = new QLineEdit(centralwidget);
        lePort->setObjectName("lePort");
        lePort->setGeometry(QRect(120, 20, 113, 21));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(70, 20, 45, 13));
        btnStart = new QPushButton(centralwidget);
        btnStart->setObjectName("btnStart");
        btnStart->setGeometry(QRect(370, 20, 80, 20));
        btnStop = new QPushButton(centralwidget);
        btnStop->setObjectName("btnStop");
        btnStop->setGeometry(QRect(470, 20, 80, 20));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(60, 270, 571, 51));
        btnTripComplete = new QPushButton(groupBox);
        btnTripComplete->setObjectName("btnTripComplete");
        btnTripComplete->setGeometry(QRect(20, 20, 80, 20));
        btnCloseComplete = new QPushButton(groupBox);
        btnCloseComplete->setObjectName("btnCloseComplete");
        btnCloseComplete->setGeometry(QRect(110, 20, 80, 20));
        btnWaveReady = new QPushButton(groupBox);
        btnWaveReady->setObjectName("btnWaveReady");
        btnWaveReady->setGeometry(QRect(200, 20, 80, 20));
        btnFault = new QPushButton(groupBox);
        btnFault->setObjectName("btnFault");
        btnFault->setGeometry(QRect(290, 20, 80, 20));
        btnClearLog = new QPushButton(groupBox);
        btnClearLog->setObjectName("btnClearLog");
        btnClearLog->setGeometry(QRect(380, 20, 80, 20));
        btnSaveLog = new QPushButton(groupBox);
        btnSaveLog->setObjectName("btnSaveLog");
        btnSaveLog->setGeometry(QRect(470, 20, 80, 20));
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(60, 60, 571, 191));
        plainLog = new QPlainTextEdit(groupBox_2);
        plainLog->setObjectName("plainLog");
        plainLog->setGeometry(QRect(20, 20, 491, 151));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 18));
        MainWindow->setMenuBar(menubar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        lePort->setText(QCoreApplication::translate("MainWindow", "5000", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\347\253\257\345\217\243", nullptr));
        btnStart->setText(QCoreApplication::translate("MainWindow", "\345\220\257\345\212\250\346\250\241\346\213\237\345\231\250", nullptr));
        btnStop->setText(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\346\211\213\345\212\250\346\250\241\346\213\237\344\272\213\344\273\266", nullptr));
        btnTripComplete->setText(QCoreApplication::translate("MainWindow", "\345\210\206\351\227\270\345\256\214\346\210\220", nullptr));
        btnCloseComplete->setText(QCoreApplication::translate("MainWindow", "\345\220\210\351\227\270\345\256\214\346\210\220", nullptr));
        btnWaveReady->setText(QCoreApplication::translate("MainWindow", "\346\263\242\345\275\242\346\225\260\346\215\256\345\260\261\347\273\252", nullptr));
        btnFault->setText(QCoreApplication::translate("MainWindow", "\346\250\241\346\213\237\346\225\205\351\232\234", nullptr));
        btnClearLog->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\346\227\245\345\277\227", nullptr));
        btnSaveLog->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230\346\227\245\345\277\227", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "\346\227\245\345\277\227", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
