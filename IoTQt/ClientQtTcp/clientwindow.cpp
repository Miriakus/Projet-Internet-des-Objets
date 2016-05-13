#include "clientwindow.h"
#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include "controlpannel.h"


ClientWindow::ClientWindow(QWidget *parent) : QWidget(parent)
{
    m_controlPannel = new ControlPannel(&sock);
    this->setFixedSize(500, 300);
    m_ipInput = new QLineEdit(this);
    m_ipInput->setGeometry(50, 60, 200, 20);
    m_connectButton = new QPushButton("Connecter", this);
    m_connectButton->setGeometry(260, 60, 100, 20);
    m_ipInput->show();
    m_connectButton->show();

    connect(m_connectButton, SIGNAL(clicked()), this, SLOT(connectToTCP()));
    connect(&sock, SIGNAL(connected()), this, SLOT(connectionOk()));
}

void ClientWindow::connectToTCP(){
    m_IP = m_ipInput->text();
    sock.connectToHost(m_IP, 42000);
}

void ClientWindow::connectionOk(){
    QMessageBox::information(this, "Info", "Vous êtes maintenant connecté!");
    this->close();
    m_controlPannel->show();
}

