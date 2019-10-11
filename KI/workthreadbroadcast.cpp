#include "workthreadbroadcast.h"
bool f_GetBroInfo = true;
QString IPAdress = "", Port="";
workthreadbroadcast::workthreadbroadcast(QUdpSocket * UDPClient, QByteArray zl_Brocast)
{
	this->UDPClient = UDPClient;
	this->zl_Brocast = zl_Brocast;
}
void workthreadbroadcast::run()
{
	f_GetBroInfo = true;
	UDPClient->writeDatagram(zl_Brocast, zl_Brocast.length(), QHostAddress::Broadcast, DEST_PORT);
	while (f_GetBroInfo)
	{
		processPengingDatagram();
	}
}
void workthreadbroadcast::processPengingDatagram()
{
	QByteArray datagram = NULL;
	datagram.resize(UDPClient->pendingDatagramSize());
	QString val = "";
	UDPClient->readDatagram(datagram.data(), datagram.size());
	UDPClient->waitForReadyRead(100);
	QString IP_First = QString::number((unsigned char)(datagram[26]), 10);
	QString IP_Second = QString::number((unsigned char)(datagram[27]), 10);
	QString IP_Third = QString::number((unsigned char)(datagram[28]), 10);
	QString IP_Fourth = QString::number((unsigned char)(datagram[29]), 10);
	IPAdress = IP_First + "." + IP_Second + "." + IP_Third + "." + IP_Fourth;
	int port = ((unsigned char)datagram[38] << 8) + (unsigned char)datagram[39];//移位运算符的优先级小于加减
	Port = QString::number(port, 10);
	if (port > 0 && IPAdress.length() > 10)
	{
		f_GetBroInfo = false;
		SetEvent(g_BroAngevent);
	}
}
workthreadbroadcast::~workthreadbroadcast()
{
}
