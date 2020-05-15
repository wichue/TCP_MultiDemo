#include "newserverdialog.h"
#include "ui_newserverdialog.h"

#include <QHostAddress>
#include <QNetworkInterface>
#include <QMessageBox>

newServerDialog::newServerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newServerDialog)
{
    ui->setupUi(this);
    setWindowTitle("创建服务器");
    ui->lineEditPort->setValidator(new QIntValidator(1,65535,this));//设置只能输入指定整数

    //查询本机IP
    QList<QHostAddress> t_list = QNetworkInterface::allAddresses();
    QStringList t_IpList;
    foreach (QHostAddress t_address, t_list)
    {
        if(t_address.protocol() == QAbstractSocket::IPv4Protocol)
            t_IpList.append(t_address.toString());
    }
    ui->lineEditIP->setText(t_IpList.at(0));

    connect(ui->buttonSure,SIGNAL(clicked(bool)),this,SLOT(slotSure()));
}

newServerDialog::~newServerDialog()
{
    delete ui;
}

/**
 *@brief 结构体保存界面输入内容
 *@param
 *@author WIN-HBUPNA
 *@return
 */
NM_TCP::TcpServerConnection newServerDialog::newTcpServer()
{
    NM_TCP::TcpServerConnection t_newServer;
    t_newServer.serverPort = ui->lineEditPort->text();
    t_newServer.serverIP = ui->lineEditIP->text();

    return t_newServer;
}

/**
 *@brief ui确认按钮做判断，触发accept
 *@param
 *@author WIN-HBUPNA
 *@return
 */
void newServerDialog::slotSure()
{
    if(ui->lineEditPort->text().isEmpty() || ui->lineEditIP->text().isEmpty())
    {
        QMessageBox::warning(this,"错误","端口和IP不能为空");
        return;
    }
    accept();
}
