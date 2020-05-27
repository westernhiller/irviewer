#ifndef IRXLOADER_H
#define IRXLOADER_H

#include <QThread>
#include <QImage>
#include <QFile>
#include <QMutex>

#include "irx.h"
#include "global.h"

class IrxLoader : public QThread
{
    Q_OBJECT

public:
    explicit IrxLoader(QString filename, QObject* parent = nullptr);
    ~IrxLoader() override;

signals:
    void irxLoaded(bool);
    void getFrame(QByteArray, QByteArray);

protected:
    void run() override;

private:
    QString         m_filename;
    IRSETTINGS*     m_pSettings;

    void            parseImage(uint8_t*, IRXHEADER&);
};

#endif // IRXLOADER_H
