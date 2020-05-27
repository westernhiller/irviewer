﻿#include "controlpanel.h"
#include "ui_controlpanel.h"
#include <QStandardPaths>
#include <QFileDialog>
#include <QIntValidator>
#include <QRegExpValidator>
#include <QSettings>
#include <QDebug>

#include "global.h"
#include "mainwindow.h"
#include "ircapturer.h"

ControlPanel::ControlPanel(QWidget *parent)
    : QDialog(parent, Qt::Dialog | Qt::WindowMinimizeButtonHint)
    , ui(new Ui::ControlPanel)
    , m_bCameraConnected(false)
{
    if(parent)
        m_pSettings = static_cast<MainWindow*>(parent)->getSettings();
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("设置"));
    ui->sliderGain->setValue(5);
    ui->sliderGain->setRange(1, 10);
    ui->sliderGain->setTickInterval(1);
    ui->editCycle->setValidator(new QIntValidator(28, 5700, this));

    if(m_pSettings)
    {
        ui->sliderGain->setValue(m_pSettings->nGain);
        ui->cbSmooth->setCheckState(m_pSettings->bSmooth? Qt::Checked : Qt::Unchecked);
        ui->cbHistogram->setCheckState(m_pSettings->bHistogram? Qt::Checked : Qt::Unchecked);
        ui->cbHighgain->setCheckState(m_pSettings->bHighgain? Qt::Checked : Qt::Unchecked);
        ui->labelGain->setText(QString::number(m_pSettings->nGain));
    }

    enableItems(false);

    connect(ui->sliderGain, SIGNAL(valueChanged(int)), this, SLOT(gainChanged(int)));
    connect(ui->btnSetIntegral, SIGNAL(clicked()), this, SLOT(onSetIntegral()));
    connect(ui->btnSetCycle, SIGNAL(clicked()), this, SLOT(onSetCycle()));
    connect(ui->cbSmooth, SIGNAL(stateChanged(int)), this, SLOT(smoothCheckBoxChanged(int)));
    connect(ui->cbHighgain, SIGNAL(stateChanged(int)), this, SLOT(highgainCheckBoxChanged(int)));
    connect(ui->cbIntegral, SIGNAL(stateChanged(int)), this, SLOT(integralCheckBoxChanged(int)));
    connect(ui->cbHistogram, SIGNAL(stateChanged(int)), this, SLOT(histogramCheckBoxChanged(int)));
    connect(ui->cbNonuniform, SIGNAL(stateChanged(int)), this, SLOT(nonuniformCheckBoxChanged(int)));
    connect(ui->btnAdjustOnsite, SIGNAL(clicked()), this, SLOT(onAdjustOnsite()));

#ifdef WIN32
    QSettings settings("HKEY_CURRENT_USER\\Software\\irviewer", QSettings::NativeFormat);
#else
    QString pathDefault = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QSettings settings(pathDefault + "/.irviewer.ini", QSettings::NativeFormat);
#endif
    QRect rt = settings.value("control/geometry", "@Rect(0 0 435 652)").toRect();
    setGeometry(rt);
    setFixedSize(rt.width(), rt.height());
}

ControlPanel::~ControlPanel()
{
    delete ui;
}

void ControlPanel::keyPressEvent(QKeyEvent* event)
{
    int keyValue = event->key();
    if(keyValue & Qt::Key_Escape)
    {
    }
}

void ControlPanel::closeEvent(QCloseEvent *event)
{
#ifdef WIN32
    QSettings settings("HKEY_CURRENT_USER\\Software\\irviewer", QSettings::NativeFormat);
#else
    QString pathDefault = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QSettings settings(pathDefault + "/.irviewer.ini", QSettings::NativeFormat);
#endif
    settings.setValue("control/geometry", geometry());

    QDialog::closeEvent(event);
}

void ControlPanel::connected(bool bConnected)
{
    enableItems(bConnected);
}

void ControlPanel::enableItems(bool bEnable)
{
    ui->sliderGain->setEnabled(bEnable);
    ui->editCycle->setEnabled(bEnable);
    ui->cbSmooth->setEnabled(bEnable);
    ui->cbIntegral->setEnabled(bEnable);
    ui->cbNonuniform->setEnabled(bEnable);
    ui->cbHistogram->setEnabled(bEnable);
    ui->cbHighgain->setEnabled(bEnable);
    ui->editCycle->setEnabled(bEnable);
    ui->editIntegral->setEnabled(bEnable);
    ui->btnSetCycle->setEnabled(bEnable);
    ui->btnSetIntegral->setEnabled(bEnable);
    ui->btnAdjustOnsite->setEnabled(bEnable);
}

void ControlPanel::onAdjustOnsite()
{
    emit adjustOnsite();
}

void ControlPanel::gainChanged(int nGain)
{
    ui->labelGain->setText(QString::number(nGain));
    m_pSettings->nGain = nGain - 1;
}

void ControlPanel::setMode(uint32_t mode)
{
    UNION_REG value;
    value.data32 = mode;
    if((value.data8[0] & 0x04) == 0x04)
        ui->cbHighgain->setChecked(true);
    else {
        ui->cbHighgain->setChecked(false);
    }

    if((value.data8[0] & 0x10) == 0x10)
        ui->cbNonuniform->setChecked(true);
    else {
        ui->cbNonuniform->setChecked(false);
    }

    if((value.data8[1] & 0x02) == 0x02)
        ui->cbIntegral->setChecked(true);
    else {
        ui->cbIntegral->setChecked(false);
    }
}

void ControlPanel::setIntegral(double integral)
{
    ui->editIntegral->setText(QString::number(integral, 10, 3));
}

void ControlPanel::setFrameCycle(double cycle)
{
    ui->editCycle->setText(QString::number(cycle, 10, 3));
    ui->editIntegral->setValidator(new QDoubleValidator(0.1, cycle - 28, 10, this));
}

void ControlPanel::onSetIntegral()
{
    double value = ui->editIntegral->text().toDouble();

    emit updateIntegral(value);
}

void ControlPanel::onSetCycle()
{
    double value = ui->editCycle->text().toDouble();
    emit updateCycle(value);
}

void ControlPanel::integralCheckBoxChanged(int state)
{
    m_pSettings->bIntegral = state == Qt::Checked;
    emit enableIntegral(m_pSettings->bIntegral);
}

void ControlPanel::histogramCheckBoxChanged(int state)
{
    m_pSettings->bHistogram = state == Qt::Checked;
}

void ControlPanel::highgainCheckBoxChanged(int state)
{
    m_pSettings->bHighgain = state == Qt::Checked;
    emit enableHighgain(m_pSettings->bHighgain);
}

void ControlPanel::nonuniformCheckBoxChanged(int state)
{
    m_pSettings->bCorrection = state == Qt::Checked;
    emit enableNonuniform(m_pSettings->bCorrection);
}

void ControlPanel::smoothCheckBoxChanged(int state)
{
    m_pSettings->bSmooth = state == Qt::Checked;
}


