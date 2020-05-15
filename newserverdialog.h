#ifndef NEWSERVERDIALOG_H
#define NEWSERVERDIALOG_H

#include <QDialog>
#include "header.h"

namespace Ui {
class newServerDialog;
}

class newServerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit newServerDialog(QWidget *parent = 0);
    ~newServerDialog();
    NM_TCP::TcpServerConnection newTcpServer();

private slots:
    void slotSure();
private:
    Ui::newServerDialog *ui;
};

#endif // NEWSERVERDIALOG_H
