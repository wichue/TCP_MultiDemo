#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include "tcpserverwin.h"
#include "mywidget.h"
#include "newserverdialog.h"

#include "header.h"

namespace Ui {
class MyMainWindow;
}

class MyMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyMainWindow(QWidget *parent = 0);
    ~MyMainWindow();

    QMap<int,QString> m_mapServerTabIndex;
    QMap<int,QString> m_mapClientTabIndex;

private:

private slots:
    void on_buttonNewServer_clicked();
    void on_buttonCloseServer_clicked();
    void on_buttonNewClient_clicked();
    void on_buttonCloseClient_clicked();
    void slotServerTabChanged(QString,QString);
    void slotClientTabChanged(QString,QString);

    void on_buttonTest_clicked();

private:
    Ui::MyMainWindow *ui;

signals:
    void sigCloseAllServer();
    void sigCloseAllClient();
};

#endif // MYMAINWINDOW_H
