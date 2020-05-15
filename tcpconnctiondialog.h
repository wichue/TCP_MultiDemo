#ifndef TCPCONNCTIONDIALOG_H
#define TCPCONNCTIONDIALOG_H

#include <QDialog>

#include "header.h"

namespace Ui {
class TcpConnctionDialog;
}

class TcpConnctionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TcpConnctionDialog(QWidget *parent = 0);
    ~TcpConnctionDialog();

    NM_TCP::TcpClientConnection currConnection();

private slots:
    void slotSave();
    void slotCheckChanged(int);

private:
    Ui::TcpConnctionDialog *ui;
};

#endif // TCPCONNCTIONDIALOG_H
