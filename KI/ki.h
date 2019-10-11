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
extern HANDLE g_BroAngevent;//角度获取响应事件
class KI : public QMainWindow
{
	Q_OBJECT

public:
	KI(QWidget *parent = 0);
	~KI();
    QTcpSocket *tcpSocket=NULL;
	void setbtnenable(bool f_enable);//设置三个功能选择按钮的可使用性，true-能用
	void RefreshTableView(float *LandmarkData, int LMCount, int view_Kind, float *LaserPose = NULL);
	void Infoview(int info_Kind,QString slotinfo, QString qstr = "");//slotinfo为槽函数中返回的信息，qstr为操作提示信息或错误信息
	void GetLMWay(QString model, QString layerID, QString scantime, QString selfangle, QString selfx, QString selfy, QString type, QString size);//靶标获取方法，要在别的类中调用
	void TcpConnInfo(bool b_conn);
	void NavWay(QString datakind, QString layerId=tr("0000"));
private:
	Ui::KIClass ui;
	const int Everypack_Setlm = 100;//每包帧数据能设置的靶标数量
	pqb PushButton=NULL;
	bool f_lmDetect = true;
	bool f_conscan = true;//是否连续扫描标志
	QByteArray zl_Getnum = NULL;
	QByteArray zl_Detect = NULL;
	QByteArray zl_Brocast = NULL;
	QByteArray zl_GetLM = NULL;
	QCheckBox *checkbox = NULL;
	QString info="";//操作后显示信息
	bool f_pointOrLine=true;
	bool grid_enabled_ = false;
	bool f_conOrDiscon = true;
	QUdpSocket * receiver=NULL;
	//初始化vtk部件
	void getRandom();
	void timerEvent(QTimerEvent* event);
	void initialtablewidget();
	void initialsplitter();
	void InsertRow(int row);
	int  m_nTimerIp;
	int get_cur_row;//靶标获取
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