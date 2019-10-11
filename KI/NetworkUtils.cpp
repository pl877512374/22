#include "resource.h"
#include "NetworkUtils.h"
const int MAX_BLOCK = 10000;
int g_nConnecttingCount = 0;
const int NET_BUF_NUM = 1000;
char acRecvDataBuf[MAX_BLOCK];
char acRecvDataBuftmp[MAX_BLOCK];
int g_nNetRecvInd = 0;
char g_cNetRecvBuf[NET_BUF_NUM][10000];
int g_nNetRecvSize[NET_BUF_NUM];
int nRecvSizetmp = 0;
typedef struct TagStructNetBuf
{
	char m_acbuf[MAX_BLOCK * 10];
	unsigned int m_u32in;
	unsigned int m_u32out;
	unsigned int m_u32size;
}StructNetBuf;
StructNetBuf g_sNetBuf;
int xorflag = 0;
void NetworkUtils::RecvThreadProc()	// 接收线程处理函数
{
	int nRecvSize = 0;
	int l_n32TcpErr = 0;//0正常，1粘帧，2断帧
	while (true)
	{
		KI * p = new KI();
		QByteArray arr=p->tcpSocket->readAll();
		p->tcpSocket->waitForReadyRead(10);
		//acRecvDataBuf,MAX_BLOCK,0);
		if (nRecvSize < 0)
		{
			continue;
		}
		if (nRecvSize > MAX_BLOCK)
		{
			printf("nRecvSize > MAX_BLOCK");
			continue;
		}
		if ((g_sNetBuf.m_u32in + nRecvSize) >= MAX_BLOCK * 10)
		{
			memset(&g_sNetBuf, 0, sizeof(g_sNetBuf));//大于最大缓存，原来的数全清0
			continue;
		}
		memcpy(&g_sNetBuf.m_acbuf[g_sNetBuf.m_u32in], acRecvDataBuf, nRecvSize*sizeof(char));
		g_sNetBuf.m_u32in += nRecvSize;
		if (g_sNetBuf.m_u32in >= MAX_BLOCK * 10)
		{
			memset(&g_sNetBuf, 0, sizeof(g_sNetBuf));
			//memcpy(&g_sNetBuf.m_acbuf[g_sNetBuf.m_u32in],acRecvDataBuf,nRecvSize*sizeof(char));
			break;
		}
		while (g_sNetBuf.m_u32out < g_sNetBuf.m_u32in)
		{
			unsigned int l_u32reallen = 0;
			if (((unsigned char)g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out] == 0xff &&
				(unsigned char)g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out + 1] == 0xaa))
			{
				//计算包长度
				if ((unsigned char)g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out] == 0x02)
				{
					l_u32reallen = ((unsigned char)g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out + 4] << 24) |
						((unsigned char)g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out + 5] << 16) |
						((unsigned char)g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out + 6] << 8) |
						((unsigned char)g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out + 7] << 0);
					l_u32reallen = l_u32reallen + 9;
				}
				else if ((unsigned char)g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out] == 0xff)
				{
					l_u32reallen = ((unsigned char)g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out + 3] << 0) |
						((unsigned char)g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out + 2] << 8);
					l_u32reallen = l_u32reallen + 4;
				}
				else
				{
					g_sNetBuf.m_u32out++;
					continue;
				}

				//判断包长度与实际缓存长度之间的关系
				if (l_u32reallen <= (g_sNetBuf.m_u32in - g_sNetBuf.m_u32out + 1))
				{
					OnNetRecv(&g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out], l_u32reallen);
					if (xorflag == 1)   //校验失败
					{
						int i;
						for (i = 1; i < l_u32reallen; i++)
						{
							if ((g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out + i] == 0x02 && g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out + i + 1] == 0x02 && g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out + i + 2] == 0x02 && g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out + i + 3] == 0x02)
								|| (g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out + i] == 0xff && g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out + i + 1] == 0xff))
							{
								g_sNetBuf.m_u32out += i;
								//memset(&g_sNetBuf,0,sizeof(gti_sNetBuf));
								xorflag = 0;
								break;
							}
						}
						if (i == l_u32reallen && xorflag == 1)
						{
							g_sNetBuf.m_u32out += l_u32reallen;
						}
						xorflag = 0;
					}
					else
					{
						g_sNetBuf.m_u32out += l_u32reallen;
					}
				}
				else if (l_u32reallen >= MAX_BLOCK)
				{
					memset(&g_sNetBuf, 0, sizeof(g_sNetBuf));//大于最大缓存，原来的数全清0
					break;
				}
				else
				{
					break;
				}
			}
			else
			{
				for (int i = 0; i < nRecvSize; i++)
				{
					if ((unsigned char)g_sNetBuf.m_acbuf[g_sNetBuf.m_u32in - nRecvSize + i] == 0x03)
					{
						l_u32reallen = g_sNetBuf.m_u32in - nRecvSize + i + 1;
						OnNetRecv(&g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out], l_u32reallen);
						g_sNetBuf.m_u32out += l_u32reallen;
						break;
					}
				}
			}
		}
	}
	if ((g_sNetBuf.m_u32in + nRecvSize) >= MAX_BLOCK * 10)
	{
		memset(&g_sNetBuf, 0, sizeof(g_sNetBuf));//大于最大缓存，原来的数全清0
	}
	if (g_sNetBuf.m_u32out >= g_sNetBuf.m_u32in)
	{
		memset(&g_sNetBuf, 0, sizeof(g_sNetBuf));
	}
}
void NetworkUtils::OnNetRecv(char *pDataBuf, int nDataBufSize)
{
	if (nDataBufSize > 0)
	{
		if ((*pDataBuf) == 0x02 && (*(pDataBuf + 1)) != 0x02)
		{
			if (nDataBufSize > 25)
			{
				memcpy(g_cNetRecvBuf[g_nNetRecvInd], pDataBuf, nDataBufSize);
				g_nNetRecvSize[g_nNetRecvInd] = nDataBufSize;
				//PostMessage(WM_NET_RECV, g_nNetRecvInd);
				g_nNetRecvInd = (g_nNetRecvInd + 1) % NET_BUF_NUM;
			}
		}
		else
		{
			if (checkXor(pDataBuf, nDataBufSize) || (*pDataBuf) == 0x02)
			{
				memcpy(g_cNetRecvBuf[g_nNetRecvInd], pDataBuf, nDataBufSize);
				g_nNetRecvSize[g_nNetRecvInd] = nDataBufSize;
				//PostMessage(WM_NET_RECV, g_nNetRecvInd);
				g_nNetRecvInd = (g_nNetRecvInd + 1) % NET_BUF_NUM;
			}
			else
			{
				xorflag = 1;
			}
		}

	}
}
void NetworkUtils::Xor(char* sendbuf, int sendlen)
{
	int i = 0;
	char check = 0;
	char* p = sendbuf;
	int len;
	if (*p == (char)0x02)
	{
		p = p + 8;
		len = sendlen;
		for (i = 8; i < len; i++)
		{
			check ^= *p++;
		}
	}
	else if (*p == (char)0xff)
	{
		*p++;
		*p++;
		for (i = 2; i < sendlen - 4; i++)
		{
			check ^= *p++;
		}
		*p++ = 0x00;
		*p++ = check;
		*p++ = 0xEE;
		*p++ = 0xEE;
	}
	else
	{
		;
	}

}

bool NetworkUtils::checkXor(char* recvbuf, int recvlen)
{
	int i = 0;
	char check = 0;
	char * p = recvbuf;
	int len;//末尾两位无需校验
	if (*p == (char)0xff)
	{
		len = recvlen - 2;
		p++;
		p++;
		for (i = 0; i < len; i++)
		{
			check ^= *p;
			p++;
		}
		p++;
	}
	else if (*p == (char)0x02)
	{
		p = p + 8;
		len = recvlen - 9;
		for (i = 0; i < len; i++)
		{
			check ^= *p++;
		}
	}
	else
	{
		return false;
	}

	if (check == *p)
	{
		return true;
	}
	else
	{
		return false;
	}
}