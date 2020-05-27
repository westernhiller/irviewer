#ifndef SUBBGDLG_H
#define SUBBGDLG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class SubbgDlg;
}

class SubbgDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SubbgDlg(QWidget *parent = nullptr);
    ~SubbgDlg();

signals:
    void startSubbg(int);
    void doneSubbg(bool);

private:
    Ui::SubbgDlg *ui;
    IRSETTINGS* m_pSettings;
    int     m_nProgress;

public slots:
    void onOk();
    void onCancel();
    void step();
};

#endif // SUBBGDLG_H
