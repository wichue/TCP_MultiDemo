#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QObject>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);//重写构造函数
    QList<QTcpSocket*> m_tcpClientSocketList;

protected:
    void incomingConnection(int socketDescriptor);//重写incomingConnection虚函数

signals:
    void receiveDataSignal(QString);

public slots:
    void sendDataSignal(QString);
    void disconnectSlot();

};

#endif // SERVER_H
