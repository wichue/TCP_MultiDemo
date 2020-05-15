#include "mywidget.h"
#include "ui_mywidget.h"

#include <QHostAddress>
#include <QHostInfo>
#include <QDebug>
#include <QNetworkInterface>
#include <QAbstractSocket>
#include <QMessageBox>

MyWidget::MyWidget(QWidget *parent,QString serverIP,quint16 serverPort,quint16 localPort,int ret) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);

    m_serverIP = serverIP;
    m_serverPort = serverPort;
    m_localPort = localPort;

    ui->pushButton_2->setEnabled(false);
    ui->pushButton->setEnabled(false);

    ui->textEditRead->document()->setMaximumBlockCount(100);//设置文本区最多显示100行
    ui->lineEditLocalPort->setValidator(new QIntValidator(1,65535,this));//设置只能输入指定整数
    ui->lineEditPort->setValidator(new QIntValidator(1,65535,this));//设置只能输入指定整数

//    QString t_localHostName = QHostInfo::localHostName();//获取主机名
//    qDebug() << t_localHostName;
    QList<QHostAddress> t_list = QNetworkInterface::allAddresses();
    QStringList t_IpList;
    foreach (QHostAddress t_address, t_list)
    {
        if(t_address.protocol() == QAbstractSocket::IPv4Protocol)
            t_IpList.append(t_address.toString());
    }
    ui->lineEditLocalIP->setText(t_IpList.at(0));//获取本机IP，显示在ui
    ui->lineEditIP->setText(m_serverIP);
    ui->lineEditPort->setText(QString::number(m_serverPort));
    ui->lineEditLocalPort->setText(QString::number(m_localPort));


    m_pTcpSocket = nullptr;
    m_pTcpSocket = new QTcpSocket(this);

    if(ret == 2)
    {
        ui->lineEditLocalPort->setEnabled(false);
        ui->checkBox->setCheckState(Qt::Checked);
    }

    connect(m_pTcpSocket,&QTcpSocket::connected,
            [=]()
                {
                    ui->textEditRead->append("和服务器连接成功！");
                    ui->pushButton_2->setEnabled(true);
                    ui->pushButton->setEnabled(true);
                    ui->buttonConnect->setEnabled(false);
                    ui->lineEditIP->setEnabled(false);
                    ui->lineEditPort->setEnabled(false);
                    ui->lineEditLocalIP->setEnabled(false);
                    ui->lineEditLocalPort->setEnabled(false);
                    ui->checkBox->setEnabled(false);
                }
            );
    //接收数据
    connect(m_pTcpSocket,&QTcpSocket::readyRead,
            [=]()
                {
                    QByteArray t_array = m_pTcpSocket->readAll();
                    ui->textEditRead->append(t_array);
                }
            );
    connect(m_pTcpSocket,&QTcpSocket::disconnected,
            [=]()
                {
                    ui->textEditRead->append("和服务器断开连接！");
                    ui->pushButton_2->setEnabled(false);
                    ui->pushButton->setEnabled(false);
                    ui->buttonConnect->setEnabled(true);
                    ui->lineEditIP->setEnabled(true);
                    ui->lineEditPort->setEnabled(true);
                    ui->lineEditLocalIP->setEnabled(true);

                    if(ui->checkBox->checkState() == 2)
                    {
                        ui->lineEditLocalPort->setEnabled(false);
                    }
                    else
                    {
                        ui->lineEditLocalPort->setEnabled(true);
                    }
                    ui->checkBox->setEnabled(true);
                }
            );
    //信息超出范围，自动滚动到最下方
    connect(ui->textEditRead,&QTextEdit::textChanged,
            [=]()
                {
                    ui->textEditRead->moveCursor(QTextCursor::End);
                }
            );
    //窗口关闭，自动断开连接
    connect(this,&MyWidget::destroyed,
            [=]()
                {
                    on_pushButton_2_clicked();
                }
            );
    
    connect(ui->checkBox,SIGNAL(stateChanged(int)),this,SLOT(slotCheckBoxChanged(int)));
}

MyWidget::~MyWidget()
{
    delete ui;
}

