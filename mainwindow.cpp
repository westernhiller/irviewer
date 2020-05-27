#include "mainwindow.h"
#include <QStandardPaths>
#include <QSettings>
#include <QLayout>
#include <QDebug>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QToolBar>
#include "optiondlg.h"
#include "subbgdlg.h"
#include "aboutdlg.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_pIrxLoader(nullptr)
    , m_pVideoSaver(nullptr)
    , m_pAnalyzer(nullptr)
    , m_bConnected(false)
    , m_bPlaying(true)
    , m_bSavingPhoto(false)
    , m_bSavingVideo(false)
    , m_nCapturers(5)
    , m_nFrames(0)
    , m_bVideo(false)
    , m_bSubbg(false)
    , m_bBgReady(false)
{
    initGUI();
    loadConfig();

#ifdef WIN32
    QSettings settings("HKEY_CURRENT_USER\\Software\\irviewer", QSettings::NativeFormat);
#else
    QString pathDefault = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QSettings settings(pathDefault + "/.irviewer.ini", QSettings::NativeFormat);
#endif
    restoreGeometry(settings.value("main/geometry").toByteArray());
    restoreState(settings.value("main/windowState").toByteArray());

    IRXHEADER header;
    header.width = m_settings.width;
    header.height = m_settings.height;
    header.bytes = 2;
    header.frames = 0;
    header.tag[0] = 'i';
    header.tag[1] = 'r';
    header.tag[2] = 'x';
    for(int i = 3; i < 8; i++)
        header.tag[i] = 0;
    m_header = QByteArray((const char*)&header, sizeof(IRXHEADER));

    m_pCanvas = new GLCanvas(this, QImage());
    setCentralWidget(m_pCanvas);

    connect(m_pCanvas, SIGNAL(pixelSelect(int, int)), this, SLOT(getPixel(int, int)));
    connect(this, SIGNAL(updateImage(QImage)), m_pCanvas, SLOT(updateImage(QImage)));

    m_pControl = new ControlPanel(this);
    connect(this, SIGNAL(camConnected(bool)), m_pControl, SLOT(connected(bool)));
    for(int i = 0; i < m_nCapturers; i++)
    {
        IrCapturer* pCapturer = new IrCapturer(this);
        connect(pCapturer, SIGNAL(getFrame(QByteArray)), this, SLOT(captureFrame(QByteArray)));
        if(i == 0)
        {
            connect(pCapturer, SIGNAL(cameraConncted(bool)), this, SLOT(connected(bool)));
            connect(pCapturer, SIGNAL(cameraConncted(bool)), m_pControl, SLOT(connected(bool)));
            connect(pCapturer, SIGNAL(updateMode(uint32_t)), m_pControl, SLOT(setMode(uint32_t)));
            connect(pCapturer, SIGNAL(updateIntegral(double)), m_pControl, SLOT(setIntegral(double)));
            connect(pCapturer, SIGNAL(updateCycle(double)), m_pControl, SLOT(setFrameCycle(double)));
            connect(m_pControl, SIGNAL(updateIntegral(double)), pCapturer, SLOT(setIntegral(double)));
            connect(m_pControl, SIGNAL(updateCycle(double)), pCapturer, SLOT(setCycle(double)));
            connect(m_pControl, SIGNAL(enableHighgain(bool)), pCapturer, SLOT(enableHighgain(bool)));
            connect(m_pControl, SIGNAL(enableNonuniform(bool)), pCapturer, SLOT(enableNonuniformityCorrection(bool)));
            connect(m_pControl, SIGNAL(enableIntegral(bool)), pCapturer, SLOT(enableIntegralAdjustion(bool)));
            connect(m_pControl, SIGNAL(adjustOnsite()), pCapturer, SLOT(adjustOnsite()));
        }
        m_CapturerList.push_back(pCapturer);
    }
    m_pIrxParser = new IrxParser(this);
    connect(this, SIGNAL(parseFrame(QByteArray, QByteArray)), m_pIrxParser, SLOT(parseImage(QByteArray, QByteArray)));
    connect(this, SIGNAL(subtractBg(bool)), m_pIrxParser, SLOT(subtractBg(bool)));
    connect(m_pIrxParser, SIGNAL(getImage(QImage)), this, SLOT(getImage(QImage)));
    connect(m_pIrxParser, SIGNAL(bgReady()), this, SLOT(bgReady()));
    m_pIrxParser->start();

    QTimer* t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(onTimer()));
    t->start(1000);
}

