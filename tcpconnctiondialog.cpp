#include "tcpconnctiondialog.h"
#include "ui_tcpconnctiondialog.h"

#include <QHostAddress>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QPushButton>
#include <QIntValidator>

TcpConnctionDialog::TcpConnctionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpConnctionDialog)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("新建客户端"));
    ui->LineEdit_LocalPort->setValidator(new QIntValidator(1,65535,this));//设置只能输入指定整数
    ui->LineEdit_TgtPort->setValidator(new QIntValidator(1,65535,this));//设置只能输入指定整数

    //查询本机IP
    QList<QHostAddress> t_list = QNetworkInterface::allAddresses();
    QStringList t_IpList;
    foreach (QHostAddress t_address, t_list)
    {
        if(t_address.protocol() == QAbstractSocket::IPv4Protocol)
            t_IpList.append(t_address.toString());
    }
    ui->LineEdit_LocalIP->setText(t_IpList.at(0));
    ui->LineEdit_TgtIP->setText(t_IpList.at(0));

    if(0 == ui->checkBox->checkState())
    {
        ui->LineEdit_LocalPort->setEnabled(true);
    }

    connect(ui->pushButton_OK,&QPushButton::clicked,this,&TcpConnctionDialog::slotSave);

    connect(ui->checkBox,SIGNAL(stateChanged(int)),this,SLOT(slotCheckChanged(int)));
}

TcpConnctionDialog::~TcpConnctionDialog()
{
    delete ui;
}

/**
 * @brief 当前界面上的连接信息
 * @return
 */
NM_TCP::TcpClientConnection TcpConnctionDialog::currConnection()
{
    NM_TCP::TcpClientConnection t_connect;

    t_connect.tgtIp = ui->LineEdit_TgtIP->text();
    t_connect.tgtPort = ui->LineEdit_TgtPort->text();
    t_connect.localIp = ui->LineEdit_LocalIP->text();
    t_connect.localPort = ui->LineEdit_LocalPort->text();
    t_connect.ret =ui->checkBox->checkState();

    return t_connect;
}

/**
 * @brief 确认创建客户端
 */
void TcpConnctionDialog::slotSave()
{
    if(ui->checkBox->checkState() ==2 )
    {
        ui->LineEdit_LocalPort->setEnabled(false);
        if(ui->LineEdit_TgtPort->text().isEmpty())
        {
            QMessageBox::warning(this,"错误","端口不能为空");
            return;
        }
    }
    else
    {
        if(ui->LineEdit_LocalPort->text().isEmpty() || ui->LineEdit_TgtPort->text().isEmpty())
        {
            QMessageBox::warning(this,"错误","端口不能为空");
            return;
        }
    }
    accept();
}

/**
 *@brief QCheckBox状态改变槽函数
 *@param
 *@author WIN-HBUPNA
 *@date
 */
void TcpConnctionDialog::slotCheckChanged(int)
{
    if(0 == ui->checkBox->checkState())
    {
        ui->LineEdit_LocalPort->setEnabled(true);
    }
    else
    {
        ui->LineEdit_LocalPort->setEnabled(false);
    }
}
