#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMutex>
#include "glcanvas.h"
#include "analyzer.h"
#include "controlpanel.h"
#include "ircapturer.h"
#include "irxparser.h"
#include "imagebutton.h"
#include "irxloader.h"
#include "irxsaver.h"
#include "irxparser.h"
#include "photothread.h"
#include "videothread.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    inline IRSETTINGS* getSettings() { return &m_settings;}

signals:
    void camConnected(bool);
    void updateImage(QImage);
    void addVideoFrame(QImage);
    void saveFrame(QByteArray);
    void stopSaving();
    void subtractBg(bool);
    void parseFrame(QByteArray, QByteArray);
    void analyze(QByteArray);
    void record(bool);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QWidget*        m_pToolBar;
    GLCanvas*       m_pCanvas;
    QLabel*         m_labelStatus;
    IrxLoader*      m_pIrxLoader;
    IrxParser*      m_pIrxParser;
    VideoThread*    m_pVideoSaver;
    IRSETTINGS      m_settings;
    QAction *       m_pActAnalyze;
    QAction *       m_pActAbout;
    QAction *       m_pActSettings;
    QAction *       m_pActPlay;
    QAction *       m_pActConnect;
    QAction *       m_pActVideo;
    QAction *       m_pActRecord;
    QAction *       m_pActSubbg;
    QAction *       m_pActSave;
    QAction *       m_pActCapture;
    QToolBar *      m_pToolbar;
    Analyzer*       m_pAnalyzer;
    ControlPanel*   m_pControl;
    QByteArray      m_currFrame;
    QByteArray      m_header;
    bool            m_bConnected;
    bool            m_bPlaying;
    bool            m_bSavingPhoto;
    bool            m_bSavingVideo;
    int             m_nCapturers;
    QList<IrCapturer*> m_CapturerList;
    QMutex          m_mutex;
    int             m_nFrames;
    bool            m_bVideo;
    bool            m_bSubbg;
    bool            m_bBgReady;

    void            loadConfig();
    void            saveConfig();
    void            initGUI();
    void            connectCamera(bool);
    void            dispMessage(QString);

public slots:
    void            toggleAnalyze();
    void            toggleSettings();
    void            toggleConnect();
    void            toggleCapture();
    void            toggleRecord();
    void            togglePlay();
    void            toggleSave();
    void            toggleVideo();
    void            toggleSubbg();
    void            toggleAbout();
    void            connected(bool);
    void            bgReady();
    void            getFrame(QByteArray, QByteArray);
    void            captureFrame(QByteArray);
    void            getImage(QImage);
    void            getPixel(int, int);
    void            onIrxLoad(bool);
    void            onIrxSaved(QString);
    void            onPhotoSaved(QString);
    void            onVideoSaved(QString);
    void            killAnalyzer();
    void            load();
    void            option();
    void            onTimer();
};
#endif // MAINWINDOW_H
