/********************************************************************************
** Form generated from reading UI file 'voltageregdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VOLTAGEREGDIALOG_H
#define UI_VOLTAGEREGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_voltageregdialog
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox;
    QRadioButton *rbIO;
    QRadioButton *rbDA;
    QGroupBox *groupBox_2;
    QLineEdit *lineEdit;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QLabel *label_3;
    QLineEdit *lineEdit_3;
    QLabel *label_4;
    QLineEdit *lineEdit_4;
    QLabel *label_5;
    QLineEdit *lineEdit_5;
    QLabel *label_6;
    QLineEdit *lineEdit_6;
    QGroupBox *groupBox_3;
    QLineEdit *lineEdit_7;
    QLabel *label_7;
    QLabel *label_8;
    QLineEdit *lineEdit_8;
    QLabel *label_10;
    QLineEdit *lineEdit_10;
    QLabel *label_11;
    QLineEdit *lineEdit_11;

    void setupUi(QDialog *voltageregdialog)
    {
        if (voltageregdialog->objectName().isEmpty())
            voltageregdialog->setObjectName("voltageregdialog");
        voltageregdialog->resize(561, 415);
        buttonBox = new QDialogButtonBox(voltageregdialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setGeometry(QRect(40, 360, 341, 32));
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);
        groupBox = new QGroupBox(voltageregdialog);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(40, 10, 261, 61));
        rbIO = new QRadioButton(groupBox);
        rbIO->setObjectName("rbIO");
        rbIO->setGeometry(QRect(10, 20, 91, 31));
        rbDA = new QRadioButton(groupBox);
        rbDA->setObjectName("rbDA");
        rbDA->setGeometry(QRect(120, 20, 121, 31));
        groupBox_2 = new QGroupBox(voltageregdialog);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(40, 80, 411, 151));
        lineEdit = new QLineEdit(groupBox_2);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(90, 30, 113, 21));
        label = new QLabel(groupBox_2);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 31, 51, 20));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(20, 70, 45, 13));
        lineEdit_2 = new QLineEdit(groupBox_2);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(90, 70, 113, 21));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(20, 110, 45, 13));
        lineEdit_3 = new QLineEdit(groupBox_2);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(90, 110, 113, 21));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(220, 30, 45, 13));
        lineEdit_4 = new QLineEdit(groupBox_2);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(280, 30, 113, 21));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(210, 70, 45, 13));
        lineEdit_5 = new QLineEdit(groupBox_2);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setGeometry(QRect(280, 70, 113, 21));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(220, 110, 45, 13));
        lineEdit_6 = new QLineEdit(groupBox_2);
        lineEdit_6->setObjectName("lineEdit_6");
        lineEdit_6->setGeometry(QRect(280, 110, 113, 21));
        groupBox_3 = new QGroupBox(voltageregdialog);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setGeometry(QRect(30, 230, 411, 111));
        lineEdit_7 = new QLineEdit(groupBox_3);
        lineEdit_7->setObjectName("lineEdit_7");
        lineEdit_7->setGeometry(QRect(90, 30, 113, 21));
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(20, 31, 51, 20));
        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(20, 70, 45, 13));
        lineEdit_8 = new QLineEdit(groupBox_3);
        lineEdit_8->setObjectName("lineEdit_8");
        lineEdit_8->setGeometry(QRect(90, 70, 113, 21));
        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(220, 30, 45, 13));
        lineEdit_10 = new QLineEdit(groupBox_3);
        lineEdit_10->setObjectName("lineEdit_10");
        lineEdit_10->setGeometry(QRect(280, 30, 113, 21));
        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(210, 70, 45, 13));
        lineEdit_11 = new QLineEdit(groupBox_3);
        lineEdit_11->setObjectName("lineEdit_11");
        lineEdit_11->setGeometry(QRect(280, 70, 113, 21));

        retranslateUi(voltageregdialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, voltageregdialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, voltageregdialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(voltageregdialog);
    } // setupUi

    void retranslateUi(QDialog *voltageregdialog)
    {
        voltageregdialog->setWindowTitle(QCoreApplication::translate("voltageregdialog", "Dialog", nullptr));
        groupBox->setTitle(QCoreApplication::translate("voltageregdialog", "\350\260\203\345\216\213\346\250\241\345\274\217", nullptr));
        rbIO->setText(QCoreApplication::translate("voltageregdialog", "IO\345\217\243\350\260\203\345\216\213 (0x31)", nullptr));
        rbDA->setText(QCoreApplication::translate("voltageregdialog", "DA\346\250\241\346\213\237\351\207\217\350\260\203\345\216\213 (0x32)", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("voltageregdialog", "\351\200\232\347\224\250\345\217\202\346\225\260", nullptr));
        label->setText(QCoreApplication::translate("voltageregdialog", "\347\233\256\346\240\207\347\224\265\345\216\213", nullptr));
        label_2->setText(QCoreApplication::translate("voltageregdialog", "\350\266\205\346\227\266\346\227\266\351\227\264", nullptr));
        label_3->setText(QCoreApplication::translate("voltageregdialog", "\351\207\207\351\233\206\351\200\232\351\201\223", nullptr));
        label_4->setText(QCoreApplication::translate("voltageregdialog", "\350\257\257\345\267\256\350\214\203\345\233\264", nullptr));
        label_5->setText(QCoreApplication::translate("voltageregdialog", "\347\224\265\346\272\220\347\261\273\345\236\213", nullptr));
        label_6->setText(QCoreApplication::translate("voltageregdialog", "\350\275\254\346\215\242\346\257\224\347\216\207", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("voltageregdialog", "\351\200\232\347\224\250\345\217\202\346\225\260", nullptr));
        label_7->setText(QCoreApplication::translate("voltageregdialog", "\347\233\256\346\240\207\347\224\265\345\216\213", nullptr));
        label_8->setText(QCoreApplication::translate("voltageregdialog", "\350\266\205\346\227\266\346\227\266\351\227\264", nullptr));
        label_10->setText(QCoreApplication::translate("voltageregdialog", "\350\257\257\345\267\256\350\214\203\345\233\264", nullptr));
        label_11->setText(QCoreApplication::translate("voltageregdialog", "\347\224\265\346\272\220\347\261\273\345\236\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class voltageregdialog: public Ui_voltageregdialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VOLTAGEREGDIALOG_H
