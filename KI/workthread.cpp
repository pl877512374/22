#include "workthread.h"
#include "resource.h"
#define PI  3.14159265358979323846
typedef struct TagStructNetTemBuf
{
	char g_byteLstDistancePkgsScanDat[14400];
	char g_byteLstAnglePkgsScanDat[7200];
	char g_byteLstEnergyPkgsScanDat[7200];
	int g_32ScanPackNo[25];//��¼ÿһȦ�е�ÿһ���Ķ�Ӧ�İ���
	int g_n32ScanDatID[25];//��¼ÿһȦ��ÿһ����Ӧ��ID
	unsigned int m_u32in;
}StructNetTemBuf;
StructNetTemBuf g_sNetTemBuf;
const int MAX_BLOCK = 10000;
int g_nConnecttingCount = 0;
const int NET_BUF_NUM = 100;
const int NET_BUFCOL_NUM = 5000;
char acRecvDataBuf[MAX_BLOCK];//ֱ�Ӷ���ΪBYTE(unsigned char)����Ϳ���ʡ�Ե�&0xff����
char acRecvDataBuftmp[MAX_BLOCK];
int g_nNetRecvInd = 0;
char g_cNetRecvBuf[NET_BUF_NUM][NET_BUFCOL_NUM];
char g_nNetRecvSize[NET_BUF_NUM];
int nRecvSizetmp = 0;
int g_n32MaxDis = 70000;
int num_Target = 0;//�б�̽������
bool b_readfinish = false;//һ֡������û�ж������һ���ı�־
int g_n32NetRcvdPkgs = 0;
int g_n32CircleNo = 0;//Ȧ�ű������仯��Ȧ��ֵ��
QTcpSocket *tcpRecSocket;
extern QString host;//�豸��IP��ַ
extern int port;//�豸�Ķ˿ں�
float *d_landmark = NULL;//�б�̽�⡢��ȡ�б���Ϣ
float g_landmarkX[500] = { 0 };//�б��ȡ��ʾ�б�
float g_landmarkY[500] = { 0 };//�б��ȡ��ʾ�б�
float m_LaserPose[4] = { 0 };
int LMID[500] = { 0 };
bool f_addAngEnergy = false;//�Ƿ���Ҫ�ǶȲ�����־λ
int n_sigcountpack = 0;//����ͳ�ư���
int wrong_framenum = 0;//����֡��
int SZ_Serial = 0;
double ang_resratio;//�Ƕȷֱ���
float wrong_prop;//������
float g_n64ArrayZhiX[7200] = { 0 };
float g_n64ArrayZhiY[7200] = { 0 };
float g_n64ArrayZhiZ[7200] = { 0 };
float g_n64ArrayCompenAngle[7200] = { 0 }, g_n64ArrayEnergy[7200] = { 0 };//�ǶȲ������������ֵ����
int sum_framenum = 0;//��֡��
extern int n_scantime;
extern int get_LarerID;
extern int n_setpackage;
bool f_tcpNetConn = false;//TCP�����Ƿ����ӱ�־
typedef struct TagNetDataStruct
{
	char m_pcData[20000];
	int  m_n32Len;
	int m_n32BufCnt;
	int m_n32Channel;
}NetDataStruct;
typedef struct TagStructNetBuf
{
	char m_acbuf[MAX_BLOCK * 10];
	unsigned int m_u32in;
	unsigned int m_u32out;
	unsigned int m_u32size;
}StructNetBuf;
StructNetBuf g_sNetBuf;
NetDataStruct g_sNetData;
int xorflag = 0;
workthread::workthread(KI *Ck, PCLFunc *pclfun, QTcpSocket *tcpSocket)
{
	this->Cki = Ck;
	this->pclfun = pclfun;
	connect(this, SIGNAL(Draw_sig(int)), this, SLOT(Draw_slot(int)));
	connect(this, SIGNAL(DetectTable_sig(int)), this, SLOT(DetectTable_slot(int)));
	connect(this, SIGNAL(InfoView_sig(QString)), this, SLOT(InfoView_slot(QString)));
	connect(this, SIGNAL(Enablebtn()), this, SLOT(Enablebtn_slot()));
}
workthread::~workthread()
{
}
void workthread::run()
{
	tcpRecSocket = new QTcpSocket;
	connect(tcpRecSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connect_false(QAbstractSocket::SocketError)));
	connect(this, SIGNAL(TcpConnInfo_sig(bool)), this, SLOT(TcpConnInfo_slot(bool)));
	tcpRecSocket->connectToHost(host, port, QTcpSocket::ReadWrite); //��������
	if (tcpRecSocket->waitForConnected(100))//�����٣�������Ҫһ����ʱ��
	{
		f_tcpNetConn = true;
	}
	else
	{
		f_tcpNetConn = false;
	}
	emit TcpConnInfo_sig(f_tcpNetConn);
	int nRecvSize = 0;
	while (!isInterruptionRequested())
	{
		QByteArray arr = "";//if (tcpRecSocket->bytesAvailable() > 0)
		arr = tcpRecSocket->readAll();
		nRecvSize = arr.size();
		tcpRecSocket->waitForReadyRead(1);
		if (nRecvSize < 0)
		{
			continue;
		}
#pragma region ��һ���������
		if ((g_sNetBuf.m_u32in + nRecvSize) >= MAX_BLOCK * 10)
		{
			memset(&g_sNetBuf, 0, sizeof(g_sNetBuf));//������󻺴棬ԭ������ȫ��0
			continue;
		}
#pragma endregion
		memcpy(&g_sNetBuf.m_acbuf[g_sNetBuf.m_u32in], arr, nRecvSize*sizeof(char));
		g_sNetBuf.m_u32in += nRecvSize;
#pragma region �ڶ����������
		if (g_sNetBuf.m_u32in >= MAX_BLOCK * 10)
		{
			memset(&g_sNetBuf, 0, sizeof(g_sNetBuf));
			break;
		}
#pragma endregion
		while (g_sNetBuf.m_u32out < g_sNetBuf.m_u32in)
		{
			int l_u32reallen = 0;
			if ((((unsigned char)g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out] == 0xff &&
				(unsigned char)g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out + 1] == 0xaa)))
			{
				l_u32reallen = ((unsigned char)g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out + 3] << 0) |
					((unsigned char)g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out + 2] << 8);
				l_u32reallen += 4;                                             //һ֡���ݵ�ʵ�ʳ���Ӧ����֡ͷ֡β4���ֽ�
			}
			else
			{
				g_sNetBuf.m_u32out++;  //�Ұ�ͷ
			}
