#include "analyzer.h"
#include <QLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include "mainwindow.h"

Analyzer::Analyzer(QWidget* parent)
    : QDialog(parent)
    , m_tooltip(nullptr)
    , m_pSettings(nullptr)
    , m_pBuffer(nullptr)
    , m_imageSize(0)
{
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    setWindowTitle(QStringLiteral("光校"));

    if(parent)
    {
        m_pSettings = static_cast<MainWindow*>(parent)->getSettings();
        m_imageSize = m_pSettings->width * m_pSettings->height;
        m_pBuffer = new uint16_t[m_imageSize];
    }

    QVBoxLayout* pLayout = new QVBoxLayout();
    QHBoxLayout* pControl = new QHBoxLayout();

    QLabel *pIdx = new QLabel(QStringLiteral("第"));
    pControl->addWidget(pIdx);
    pIdx->setFixedWidth(20);

    m_pSpinBox = new QSpinBox();
    m_pSpinBox->setRange(1, m_pSettings->width);
    m_pSpinBox->setValue(m_pSettings->width / 2);
    m_pSpinBox->setFixedWidth(80);
    pControl->addWidget(m_pSpinBox);
    connect(m_pSpinBox, SIGNAL(valueChanged(int)), this, SLOT(idxChanged(int)));

    QRadioButton *pRow = new QRadioButton(QStringLiteral("行"));
    QRadioButton *pCol = new QRadioButton(QStringLiteral("列"));
    pRow->setFixedWidth(80);
    pCol->setFixedWidth(80);

    connect(pRow, SIGNAL(clicked()), this, SLOT(chooseRowCol()));
    connect(pCol, SIGNAL(clicked()), this, SLOT(chooseRowCol()));

    m_groupButton = new QButtonGroup(this);
    m_groupButton->addButton(pRow, 0);
    m_groupButton->addButton(pCol, 1);
    pRow->setChecked(true); //默认选中行
    pControl->addWidget(pRow);
    pControl->addWidget(pCol);

/*
 *     QLabel* pColor = new QLabel();
    pColor->setStyleSheet(QString("background-color: yellow"));
    pColor->setFixedSize(16, 16);
    pControl->addWidget(pColor);
*/
    QPushButton* pbtnSnapshot = new QPushButton(QStringLiteral("快照"));
    pControl->addWidget(pbtnSnapshot);
    pbtnSnapshot->setFixedWidth(50);
    connect(pbtnSnapshot, SIGNAL(clicked()), this, SLOT(dumpImage()));

    pLayout->addLayout(pControl);

    QHBoxLayout* pStatistics = new QHBoxLayout();
    QLabel *pMax = new QLabel(QStringLiteral("最大值:"));
    m_pMax = new QLabel("0");
    QLabel *pMin = new QLabel(QStringLiteral("最小值:"));
    m_pMin = new QLabel("0");
    QLabel *pAvg = new QLabel(QStringLiteral("平均值:"));
    m_pAvg = new QLabel("0");
    QLabel *pStddev = new QLabel(QStringLiteral("均方根值:"));
    m_pStddev = new QLabel("0");

    pStatistics->addWidget(pMax);
    pStatistics->addWidget(m_pMax);
    pStatistics->addWidget(pMin);
    pStatistics->addWidget(m_pMin);
    pStatistics->addWidget(pAvg);
    pStatistics->addWidget(m_pAvg);
    pStatistics->addWidget(pStddev);
    pStatistics->addWidget(m_pStddev);
    pLayout->addLayout(pStatistics);

    createChart(QStringLiteral("图像分析第%1行").arg(m_pSettings->width / 2));

    m_pSeries = new QLineSeries();
    m_pSeries->setPen(QPen(Qt::yellow));
    m_pChart->addSeries(m_pSeries);
    m_pSeries->attachAxis(m_axisX);
    m_pSeries->attachAxis(m_axisY);
    m_pSeries->setPointLabelsVisible(false);
    connect(m_pSeries, SIGNAL(hovered(QPointF, bool)), this, SLOT(tooltip(QPointF, bool)));

    m_pChartView = new QChartView(m_pChart);
    pLayout->addWidget(m_pChartView);

    setLayout(pLayout);
#ifdef WIN32
    QSettings settings("HKEY_CURRENT_USER\\Software\\irviewer", QSettings::NativeFormat);
#else
    QString pathDefault = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QSettings settings(pathDefault + "/.irviewer.ini", QSettings::NativeFormat);
#endif
    setGeometry(settings.value("analyzer/geometry", "@Rect(137 125 615 520)").toRect());
}

Analyzer::~Analyzer()
{
    if(m_pBuffer)
        delete [] m_pBuffer;
}

void Analyzer::idxChanged(int idx)
{
    chooseRowCol();
}

