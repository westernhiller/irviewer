#include "optiondlg.h"
#include "ui_optiondlg.h"
#include <QStandardPaths>
#include <QFileDialog>
#include <QIntValidator>
#include <QRegExpValidator>
#include <QSettings>
#include <QDebug>

#include "global.h"
#include "mainwindow.h"

OptionDlg::OptionDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionDlg)
{
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    if(parent)
        m_pSettings = static_cast<MainWindow*>(parent)->getSettings();

    ui->setupUi(this);
    setWindowTitle(QStringLiteral("选项"));
    ui->labelPath->setText(m_pSettings->path);

    if(m_pSettings)
    {
        ui->labelPath->setText(m_pSettings->path);
        ui->name->setText(m_pSettings->name);
        ui->width->setText(QString::number(m_pSettings->width));
        ui->height->setText(QString::number(m_pSettings->height));
        ui->ipaddr->setText(m_pSettings->ip);
        ui->portnum->setText(QString::number(m_pSettings->port));
    }

    connect(ui->btnBrowse, SIGNAL(clicked()), this, SLOT(onBrowse()));
    connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(onOK()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(onCancel()));

#ifdef WIN32
    QSettings settings("HKEY_CURRENT_USER\\Software\\irviewer", QSettings::NativeFormat);
#else
    QString pathDefault = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QSettings settings(pathDefault + "/.swirview.ini", QSettings::NativeFormat);
#endif
    QRect rt = settings.value("option/geometry", "@Rect(0 0 426 352)").toRect();
    setGeometry(rt);
    setFixedSize(rt.width(), rt.height());
}

OptionDlg::~OptionDlg()
{
    delete ui;
}

void OptionDlg::keyPressEvent(QKeyEvent* event)
{
    int keyValue = event->key();
    if(keyValue & Qt::Key_Escape)
    {
    }
}

void OptionDlg::closeEvent(QCloseEvent *event)
{
#ifdef WIN32
    QSettings settings("HKEY_CURRENT_USER\\Software\\irviewer", QSettings::NativeFormat);
#else
    QString pathDefault = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QSettings settings(pathDefault + "/.irviewer.ini", QSettings::NativeFormat);
#endif
    settings.setValue("option/geometry", geometry());

    QDialog::closeEvent(event);
}

void OptionDlg::onBrowse()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setDirectory(m_pSettings->path);
    fileDialog->setFileMode(QFileDialog::DirectoryOnly);
    if(fileDialog->exec() == QDialog::Accepted)
    {
        m_pSettings->path = fileDialog->selectedFiles()[0];
        ui->labelPath->setText(m_pSettings->path);
    }
}

void OptionDlg::onOK()
{
    m_pSettings->name = ui->name->text();
    m_pSettings->width = ui->width->text().toInt();
    m_pSettings->height = ui->height->text().toInt();
    m_pSettings->path = ui->labelPath->text();
    m_pSettings->port = ui->portnum->text().toInt();
    m_pSettings->ip = ui->ipaddr->text();

    close();
}

void OptionDlg::onCancel()
{
    close();
}