MainWindow::~MainWindow()
{
    for(int i = 0; i < m_nCapturers; i++)
        delete m_CapturerList[i];
    m_CapturerList.clear();

    if(m_pAnalyzer)
        delete m_pAnalyzer;
    if(m_pControl)
        delete m_pControl;
    if(m_pIrxLoader)
        delete m_pIrxLoader;
    if(m_pIrxParser)
        delete m_pIrxParser;
    if(m_pVideoSaver)
        delete m_pVideoSaver;
}

void MainWindow::initGUI()
{
    setWindowTitle(QStringLiteral("红外相机上位机软件"));
    statusBar()->addPermanentWidget(new QLabel(QStringLiteral("中国科学院上海技术物理研究所")));
    statusBar()->setStyleSheet("QStatusBar::item{border: 0px}");
    m_labelStatus  = new QLabel(this);
    statusBar()->addWidget(m_labelStatus);

    QAction *actOpen = new QAction(QPixmap::fromImage(QImage(":/icons/open.png")), QStringLiteral("打开"), this);
    actOpen->setShortcut(tr("CTRL+O"));
    QAction *actQuit = new QAction(QPixmap::fromImage(QImage(":/icons/power.png")), QStringLiteral("退出"), this);
    actQuit->setShortcut(tr("CTRL+Q"));

    QMenu *file = menuBar()->addMenu(QStringLiteral("文件"));
    file->addAction(actOpen);
    file->addSeparator();
    file->addAction(actQuit);
    connect(actOpen, SIGNAL(triggered()), this, SLOT(load()));
    connect(actQuit, SIGNAL(triggered()), this, SLOT(close()));

    QAction *actOption = new QAction(QPixmap::fromImage(QImage(":/icons/settings.png")), QStringLiteral("选项"), this);
    QMenu *tools = menuBar()->addMenu(QStringLiteral("工具"));
    tools->addAction(actOption);

    connect(actOption, SIGNAL(triggered()), this, SLOT(option()));

    m_pActAnalyze = new QAction(QPixmap::fromImage(QImage(":/icons/analyze.png")),QStringLiteral("光校"), this);
    m_pActAnalyze->setCheckable(true);
    m_pActAnalyze->setChecked(false);
    m_pActAnalyze->setEnabled(false);
    connect(m_pActAnalyze, SIGNAL(triggered()), this, SLOT(toggleAnalyze()));
    statusBar();

    m_pActSettings = new QAction(QPixmap::fromImage(QImage(":/icons/cam.png")), QStringLiteral("相机参数"), this);
    m_pActSettings->setCheckable(true);
    m_pActSettings->setChecked(false);
    m_pActSettings->setEnabled(false);
    connect(m_pActSettings, SIGNAL(triggered()), this, SLOT(toggleSettings()));

    QMenu *view = menuBar()->addMenu(QStringLiteral("视图"));
    view->addAction(m_pActAnalyze);
    view->addAction(m_pActSettings);

    m_pActAbout = new QAction(QPixmap::fromImage(QImage(":/icons/about.png")), QStringLiteral("关于"), this);
    QMenu *help = menuBar()->addMenu(QStringLiteral("帮助"));
    help->addAction(m_pActAbout);

    QToolBar *toolBar = new QToolBar();
//    toolBar->setMovable(false);     // 设置工具栏不可移动,默认是可移动
    toolBar->setIconSize(QSize(48, 48));
    addToolBar(toolBar);

    m_pActSave = new QAction(QPixmap::fromImage(QImage(":/icons/capture.png")), QStringLiteral("保存"), this);
    m_pActSubbg = new QAction(QPixmap::fromImage(QImage(":/icons/bg.png")), QStringLiteral("背景减"), this);
    m_pActVideo = new QAction(QPixmap::fromImage(QImage(":/icons/video.png")), QStringLiteral("保存多帧"), this);
    m_pActCapture = new QAction(QPixmap::fromImage(QImage(":/icons/photo.png")), QStringLiteral("快照"), this);
    m_pActRecord = new QAction(QPixmap::fromImage(QImage(":/icons/record.png")),QStringLiteral("录制视频"), this);
    m_pActConnect = new QAction(QPixmap::fromImage(QImage(":/icons/connect.png")),QStringLiteral("连接设备"), this);
    m_pActPlay = new QAction(QPixmap::fromImage(QImage(":/icons/play.png")), QStringLiteral("实时预览"), this);

    m_pActSave->setEnabled(false);
    m_pActSubbg->setEnabled(false);
    m_pActVideo->setEnabled(false);
    m_pActCapture->setEnabled(false);
    m_pActRecord->setEnabled(false);
    m_pActPlay->setEnabled(false);

    connect(actOption, SIGNAL(triggered()), this, SLOT(toggleSettings()));
    connect(m_pActConnect, SIGNAL(triggered()), this, SLOT(toggleConnect()));
    connect(m_pActPlay, SIGNAL(triggered()), this, SLOT(togglePlay()));
    connect(m_pActSubbg, SIGNAL(triggered()), this, SLOT(toggleSubbg()));
    connect(m_pActVideo, SIGNAL(triggered()), this, SLOT(toggleVideo()));
    connect(m_pActCapture, SIGNAL(triggered()), this, SLOT(toggleCapture()));
    connect(m_pActRecord, SIGNAL(triggered()), this, SLOT(toggleRecord()));
    connect(m_pActSave, SIGNAL(triggered()), this, SLOT(toggleSave()));
    connect(m_pActAbout, SIGNAL(triggered()), this, SLOT(toggleAbout()));

    toolBar->addAction(actOpen);
    toolBar->addSeparator();
    toolBar->addAction(actOption);
    toolBar->addSeparator();
    toolBar->addAction(m_pActConnect);
    toolBar->addAction(m_pActPlay);
    toolBar->addAction(m_pActSubbg);
    toolBar->addAction(m_pActSave);
    toolBar->addAction(m_pActVideo);
    toolBar->addSeparator();
    toolBar->addAction(m_pActCapture);
    toolBar->addAction(m_pActRecord);
    toolBar->addSeparator();
    toolBar->addAction(m_pActAnalyze);
    toolBar->addAction(m_pActSettings);
    toolBar->addAction(actQuit);
    toolBar->addAction(m_pActAbout);
}