#pragma region �жϰ�������ʵ�ʻ��泤��֮��Ĺ�ϵ
			if (l_u32reallen <= (g_sNetBuf.m_u32in - g_sNetBuf.m_u32out + 1))
			{
				//���ݷ�֡����
				OnNetRecv(&g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out], l_u32reallen);//����һ֡����
				//1.У�鴦��
#pragma region ���У�����
				if (xorflag == 1)
				{
					int i;
					for (i = 1; i < l_u32reallen; i++)
					{
						if ((g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out + i] == 0x02 && g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out + i + 1] == 0x02 && g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out + i + 2] == 0x02 && g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out + i + 3] == 0x02)
							|| (g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out + i] == 0xff && g_sNetBuf.m_acbuf[g_sNetBuf.m_u32out + i + 1] == 0xff))
						{
							g_sNetBuf.m_u32out += i;
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
#pragma endregion
				else
					g_sNetBuf.m_u32out += l_u32reallen;
			}
#pragma endregion
#pragma region �������������
			else if (l_u32reallen >= MAX_BLOCK)
			{
				memset(&g_sNetBuf, 0, sizeof(g_sNetBuf));//������󻺴棬ԭ������ȫ��0
				continue;
			}
#pragma endregion
			else
			{
				break;
			}
		}
#pragma region ���Ĵ��������
		if ((g_sNetBuf.m_u32in + nRecvSize) >= MAX_BLOCK * 10)
		{
			//������󻺴棬ԭ������ȫ��0
			memset(&g_sNetBuf, 0, sizeof(g_sNetBuf));//������󻺴棬ԭ������ȫ��0
			continue;
		}
#pragma endregion
#pragma region ������������
		if (g_sNetBuf.m_u32out >= g_sNetBuf.m_u32in)//��ʱ֤��������������ݶ��Ѿ�������ɣ��������
		{
			//��ʱ֤��������������ݶ��Ѿ�������ɣ��������");
			memset(&g_sNetBuf, 0, sizeof(g_sNetBuf));//������󻺴棬ԭ������ȫ��0
		}
#pragma endregion
	}
}

#pragma region TCP��������״̬�ۺ���
void workthread::TcpConnInfo_slot(bool f_conn)
{
	Cki->TcpConnInfo(f_conn);
}
#pragma endregion

