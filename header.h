#ifndef HEADER_H
#define HEADER_H

#include <QObject>

namespace NM_TCP {

/**
 * @brief 创建TCP客户端需要的接口信息
 */
struct TcpClientConnection{
    QString tgtIp;       /*!< 服务器IP >*/
    QString tgtPort;     /*!<  >*/
    QString localIp;     /*!<  >*/
    QString localPort;   /*!<  >*/
    int ret;
};

struct TcpServerConnection{
    QString serverPort;
    QString serverIP;
};
}

#endif // HEADER_H
