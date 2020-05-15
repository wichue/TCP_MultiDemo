#include "server.h"

#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>

Server::Server(QObject *parent) : QTcpServer(parent)
{
}

/**
 *@brief 重写incomingConnection虚函数
 *@param
 *@author WIN-HBUPNA
 *@return
 */
void Server::incomingConnection(int socketDescriptor)
{
    QTcpSocket *t_pTcpSocket = new QTcpSocket(this);//有新连接时，新建一个tcpsocket
    t_pTcpSocket->setSocketDescriptor(socketDescriptor);//关联新建的通信套接字
    m_tcpClientSocketList.append(t_pTcpSocket);
    //获取客户端的IP和端口
    QString t_ip = t_pTcpSocket->peerAddress().toString();
    quint16 t_port = t_pTcpSocket->peerPort();
    QString t_str = QString("[%1:%2] 连接成功").arg(t_ip).arg(t_port);
    emit receiveDataSignal(t_str);

    connect(t_pTcpSocket,&QTcpSocket::readyRead,
            [=]()
                {
                    QByteArray t_array = t_pTcpSocket->readAll();
                    QString t_Msg = QString(t_array);
                    emit receiveDataSignal(t_Msg);
                }
    );
}

/**
 *@brief 发送数据
 *@param
 *@author WIN-HBUPNA
 *@return
 */
void Server::sendDataSignal(QString t_str)
{
    for(int i =0;i < m_tcpClientSocketList.count();i++)
    {
        QTcpSocket *t_pItem = m_tcpClientSocketList.at(i);
        t_pItem->write(t_str.toUtf8().data());
    }
}

/**
 *@brief 断开连接槽函数
 *@param
 *@author WIN-HBUPNA
 *@return
 */
void Server::disconnectSlot()
{
    for(int i =0;i < m_tcpClientSocketList.count();i++)
    {
        QTcpSocket *t_pItem = m_tcpClientSocketList.at(i);
        t_pItem->disconnectFromHost();
        t_pItem->close();
    }
}
