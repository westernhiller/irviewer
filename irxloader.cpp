#include "irxloader.h"
#include <QDebug>
#include <QDateTime>

#include "mainwindow.h"

using namespace std;
using namespace cv;

IrxLoader::IrxLoader(QString filename, QObject* parent)
    : QThread(parent)
    , m_filename(filename)
    , m_pSettings(nullptr)
{
    if(parent)
    {
        MainWindow* pDlg = static_cast<MainWindow*>(parent);
        m_pSettings = pDlg->getSettings();
    }
}

IrxLoader::~IrxLoader()
{
    requestInterruption();
    quit();
    wait();
}

void IrxLoader::run()
{
    QFile inFile(m_filename);
    if(!inFile.open(QFile::ReadOnly))
    {
        qDebug() << "Could not open file for reading";
        emit irxLoaded(false);
        return;
    }

    const QByteArray in = inFile.readAll();
    inFile.close();

    QByteArray head = in.left(sizeof(IRXHEADER));
    IRXHEADER header;
    memcpy(&header, head.data(), sizeof(IRXHEADER));
    if((header.tag[0] != 'i') || (header.tag[1] != 'r') || (header.tag[2] != 'x'))
    {
        qDebug() << "File is not valid irx format.";
        emit irxLoaded(false);
        return;
    }

    if(m_pSettings)
    {
        m_pSettings->width = header.width;
        m_pSettings->height = header.height;
    }
    int start = sizeof(IRXHEADER);
    int imagesize = header.width * header.height * header.bytes;
    for(int i = 0; i < header.frames; i++, start += imagesize)
    {
        QByteArray buffer = in.mid(start, imagesize);
        if(buffer.size() != imagesize)
        {
            qDebug() << "File is damaged.";
            emit irxLoaded(false);
            return;
        }
        emit getFrame(head, buffer);
        usleep(30000);
    }

    emit irxLoaded(true);
}
