#ifndef GLOBAL_H
#define GLOBAL_H

#include <QImage>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/videoio/videoio.hpp"

const int SWIRWIDTH = 1280;
const int SWIRHEIGHT = 1024;
const int FRAME_PMNUM =	16;
const int FRAME_PXCNT = SWIRWIDTH * SWIRHEIGHT;
const int FRAMEBUFSIZE = FRAME_PXCNT * 2 + 64;

typedef struct _framebuffer {
        uint16_t image[FRAME_PXCNT];
        uint32_t param[FRAME_PMNUM];
} PACKAGEBUFFER;
typedef PACKAGEBUFFER* LPPACKAGEBUFFER;

typedef struct
{
    QString path;
    QString name;
    QString ip;
    int port;
    int width;
    int height;
    int nGain;
    int nSubbg;
    bool bHighgain;
    bool bHistogram;
    bool bSmooth;
    bool bIntegral;
    bool bCorrection;
} IRSETTINGS;

void msleep(unsigned int msec);
QImage mat2qimage(const cv::Mat &mat);
cv::Mat qimage2mat(const QImage &qimage);

#endif // GLOBAL_H
