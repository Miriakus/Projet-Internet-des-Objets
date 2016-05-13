#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QWidget>
#include <QTcpSocket>
#include <QLineEdit>
#include <QPushButton>
#include "controlpannel.h"

class ClientWindow : public QWidget
{
    Q_OBJECT

    public:
        explicit ClientWindow(QWidget *parent = 0);
        QString m_IP;
        int m_port;
        QTcpSocket sock;
        QLineEdit *m_ipInput;
        QPushButton *m_connectButton;
        ControlPannel *m_controlPannel;

//    signals:

    public slots:
        void connectToTCP();
        void connectionOk();
};

#endif // CLIENTWINDOW_H
