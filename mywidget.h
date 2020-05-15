#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent,QString serverIP,quint16 serverPort,quint16 localPort,int ret);
    ~MyWidget();

    quint16 getPort();//获取端口
    void receiveIndex(int);

signals:
    void sigUpdateClientTabText(QString,QString);

private slots:
    void on_pushButton_clicked();
    void on_buttonConnect_clicked();
    void on_pushButton_2_clicked();
    void slotCheckBoxChanged(int);
    void slotCloseAllClient();
private:
    Ui::MyWidget *ui;

    QTcpSocket *m_pTcpSocket;
    QString m_serverIP;
    quint16 m_serverPort;
    quint16 m_localPort;
    QString m_currIP_Port;
    int m_index;
};

#endif // MYWIDGET_H
