#include "aboutdlg.h"
#include <QLayout>
#include <QSettings>
#include <QPushButton>
#include "imagebutton.h"

AboutDlg::AboutDlg(QWidget *parent)
    : QDialog(parent)
{
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    QVBoxLayout* pLayout = new QVBoxLayout();
    ImageButton* sitp = new ImageButton(":/icons/sitp.png");
    sitp->setFixedSize(484, 60);
    pLayout->addWidget(sitp);
    QLabel* pInfo = new QLabel(QStringLiteral("红外相机上位机软件V1.0"));
    pInfo->setAlignment(Qt::AlignCenter);
    QLabel* pCopyright = new QLabel(QStringLiteral("@版权所有"));
    pCopyright->setAlignment(Qt::AlignCenter);
    pLayout->addSpacing(50);
    pLayout->addWidget(pInfo);
    pLayout->addSpacing(50);
    pLayout->addWidget(pCopyright);

    QHBoxLayout* pHLayout = new QHBoxLayout();
    QPushButton* pbtnClose = new QPushButton(QStringLiteral("关闭"));
    pHLayout->addSpacing(400);
    pHLayout->addWidget(pbtnClose);
    pLayout->addSpacing(50);
    pLayout->addLayout(pHLayout);
    setLayout(pLayout);

    connect(pbtnClose, SIGNAL(clicked()), this, SLOT(onClose()));
#ifdef WIN32
    QSettings settings("HKEY_CURRENT_USER\\Software\\irviewer", QSettings::NativeFormat);
#else
    QString pathDefault = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QSettings settings(pathDefault + "/.swirview.ini", QSettings::NativeFormat);
#endif
    QRect rt = settings.value("about/geometry", "@Rect(0 0 426 352)").toRect();
    setGeometry(rt);
    setFixedSize(rt.width(), rt.height());
}

AboutDlg::~AboutDlg()
{
}

void AboutDlg::onClose()
{
    accept();
}

void AboutDlg::closeEvent(QCloseEvent *event)
{
#ifdef WIN32
    QSettings settings("HKEY_CURRENT_USER\\Software\\irviewer", QSettings::NativeFormat);
#else
    QString pathDefault = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QSettings settings(pathDefault + "/.irviewer.ini", QSettings::NativeFormat);
#endif
    settings.setValue("about/geometry", geometry());

    QDialog::closeEvent(event);
}
