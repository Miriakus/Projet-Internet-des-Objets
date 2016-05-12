#ifndef CONTROLPANNEL_H
#define CONTROLPANNEL_H

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QLCDNumber>
#include <QTcpSocket>
#include <QByteArray>

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
        QByteArray *m_readBuffer;


    signals:

    public slots:
        void changeFreq();
        void resServer();
        void reqInfos();

};

#endif // CONTROLPANNEL_H