void workthread::OnNetRecv(char *pDataBuf, int nDataBufSize)
{
	if (nDataBufSize > 0)
	{
		if (checkXor(pDataBuf, nDataBufSize))
		{
			memcpy(g_cNetRecvBuf[g_nNetRecvInd], pDataBuf, nDataBufSize);
			g_nNetRecvSize[g_nNetRecvInd] = nDataBufSize;
			switch ((BYTE)g_cNetRecvBuf[g_nNetRecvInd][22])
			{
#pragma region 0x02
			case 0x02:
			{
						 g_n32NetRcvdPkgs++;
						 int l_n32ScanDatCircleNo = 0;//��ǰ���ݰ���Ȧ��
						 int l_n32ScanDatPackNo = 0;//��Ȧ����
						 l_n32ScanDatCircleNo = (unsigned char)(g_cNetRecvBuf[g_nNetRecvInd][81]);//����Ȧ��
						 l_n32ScanDatPackNo = (unsigned char)(g_cNetRecvBuf[g_nNetRecvInd][82]);//�������
						 if (g_n32NetRcvdPkgs == 1)//����ǵ�һ������
						 {
							 g_n32CircleNo = l_n32ScanDatCircleNo;               //��¼��һ�����ݵ�Ȧ��
						 }
#pragma region Ȧ�ŷ����仯������ݴ���
						 if (l_n32ScanDatCircleNo != g_n32CircleNo)
						 {
							 sum_framenum++;
							 int sum_pack = 0;
							 for (int i = 0; i < 25; i++)
							 {
								 sum_pack += g_sNetTemBuf.g_32ScanPackNo[i];
							 }
							 int n_scanmode = (unsigned char)g_cNetRecvBuf[g_nNetRecvInd][80];//�ֱ��� 0:0.05 1:0.1
							 if (sum_pack == 300 || sum_pack == 78)//1-24��ͻ�1-12���
							 {
#pragma region ɨ������ + �б����ݣ��б�̽�⣩ 
								 if ((BYTE)g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][77] == 0 && (BYTE)g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][78] == 1)//ɨ������ + �б����ݣ��б�̽�⣩
								 {
									 int MarkLM = (BYTE)g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][84];
									 if (MarkLM == 1)
									 {
										 num_Target = (BYTE)g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][85];
										 if (num_Target > 0)
										 {
											 d_landmark = new float[3 * num_Target + 3];
											 for (int i = 0; i < num_Target; i++)
											 {
												 int X_Pos = ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][87 + i * 20] & 0xFF) << 24) + ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][88 + i * 20] & 0xFF) << 16) +
													 ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][89 + i * 20] & 0xFF) << 8) + (BYTE)g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][90 + i * 20];
												 int Y_Pos = ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][91 + i * 20] & 0xFF) << 24) + ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][92 + i * 20] & 0xFF) << 16) +
													 ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][93 + i * 20] & 0xFF) << 8) + (BYTE)g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][94 + i * 20];
												 int l_Kind = (BYTE)g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][95];
												 int s_dlandmark = 3 * i;
												 d_landmark[s_dlandmark] = l_Kind;
												 d_landmark[s_dlandmark + 1] = X_Pos / 1000.0;
												 d_landmark[s_dlandmark + 2] = Y_Pos / 1000.0;
												 emit DetectTable_sig(1);//̽�⵽�İб���ʾ�������
											 }
										 }
									 }
								 }