void MainWindow::dispMessage(QString msg)
{
    m_labelStatus->setText(msg);
}

void MainWindow::killAnalyzer()
{
    m_pActAnalyze->setIcon(QPixmap::fromImage(QImage(":/icons/analyze.png")));
    m_pAnalyzer->hide();
}

void MainWindow::toggleAbout()
{
    AboutDlg dlg(this);
    dlg.exec();
}

void MainWindow::toggleAnalyze()
{
    if (m_pActAnalyze->isChecked())
    {
        if(!m_pAnalyzer)
        {
            m_pAnalyzer = new Analyzer(this);
            m_pActAnalyze->setIcon(QPixmap::fromImage(QImage(":/icons/analyzepressed.png")));
            connect(this, SIGNAL(analyze(QByteArray)), m_pAnalyzer, SLOT(updateImage(QByteArray)));
            connect(m_pAnalyzer, SIGNAL(finishAnalyzing()), this, SLOT(killAnalyzer()));
        }
        emit analyze(m_currFrame);
        m_pAnalyzer->show();
    }
    else
    {
        m_pActAnalyze->setIcon(QPixmap::fromImage(QImage(":/icons/analyze.png")));
        m_pAnalyzer->hide();
    }
}

void MainWindow::toggleSettings()
{
    if (m_pActSettings->isChecked()) {
        m_pActSettings->setIcon(QPixmap::fromImage(QImage(":/icons/campressed.png")));
        m_pControl->show();
    } else {
        m_pActSettings->setIcon(QPixmap::fromImage(QImage(":/icons/cam.png")));
        m_pControl->hide();
    }
}

