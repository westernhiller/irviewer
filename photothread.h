#ifndef PHOTOTHREAD_H
#define PHOTOTHREAD_H

#include <QThread>
#include <QImage>
#include <QMutex>

#include "global.h"

class PhotoThread : public QThread
{
    Q_OBJECT

public:
    explicit PhotoThread(QImage image, QObject* parent = nullptr);
    ~PhotoThread() override;

signals:
    void photoSaved(QString photoFile);

protected:
    void run() override;

private:
    IRSETTINGS*     m_pSettings;
    QImage          m_image;
};

#endif // PHOTOTHREAD_H
