#ifndef IRXPARSER_H
#define IRXPARSER_H

#include <QThread>
#include <QImage>
#include <QMutex>
#include "global.h"

class IrxParser : public QThread
{
    Q_OBJECT
public:
    explicit IrxParser(QObject* parent = nullptr);
    ~IrxParser() override;

signals:
    void getImage(QImage);
    void stepBg();
    void bgReady();

protected:
    void run() override;

private:
    QMutex          m_mutex;
    IRSETTINGS*     m_pSettings;
    QList<QByteArray > m_headList;
    QList<QByteArray > m_imageList;
    QList<cv::Mat > m_smoothList;
    bool            m_bSubBg;
    int             m_nFramesBg;
    float*          m_pBackground;

    cv::Mat parse(QByteArray, QByteArray);
    cv::Mat smooth(cv::Mat&);

public slots:
    void parseImage(QByteArray, QByteArray);
    void startSubbg(int nFrames);
    void doneSubbg(bool bOK);
    void subtractBg(bool);
};

#endif // IRXPARSER_H