void MainWindow::getImage(QImage image)
{
    // smooth
    m_nFrames++;

    if(m_bPlaying)
    {
        emit updateImage(image.copy());
        if(m_bSavingPhoto)
        {
            m_bSavingPhoto = false;
            PhotoThread* pPhotoSaver = new PhotoThread(image.copy(), this);
            connect(pPhotoSaver, SIGNAL(photoSaved(QString)), this, SLOT(onPhotoSaved(QString)));
            pPhotoSaver->start();
        }
        if(m_bSavingVideo)
            emit addVideoFrame(image.copy());
    }
}

void MainWindow::onPhotoSaved(QString filename)
{
    dispMessage(QStringLiteral("图像数据文件") + filename + QStringLiteral("已保存."));
    PhotoThread* pPhotoSaver = (PhotoThread*)sender();
    delete pPhotoSaver;
}

void MainWindow::bgReady()
{
    m_bBgReady = true;
    m_bSubbg = true;
    emit subtractBg(true);
    dispMessage(QStringLiteral("背景已提取"));
    m_pActSubbg->setIcon(QPixmap::fromImage(QImage(":/icons/bgpressed.png")));
}

void MainWindow::toggleSubbg()
{
    if(m_bSubbg)
    {
        m_bSubbg = false;
        emit subtractBg(false);
        m_pActSubbg->setIcon(QPixmap::fromImage(QImage(":/icons/bg.png")));
    }
    else
    {
        if(m_bBgReady)
        {
            m_bSubbg = true;
            emit subtractBg(true);
            m_pActSubbg->setIcon(QPixmap::fromImage(QImage(":/icons/bgpressed.png")));
            return;
        }

        SubbgDlg* pDlg = new SubbgDlg(this);
        connect(pDlg, SIGNAL(startSubbg(int)), m_pIrxParser, SLOT(startSubbg(int)));
        connect(pDlg, SIGNAL(doneSubbg(bool)), m_pIrxParser, SLOT(doneSubbg(bool)));
        connect(m_pIrxParser, SIGNAL(stepBg()), pDlg, SLOT(step()));
        pDlg->show();
    }
}

void MainWindow::onTimer()
{
    if(!m_bConnected)
        return;

    static int pos = 0;
    static int framebuffer[] = {0,0,0,0,0,0,0,0,0,0};

    m_mutex.lock();
    framebuffer[pos++] = m_nFrames;
    m_nFrames = 0;
    m_mutex.unlock();

    if(pos == 10)
        pos = 0;

    int nSum = 0;
    for(int i = 0; i < 10; i++)
        nSum += framebuffer[i];
    float fFrame = (float)nSum / 10.0f;

    statusBar()->showMessage(QString("FPS: %1").arg(fFrame));
}

void MainWindow::toggleSave()
{
    IrxSaver* pIrxSaver = new IrxSaver(1, this);
    connect(this, SIGNAL(saveFrame(QByteArray)), pIrxSaver, SLOT(addFrame(QByteArray)));
    connect(pIrxSaver, SIGNAL(irxSaved(QString)), this, SLOT(onIrxSaved(QString)));
    pIrxSaver->start();
    emit saveFrame(m_currFrame);
}

