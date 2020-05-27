#ifndef ANALYZER_H
#define ANALYZER_H

#include <QDialog>
#include <QImage>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

#include "global.h"
#include "glcanvas.h"
#include "callout.h"

class Analyzer : public QDialog
{
    Q_OBJECT
public:
    explicit Analyzer(QWidget* parent = nullptr);
    ~Analyzer();

signals:
    void imageSaved(QString);
    void finishAnalyzing();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    QSpinBox*       m_pSpinBox;
    QLineEdit*      m_pImageName;
    QLineSeries*    m_pSeries;
    QChart  *       m_pChart;
    QChartView *    m_pChartView;
    QVector<QPointF > m_testData;
    QValueAxis *    m_axisX;
    QValueAxis *    m_axisY;
    Callout *       m_tooltip;
    IRSETTINGS*     m_pSettings;
    QLabel*         m_pMax;
    QLabel*         m_pMin;
    QLabel*         m_pAvg;
    QLabel*         m_pStddev;
    QButtonGroup*   m_groupButton;
    uint16_t*       m_pBuffer;
    bool            m_bRow;
    int             m_imageSize;

    void createChart(QString title);
    void analyze();
    QImage imageFromChart();

public slots:
    void tooltip(QPointF point, bool state);
    void updateImage(QByteArray);
    void idxChanged(int);
    void chooseRowCol();
    void dumpImage();
};

#endif // ANALYZEDIALOG_H
