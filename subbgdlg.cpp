#include "subbgdlg.h"
#include "ui_subbgdlg.h"
#include "mainwindow.h"

SubbgDlg::SubbgDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SubbgDlg)
    , m_nProgress(0)
{
    if(parent)
        m_pSettings = static_cast<MainWindow*>(parent)->getSettings();

    ui->setupUi(this);

    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    ui->progressBar->setEnabled(false);
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(m_nProgress);
    ui->spinBox->setRange(1, 10000);
    ui->spinBox->setValue(m_pSettings? m_pSettings->nSubbg : 100);

    connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(onOk()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(onCancel()));
}

SubbgDlg::~SubbgDlg()
{
    delete ui;
}

void SubbgDlg::step()
{
    ui->progressBar->setValue(++m_nProgress);
    if(m_nProgress == m_pSettings->nSubbg)
    {
        emit doneSubbg(true);
        accept();
    }
}

void SubbgDlg::onOk()
{
    ui->btnOK->setEnabled(false);
    m_pSettings->nSubbg = ui->spinBox->text().toInt();
    m_nProgress = 0;
    ui->progressBar->setValue(m_nProgress);
    ui->spinBox->setEnabled(false);
    ui->progressBar->setEnabled(true);
    emit startSubbg(m_pSettings->nSubbg);
}

void SubbgDlg::onCancel()
{
    ui->progressBar->setEnabled(false);
    ui->btnOK->setEnabled(true);
    m_nProgress = 0;
    ui->progressBar->setValue(m_nProgress);
    ui->spinBox->setEnabled(true);
    emit doneSubbg(false);
}