#pragma endregion
#pragma region ɨ������ + λ������+ �б����ݣ������� 
								 //ɨ������ + λ������+ �б����ݣ�������
								 if ((BYTE)g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][77] == 1 && (BYTE)g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][78] == 1)
								 {
									 int MarkPosi = (BYTE)g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][83];
									 if (MarkPosi == 1)
									 {
										 int R_DirecAngle = 0;
										 int R_EffecMark = 0;
										 int R_XPos = 0;
										 int R_YPos = 0;
										 R_EffecMark = g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][85];//������Ч��־
										 R_DirecAngle = ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][86] & 0xFF) << 24) + ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][87] & 0xFF) << 16) +
											 ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][88] & 0xFF) << 8) + (BYTE)g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][89];//����Ƕ�
										 R_XPos = ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][90] & 0xFF) << 24) + ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][91] & 0xFF) << 16) +
											 ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][92] & 0xFF) << 8) + (BYTE)g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][93];//������X����
										 R_YPos = ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][94] & 0xFF) << 24) + ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][95] & 0xFF) << 16) +
											 ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][96] & 0xFF) << 8) + (BYTE)g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][97];//������Y����
										 float DirecAngle = R_DirecAngle / 1000.0;
										 m_LaserPose[0] = R_EffecMark;
										 m_LaserPose[1] = DirecAngle;
										 m_LaserPose[2] = R_XPos;
										 m_LaserPose[3] = R_YPos;
										 num_Target = (BYTE)g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][101];//�б�����
										 if (num_Target > 0)
										 {
											 d_landmark = new float[3 * num_Target];
											 for (int i = 0; i < num_Target; i++)
											 {
												 int X_Pos = ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][103 + i * 20] & 0XFF) << 24) + ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][104 + i * 20] & 0XFF) << 16) +
													 ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][105 + i * 20] & 0XFF) << 8) + (BYTE)g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][106];//�б�X����
												 int Y_Pos = ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][107 + i * 20] & 0XFF) << 24) + ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][108 + i * 20] & 0XFF) << 16) +
													 ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][109 + i * 20] & 0XFF) << 8) + (BYTE)g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][110];//�б�Y����
												 int l_Kind = (BYTE)g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][111];
												 int s_dlandmark = 3 * i;
												 d_landmark[s_dlandmark] = l_Kind;
												 d_landmark[s_dlandmark + 1] = X_Pos / 1000.0;
												 d_landmark[s_dlandmark + 2] = Y_Pos / 1000.0;
												 emit DetectTable_sig(1);//̽�⵽�İб���ʾ�������
											 }
										 }
									 }
									 else
									 {
										 int MarkLM = (BYTE)g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][84];
										 if (MarkLM == 1)
										 {
											 num_Target = (BYTE)g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][101];//�б�����
											 if (num_Target > 0)
											 {
												 d_landmark = new float[3 * num_Target];
												 for (int i = 0; i < num_Target; i++)
												 {
													 int X_Pos = ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][103 + i * 20] & 0XFF) << 24) + ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][104 + i * 20] & 0XFF) << 16) +
														 ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][105 + i * 20] & 0XFF) << 8) + (BYTE)g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][106];//�б�X����
													 int Y_Pos = ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][107 + i * 20] & 0XFF) << 24) + ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][108 + i * 20] & 0XFF) << 16) +
														 ((g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][109 + i * 20] & 0XFF) << 8) + (BYTE)g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][110];//�б�Y����
													 int l_Kind = (BYTE)g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[0]][111];
													 int s_dlandmark = 3 * i;
													 d_landmark[s_dlandmark] = l_Kind;
													 d_landmark[s_dlandmark + 1] = X_Pos / 1000.0;
													 d_landmark[s_dlandmark + 2] = Y_Pos / 1000.0;
													 emit DetectTable_sig(1);//̽�⵽�İб���ʾ�������
												 }
											 }
										 }
									 }
								 }
#pragma endregion 
#pragma region �Ƕȷֱ���0.05
								 if (n_scanmode == 0)//�Ƕȷֱ���0.05
								 {
									 ang_resratio = 0.05;
									 g_sNetTemBuf.m_u32in = 0;
									 for (int i = 1; i < 13; i++)//1-12��Ϊ����ֵ
									 {
										 memcpy(&g_sNetTemBuf.g_byteLstDistancePkgsScanDat[g_sNetTemBuf.m_u32in], &g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[i]][86], 1200);
										 g_sNetTemBuf.m_u32in += 1200;
									 }
									 if (g_cNetRecvBuf[g_nNetRecvInd][79] == 6)//����+�Ƕ�+����
									 {
										 g_sNetTemBuf.m_u32in = 0;
										 f_addAngEnergy = true;//�ǶȲ�����־λΪTRUE
										 for (int i = 13; i < 19; i++)//13-18��Ϊ�ǶȲ���ֵ
										 {
											 memcpy(&g_sNetTemBuf.g_byteLstAnglePkgsScanDat[g_sNetTemBuf.m_u32in], &g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[i]][86], 1200);
											 g_sNetTemBuf.m_u32in += 1200;
										 }
										 g_sNetTemBuf.m_u32in = 0;
										 for (int i = 19; i < 25; i++)//19-24��Ϊ����ֵ
										 {
											 memcpy(&g_sNetTemBuf.g_byteLstEnergyPkgsScanDat[g_sNetTemBuf.m_u32in], &g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[i]][86], 1200);
											 g_sNetTemBuf.m_u32in += 1200;
										 }
									 }
								 }
