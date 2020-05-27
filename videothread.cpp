#include "videothread.h"
#include <QDateTime>
#include <QStandardPaths>
#include "mainwindow.h"

using namespace std;
using namespace cv;

VideoThread::VideoThread(QObject* parent)
    : QThread(parent)
    , m_pSettings(nullptr)
    , m_pVideoSaver(nullptr)
    , m_framecnt(0)
{
    if(parent)
    {
        MainWindow* pDlg = static_cast<MainWindow*>(parent);
        m_pSettings = pDlg->getSettings();
    }
}

VideoThread::~VideoThread()
{
    requestInterruption();
    stopRecording();
    quit();
    wait();
}

void VideoThread::run()
{
    while(!isInterruptionRequested())
    {
        m_mutex.lock();
        if(!m_imageList.isEmpty())
        {
            QImage image = m_imageList.front();
            m_imageList.pop_front();
            Mat matImg = qimage2mat(image);
            if(m_pVideoSaver)
                m_pVideoSaver->write(matImg);
            m_framecnt++;
        }
        m_mutex.unlock();
        usleep(10000);
    }
}

void VideoThread::record(bool bRecording)
{
    if(bRecording)
        startRecording();
    else
        stopRecording();
}

void VideoThread::startRecording()
{
    QString path = m_pSettings ? m_pSettings->path : QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    m_videofile = path + tr("/") + QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss") + tr(".avi");
    m_mutex.lock();
    m_pVideoSaver = new VideoWriter(m_videofile.toStdString(),
                                    VideoWriter::fourcc('M', 'J', 'P', 'G'),
//                                    VideoWriter::fourcc('M', 'P', '4','2'),
                                    25,
                                    Size(m_pSettings->width, m_pSettings->height),
                                    false);

    m_framecnt = 0;
    m_mutex.unlock();
}

void VideoThread::stopRecording()
{
    bool bfinished = false;
    m_mutex.lock();
    if(m_pVideoSaver)
    {
        delete m_pVideoSaver;
        m_pVideoSaver = nullptr;
        bfinished = true;
    }
    m_mutex.unlock();

    if(bfinished)
        emit videoSaved(m_videofile);
}

void VideoThread::addImage(QImage image)
{
    m_mutex.lock();
    m_imageList.push_back(image);
    m_mutex.unlock();
}
