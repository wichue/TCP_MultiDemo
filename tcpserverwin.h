#ifndef TCPSERVERWIN_H
#define TCPSERVERWIN_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

#include "server.h"

namespace Ui {
class tcpServerWin;
}

class tcpServerWin : public QMainWindow
{
    Q_OBJECT

public:
    explicit tcpServerWin(QWidget *parent,QString ip,quint16 port);
    ~tcpServerWin();
    QString m_currIP_Port;
    void receiceServerIndex(int);

private slots:
    void on_buttonSend_clicked();
    void on_buttonClose_clicked();
    void slotStartServer();
    void slotStopServer();

public slots:
    void dealReceiveData(QString);
    void slotCloseAllServer();

private:
    Ui::tcpServerWin *ui;
    Server *m_pTcpServer;
    int m_index;

signals:
    void sendDataToClient(QString);//发送信号给客户端
    void disconnectSignal();
    void sigUpdateServerTabText(QString,QString);
};

#endif // TCPSERVERWIN_H