#pragma endregion
#pragma region �Ƕȷֱ���0.1
								 else//�Ƕȷֱ���0.1
								 {
									 ang_resratio = 0.1;
									 g_sNetTemBuf.m_u32in = 0;
									 for (int i = 1; i < 7; i++)//1-6��Ϊ����ֵ
									 {
										 memcpy(&g_sNetTemBuf.g_byteLstDistancePkgsScanDat[g_sNetTemBuf.m_u32in], &g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[i]][86], 1200);
										 g_sNetTemBuf.m_u32in += 1200;
									 }
									 int n = (unsigned char)g_cNetRecvBuf[g_nNetRecvInd, 79];//�ֱ���0.1��ʱ��Ϊ6��0327
									 if ((unsigned char)g_cNetRecvBuf[g_nNetRecvInd, 79] == 6)
									 {
										 f_addAngEnergy = true;
										 g_sNetTemBuf.m_u32in = 0;
										 for (int i = 7; i < 10; i++)//7-9��Ϊ�Ƕ�
										 {
											 memcpy(&g_sNetTemBuf.g_byteLstAnglePkgsScanDat[g_sNetTemBuf.m_u32in], &g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[i]][86], 1200);
											 g_sNetTemBuf.m_u32in += 1200;
										 }
										 g_sNetTemBuf.m_u32in = 0;
										 for (int i = 10; i < 13; i++)//10-12��Ϊ����
										 {
											 memcpy(&g_sNetTemBuf.g_byteLstEnergyPkgsScanDat[g_sNetTemBuf.m_u32in], &g_cNetRecvBuf[g_sNetTemBuf.g_n32ScanDatID[i]][86], 1200);
											 g_sNetTemBuf.m_u32in += 1200;
										 }
									 }
								 }
#pragma endregion
								 int nDataZhiCount = 0;
								 int nDataJi = 0;//����ֵ
								 double EachPointAngle = 0;//�Ƕ�ֵ
								 if (f_addAngEnergy)
								 {
									 for (int i = 0; i < sizeof(g_sNetTemBuf.g_byteLstAnglePkgsScanDat) / sizeof(g_sNetTemBuf.g_byteLstAnglePkgsScanDat[0]); i++)//�����ǶȲ���ֵ
									 {
										 g_n64ArrayCompenAngle[i] = (unsigned char)(g_sNetTemBuf.g_byteLstAnglePkgsScanDat[i]);
									 }
									 for (int k = 0; k < sizeof(g_sNetTemBuf.g_byteLstEnergyPkgsScanDat) / sizeof(g_sNetTemBuf.g_byteLstEnergyPkgsScanDat[0]); k++)//��������ֵ
									 {
										 g_n64ArrayEnergy[k] = (unsigned char)(g_sNetTemBuf.g_byteLstEnergyPkgsScanDat[k]);
									 }
								 }
								 int len = sizeof(g_sNetTemBuf.g_byteLstDistancePkgsScanDat) / sizeof(g_sNetTemBuf.g_byteLstDistancePkgsScanDat[0]);
								 for (int m = 0; m <len; m += 2)//��������ֵ
								 {
									 nDataJi = ((g_sNetTemBuf.g_byteLstDistancePkgsScanDat[m] & 0xFF) << 8) + (BYTE)g_sNetTemBuf.g_byteLstDistancePkgsScanDat[m + 1];//(BYTE)һ��Ҫ�ӣ����Ӿͻ���ָ�ֵ
									 if (f_addAngEnergy)
										 EachPointAngle = nDataZhiCount * ang_resratio + (double)g_n64ArrayCompenAngle[nDataZhiCount] / 100;
									 else
										 EachPointAngle = nDataZhiCount * ang_resratio;
									 if (nDataJi > g_n32MaxDis) //�����ֵ
										 nDataJi = g_n32MaxDis;
									 g_n64ArrayZhiX[nDataZhiCount] = (float)(nDataJi *cos((PI / 180) * EachPointAngle)) / 1000;
									 g_n64ArrayZhiY[nDataZhiCount] = (float)(nDataJi *sin((PI / 180) * EachPointAngle)) / 1000;
									 g_n64ArrayZhiZ[nDataZhiCount] = 1;
									 nDataZhiCount++;
								 }
								 emit Draw_sig(1);//����
							 }
							 else
							 {
								 int n = sum_pack;
								 wrong_framenum++;
								 qDebug() << n << endl;
							 }
							 wrong_prop = (float)wrong_framenum / (float)sum_framenum;
							 memset(&g_sNetData, 0, sizeof(g_sNetData));
							 memset(&g_sNetTemBuf, 0, sizeof(g_sNetTemBuf));
							 g_n32CircleNo = l_n32ScanDatCircleNo;//���仯���Ȧ�����¸�ֵ��Ȧ�ű���
							 SZ_Serial = 0;
						 }
