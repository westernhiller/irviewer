#ifndef ABOUTDLG_H
#define ABOUTDLG_H

#include <QDialog>

class AboutDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDlg(QWidget *parent = nullptr);
    ~AboutDlg();

protected:
    void closeEvent(QCloseEvent *event) override;

private:

public slots:
    void onClose();
};

#endif // ABOUTDLG_H
