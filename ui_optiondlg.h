/********************************************************************************
** Form generated from reading UI file 'optiondlg.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTIONDLG_H
#define UI_OPTIONDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "ipctrl.h"

QT_BEGIN_NAMESPACE

class Ui_OptionDlg
{
public:
    QLabel *labelip;
    QLineEdit *portnum;
    IPCtrl *ipaddr;
    QLabel *label_2;
    QLabel *labelip_2;
    QLabel *label_3;
    QLineEdit *width;
    QLabel *label_4;
    QLineEdit *height;
    QLabel *label_5;
    QPushButton *btnBrowse;
    QLabel *labelPath;
    QLineEdit *name;
    QPushButton *btnCancel;
    QPushButton *btnOK;

    void setupUi(QWidget *OptionDlg)
    {
        if (OptionDlg->objectName().isEmpty())
            OptionDlg->setObjectName(QString::fromUtf8("OptionDlg"));
        OptionDlg->resize(426, 352);
        labelip = new QLabel(OptionDlg);
        labelip->setObjectName(QString::fromUtf8("labelip"));
        labelip->setGeometry(QRect(40, 80, 68, 15));
        portnum = new QLineEdit(OptionDlg);
        portnum->setObjectName(QString::fromUtf8("portnum"));
        portnum->setGeometry(QRect(320, 120, 81, 21));
        ipaddr = new IPCtrl(OptionDlg);
        ipaddr->setObjectName(QString::fromUtf8("ipaddr"));
        ipaddr->setGeometry(QRect(170, 70, 231, 31));
        ipaddr->setFrameShape(QFrame::StyledPanel);
        ipaddr->setFrameShadow(QFrame::Raised);
        label_2 = new QLabel(OptionDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(40, 120, 68, 15));
        labelip_2 = new QLabel(OptionDlg);
        labelip_2->setObjectName(QString::fromUtf8("labelip_2"));
        labelip_2->setGeometry(QRect(40, 30, 101, 16));
        label_3 = new QLabel(OptionDlg);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(40, 160, 68, 15));
        width = new QLineEdit(OptionDlg);
        width->setObjectName(QString::fromUtf8("width"));
        width->setGeometry(QRect(170, 160, 81, 21));
        label_4 = new QLabel(OptionDlg);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(280, 160, 21, 21));
        height = new QLineEdit(OptionDlg);
        height->setObjectName(QString::fromUtf8("height"));
        height->setGeometry(QRect(320, 160, 81, 21));
        label_5 = new QLabel(OptionDlg);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(40, 200, 81, 16));
        btnBrowse = new QPushButton(OptionDlg);
        btnBrowse->setObjectName(QString::fromUtf8("btnBrowse"));
        btnBrowse->setGeometry(QRect(320, 199, 81, 21));
        labelPath = new QLabel(OptionDlg);
        labelPath->setObjectName(QString::fromUtf8("labelPath"));
        labelPath->setGeometry(QRect(80, 230, 321, 31));
        name = new QLineEdit(OptionDlg);
        name->setObjectName(QString::fromUtf8("name"));
        name->setGeometry(QRect(170, 20, 231, 31));
        btnCancel = new QPushButton(OptionDlg);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
        btnCancel->setGeometry(QRect(90, 290, 81, 41));
        btnOK = new QPushButton(OptionDlg);
        btnOK->setObjectName(QString::fromUtf8("btnOK"));
        btnOK->setGeometry(QRect(250, 290, 91, 41));

        retranslateUi(OptionDlg);

        QMetaObject::connectSlotsByName(OptionDlg);
    } // setupUi

    void retranslateUi(QWidget *OptionDlg)
    {
        OptionDlg->setWindowTitle(QCoreApplication::translate("OptionDlg", "\351\200\211\351\241\271", nullptr));
        labelip->setText(QCoreApplication::translate("OptionDlg", "IP\345\234\260\345\235\200\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("OptionDlg", "\347\253\257\345\217\243\345\217\267\357\274\232", nullptr));
        labelip_2->setText(QCoreApplication::translate("OptionDlg", "\347\233\270\346\234\272\345\220\215\347\247\260\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("OptionDlg", "\345\210\206\350\276\250\347\216\207\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("OptionDlg", "x", nullptr));
        label_5->setText(QCoreApplication::translate("OptionDlg", "\345\255\230\345\202\250\350\267\257\345\276\204\357\274\232", nullptr));
        btnBrowse->setText(QCoreApplication::translate("OptionDlg", "...", nullptr));
        labelPath->setText(QCoreApplication::translate("OptionDlg", "Desktop", nullptr));
        btnCancel->setText(QCoreApplication::translate("OptionDlg", "\345\217\226\346\266\210", nullptr));
        btnOK->setText(QCoreApplication::translate("OptionDlg", "\347\241\256\350\256\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OptionDlg: public Ui_OptionDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONDLG_H