#pragma endregion
						 g_sNetTemBuf.g_n32ScanDatID[SZ_Serial] = g_nNetRecvInd;//��¼ÿһȦ��ÿһ�����ݵ�ID
						 g_sNetTemBuf.g_32ScanPackNo[SZ_Serial] = l_n32ScanDatPackNo;//��¼ÿһȦ��ÿһ�����ݵİ���
						 SZ_Serial++;
						 if (SZ_Serial > 25)
							 SZ_Serial = 0;
			}
				break;
#pragma endregion
#pragma region 0x06
			case 0x06:
			{
						 switch ((BYTE)g_cNetRecvBuf[g_nNetRecvInd][23])
						 {
						 case 3:
						 {
								   if ((BYTE)g_cNetRecvBuf[g_nNetRecvInd][27] == n_setpackage + 1)
								   {
									   if ((BYTE)g_cNetRecvBuf[g_nNetRecvInd][29] == 1)
									   {
										   emit InfoView_sig(QString::fromLocal8Bit("�б���Ϣ������..."));
									   }
								   }
								   if ((BYTE)g_cNetRecvBuf[g_nNetRecvInd][27] == n_setpackage)
								   {
									   if ((BYTE)g_cNetRecvBuf[g_nNetRecvInd][29] == 1)
									   {
										   emit InfoView_sig(QString::fromLocal8Bit("�б����óɹ���"));
									   }
								   }
						 }
							 break;
						 case 4:
						 {
								   if ((BYTE)g_cNetRecvBuf[g_nNetRecvInd][29] == 1)
								   {
									   emit InfoView_sig("");
								   }
						 }
							 break;
						 case 5://�б��ȡ
						 {
									int l_LocalID = 0, l_GX_Pos = 0, l_GY_Pos = 0, l_Shape = 0, l_diameter = 0;
									num_Target = (BYTE)g_cNetRecvBuf[g_nNetRecvInd][31];
									if (num_Target > 0)
									{
										d_landmark = new float[6 * num_Target];
#pragma region �б���ʾ
#pragma endregion
										for (int i = 0; i < num_Target; i++)
										{
											l_LocalID = (BYTE)(g_cNetRecvBuf[g_nNetRecvInd][32 + i * 20]);//����ID
											l_GX_Pos = ((g_cNetRecvBuf[g_nNetRecvInd][33 + i * 20] & 0xFF) << 24) + ((g_cNetRecvBuf[g_nNetRecvInd][34 + i * 20] & 0xFF) << 16) +
												((g_cNetRecvBuf[g_nNetRecvInd][35 + i * 20] & 0xFF) << 8) + (BYTE)(g_cNetRecvBuf[g_nNetRecvInd][36 + i * 20]);
											l_GY_Pos = ((g_cNetRecvBuf[g_nNetRecvInd][37 + i * 20] & 0xFF) << 24) + ((g_cNetRecvBuf[g_nNetRecvInd][38 + i * 20] & 0xFF) << 16) +
												((g_cNetRecvBuf[g_nNetRecvInd][39 + i * 20] & 0xFF) << 8) + (BYTE)(g_cNetRecvBuf[g_nNetRecvInd][40 + i * 20]);//��������&&�������ݴ�1120
											l_Shape = (BYTE)(g_cNetRecvBuf[g_nNetRecvInd][42 + i * 20]);//��״
											l_diameter = (BYTE)(g_cNetRecvBuf[g_nNetRecvInd][43 + i * 20]);//�ߴ�
											LMID[i] = l_LocalID + 1;
											int l_Kind = (BYTE)g_cNetRecvBuf[g_nNetRecvInd][41];
											g_landmarkX[i] = l_GX_Pos;
											g_landmarkY[i] = l_GY_Pos;
											int s_dlandmark = 6 * i;
											d_landmark[s_dlandmark] = l_LocalID;
											d_landmark[s_dlandmark + 1] = l_GX_Pos;
											d_landmark[s_dlandmark + 2] = l_GY_Pos;
											d_landmark[s_dlandmark + 3] = l_diameter;
											d_landmark[s_dlandmark + 4] = l_Shape;
											d_landmark[s_dlandmark + 5] = get_LarerID;
										}
										emit DetectTable_sig(2);
										emit Draw_sig(2);
									}
									int CurrCount_Package = ((g_cNetRecvBuf[g_nNetRecvInd][28] & 0xFF) << 8) + (BYTE)g_cNetRecvBuf[g_nNetRecvInd][29];
									if (CurrCount_Package <= n_scantime)
									{
										QString getinfo = QString::fromLocal8Bit("��") + QString::number(CurrCount_Package) + QString::fromLocal8Bit("�λ�ȡ�ɹ���");
										emit InfoView_sig(getinfo);
										QString QSCurrCount_Package = QString("%1").arg(CurrCount_Package, 4, 16, QLatin1Char('0'));
										QString SendOrd = "001E00000000000001010007000000000000000006050000" + QSCurrCount_Package + "0001";
										QString BCC = GetBCCXorCode(SendOrd);
										SendOrd = "FFAA" + SendOrd + BCC + "EEEE";
										QByteArray send_GetMess = QString2Hex(SendOrd);
										tcpRecSocket->write(send_GetMess);
										if (CurrCount_Package == n_scantime)
											emit Enablebtn();
									}
						 }
							 break;
						 case 0x06://�б��ѯ
						 {
									   int l_LocalID = 0, l_GX_Pos = 0, l_GY_Pos = 0, l_Shape = 0, l_diameter = 0, layerID = 0;
									   if ((BYTE)g_cNetRecvBuf[g_nNetRecvInd][27] == 0)
										   num_Target = ((g_cNetRecvBuf[g_nNetRecvInd][28] & 0xff) << 8) + (BYTE)(g_cNetRecvBuf[g_nNetRecvInd][29]);;
									   if (num_Target > 0)
									   {
										   if ((BYTE)g_cNetRecvBuf[g_nNetRecvInd][27] != 0)
										   {
											   d_landmark = new float[6 * num_Target];
#pragma region �б���ʾ
#pragma endregion
											   for (int i = 0; i < num_Target; i++)
											   {
												   l_LocalID = ((g_cNetRecvBuf[g_nNetRecvInd][28 + i * 14] & 0xff) << 8) + (BYTE)(g_cNetRecvBuf[g_nNetRecvInd][29 + i * 14]);//����ID
												   LMID[i] = l_LocalID + 1;
												   l_GX_Pos = ((g_cNetRecvBuf[g_nNetRecvInd][30 + i * 14] & 0xFF) << 24) + ((g_cNetRecvBuf[g_nNetRecvInd][31 + i * 14] & 0xFF) << 16) +
													   ((g_cNetRecvBuf[g_nNetRecvInd][32 + i * 14] & 0xFF) << 8) + (BYTE)(g_cNetRecvBuf[g_nNetRecvInd][33 + i * 14]);
												   l_GY_Pos = ((g_cNetRecvBuf[g_nNetRecvInd][34 + i * 14] & 0xFF) << 24) + ((g_cNetRecvBuf[g_nNetRecvInd][35 + i * 14] & 0xFF) << 16) +
													   ((g_cNetRecvBuf[g_nNetRecvInd][36 + i * 14] & 0xFF) << 8) + (BYTE)(g_cNetRecvBuf[g_nNetRecvInd][37 + i * 14]);//��������&&�������ݴ�1114
												   l_Shape = (BYTE)(g_cNetRecvBuf[g_nNetRecvInd][40 + i * 14]);//��״
												   l_diameter = (BYTE)(g_cNetRecvBuf[g_nNetRecvInd][41 + i * 14]);//�ߴ�
												   layerID = ((g_cNetRecvBuf[g_nNetRecvInd][38 + i * 14] & 0xff) << 8) + (BYTE)(g_cNetRecvBuf[g_nNetRecvInd][39 + i * 14]);
												   g_landmarkX[i] = l_GX_Pos;
												   g_landmarkY[i] = l_GY_Pos;
												   int s_dlandmark = 6 * i;
												   d_landmark[s_dlandmark] = l_LocalID;
												   d_landmark[s_dlandmark + 1] = l_GX_Pos;
												   d_landmark[s_dlandmark + 2] = l_GY_Pos;
												   d_landmark[s_dlandmark + 3] = l_diameter;
												   d_landmark[s_dlandmark + 4] = l_Shape;
												   d_landmark[s_dlandmark + 5] = layerID;
												   QByteArray Sec_SendOrd = QString2Hex(tr("FFAA001E00000000000001010007000000000000000006060000000100010019EEEE"));
												   tcpRecSocket->write(Sec_SendOrd);
											   }
											   emit DetectTable_sig(2);
											   emit Draw_sig(2);
										   }
										   else
										   {
											   QByteArray Fir_SendOrd = QString2Hex(tr("FFAA001E0000000000000101000700000000000000000606000000020001001AEEEE"));
											   tcpRecSocket->write(Fir_SendOrd);
										   }
									   }
						 }
							 break;
						 default:
							 break;
						 }
			}
				break;
