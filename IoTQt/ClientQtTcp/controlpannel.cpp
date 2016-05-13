#include "controlpannel.h"
#include <QPushButton>
#include <QSlider>
#include <QLCDNumber>
#include <QTextStream>
#include <QTcpSocket>
#include <iostream>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>
#include <QArrayData>


ControlPannel::ControlPannel(QTcpSocket *sock, QWidget *parent) : QWidget(parent)
{

    m_sock = sock;
    this->setFixedSize(1500, 500);
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

    m_infosCpu = new QTextEdit(this);
    m_infosCpu->setGeometry(10, 200, 150, 150);
    m_infosCpu->setReadOnly(true);
    m_infosCpu->show();

    m_infosRam = new QTextEdit(this);
    m_infosRam->setGeometry(160, 200, 150, 150);
    m_infosRam->setReadOnly(true);
    m_infosRam->show();

    m_infosSwap = new QTextEdit(this);
    m_infosSwap->setGeometry(310, 200, 150, 150);
    m_infosSwap->setReadOnly(true);
    m_infosSwap->show();

    m_infosDisk = new QTextEdit(this);
    m_infosDisk->setGeometry(460, 200, 150, 150);
    m_infosDisk->setReadOnly(true);
    m_infosDisk->show();

    m_infosNetwork = new QTextEdit(this);
    m_infosNetwork->setGeometry(610, 200, 150, 150);
    m_infosNetwork->setReadOnly(true);
    m_infosNetwork->show();

    m_infosTime = new QTextEdit(this);
    m_infosTime->setGeometry(760, 200, 150, 150);
    m_infosTime->setReadOnly(true);
    m_infosTime->show();


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

    QJsonDocument docJson(QJsonDocument::fromJson(QString(m_sock->readAll()).toUtf8()));
    QJsonObject objJson = docJson.object();
    QJsonObject cpu = objJson["cpu"].toObject();
    QJsonObject ram = objJson["ram"].toObject();
    QJsonObject swap = objJson["swap"].toObject();
    QJsonObject disk = objJson["disk"].toObject();
    QJsonObject net = objJson["network"].toObject();
    QJsonObject time = objJson["time"].toObject();

    displayInfosCpu(cpu);
    displayInfosRam(ram);
    displayInfosSwap(swap);
    displayInfosDisk(disk);
    displayInfosNetwork(net);
    displayInfosTime(time);
}

void ControlPannel::displayInfosCpu(QJsonObject cpu){
    int user = cpu["user"].toInt();
    int nice = cpu["nice"].toInt();
    int system = cpu["system"].toInt();
    int idle = cpu["idle"].toInt();
    int pcentUsed = cpu["pcentUsed"].toInt();
    m_infosCpu->setText("User :"+QString::number(user)+"\nNice :"+QString::number(nice)+"\nSystem :"+QString::number(system)+"\nIdle :"+QString::number(idle)+"\nPcentUsed :"+QString::number(pcentUsed));
    //m_infosCpu->append("Nice :"+QString::number(nice));
    //m_infosCpu->append("System :"+QString::number(system));
    //m_infosCpu->append("Idle :"+QString::number(idle));
    //m_infosCpu->append("PcentUsed :"+QString::number(pcentUsed));
}

void ControlPannel::displayInfosRam(QJsonObject ram){
    int total = ram["total"].toInt();
    int free = ram["free"].toInt();
    int buffers = ram["buffers"].toInt();
    int cached = ram["cached"].toInt();
    int used = ram["used"].toInt();
    int pcentUsed = ram["pcentUsed"].toInt();
    m_infosRam->setText("Total :"+QString::number(total)+"\nFree :"+QString::number(free)+"\nBuffers :"+QString::number(buffers)+"\nCached :"+QString::number(cached)+"\nUsed :"+QString::number(used)+"\nPcentUsed :"+QString::number(pcentUsed));
//    m_infosRam->append("Total :"+QString::number(total));
//    m_infosRam->append("Free :"+QString::number(free));
//    m_infosRam->append("Buffers :"+QString::number(buffers));
//    m_infosRam->append("Cached :"+QString::number(cached));
//    m_infosRam->append("Used :"+QString::number(used));
//    m_infosRam->append("PcentUsed :"+QString::number(pcentUsed));
}

void ControlPannel::displayInfosSwap(QJsonObject swap){
    int total = swap["total"].toInt();
    int free = swap["free"].toInt();
    int cached = swap["cached"].toInt();
    int used = swap["used"].toInt();
    int pcentUsed = swap["pcentUsed"].toInt();
    m_infosSwap->setText("Total :"+QString::number(total)+"\nFree :"+QString::number(free)+"\nCached :"+QString::number(cached)+"\nUsed :"+QString::number(used)+"\nPcentUsed :"+QString::number(pcentUsed));
//    m_infosSwap->append("Total :"+QString::number(total));
//    m_infosSwap->append("Free :"+QString::number(free));
//    m_infosSwap->append("Cached :"+QString::number(cached));
//    m_infosSwap->append("Used :"+QString::number(used));
//    m_infosSwap->append("PcentUsed :"+QString::number(pcentUsed));
}

void ControlPannel::displayInfosDisk(QJsonObject disk){
    int totalRead = disk["totalRead"].toInt();
    int totalWrite = disk["totalWrite"].toInt();
    int totalTimeActive = disk["totalTimeActive"].toInt();
    int debitRead= disk["debitRead"].toInt();
    int debitWrite = disk["totalWrite"].toInt();
    int pcentActive = disk["pcentActive"].toInt();
    m_infosDisk->setText("Total Read :"+QString::number(totalRead)+"\nTotal Write:"+QString::number(totalWrite)+"\nTotal Time Active:"+QString::number(totalTimeActive)+"\nDebit Read:"+QString::number(debitRead)+"\nDebit Write:"+QString::number(debitWrite)+"\nPcentActive:"+QString::number(pcentActive));
//    m_infosDisk->append("Total Read :"+QString::number(totalRead));
//    m_infosDisk->append("Total Write:"+QString::number(totalWrite));
//    m_infosDisk->append("Total Time Active:"+QString::number(totalTimeActive));
//    m_infosDisk->append("Debit Read:"+QString::number(debitRead));
//    m_infosDisk->append("Debit Write:"+QString::number(debitWrite));
//    m_infosDisk->append("PcentActive:"+QString::number(pcentActive));
}

void ControlPannel::displayInfosNetwork(QJsonObject net){
    int totalDown = net["totalDown"].toInt();
    int totalUp = net["totalUp"].toInt();
    int debitDown = net["debitDown"].toInt();
    int debitUp = net["debitUp"].toInt();
    m_infosNetwork->setText("Total Download :"+QString::number(totalDown)+"\nTotal Upload :"+QString::number(totalUp)+"\nDebit Download :"+QString::number(debitDown)+"\nDebit Upload :"+QString::number(debitUp));
//    m_infosNetwork->append("Total Download :"+QString::number(totalDown));
//    m_infosNetwork->append("Total Upload :"+QString::number(totalUp));
//    m_infosNetwork->append("Debit Download :"+QString::number(debitDown));
//    m_infosNetwork->append("Debit Upload :"+QString::number(debitUp));
}

void ControlPannel::displayInfosTime(QJsonObject time){
    int sec = time["sec"].toInt();
    int microsec = time["microsec"].toInt();
    m_infosTime->setText("Seconds :"+QString::number(sec)+"\nMicroseconds :"+QString::number(microsec));
//    m_infosTime->append("Seconds :"+QString::number(sec));
//    m_infosTime->append("Microseconds :"+QString::number(microsec));
}

