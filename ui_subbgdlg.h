/********************************************************************************
** Form generated from reading UI file 'subbgdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUBBGDLG_H
#define UI_SUBBGDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_SubbgDlg
{
public:
    QLabel *label;
    QSpinBox *spinBox;
    QLabel *label_2;
    QPushButton *btnOK;
    QPushButton *btnCancel;
    QProgressBar *progressBar;

    void setupUi(QDialog *SubbgDlg)
    {
        if (SubbgDlg->objectName().isEmpty())
            SubbgDlg->setObjectName(QString::fromUtf8("SubbgDlg"));
        SubbgDlg->resize(528, 372);
        QFont font;
        font.setPointSize(9);
        SubbgDlg->setFont(font);
        label = new QLabel(SubbgDlg);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(70, 30, 401, 81));
        QFont font1;
        font1.setPointSize(24);
        label->setFont(font1);
        spinBox = new QSpinBox(SubbgDlg);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setGeometry(QRect(280, 150, 71, 31));
        label_2 = new QLabel(SubbgDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(130, 140, 121, 51));
        btnOK = new QPushButton(SubbgDlg);
        btnOK->setObjectName(QString::fromUtf8("btnOK"));
        btnOK->setGeometry(QRect(100, 290, 141, 61));
        btnCancel = new QPushButton(SubbgDlg);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
        btnCancel->setGeometry(QRect(290, 290, 141, 61));
        progressBar = new QProgressBar(SubbgDlg);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(50, 210, 431, 41));
        progressBar->setValue(24);

        retranslateUi(SubbgDlg);

        QMetaObject::connectSlotsByName(SubbgDlg);
    } // setupUi

    void retranslateUi(QDialog *SubbgDlg)
    {
        SubbgDlg->setWindowTitle(QCoreApplication::translate("SubbgDlg", "\350\203\214\346\231\257\346\217\220\345\217\226", nullptr));
        label->setText(QCoreApplication::translate("SubbgDlg", "\350\257\267\347\241\256\350\256\244\351\225\234\345\244\264\347\233\226\345\267\262\347\233\226\345\245\275\357\274\201", nullptr));
        label_2->setText(QCoreApplication::translate("SubbgDlg", "\350\256\276\347\275\256\350\203\214\346\231\257\345\270\247\346\225\260\357\274\232", nullptr));
        btnOK->setText(QCoreApplication::translate("SubbgDlg", "\345\274\200\345\247\213\346\217\220\345\217\226\350\203\214\346\231\257", nullptr));
        btnCancel->setText(QCoreApplication::translate("SubbgDlg", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SubbgDlg: public Ui_SubbgDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUBBGDLG_H