void MainWindow::onIrxSaved(QString filename)
{
    dispMessage(QStringLiteral("图像数据文件") + filename + QStringLiteral("已保存."));
    IrxSaver* pIrxSaver = (IrxSaver *)sender();
    delete pIrxSaver;
}

void MainWindow::toggleVideo()
{
    if(m_bVideo)
    {
        emit stopSaving();
        m_pActVideo->setIcon(QPixmap::fromImage(QImage(":/icons/video.png")));
        m_pActVideo->setToolTip(QStringLiteral("停止保存多帧数据"));
    }
    else
    {
        dispMessage(QStringLiteral("正在保存多帧图像数据..."));
        m_pActVideo->setToolTip(QStringLiteral("保存多帧数据"));
        m_pActVideo->setIcon(QPixmap::fromImage(QImage(":/icons/videopressed.png")));
        IrxSaver* pIrxSaver = new IrxSaver(1000, this);
        connect(this, SIGNAL(saveFrame(QByteArray)), pIrxSaver, SLOT(addFrame(QByteArray)));
        connect(this, SIGNAL(stopSaving()), pIrxSaver, SLOT(stopSaving()));
        connect(pIrxSaver, SIGNAL(irxSaved(QString)), this, SLOT(onIrxSaved(QString)));
        pIrxSaver->start();
        emit saveFrame(m_currFrame);
    }
    m_bVideo = !m_bVideo;
}

void MainWindow::onVideoSaved(QString filename)
{
    dispMessage(QStringLiteral("视频") + filename + QStringLiteral("已保存."));
}

void MainWindow::toggleRecord()
{
    if(!m_bSavingVideo)
    {
        if(!m_pVideoSaver)
        {
            m_pVideoSaver = new VideoThread(this);
            connect(this, SIGNAL(addVideoFrame(QImage)), m_pVideoSaver, SLOT(addImage(QImage)));
            connect(this, SIGNAL(record(bool)), m_pVideoSaver, SLOT(record(bool)));
            connect(m_pVideoSaver, SIGNAL(videoSaved(QString)), this, SLOT(onVideoSaved(QString)));
            m_pVideoSaver->start();
        }
        statusBar()->showMessage(QStringLiteral("正在录制视频..."));
        m_pActRecord->setIcon(QPixmap::fromImage(QImage(":/icons/stop.png")));
        m_pActRecord->setToolTip(QStringLiteral("停止录制视频"));
        emit record(true);
    }
    else
    {
        statusBar()->showMessage(QStringLiteral("视频已保存..."));
        m_pActRecord->setIcon(QPixmap::fromImage(QImage(":/icons/record.png")));
        m_pActRecord->setToolTip(QStringLiteral("录制视频"));
        emit record(false);
    }
    m_bSavingVideo = !m_bSavingVideo;
}

void MainWindow::toggleCapture()
{
    m_bSavingPhoto = true;
    statusBar()->showMessage(QStringLiteral("正在保存图像..."));
}

void MainWindow::togglePlay()
{
    if(m_bPlaying)
    {
        statusBar()->showMessage(QStringLiteral("停止预览"));
        m_pActPlay->setIcon(QPixmap::fromImage(QImage(":/icons/play.png")));
        m_pActSubbg->setEnabled(false);
        m_pActVideo->setEnabled(false);
        m_pActRecord->setEnabled(false);
    }
    else
    {
        statusBar()->showMessage(QStringLiteral("开始预览"));
        m_pActPlay->setIcon(QPixmap::fromImage(QImage(":/icons/pause.png")));
        m_pActSubbg->setEnabled(true);
        m_pActVideo->setEnabled(true);
        m_pActRecord->setEnabled(true);
    }
    m_mutex.lock();
    m_bPlaying = !m_bPlaying;
    m_mutex.unlock();
}

