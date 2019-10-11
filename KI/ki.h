#ifndef KI_H
#define KI_H
#include <QtWidgets/QMainWindow>
#include "ui_ki.h"
#include "5.7.0\QtCore\private\cp949codetbl_p.h"
#include <QtNetwork/QtNetwork>
#include <QCloseEvent>
#include<QMessageBox>
#include <QTextStream>
#include <QStandardItemModel>
#include <QCheckBox>
#include<QSplitter>
#include<QGridLayout>
#include "PCLFunc.h"
#define LOCAL_PORT 7000
#define DEST_PORT 6060
typedef QPushButton* pqb;
extern HANDLE g_BroAngevent;//�ǶȻ�ȡ��Ӧ�¼�
class KI : public QMainWindow
{
	Q_OBJECT

public:
	KI(QWidget *parent = 0);
	~KI();
    QTcpSocket *tcpSocket=NULL;
	void setbtnenable(bool f_enable);//������������ѡ��ť�Ŀ�ʹ���ԣ�true-����
	void RefreshTableView(float *LandmarkData, int LMCount, int view_Kind, float *LaserPose = NULL);
	void Infoview(int info_Kind,QString slotinfo, QString qstr = "");//slotinfoΪ�ۺ����з��ص���Ϣ��qstrΪ������ʾ��Ϣ�������Ϣ
	void GetLMWay(QString model, QString layerID, QString scantime, QString selfangle, QString selfx, QString selfy, QString type, QString size);//�б��ȡ������Ҫ�ڱ�����е���
	void TcpConnInfo(bool b_conn);
	void NavWay(QString datakind, QString layerId=tr("0000"));
private:
	Ui::KIClass ui;
	const int Everypack_Setlm = 100;//ÿ��֡���������õİб�����
	pqb PushButton=NULL;
	bool f_lmDetect = true;
	bool f_conscan = true;//�Ƿ�����ɨ���־
	QByteArray zl_Getnum = NULL;
	QByteArray zl_Detect = NULL;
	QByteArray zl_Brocast = NULL;
	QByteArray zl_GetLM = NULL;
	QCheckBox *checkbox = NULL;
	QString info="";//��������ʾ��Ϣ
	bool f_pointOrLine=true;
	bool grid_enabled_ = false;
	bool f_conOrDiscon = true;
	QUdpSocket * receiver=NULL;
	//��ʼ��vtk����
	void getRandom();
	void timerEvent(QTimerEvent* event);
	void initialtablewidget();
	void initialsplitter();
	void InsertRow(int row);
	int  m_nTimerIp;
	int get_cur_row;//�б��ȡ
	int *lmid = NULL;
	PCLFunc *pclfunc = NULL;
signals:
	void sig_BroInfo();
private slots:
    void slot_BroInfo();
	void MoveZUP(void);
	void MoveZDOWN(void);
	void MoveYUP(void);
	void MoveYDOWN(void);
	void MoveXUP(void);
	void MoveXDOWN(void);
	void RotateClick();
	void PointTest();
	void Cylinder();
	void ConnClick(void);
	void Broadcast(void);
	void CBClick();
	void LandMarkDetect_Click();
	void Con_Click();
	void LMGetClick();
	void LMSettingClick();
	void LMQueryClick();
	void NavClick();
	void slotRowClicked(const QModelIndex &index);
	void slot_delete();
	void slot_addrow(int);
	void slot_CellClick(int, int);
	void slot_cellChanged(int, int);
protected:
	void closeEvent(QCloseEvent *event);
};

#endif // PCLVISUALIZER_H