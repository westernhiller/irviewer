#ifndef IRXSAVER_H
#define IRXSAVER_H

#include <QThread>
#include <QImage>
#include <QFile>
#include <QMutex>

#include "irx.h"
#include "global.h"

class IrxSaver : public QThread
{
    Q_OBJECT

public:
    explicit IrxSaver(int frames = 1, QObject* parent = nullptr);
    ~IrxSaver() override;

signals:
    void irxSaved(QString);

protected:
    void run() override;

private:
    IRXHEADER       m_header;
    int             m_imagesize;
    int             m_frames;
    IRSETTINGS*     m_pSettings;
    QFile*          m_pFile;
    QList<QByteArray> m_dataList;
    QMutex          m_mutex;
    QString         m_filename;

public slots:
    void            addFrame(QByteArray);
    void            stopSaving();
};

#endif // IRXSAVER_H