void MainWindow::load()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(QStringLiteral("调入红外图像数据"));
    fileDialog->setDirectory(m_settings.path);
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setNameFilter(QStringLiteral("红外图像数据 (*.irx)"));
    fileDialog->setViewMode(QFileDialog::Detail);
    if(fileDialog->exec() == QDialog::Accepted)
    {
        if(m_bConnected)
            toggleConnect();
        QString sFile = fileDialog->selectedFiles()[0];
        sFile.replace(QChar('/'), QChar('\\'));
        m_pIrxLoader = new IrxLoader(sFile, this);
        connect(m_pIrxLoader, SIGNAL(irxLoaded(bool)), this, SLOT(onIrxLoad(bool)));
        connect(m_pIrxLoader, SIGNAL(getFrame(QByteArray, QByteArray)), this, SLOT(getFrame(QByteArray, QByteArray)));
        m_pIrxLoader->start();
    }
}

void MainWindow::option()
{
    statusBar()->showMessage(QStringLiteral("设置选项"));
    OptionDlg dlg(this);
    dlg.exec();
}

void MainWindow::onIrxLoad(bool bOK)
{
    if(bOK)
    {
        statusBar()->showMessage(QStringLiteral("调入文件成功！"));
        m_pActAnalyze->setEnabled(true);
        m_pActSave->setEnabled(true);
    }
    else
    {
        statusBar()->showMessage(QStringLiteral("调入文件失败！"));
        QMessageBox::information(NULL, QStringLiteral("背景提取"), QStringLiteral("调入文件失败！"));
    }
    delete m_pIrxLoader;
    m_pIrxLoader = nullptr;
}

void MainWindow::toggleConnect()
{
    if(m_bConnected)
    {
        if(m_bPlaying)
            togglePlay();
        m_pActSave->setEnabled(false);
        m_pActSubbg->setEnabled(false);
        m_pActVideo->setEnabled(false);
        m_pActCapture->setEnabled(false);
        m_pActRecord->setEnabled(false);
        m_pActPlay->setEnabled(false);
    }
    else
    {
        statusBar()->showMessage(QStringLiteral("正在连接红外设备..."));
    }
    connectCamera(!m_bConnected);
}

void MainWindow::connected(bool bConnected)
{
    if(bConnected)
    {
        m_bPlaying = false;
        togglePlay();
        m_pActSave->setEnabled(true);
        m_pActSubbg->setEnabled(true);
        m_pActVideo->setEnabled(true);
        m_pActCapture->setEnabled(true);
        m_pActRecord->setEnabled(true);
        m_pActPlay->setEnabled(true);
        m_pActSettings->setEnabled(true);
        m_pActAnalyze->setEnabled(true);
        m_pActConnect->setIcon(QPixmap::fromImage(QImage(":/icons/disconnect.png")));
        m_pActConnect->setToolTip(QStringLiteral("断开设备连接"));
        statusBar()->showMessage(QStringLiteral("红外设备已连接"));
    }
    else {
        m_pActSubbg->setEnabled(false);
        m_pActVideo->setEnabled(false);
        m_pActRecord->setEnabled(false);
        m_pActPlay->setEnabled(false);
        m_pActSettings->setEnabled(false);
        if(m_currFrame.isEmpty())
        {
            m_pActSave->setEnabled(false);
            m_pActAnalyze->setEnabled(false);
            m_pActCapture->setEnabled(false);
        }
        if(m_bVideo)
            toggleVideo();
        if(m_bSavingVideo)
            toggleRecord();
        m_pActConnect->setIcon(QPixmap::fromImage(QImage(":/icons/connect.png")));
        m_pActConnect->setToolTip(QStringLiteral("连接设备"));
        statusBar()->showMessage(QStringLiteral("红外设备未连接"));
    }
    m_bConnected = bConnected;
    emit camConnected(m_bConnected);
}

