#ifndef CONTROLPANNEL_H
#define CONTROLPANNEL_H

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QLCDNumber>
#include <QTcpSocket>
#include <QByteArray>
#include <QJsonObject>
#include <QTextEdit>

class ControlPannel : public QWidget
{
    Q_OBJECT

    public:
        explicit ControlPannel(QTcpSocket *socket, QWidget *parent = 0);
        QSlider *m_sliderFreq;
        QPushButton *m_buttonFreq;
        QLCDNumber *m_lcdNumber;
        QTcpSocket *m_sock;
        QPushButton *m_buttonInfos;
        QJsonObject *m_readBuffer;
        QTextEdit *m_infosCpu;
        QTextEdit *m_infosRam;
        QTextEdit *m_infosSwap;
        QTextEdit *m_infosDisk;
        QTextEdit *m_infosNetwork;
        QTextEdit *m_infosTime;
        void displayInfosCpu(QJsonObject cpu);
        void displayInfosRam(QJsonObject ram);
        void displayInfosSwap(QJsonObject swap);
        void displayInfosDisk(QJsonObject disk);
        void displayInfosNetwork(QJsonObject net);
        void displayInfosTime(QJsonObject time);


    signals:

    public slots:
        void changeFreq();
        void resServer();
        void reqInfos();

};

#endif // CONTROLPANNEL_H
