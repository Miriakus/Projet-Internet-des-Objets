#include "controlpannel.h"
#include <QPushButton>
#include <QSlider>
#include <QLCDNumber>
#include <QTextStream>
#include <QTcpSocket>

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
    m_buttonFreq->setGeometry(160, 50, 150, 50);
    m_buttonFreq->show();

    connect(m_sliderFreq, SIGNAL(valueChanged(int)), m_lcdNumber, SLOT(display(int)));
    connect(m_buttonFreq, SIGNAL(clicked()), this, SLOT(changeFreq()));

}

void ControlPannel::changeFreq(){
    QTextStream request(m_sock);
    request << "CH_FREQ " << m_sliderFreq->value() << endl;
}

