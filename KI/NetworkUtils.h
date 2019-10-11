
#if !defined(__NETWORKUTILS_H)
#define __NETWORKUTILS_H
#include "ki.h"
#include <QtNetwork/QtNetwork>
class NetworkUtils
{
	// 网络连接结构体
	typedef struct tagNetConnection{
		int nID;
		char *pcSendDataBuf;
		int nSendDataBufSize;
		//结构体默认构造函数，初始化变量
	}NET_CONNECTION;
	//const int MAX_BLOCK = 10000;     //与串口SerialUtils.h中的定义重复，放stdafx.h中定义
	const int MAX_CONNECT_NUM = 10;		// 最大连接数
	void OnNetRecv(char *pDataBuf, int nDataBufSize);
	void RecvThreadProc();
	void Xor(char* pbuf, int len);
	bool checkXor(char* recvbuf, int recvlen);
};
#endif