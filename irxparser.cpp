#include "irxparser.h"
#include <QDebug>
#include <QDateTime>

#include "global.h"
#include "mainwindow.h"
#include "irx.h"

using namespace std;
using namespace cv;

#define SMOOTH_FRAME_NUM    5

IrxParser::IrxParser(QObject* parent)
    : QThread(parent)
    , m_bSubBg(false)
	, m_nFramesBg(0)
    , m_pBackground(nullptr)
{
    if(parent)
    {
        MainWindow* pDlg = static_cast<MainWindow*>(parent);
        m_pSettings = pDlg->getSettings();
    }
}

IrxParser::~IrxParser()
{
    requestInterruption();
    if(m_pBackground)
        delete[] m_pBackground;
    quit();
    wait();
}

void IrxParser::run()
{
    while(!isInterruptionRequested())
    {
        m_mutex.lock();
        if(!m_headList.isEmpty() && !m_imageList.isEmpty())
        {
            QByteArray head = m_headList.front();
            QByteArray image = m_imageList.front();
            m_headList.pop_front();
            m_imageList.pop_front();
            cv::Mat matImage = parse(head, image);
            if(matImage.data)
            {
                if(m_nFramesBg > 0)
                    emit stepBg();

                if(m_pSettings->bSmooth)
                    matImage = smooth(matImage);
                if(m_pSettings->bHistogram)
                    equalizeHist(matImage, matImage);

                emit getImage(mat2qimage(matImage));
            }
        }
        usleep(10000);
        m_mutex.unlock();
    }
}

cv::Mat IrxParser::smooth(cv::Mat& matImage)
{
    cv::Mat result;
    if(m_smoothList.size() == SMOOTH_FRAME_NUM)
    {
        cv::Mat frame = m_smoothList.front();
        frame.release();
        m_smoothList.pop_front();
    }
    m_smoothList.push_back(matImage);
    cv::Mat matAccumulate = cv::Mat::zeros(matImage.rows, matImage.cols, CV_32F);
    for(int i = 0; i < m_smoothList.size(); i++)
        cv::accumulate(m_smoothList[i], matAccumulate);
    matAccumulate /= m_smoothList.size();
    matAccumulate.convertTo(result, CV_8UC1);

    return result;
}

void IrxParser::parseImage(QByteArray head, QByteArray data)
{
    m_headList.push_back(head);
    m_imageList.push_back(data);
}

void IrxParser::startSubbg(int nFrames)
{
    m_nFramesBg = nFrames;
    int imagesize = m_pSettings->height * m_pSettings->width;
    if(!m_pBackground)
        m_pBackground = new float[imagesize];
    for(int i = 0; i < imagesize; i++)
        m_pBackground[i] = 0;
}

void IrxParser::subtractBg(bool bSubBg)
{
    m_bSubBg = bSubBg;
}

void IrxParser::doneSubbg(bool bOK)
{
    if(bOK)
    {
        int imagesize = m_pSettings->height * m_pSettings->width;
        for(int i = 0; i < imagesize; i++)
            m_pBackground[i] /= m_nFramesBg;
        emit bgReady();
    }
    m_nFramesBg = 0;
}

cv::Mat IrxParser::parse(QByteArray head, QByteArray data)
{
    Mat matFrame;
    static uint8_t unit[10] = {1, 2, 3, 4, 6, 8, 12, 16, 24, 32};
    double alpha = 1.0 / unit[m_pSettings->nGain];

    IRXHEADER header;
    memcpy(&header, head.data(), sizeof(IRXHEADER));
    int imagesize = header.width * header.height * header.bytes;
    if(data.size() < imagesize)
        return matFrame;

    matFrame.create(header.height, header.width, CV_8UC1);
    int pixelcount = header.width * header.height;
    uint16_t* buffer = new uint16_t[pixelcount];
    memcpy(buffer, data.data(), imagesize);

    if(m_nFramesBg > 0)
    {
        for(int k = 0; k < pixelcount; k++)
            m_pBackground[k] += buffer[k];
    }
    double acc = 0.0;
    for(int k = 0; k < pixelcount; k++)
        acc = acc + buffer[k] - (m_bSubBg ? m_pBackground[k] : 0);
    int avg= static_cast<int>(acc/pixelcount);
    int k = 0;
    for(int i = 0; i < header.height; i++)
    {
        uchar* ptr = static_cast<uchar*>(matFrame.ptr<uchar>(i));
        for(int j = 0; j < header.width; j++, k++)
            ptr[j] = saturate_cast<uchar>((buffer[k] - avg - (m_bSubBg ? m_pBackground[k] : 0))* alpha + 128);
    }
    delete[] buffer;

    return matFrame;
}