#pragma endregion
			default:
				break;
			}
			g_nNetRecvInd = (g_nNetRecvInd + 1) % NET_BUF_NUM;
		}
		else
		{
			xorflag = 1;
		}
	}
}

#pragma region TCP����ָ��
void workthread::Sendzhiling(QByteArray send)
{
	if (f_tcpNetConn)
	{
		if (tcpRecSocket != NULL)
		{
			tcpRecSocket->write(send, send.length());
		}
	}
	else
	{
		emit InfoView_sig(QString::fromLocal8Bit("����δ���ӣ�"));
	}
}
#pragma endregion


#pragma region TCP�Ͽ�����
bool workthread::DisconnectServer()
{
	if (f_tcpNetConn)
	{
		if (tcpRecSocket != NULL)
		{
			tcpRecSocket->disconnectFromHost();
			Sleep(100);
			QByteArray a = tcpRecSocket->readAll();
			if (a.length() == 0)
			{
				return true;
				tcpRecSocket = NULL;
				f_tcpNetConn = false;
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		emit InfoView_sig(QString::fromLocal8Bit("����δ���ӣ�"));
	}
}
#pragma endregion

void workthread::DetectTable_slot(int slot_view_Kind)
{
	if (d_landmark != NULL)
	{
		Cki->RefreshTableView(d_landmark, num_Target, slot_view_Kind, m_LaserPose);
	}
}

void workthread::Draw_slot(int Draw_kind)
{
	if (Draw_kind == 1)
	{
		if (m_LaserPose == NULL)
		{
			m_LaserPose[0] = 0;
			m_LaserPose[1] = 0;
			m_LaserPose[2] = 0;
			m_LaserPose[3] = 0;
		}
		//�ڲۺ����е���MainWindow��Ŀؼ���������
		pclfun->AddCloudPointArray(g_n64ArrayZhiX, g_n64ArrayZhiY, g_n64ArrayZhiZ, d_landmark, m_LaserPose, 7200, num_Target);
	}
	if (Draw_kind == 2)//�б��ȡ
	{
		if (g_landmarkX != NULL&&g_landmarkY != NULL)
		{
			pclfun->SetLMArrayDisplay(g_landmarkX, g_landmarkY, LMID, num_Target);
		}
	}
	if (d_landmark)
	{
		delete[]d_landmark;
		d_landmark = NULL;
	}
}

void workthread::InfoView_slot(QString info)
{
	Cki->Infoview(1, info);
}

void workthread::Enablebtn_slot()
{
	Cki->setbtnenable(true);
}

#pragma region ��ȡ�ַ�����ʽУ����
QString GetBCCXorCode(QString str)
{
	QByteArray data = QString2Hex(str);
	BYTE CheckCode = 0;//������char,��char�ͻ��и����Ľ��
	int len = data.length();
	for (int i = 0; i < len; i++)
	{
		CheckCode ^= data[i];
	}
	return QString("%1").arg(CheckCode, 4, 16, QLatin1Char('0'));
}
#pragma endregion

#pragma region �ַ���ת��ΪQByteArray
QByteArray QString2Hex(QString str)
{
	QByteArray senddata;
	int hexdata, lowhexdata;
	int hexdatalen = 0;
	int len = str.length();
	senddata.resize(len / 2);
	char lstr, hstr;
	for (int i = 0; i < len;)
	{
		hstr = str[i].toLatin1();
		if (hstr == ' ')
		{
			i++;
			continue;
		}
		i++;
		if (i >= len)
			break;
		lstr = str[i].toLatin1();
		hexdata = ConvertHexChar(hstr);
		lowhexdata = ConvertHexChar(lstr);
		if ((hexdata == 16) || (lowhexdata == 16))
			break;
		else
			hexdata = hexdata * 16 + lowhexdata;
		i++;
		senddata[hexdatalen] = (char)hexdata;
		hexdatalen++;
	}
	senddata.resize(hexdatalen);
	return senddata;

}
char ConvertHexChar(char ch)
{
	if ((ch >= '0') && (ch <= '9'))
		return ch - 0x30;
	else if ((ch >= 'A') && (ch <= 'F'))
		return ch - 'A' + 10;
	else if ((ch >= 'a') && (ch <= 'f'))
		return ch - 'a' + 10;
	else return (-1);
}
#pragma endregion 

#pragma region У��λ�˶�
void workthread::Xor(char* sendbuf, int sendlen)
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
bool workthread::checkXor(char* recvbuf, int recvlen)
{
	int i = 0;
	char check = 0;
	char * p = recvbuf;
	int len;//ĩβ��λ����У��
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
#pragma endregion



