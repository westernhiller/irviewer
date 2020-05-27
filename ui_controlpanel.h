/********************************************************************************
** Form generated from reading UI file 'controlpanel.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLPANEL_H
#define UI_CONTROLPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ControlPanel
{
public:
    QPushButton *btnSetIntegral;
    QCheckBox *cbHistogram;
    QLabel *label_5;
    QLabel *label_8;
    QCheckBox *cbIntegral;
    QPushButton *btnSetCycle;
    QLineEdit *editIntegral;
    QCheckBox *cbNonuniform;
    QLabel *labelGain;
    QCheckBox *cbHighgain;
    QCheckBox *cbSmooth;
    QLabel *label_3;
    QPushButton *btnAdjustOnsite;
    QSlider *sliderGain;
    QLineEdit *editCycle;

    void setupUi(QWidget *ControlPanel)
    {
        if (ControlPanel->objectName().isEmpty())
            ControlPanel->setObjectName(QString::fromUtf8("ControlPanel"));
        ControlPanel->resize(435, 293);
        btnSetIntegral = new QPushButton(ControlPanel);
        btnSetIntegral->setObjectName(QString::fromUtf8("btnSetIntegral"));
        btnSetIntegral->setGeometry(QRect(360, 250, 31, 20));
        cbHistogram = new QCheckBox(ControlPanel);
        cbHistogram->setObjectName(QString::fromUtf8("cbHistogram"));
        cbHistogram->setGeometry(QRect(30, 80, 181, 18));
        label_5 = new QLabel(ControlPanel);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(70, 250, 101, 16));
        label_8 = new QLabel(ControlPanel);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(33, 20, 61, 16));
        cbIntegral = new QCheckBox(ControlPanel);
        cbIntegral->setObjectName(QString::fromUtf8("cbIntegral"));
        cbIntegral->setGeometry(QRect(200, 80, 191, 18));
        btnSetCycle = new QPushButton(ControlPanel);
        btnSetCycle->setObjectName(QString::fromUtf8("btnSetCycle"));
        btnSetCycle->setGeometry(QRect(360, 210, 31, 20));
        editIntegral = new QLineEdit(ControlPanel);
        editIntegral->setObjectName(QString::fromUtf8("editIntegral"));
        editIntegral->setGeometry(QRect(240, 250, 111, 20));
        editIntegral->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        editIntegral->setReadOnly(false);
        cbNonuniform = new QCheckBox(ControlPanel);
        cbNonuniform->setObjectName(QString::fromUtf8("cbNonuniform"));
        cbNonuniform->setGeometry(QRect(30, 120, 151, 18));
        labelGain = new QLabel(ControlPanel);
        labelGain->setObjectName(QString::fromUtf8("labelGain"));
        labelGain->setGeometry(QRect(360, 20, 31, 20));
        labelGain->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        cbHighgain = new QCheckBox(ControlPanel);
        cbHighgain->setObjectName(QString::fromUtf8("cbHighgain"));
        cbHighgain->setGeometry(QRect(200, 120, 151, 20));
        cbSmooth = new QCheckBox(ControlPanel);
        cbSmooth->setObjectName(QString::fromUtf8("cbSmooth"));
        cbSmooth->setGeometry(QRect(30, 160, 121, 18));
        label_3 = new QLabel(ControlPanel);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(70, 210, 91, 16));
        btnAdjustOnsite = new QPushButton(ControlPanel);
        btnAdjustOnsite->setObjectName(QString::fromUtf8("btnAdjustOnsite"));
        btnAdjustOnsite->setGeometry(QRect(200, 150, 191, 31));
        sliderGain = new QSlider(ControlPanel);
        sliderGain->setObjectName(QString::fromUtf8("sliderGain"));
        sliderGain->setGeometry(QRect(30, 50, 361, 16));
        sliderGain->setMinimum(1);
        sliderGain->setMaximum(10);
        sliderGain->setPageStep(1);
        sliderGain->setOrientation(Qt::Horizontal);
        editCycle = new QLineEdit(ControlPanel);
        editCycle->setObjectName(QString::fromUtf8("editCycle"));
        editCycle->setGeometry(QRect(240, 210, 111, 20));
        editCycle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        editCycle->setReadOnly(false);

        retranslateUi(ControlPanel);

        QMetaObject::connectSlotsByName(ControlPanel);
    } // setupUi

    void retranslateUi(QWidget *ControlPanel)
    {
        ControlPanel->setWindowTitle(QCoreApplication::translate("ControlPanel", "Form", nullptr));
        btnSetIntegral->setText(QCoreApplication::translate("ControlPanel", "\350\256\276\347\275\256", nullptr));
        cbHistogram->setText(QCoreApplication::translate("ControlPanel", "\347\233\264\346\226\271\345\233\276\345\235\207\350\241\241", nullptr));
        label_5->setText(QCoreApplication::translate("ControlPanel", "\345\270\247\347\247\257\345\210\206\346\227\266\351\227\264(ms)", nullptr));
        label_8->setText(QCoreApplication::translate("ControlPanel", "\345\257\271\346\257\224\345\272\246", nullptr));
        cbIntegral->setText(QCoreApplication::translate("ControlPanel", "\345\274\200\345\220\257\350\207\252\345\212\250\345\270\247\347\247\257\345\210\206\346\227\266\351\227\264\350\260\203\346\225\264", nullptr));
        btnSetCycle->setText(QCoreApplication::translate("ControlPanel", "\350\256\276\347\275\256", nullptr));
        cbNonuniform->setText(QCoreApplication::translate("ControlPanel", "\345\274\200\345\220\257\351\235\236\345\235\207\345\214\200\346\200\247\346\240\241\346\255\243", nullptr));
        labelGain->setText(QCoreApplication::translate("ControlPanel", "4", nullptr));
        cbHighgain->setText(QCoreApplication::translate("ControlPanel", "\350\256\276\347\275\256\346\216\242\346\265\213\345\231\250\351\253\230\345\242\236\347\233\212", nullptr));
        cbSmooth->setText(QCoreApplication::translate("ControlPanel", "\345\274\200\345\220\257\345\244\232\345\270\247\345\271\263\346\273\221", nullptr));
        label_3->setText(QCoreApplication::translate("ControlPanel", "\345\270\247\345\221\250\346\234\237(ms)", nullptr));
        btnAdjustOnsite->setText(QCoreApplication::translate("ControlPanel", "\347\216\260\345\234\272\344\277\256\346\255\243\346\240\241\346\255\243\347\263\273\346\225\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ControlPanel: public Ui_ControlPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLPANEL_H
