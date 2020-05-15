#include "mymainwindow.h"
#include "ui_mymainwindow.h"

#include <QDialog>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QTcpServer>
#include <QMessageBox>
#include <QMap>

#include "tcpconnctiondialog.h"

MyMainWindow::MyMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyMainWindow)
{
    ui->setupUi(this);
    setWindowTitle("TCP测试工具");
    ui->buttonTest->hide();
    connect(this,&MyMainWindow::destroyed,
            [=]()
                {
                    on_buttonCloseClient_clicked();
                    on_buttonCloseServer_clicked();
                }
            );
    connect(ui->actionQuit,&QAction::triggered,
            [=]()
                {
                    on_buttonCloseClient_clicked();
                    on_buttonCloseServer_clicked();
                    this->close();
                }
            );
}

MyMainWindow::~MyMainWindow()
{
    delete ui;
}

/**
 *@brief 新建服务器
 *@param
 *@author WIN-HBUPNA
 *@return
 */
void MyMainWindow::on_buttonNewServer_clicked()
{
    newServerDialog *t_pNewServer = new newServerDialog(this);
    int ret = t_pNewServer->exec();
    if(ret == QDialog::Accepted)
    {
        NM_TCP::TcpServerConnection t_newServer = t_pNewServer->newTcpServer();
        tcpServerWin *tcpServer = new tcpServerWin(this,t_newServer.serverIP,t_newServer.serverPort.toInt());

        QString t_str = QString("%1:%2").arg(t_newServer.serverIP).arg(t_newServer.serverPort);
        int t_index = ui->tabWidgetServer->addTab(tcpServer,t_str);
        tcpServer->receiceServerIndex(t_index);
        QString t_strServer = QString::number(t_index) + t_str;
        m_mapServerTabIndex[t_index] = t_strServer;

        //关闭所有服务器
        connect(this,SIGNAL(sigCloseAllServer()),tcpServer,SLOT(slotCloseAllServer()));
        //IP和端口改变，修改Tab标签
        connect(tcpServer,SIGNAL(sigUpdateServerTabText(QString,QString)),this,SLOT(slotServerTabChanged(QString,QString)));
    }
}

/**
 *@brief 关闭所有服务器
 *@param
 *@author WIN-HBUPNA
 *@return
 */
void MyMainWindow::on_buttonCloseServer_clicked()
{
    ui->tabWidgetServer->clear();
    emit sigCloseAllServer();
}

/**
 *@brief 新建客户端
 *@param
 *@author WIN-HBUPNA
 *@return
 */
void MyMainWindow::on_buttonNewClient_clicked()
{
    TcpConnctionDialog t_clientConnection(this);
    int ret = t_clientConnection.exec();
    if(ret == QDialog::Accepted)
    {
        NM_TCP::TcpClientConnection t_connection = t_clientConnection.currConnection();

        MyWidget *tcpClient = new MyWidget(this,
                                           t_connection.tgtIp,
                                           t_connection.tgtPort.toInt(),
                                           t_connection.localPort.toInt(),
                                           t_connection.ret);
        QString t_str = QString("%1:%2").arg(t_connection.tgtIp).arg(t_connection.tgtPort);
        int t_index = ui->tabWidgetClient->addTab(tcpClient,t_str);


        QString t_strClient = QString::number(t_index) + t_str;
        m_mapClientTabIndex[t_index] = t_strClient;

        qDebug() << t_strClient;
        tcpClient->receiveIndex(t_index);
        connect(this,SIGNAL(sigCloseAllClient()),tcpClient,SLOT(slotCloseAllClient()));
        connect(tcpClient,SIGNAL(sigUpdateClientTabText(QString,QString)),this,SLOT(slotClientTabChanged(QString,QString)),Qt::QueuedConnection);
    }
}

/**
 *@brief 关闭所有客户端按钮
 *@param
 *@author WIN-HBUPNA
 *@return
 */
void MyMainWindow::on_buttonCloseClient_clicked()
{
    ui->tabWidgetClient->clear();
    emit sigCloseAllClient();
}

/**
 *@brief 服务器Tab标签改变槽函数
 *@param
 *@author WIN-HBUPNA
 *@return
 */
void MyMainWindow::slotServerTabChanged(QString t_newIP_Port, QString t_currIP_Port)
{
    for(QMap<int,QString>::iterator it = m_mapServerTabIndex.begin();it != m_mapServerTabIndex.end();it++)
    {
        if(it.value() == t_currIP_Port)
        {
            m_mapServerTabIndex[it.key()] = t_newIP_Port;
            ui->tabWidgetServer->setTabText(it.key(),t_newIP_Port.remove(0,1));
        }
    }
}

/**
 *@brief 客户端Tab标签改变槽函数
 *@param
 *@author WIN-HBUPNA
 *@return
 */
void MyMainWindow::slotClientTabChanged(QString t_newIP_Port, QString t_currIP_Port)
{
    for(QMap<int,QString>::iterator it = m_mapClientTabIndex.begin(); it != m_mapClientTabIndex.end(); it++)
    {
        if(it.value() == t_currIP_Port)
        {
            m_mapClientTabIndex[it.key()] = t_newIP_Port;
            ui->tabWidgetClient->setTabText(it.key(),t_newIP_Port.remove(0,1));
        }
    }
}

/**
 *@brief 测试按钮
 *@param
 *@author WIN-HBUPNA
 *@return
 */
void MyMainWindow::on_buttonTest_clicked()
{
    for(QMap<int,QString>::iterator it = m_mapClientTabIndex.begin(); it != m_mapClientTabIndex.end(); it++)
    {
        qDebug () <<it.key()<<":"<<it.value()<<endl;
    }
}
