#pragma once
#include <QThread>
#include <QtNetwork/QtNetwork>
#include "ki.h"
#include "getlmpara.h"
#include "PCLFunc.h"
extern bool f_tcpNetConn;
QByteArray QString2Hex(QString str);
QString GetBCCXorCode(QString str);
char ConvertHexChar(char ch);
class workthread:public QThread
{
	Q_OBJECT
public:
	workthread(KI *Ck, PCLFunc *pclfun, QTcpSocket *tcpSocket);
	~workthread();
	bool DisconnectServer();
	void Sendzhiling(QByteArray send);
private:
	KI *Cki=NULL;
	PCLFunc *pclfun=NULL;
signals:
	void Draw_sig(int);
	void DetectTable_sig(int);
	void TcpConnInfo_sig(bool);
	void InfoView_sig(QString);
	void Enablebtn();//使主界面按钮恢复可用
public slots:
	void Draw_slot(int);
	void DetectTable_slot(int);
	void TcpConnInfo_slot(bool);
	void InfoView_slot(QString);
	void Enablebtn_slot();
protected:
	void run();
	void Xor(char* sendbuf, int sendlen);
	bool checkXor(char* recvbuf, int recvlen);
	void OnNetRecv(char *pDataBuf, int nDataBufSize);

};

