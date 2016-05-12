#include "controlpannel.h"
#include <QPushButton>
#include <QSlider>
#include <QLCDNumber>
#include <QTextStream>
#include <QTcpSocket>
#include <iostream>

ControlPannel::ControlPannel(QTcpSocket *sock, QWidget *parent) : QWidget(parent)
{

    m_sock = sock;
    this->setFixedSize(600, 500);
    m_lcdNumber = new QLCDNumber(this);
    m_lcdNumber->move(50, 20);
    m_lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    m_lcdNumber->show();

    m_sliderFreq = new QSlider(Qt::Horizontal, this);
    m_sliderFreq->setRange(100, 10000);
    m_sliderFreq->setGeometry(10, 60, 150, 20);
    m_sliderFreq->show();

    m_buttonFreq = new QPushButton(this);
    m_buttonFreq->setText("Changer la fréquence");
    m_buttonFreq->setGeometry(160, 50, 200, 50);
    m_buttonFreq->show();

    m_buttonInfos = new QPushButton(this);
    m_buttonInfos->setText("Infos système embarqué");
    m_buttonInfos->setGeometry(160, 100, 200, 50);
    m_buttonInfos->show();

    connect(m_sliderFreq, SIGNAL(valueChanged(int)), m_lcdNumber, SLOT(display(int)));
    connect(m_buttonFreq, SIGNAL(clicked()), this, SLOT(changeFreq()));
    connect(m_sock, SIGNAL(readyRead()), this, SLOT(resServer()));
    connect(m_buttonInfos, SIGNAL(clicked()), this, SLOT(reqInfos()));
}

void ControlPannel::changeFreq(){
    QTextStream request(m_sock);
    request << "CH_FREQ " << m_sliderFreq->value() << endl;
}

void ControlPannel::reqInfos(){
    QTextStream request(m_sock);
    request << "CAPT_JSON_INTERVAL" << endl;
}

void ControlPannel::resServer(){
    //*m_readBuffer = m_sock->readAll();
    //std::cout<<m_readBuffer<<endl;
}

