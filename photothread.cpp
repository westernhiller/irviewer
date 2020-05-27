#include "photothread.h"
#include <QDateTime>
#include <QDebug>
#include <QStandardPaths>
#include "mainwindow.h"

PhotoThread::PhotoThread(QImage image, QObject* parent)
    : QThread(parent)
    , m_pSettings(nullptr)
    , m_image(image)
{
    if(parent)
    {
        MainWindow* pDlg = static_cast<MainWindow*>(parent);
        m_pSettings = pDlg->getSettings();
    }
}

PhotoThread::~PhotoThread()
{
    requestInterruption();
    quit();
    wait();
}

void PhotoThread::run()
{
    QString path = m_pSettings ? m_pSettings->path : QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    QDateTime timeNow = QDateTime::currentDateTime();
    QString time = timeNow.toString("yyyy-MM-dd-HH-mm-ss-zzz");
    QString filename = path + tr("/") + time + ".png";

    m_image.save(filename);

    emit photoSaved(filename);
}
