#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>

#include "global.h"

class VideoThread : public QThread
{
    Q_OBJECT

public:
    explicit VideoThread(QObject* parent = nullptr);
    ~VideoThread() override;

signals:
    void videoSaved(QString);

protected:
    void run() override;

private:
    IRSETTINGS*     m_pSettings;
    cv::VideoWriter* m_pVideoSaver;
    int             m_framecnt;
    QString         m_videofile;
    QList<QImage >  m_imageList;
    QMutex          m_mutex;

    void            startRecording();
    void            stopRecording();

public slots:
    void record(bool);
    void addImage(QImage);
};

#endif // VIDEOTHREAD_H
