#include "irxsaver.h"
#include <QDebug>
#include <QDateTime>

#include "mainwindow.h"

IrxSaver::IrxSaver(int frames, QObject* parent)
    : QThread(parent)
    , m_frames(frames)
    , m_pFile(nullptr)
{
    if(parent)
    {
        MainWindow* pDlg = static_cast<MainWindow*>(parent);
        m_pSettings = pDlg->getSettings();
    }
    m_header.width = m_pSettings->width;
    m_header.height = m_pSettings->height;
    m_header.bytes = 2;
    m_header.frames = 0;
    m_header.tag[0] = 'i';
    m_header.tag[1] = 'r';
    m_header.tag[2] = 'x';
    for(int i = 3; i < 8; i++)
        m_header.tag[i] = 0;

    m_imagesize = m_header.width * m_header.height * m_header.bytes;
}

IrxSaver::~IrxSaver()
{
    requestInterruption();
    stopSaving();
    quit();
    wait();
}

void IrxSaver::stopSaving()
{
    if(m_pFile)
    {
        QDataStream out(m_pFile);
        m_pFile->seek(0);
        out.writeRawData((const char *)&m_header, sizeof(IRXHEADER));
        m_pFile->close();
        delete m_pFile;
        m_pFile = nullptr;
        if(!m_filename.isEmpty())
            emit irxSaved(m_filename);
    }
}

void IrxSaver::run()
{
    QString path = m_pSettings ? m_pSettings->path : QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    QDateTime timeNow = QDateTime::currentDateTime();
    QString time = timeNow.toString("yyyy-MM-dd-HH-mm-ss-zzz");
    m_filename = path + tr("/") + time + ".irx";

    m_pFile = new QFile(m_filename);
    if(!m_pFile->open( QIODevice::WriteOnly))
    {
        qDebug() << "Failed openning file " << m_filename << " for writing";
        return;
    }
    QDataStream out(m_pFile);
    m_header.frames = 0;
    out.writeRawData((const char *)&m_header, sizeof(IRXHEADER));

    while(!isInterruptionRequested())
    {
        if(!m_dataList.empty())
        {
            m_mutex.lock();
            QByteArray data = m_dataList.front();
            m_dataList.pop_front();
            m_mutex.unlock();
            out.writeRawData(data.constData(), m_imagesize);
            if(++m_header.frames == m_frames)
            {
                stopSaving();
                break;
            }
        }
        usleep(10000);
    }

//    emit irxSaved(m_filename);
}

void IrxSaver::addFrame(QByteArray data)
{
    if(data.size() < m_imagesize)
        return;

    m_mutex.lock();
    m_dataList.push_back(data);
    m_mutex.unlock();
}

