
#if !defined(__NETWORKUTILS_H)
#define __NETWORKUTILS_H
#include "ki.h"
#include <QtNetwork/QtNetwork>
class NetworkUtils
{
	// �������ӽṹ��
	typedef struct tagNetConnection{
		int nID;
		char *pcSendDataBuf;
		int nSendDataBufSize;
		//�ṹ��Ĭ�Ϲ��캯������ʼ������
	}NET_CONNECTION;
	//const int MAX_BLOCK = 10000;     //�봮��SerialUtils.h�еĶ����ظ�����stdafx.h�ж���
	const int MAX_CONNECT_NUM = 10;		// ���������
	void OnNetRecv(char *pDataBuf, int nDataBufSize);
	void RecvThreadProc();
	void Xor(char* pbuf, int len);
	bool checkXor(char* recvbuf, int recvlen);
};
#endif