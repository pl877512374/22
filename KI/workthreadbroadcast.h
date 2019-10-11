#ifndef WORKTHREADBROADCAST_H
#define WORKTHREADBROADCAST_H
#include <QThread>
#include <QtNetwork/QtNetwork>
#include "ki.h"
#include <QObject>
extern  QString IPAdress;
extern QString Port;
extern bool f_GetBroInfo;
class workthreadbroadcast :public QThread
{
	Q_OBJECT

public:
	workthreadbroadcast(QUdpSocket * UDPClient, QByteArray zl_Brocast);
	~workthreadbroadcast();
private:
	void run();
	void processPengingDatagram();
	QUdpSocket * UDPClient;
	QByteArray zl_Brocast;
	KI *pKI;
	
};

#endif // WORKTHREADBROADCAST_H