/**
 *@brief 获取端口
 *@param
 *@author WIN-HBUPNA
 *@date
 */
quint16 MyWidget::getPort()
{
    return ui->lineEditPort->text().toInt();
}

void MyWidget::receiveIndex(int t_index)
{
        m_index = t_index;
        qDebug() <<m_index;
        QString t_IP_Port = QString("%1:%2").arg(ui->lineEditIP->text()).arg(ui->lineEditPort->text());
        m_currIP_Port = QString::number(m_index) + t_IP_Port;
}

/**
 *@brief 发送数据按钮触发
 *@param
 *@author WIN-HBUPNA
 *@date
 */
void MyWidget::on_pushButton_clicked()
{
    if(nullptr == m_pTcpSocket)
    {
        return;
    }
    QString t_str = ui->textEditWrite->toPlainText();
    m_pTcpSocket->write(t_str.toUtf8().data());
}

/**
 *@brief 连接服务器按钮触发，指定端口被进程占用时，再次connectToHost就会使用随机端口
 *@param
 *@author WIN-HBUPNA
 *@date
 */
void MyWidget::on_buttonConnect_clicked()
{
    QString t_newIP_Port = QString::number(m_index) + QString("%1:%2").arg(ui->lineEditIP->text()).arg(ui->lineEditPort->text());
    emit sigUpdateClientTabText(t_newIP_Port,m_currIP_Port);
    m_currIP_Port = t_newIP_Port;


    if(ui->checkBox->checkState() == 2)
    {
        m_pTcpSocket->connectToHost(QHostAddress(ui->lineEditIP->text()),ui->lineEditPort->text().toInt());
        m_pTcpSocket->waitForConnected();
        if(m_pTcpSocket->localPort() == 0)
        {
            ui->textEditRead->append("连接失败，请检查服务器端口！");
            return;
        }
    }
    else
    {
        if(m_pTcpSocket->bind(ui->lineEditLocalPort->text().toInt()))
            qDebug() << "绑定成功";
        else
            qDebug() << "绑定失败";
        m_pTcpSocket->connectToHost(QHostAddress(ui->lineEditIP->text()),ui->lineEditPort->text().toInt());
        m_pTcpSocket->waitForConnected();
        qDebug() << m_pTcpSocket->localPort();

        m_pTcpSocket->connectToHost(QHostAddress(ui->lineEditIP->text()),ui->lineEditPort->text().toInt());
        m_pTcpSocket->waitForConnected();
        qDebug() << m_pTcpSocket->localPort();
        if(m_pTcpSocket->localPort() == 0)
        {
            ui->textEditRead->append("连接失败，请检查服务器端口！");
            return;
        }
        if(m_pTcpSocket->localPort() != ui->lineEditLocalPort->text().toInt())
        {
            QString t_str = QString("提示:指定端口被进程占用,当前使用随机端口连接,端口号：%1").arg(QString::number(m_pTcpSocket->localPort()));
            ui->textEditRead->append(t_str);
        }
    }
}

/**
 *@brief 断开当前连接按钮触发
 *@param 
 *@author WIN-HBUPNA
 *@date 
 */
void MyWidget::on_pushButton_2_clicked()
{
    if(nullptr == m_pTcpSocket)
    {
        return;
    }
    m_pTcpSocket->disconnectFromHost();
    m_pTcpSocket->waitForDisconnected();
    m_pTcpSocket->close();
}
/**
 *@brief checkBox状态改变槽函数
 *@param 
 *@author WIN-HBUPNA
 *@date 
 */
void MyWidget::slotCheckBoxChanged(int)
{
    if(ui->checkBox->checkState() == 2)
    {
        ui->lineEditLocalPort->setEnabled(false);
    }
    else
    {
        ui->lineEditLocalPort->setEnabled(true);
    }
}

/**
 *@brief 关闭所有客户端
 *@param
 *@author WIN-HBUPNA
 *@return
 */
void MyWidget::slotCloseAllClient()
{
    if(nullptr == m_pTcpSocket)
    {
        return;
    }
    m_pTcpSocket->disconnectFromHost();
    m_pTcpSocket->waitForDisconnected();
    m_pTcpSocket->close();
}