void MainWindow::connectCamera(bool bConnect)
{
    for(int i = 0; i < m_nCapturers; i++)
    {
        if(bConnect)
            m_CapturerList[i]->start();
        else
            m_CapturerList[i]->stop();
    }
}

void MainWindow::getFrame(QByteArray header, QByteArray frame)
{
    m_currFrame = frame;
    emit parseFrame(header, frame);
}

void MainWindow::captureFrame(QByteArray frame)
{
    m_currFrame = frame;
    emit parseFrame(m_header, frame);
    if(m_bVideo)
        emit saveFrame(frame);
}


void MainWindow::getPixel(int x, int y)
{
    if(m_currFrame.isEmpty())
        return;

    uint16_t *ptr = (uint16_t *)(m_currFrame.constData());
    statusBar()->showMessage(QString("(%1, %2) = %3").arg(x).arg(y).arg(ptr[y * m_settings.width + x]));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
#ifdef WIN32
    QSettings settings("HKEY_CURRENT_USER\\Software\\irviewer", QSettings::NativeFormat);
#else
    QString pathDefault = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QSettings settings(pathDefault + "/.swirview.ini", QSettings::NativeFormat);
#endif
    settings.setValue("main/geometry", saveGeometry());
    settings.setValue("main/windowState", saveState());

    saveConfig();

    QMainWindow::closeEvent(event);
}

void MainWindow::loadConfig()
{
    QString pathDefault = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
#ifdef WIN32
    QSettings settings("HKEY_CURRENT_USER\\Software\\irviewer", QSettings::NativeFormat);
#else
    QSettings settings(pathDefault + "/.swirview.ini", QSettings::NativeFormat);
#endif
    settings.beginGroup("swir");
    m_settings.ip = settings.value("ipaddress", "192.168.1.10").toString();
    m_settings.port = settings.value("port", "2001").toInt();
    m_settings.width = settings.value("width", "1280").toInt();
    m_settings.height = settings.value("height", "1024").toInt();
    m_settings.name = settings.value("name", "").toString();
    m_settings.nGain = settings.value("gain", "4").toInt();
    m_settings.nSubbg = settings.value("subbg", "100").toInt();
    m_settings.bHighgain = settings.value("highgain", "false").toBool();
    m_settings.bSmooth = settings.value("smooth", "false").toBool();
    m_settings.bHistogram = settings.value("histogram", "false").toBool();
    m_settings.bIntegral = settings.value("integral", "false").toBool();
    m_settings.bCorrection = settings.value("correction", "false").toBool();
    settings.endGroup();

    settings.beginGroup("common");   
    m_settings.path = settings.value("path", pathDefault).toString();
#ifdef WIN32
    m_settings.path.replace(QChar('/'), QChar('\\'));
#endif
    settings.endGroup();
}

void MainWindow::saveConfig()
{
#ifdef WIN32
    QSettings settings("HKEY_CURRENT_USER\\Software\\irviewer", QSettings::NativeFormat);
#else
    QString pathDefault = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QSettings settings(pathDefault + "/.swirview.ini", QSettings::NativeFormat);
#endif
    settings.beginGroup("swir");
    settings.setValue("ipaddress", m_settings.ip);
    settings.setValue("port", m_settings.port);
    settings.setValue("width", m_settings.width);
    settings.setValue("height", m_settings.height);
    settings.setValue("gain", m_settings.nGain);
    settings.setValue("subbg", m_settings.nSubbg);
    settings.setValue("name", m_settings.name);
    settings.setValue("highgain", m_settings.bHighgain);
    settings.setValue("smooth", m_settings.bSmooth);
    settings.setValue("histogram", m_settings.bHistogram);
    settings.setValue("integral", m_settings.bIntegral);
    settings.setValue("correction", m_settings.bCorrection);
    settings.endGroup();

    settings.beginGroup("common");
    settings.setValue("path", m_settings.path);
    settings.endGroup();
}

