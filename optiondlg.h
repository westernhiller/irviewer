#ifndef OPTIONDLG_H
#define OPTIONDLG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class OptionDlg;
}

class OptionDlg : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDlg(QWidget *parent = nullptr);
    ~OptionDlg() override;

signals:

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::OptionDlg *ui;
    IRSETTINGS*   m_pSettings;

public slots:
    void onBrowse();
    void onOK();
    void onCancel();
};

#endif // OPTIONDLG_H