void Analyzer::chooseRowCol()
{
    switch(m_groupButton->checkedId())
    {
        case 0:
            m_pChart->setTitle(QStringLiteral("图像分析第") + m_pSpinBox->text() + QStringLiteral("行"));
            m_pSpinBox->setRange(1, m_pSettings->width);
            m_axisX->setRange(0, m_pSettings->width);
            break;
        case 1:
            m_pChart->setTitle(QStringLiteral("图像分析第") + m_pSpinBox->text() + QStringLiteral("列"));
            m_pSpinBox->setRange(1, m_pSettings->height);
            m_axisX->setRange(0, m_pSettings->height);
            break;
    }
    analyze();
}

void Analyzer::keyPressEvent(QKeyEvent* event)
{
    int keyValue = event->key();
    if(keyValue & Qt::Key_Escape)
    {
    }
}

void Analyzer::closeEvent(QCloseEvent *event)
{
#ifdef WIN32
    QSettings settings("HKEY_CURRENT_USER\\Software\\irviewer", QSettings::NativeFormat);
#else
    QString pathDefault = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QSettings settings(pathDefault + "/.irviewer.ini", QSettings::NativeFormat);
#endif
    settings.setValue("analyzer/geometry", geometry());
    QDialog::closeEvent(event);
    emit finishAnalyzing();
}

void Analyzer::createChart(QString title)
{
    m_pChart = new QChart();
    m_pChart->legend()->hide();
    m_axisX = new QValueAxis;
    m_axisX->setTickCount(9);
    m_axisX->setMinorTickCount(1);
    m_axisY = new QValueAxis;
    m_axisY->setTickCount(17);
    m_axisY->setMinorTickCount(1);
    m_pChart->addAxis(m_axisX, Qt::AlignBottom);
    m_pChart->addAxis(m_axisY, Qt::AlignLeft);
    m_pChart->setTitle(title);
    m_pChart->setTheme(static_cast<QChart::ChartTheme>(2));

    m_axisX->setLabelFormat("%g");
    m_axisX->setTitleText(QStringLiteral("X轴"));
    m_axisX->setRange(0, m_pSettings->width);
    m_axisY->setRange(0, pow(2, 14));
    m_axisY->setTitleText(QStringLiteral("灰度值"));
}

void Analyzer::tooltip(QPointF point, bool state)
{
    if (m_tooltip == nullptr)
        m_tooltip = new Callout(m_pChart);

    if (state) {
        m_tooltip->setText(QString("(%1,%2)").arg(ceil(point.x())).arg(ceil(point.y())));
        m_tooltip->setAnchor(point);
        m_tooltip->setZValue(11);
        m_tooltip->updateGeometry();
        m_tooltip->show();
    } else {
        m_tooltip->hide();
    }
}

void Analyzer::dumpImage()
{
    QImage snapshot = imageFromChart();
    QDateTime timeNow = QDateTime::currentDateTime();
    QString time = timeNow.toString("yyyy-MM-dd-HH-mm-ss-zzz");
    QString path = m_pSettings ? m_pSettings->path : QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString filename = path + tr("/") + time + ".png";

    snapshot.save(filename);
}

QImage Analyzer::imageFromChart()
{
    QScreen* screen = QGuiApplication::primaryScreen();
    // same coordinate as geometry at line 196
    QRect rt = m_pChartView->geometry();
    QPixmap p = screen->grabWindow(winId(), rt.x(), rt.y(), rt.width(), rt.height());
    return p.toImage();
}

void Analyzer::updateImage(QByteArray image)
{
    memcpy(m_pBuffer, image.constData(), m_imageSize * 2);
    analyze();
}

void Analyzer::analyze()
{
    QVector<QPointF > testData;
    QVector<uint16_t> buffer;
    int idx = m_pSpinBox->text().toInt();
    if(m_bRow)
    {
        uint16_t* ptr = m_pBuffer + idx * m_pSettings->width;
        for(int j = 0; j < m_pSettings->width; j++)
            buffer.push_back(ptr[j]);

    } else {
        uint16_t* ptr = m_pBuffer + idx;
        for(int j = 0; j < m_pSettings->height; j++, ptr += m_pSettings->width)
            buffer.push_back(*ptr);
    }
    for(int i = 0; i < buffer.size(); i++)
        testData.push_back(QPointF(i, buffer[i]));
    m_pSeries->replace(testData);
    float vmax = 0;
    float vmin = pow(2, 16);
    float vsum = 0;
    for(int i = 0; i < buffer.size(); i++)
    {
        vsum += buffer[i];
        if(buffer[i] < vmin)
            vmin = buffer[i];
        else if(buffer[i] > vmax)
            vmax = buffer[i];
    }
    float vavg = vsum / buffer.size();
    vsum = 0;
    for(int i = 0; i < buffer.size(); i++)
    {
        float delta = vavg - buffer[i];
        vsum += delta * delta;
    }
    float vstddev = sqrt(vsum / buffer.size());
    m_pMax->setText(QString::number(vmax));
    m_pMin->setText(QString::number(vmin));
    m_pAvg->setText(QString::number(vavg));
    m_pStddev->setText(QString::number(vstddev));
}


