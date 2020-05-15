#include "tcpserverwin.h"
#include "ui_tcpserverwin.h"

#include <QHostInfo>
#include <QDebug>
#include <QNetworkInterface>

tcpServerWin::tcpServerWin(QWidget *parent,QString ip,quint16 port) :
    QMainWindow(parent),
    ui(new Ui::tcpServerWin)
{
    ui->setupUi(this);
    ui->statusBar->hide();
    ui->lineEditLocalIP->setText(ip);
    ui->lineEditPort->setText(QString::number(port));


    ui->buttonSend->setEnabled(false);
    ui->buttonClose->setEnabled(false);
    ui->buttonStop->setEnabled(false);

    ui->textEditRead->document()->setMaximumBlockCount(100);//接收信息最多显示100条
    //信息超出界面，自动滚动到最下方
    connect(ui->textEditRead,&QTextEdit::textChanged,
            [=]()
                {
                    ui->textEditRead->moveCursor(QTextCursor::End);
                }
            );
    ui->lineEditLocalIP->setText(ip);
    ui->lineEditPort->setText(QString::number(port));

    m_pTcpServer = new Server(this);

    connect(m_pTcpServer,SIGNAL(receiveDataSignal(QString)),this,SLOT(dealReceiveData(QString)));//接收数据显示到ui
    connect(this,SIGNAL(sendDataToClient(QString)),m_pTcpServer,SLOT(sendDataSignal(QString)));//发送数据
    connect(this,tcpServerWin::disconnectSignal,m_pTcpServer,&Server::disconnectSlot);//服务器断开当前连接

    connect(ui->buttonStart,SIGNAL(clicked(bool)),this,SLOT(slotStartServer()));//启动服务器
    connect(ui->buttonStop,SIGNAL(clicked(bool)),this,SLOT(slotStopServer()));//停止服务器
}
tcpServerWin::~tcpServerWin()
{
    delete ui;
}

/**
 *@brief 接收index
 *@param
 *@author WIN-HBUPNA
 *@return
 */
void tcpServerWin::receiceServerIndex(int t_index)
{
    m_index = t_index;
    m_currIP_Port =  QString::number(m_index) + QString("%1:%2").arg(ui->lineEditLocalIP->text()).arg(ui->lineEditPort->text());
}

/**
 *@brief ui发送数据按钮
 *@param
 *@author WIN-HBUPNA
 *@return
 */
void tcpServerWin::on_buttonSend_clicked()
{
    QString t_str = ui->textEditWrite->toPlainText();
    emit sendDataToClient(t_str);
}

/**
 *@brief 接收数据槽函数
 *@param
 *@author WIN-HBUPNA
 *@return
 */
void tcpServerWin::dealReceiveData(QString t_Msg)
{
    ui->textEditRead->append(t_Msg);
}

/**
 *@brief 关闭所有服务器槽函数
 *@param
 *@author WIN-HBUPNA
 *@return
 */
void tcpServerWin::slotCloseAllServer()
{
    m_pTcpServer->close();//停止监听
    emit disconnectSignal();
}

/**
 *@brief 断开当前所有服务器
 *@param
 *@author WIN-HBUPNA
 *@return
 */
void tcpServerWin::on_buttonClose_clicked()
{
    emit disconnectSignal();
}

/**
 *@brief ui启动服务器按钮
 *@param
 *@author WIN-HBUPNA
 *@return
 */
void tcpServerWin::slotStartServer()
{
    bool t_isOK = m_pTcpServer->listen(QHostAddress(ui->lineEditLocalIP->text()),ui->lineEditPort->text().toInt());//监听
    if(false == t_isOK)
    {
        ui->textEditRead->append("服务器监听失败，请检查IP和端口！");
        return;
    }
    else
    {
        ui->textEditRead->append("服务器启动成功！");
        ui->buttonStart->setEnabled(false);
        ui->buttonStop->setEnabled(true);
        ui->buttonSend->setEnabled(true);
        ui->buttonClose->setEnabled(true);
        ui->lineEditLocalIP->setEnabled(false);
        ui->lineEditPort->setEnabled(false);

        QString t_newIP_Port = QString::number(m_index) + QString("%1:%2").arg(ui->lineEditLocalIP->text()).arg(ui->lineEditPort->text());
        emit sigUpdateServerTabText(t_newIP_Port,m_currIP_Port);
        m_currIP_Port = t_newIP_Port;
    }
}

/**
 *@brief ui停止服务器
 *@param
 *@author WIN-HBUPNA
 *@return
 */
void tcpServerWin::slotStopServer()
{
    m_pTcpServer->close();//停止监听
    emit disconnectSignal();
    ui->textEditRead->append("服务器已断开!");

    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setEnabled(false);
    ui->buttonSend->setEnabled(false);
    ui->buttonClose->setEnabled(false);
    ui->lineEditLocalIP->setEnabled(true);
    ui->lineEditPort->setEnabled(true);
}
