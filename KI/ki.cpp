#include <string>
#include <QFileDialog>
#include <iostream>
#include "ki.h"
#include <QFile>
#include <QRegExp>
#include <QStringList>
#include "workthread.h"
#include "getlmpara.h"
#include "navpara.h"
#include "workthreadbroadcast.h"
#include <QDesktopWidget>
using namespace std;
QString host;
QString str_hintinfo;
int port;
int n_setpackage = 0;
int get_LarerID = 1;//�б��ȡ��ʱ�����õĲ�ID��Ҫ��Workthread�����õ���������ͷ�ļ��ж���
workthread *workThread;
GetLmPara *getlmpara;
navpara *navpar;
QStandardItemModel *model;//tableview��ʹ��
HANDLE g_BroAngevent = CreateEvent(NULL, TRUE, FALSE, NULL);
workthreadbroadcast *workThreadBroad = NULL;
KI::KI(QWidget *parent)
: QMainWindow(parent)
{
	ui.setupUi(this);
	//��ʼ��
	pclfunc = new PCLFunc();
	pclfunc->initialVtkWidget(ui.groupBox);
	initialsplitter();
	initialtablewidget();
}

#pragma region ��ʼ��splitter
void KI::initialsplitter()
{
	QDesktopWidget* desktopWidget = QApplication::desktop();
	//�õ��ͻ�������
	QRect clientRect = desktopWidget->availableGeometry();
	////�õ�Ӧ�ó������
	//QRect clientRect = desktopWidget->screenGeometry();

	int nWidth = clientRect.width();
	int nHeight = clientRect.height();
	QSplitter *m_splitter = new QSplitter(Qt::Horizontal, this);

	QSplitter *splitter1 = new QSplitter(Qt::Vertical);
	splitter1->addWidget(ui.groupBox1);
	splitter1->setMinimumWidth(nWidth*0.2);
	splitter1->setMaximumWidth(nWidth*0.3);
	m_splitter->addWidget(splitter1);
	//splitter1->setFixedWidth(nWidth*0.3);

	QSplitter *splitter2 = new QSplitter(Qt::Vertical);
	splitter2->addWidget(ui.groupBox2);
	splitter2->setMinimumWidth(nWidth*0.7);
	splitter2->setMaximumWidth(nWidth*0.8);
	splitter2->setMinimumHeight(nHeight*0.9);
	splitter2->setMaximumHeight(nHeight);
	m_splitter->addWidget(splitter2);

	QSplitter *splitter3 = new QSplitter(Qt::Vertical, splitter2);
	splitter3->addWidget(pclfunc->qvtk);
	splitter3->setMinimumHeight(nHeight*0.8);
	splitter3->setMaximumHeight(nHeight*0.9);

	QSplitter *splitter4 = new QSplitter(Qt::Horizontal, splitter3);
	splitter4->addWidget(ui.tabWidget);
	splitter4->setFixedHeight(nHeight*0.2);
	m_splitter->addWidget(splitter2);

	QGridLayout *mainly = new QGridLayout;
	mainly->addWidget(m_splitter);
	this->setLayout(mainly);
	this->setCentralWidget(m_splitter);

}
#pragma endregion

#pragma region ��ʱ������Ҫ��ָ��
void KI::timerEvent(QTimerEvent* event)
{
	workThread->Sendzhiling(zl_Getnum);
}
#pragma endregion

#pragma region �߱�
void KI::CBClick()//�߱�
{
	pclfunc->Chibiao(grid_enabled_);
	grid_enabled_ = !grid_enabled_;
}
#pragma endregion

#pragma region ����ļ��б�
void KI::Cylinder()
{
	bool f_Calcu = false;
	int i = 0;
	int id[12000];
	float x[12000];
	float y[12000];
	float type[12000];
	float size[12000];
	float layer[12000];
	lmid = new int[12000];
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Images (*.lmk)"));
	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		while (!file.atEnd())
		{
			QByteArray line = file.readLine();
			QString str(line);
			QStringList sections = str.trimmed().split(' ', QString::SkipEmptyParts);
			if (f_Calcu)
			{
				id[i] = sections.at(0).trimmed().toInt();
				x[i] = sections.at(1).trimmed().toFloat();
				y[i] = sections.at(2).trimmed().toFloat();
				layer[i] = sections.at(6).trimmed().toFloat();
				type[i] = sections.at(3).trimmed().toFloat();
				size[i] = sections.at(5).trimmed().toFloat();
				lmid[i] = id[i] + 1;
				i++;
			}
			if (sections.at(0).trimmed().toStdString() == "globID")
			{
				f_Calcu = true;
			}
		}
		file.close();
	}
	float *landmark = NULL;
	landmark = new float[i * 6];
	for (int k = 0; k < i; k++)
	{
		int start = 6 * k;
		landmark[start] = id[k];
		landmark[start + 1] = x[k];
		landmark[start + 2] = y[k];
		landmark[start + 3] = size[k];
		landmark[start + 4] = type[k];
		landmark[start + 5] = layer[k];
	}
	RefreshTableView(landmark, i, 2);
	if (landmark)delete[] landmark; landmark = NULL;
	pclfunc->SetLMArrayDisplay(x, y, lmid, i);
	if (lmid) delete[] lmid; lmid = NULL;
}
#pragma endregion

#pragma region ��/��ģʽ�л�
void KI::PointTest(void)//��/���л�
{
	if (f_pointOrLine)
	{
		pclfunc->PointOrPolyline(f_pointOrLine);
		ui.btn_PointTest->setText(QString::fromLocal8Bit("��"));
		f_pointOrLine = false;
	}
	else
	{
		pclfunc->PointOrPolyline(f_pointOrLine);
		ui.btn_PointTest->setText(QString::fromLocal8Bit("��"));
		f_pointOrLine = true;
	}
}
#pragma endregion

#pragma region ��ת
void KI::RotateClick()
{
	pclfunc->RotateViewTo();
}
#pragma endregion

#pragma region �仯�ӽ�
//��ת�������������������������ľ�ͷ�����λ������Z����ʱ��90�� ������ת��ָ��������ʱ��Χ����ת������������ת

void KI::MoveZDOWN(void)
{
	pclfunc->setViewTo(2, -1);
}
void KI::MoveZUP(void)
{
	pclfunc->setViewTo(2, 1);
}
void KI::MoveYDOWN(void)
{
	pclfunc->setViewTo(1, -1);
}
void KI::MoveYUP(void)
{
	pclfunc->setViewTo(1, 1);
}
void KI::MoveXDOWN(void)
{
	pclfunc->setViewTo(0, -1);
}
void KI::MoveXUP(void)
{
	pclfunc->setViewTo(0, 1);
}
#pragma endregion 

#pragma region �ж��Ƿ�ΪIP��ַ
int isipv4str(QString ip)
{
	char*  ch;
	QByteArray ba = ip.toLatin1(); // must
	ch = ba.data();
	int i, a[4];
	if (sscanf(ch, "%d.%d.%d.%d", &a[0], &a[1], &a[2], &a[3]) != 4)
		return 0;
	for (i = 0; i < 4; i++) if (a[i] < 0 || a[i] > 255) return 0;
	return 1;
}
#pragma endregion 

#pragma region �������ӶϿ�
void KI::ConnClick(void)
{
	if (f_conOrDiscon)
	{
		host = ui.hostLineEdit->text();
		port = ui.portLineEdit->text().toInt();
		if (host.length() > 7 && port > 0)
		{
			if (isipv4str(host) == 1)
			{
				try
				{
					workThread = new workthread(this, pclfunc, NULL);
					workThread->start();
				}
				catch (exception& e)
				{
					str_hintinfo = e.what();
					Infoview(2, "", str_hintinfo);
				}
			}
			else
			{
				str_hintinfo = QString::fromLocal8Bit("����ʧ�ܣ�������IP��д��ʽ����");
				Infoview(2, "", str_hintinfo);
			}
		}
		else
		{
			str_hintinfo = QString::fromLocal8Bit("IP��ַ��˿ں���д����");
			Infoview(2, "", str_hintinfo);
		}
	}
	else
	{
		if (!f_conscan)
			Con_Click();
		if (workThread != NULL)
		{
			workThread->terminate();
		}
		if (workThread->DisconnectServer())
		{
			workThread = NULL;
			f_conOrDiscon = true;
			ui.btn_Conn->setText(QString::fromLocal8Bit("TCP����"));
			str_hintinfo = QString::fromLocal8Bit("�����ѶϿ���");
		}
		else
		{
			str_hintinfo = QString::fromLocal8Bit("����Ͽ�ʧ�ܣ������¶Ͽ���");
		}
		Infoview(2, "", str_hintinfo);
	}
}
#pragma endregion

#pragma region TCP�������ӷ���
void KI::TcpConnInfo(bool b_conn)
{
	if (b_conn)
	{
		ui.btn_Conn->setText(QString::fromLocal8Bit("�Ͽ�"));
		str_hintinfo = QString::fromLocal8Bit("�������ӳɹ���");
		Infoview(2, "", str_hintinfo);
		f_conOrDiscon = false;
		ui.btn_Continue->setEnabled(true);
	}
	else
	{
		str_hintinfo = QString::fromLocal8Bit("��������ʧ�ܣ�");
		Infoview(2, "", str_hintinfo);
	}
}
#pragma endregion

#pragma region �㲥
void KI::Broadcast(void)
{
	//connect(this, SIGNAL(sig_BroInfo()), this, SLOT(slot_BroInfo()));
	///*QString str_hintinfo = QString::fromLocal8Bit("���ڹ㲥....");*/
	//emit sig_BroInfo();
	if (receiver != NULL)
	{
		receiver->disconnectFromHost();
		receiver = NULL;
	}
	receiver = new QUdpSocket(this);
	/////�󶨣���һ������Ϊ�˿ںţ��ڶ���ʾ����������ַ���Ӹù㲥
	receiver->bind(LOCAL_PORT, QUdpSocket::ShareAddress);
	zl_Brocast = QString2Hex(tr("FFAA001E0000000000000101000700000000000000000406000000000000001BEEEE"));
	str_hintinfo = QString::fromLocal8Bit("���ڹ㲥....");
	Infoview(2, "", str_hintinfo);
	workThreadBroad = new workthreadbroadcast(receiver, zl_Brocast);
	workThreadBroad->start();
	DWORD dw = WaitForSingleObject(g_BroAngevent, 1000);
	ResetEvent(g_BroAngevent);//�����٣��������set�˵Ļ��ͻ�һֱ���ָ�״̬
	switch (dw)
	{
	case WAIT_OBJECT_0:
		ui.hostLineEdit->setText(IPAdress);
		ui.portLineEdit->setText(Port);
		ui.btn_BroadCast->setEnabled(false);
		str_hintinfo = QString::fromLocal8Bit("�ɹ����յ�IP���뵥�����Ӱ�ť��");
		Infoview(2, "", str_hintinfo);
		break;
	case WAIT_TIMEOUT:		//��ʱδ�յ��ظ�
		str_hintinfo = QString::fromLocal8Bit("�㲥ʧ�ܣ���ȷ���豸�Ƿ��Ѿ����Ӳ����¹㲥��");
		Infoview(2, "", str_hintinfo);
		f_GetBroInfo = false;
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�㲥ʧ�ܣ���ȷ���豸�Ƿ��Ѿ����Ӳ����¹㲥��"));
		break;
	case WAIT_FAILED:
		str_hintinfo = QString::fromLocal8Bit("�㲥ʧ�ܣ���ȷ���豸�Ƿ��Ѿ����Ӳ����¹㲥��");
		Infoview(2, "", str_hintinfo);
		f_GetBroInfo = false;
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�㲥ʧ�ܣ���ȷ���豸�Ƿ��Ѿ����Ӳ����¹㲥��"));
		break;
	}
}
#pragma endregion

void KI::slot_BroInfo()
{
	Infoview(2, "", QString::fromLocal8Bit("���ڹ㲥...."));
}
#pragma region ���ڹر��¼�
void KI::closeEvent(QCloseEvent *event)
{
	//TODO: ���˳�����֮ǰ��ʵ��ϣ�����Ĳ��� 
	if (QMessageBox::question(this,
		tr("Quit"),
		tr("Are you sure to quit this application?"),
		QMessageBox::Yes, QMessageBox::No)
		== QMessageBox::Yes)
	{
		killTimer(m_nTimerIp);
		if (tcpSocket != NULL)
		{
			tcpSocket->disconnectFromHost();
			tcpSocket->close();
			tcpSocket = NULL;
		}
		if (workThread != NULL)
		{
			workThread->terminate();
			workThread = NULL;
		}
		if (workThreadBroad != NULL)
		{
			workThreadBroad->requestInterruption();
			workThreadBroad->quit();
		}
		event->accept();//���Ὣ�¼����ݸ�����ĸ����
	}
	else
		event->ignore();
}
#pragma endregion

#pragma region �б�̽��ģʽ
void KI::LandMarkDetect_Click()
{
	if (f_tcpNetConn)
	{
		f_lmDetect = true;
		zl_Detect = QString2Hex(tr("FFAA0020000000000000010100070000000000000000060400000001010600500073EEEE"));
		tcpSocket->write(zl_Detect);
		if (model != NULL)
		{
			delete model;
			model = NULL;
		}
		pclfunc->DeleteCurrLM();
		model = new QStandardItemModel();
		model->setColumnCount(3);
		model->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("���"));
		model->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("X����(mm)"));
		model->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("Y����(mm)"));
		ui.TB_LMDetectInfo->setModel(model);
		///������Դ����������
		QHeaderView *theader = ui.TB_LMDetectInfo->verticalHeader();
		theader->setHidden(true);
		///
		ui.TB_LMDetectInfo->setSelectionBehavior(QAbstractItemView::SelectRows);//����ѡ��ģʽΪѡ����
		ui.TB_LMDetectInfo->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
		ui.TB_LMDetectInfo->setColumnWidth(0, 40);
		ui.TB_LMDetectInfo->setColumnWidth(0, 60);
		ui.TB_LMDetectInfo->setColumnWidth(0, 60);
		info = QString::fromLocal8Bit("�б�̽��ģʽ���óɹ���");
		if (lmid) delete[] lmid; lmid = NULL;
	}
	else
	{
		QString str = QString::fromLocal8Bit("����δ���ӻ��ѶϿ���");
		Infoview(2, "", str);
	}
}
#pragma endregion

#pragma region ����ɨ��
void KI::Con_Click()
{
	if (f_conscan)
	{
		m_nTimerIp = startTimer(160);
		ui.btn_Continue->setText(QString::fromLocal8Bit("ֹͣ����ɨ��"));
		f_conscan = false;
		QString qstr = QString::fromLocal8Bit("��ʼ����ɨ�裡");
		Infoview(2, "", qstr);
		ui.btn_MarkDetect->setEnabled(false);
		ui.btn_LMGet->setEnabled(false);
		ui.btn_nav->setEnabled(false);
	}
	else
	{
		killTimer(m_nTimerIp);
		ui.btn_Continue->setText(QString::fromLocal8Bit("����ɨ��"));
		f_conscan = true;
		QString qstr = QString::fromLocal8Bit("ֹͣ����ɨ�裡");
		Infoview(2, "", qstr);
		setbtnenable(true);

	}
}
#pragma endregion

#pragma region �б��ȡģʽ
void KI::LMGetClick()
{
	getlmpara = new GetLmPara(this);//
	getlmpara->setWindowModality(Qt::ApplicationModal);
	getlmpara->show();
}
void KI::GetLMWay(QString getmodel, QString layerID, QString scantime, QString selfangle, QString selfx, QString selfy, QString type, QString size)
{
	if (f_tcpNetConn)
	{
		f_lmDetect = false;//�б�̽���־λ
		killTimer(m_nTimerIp);
		get_LarerID = layerID.toInt();
		QString qsget = tr("002E00000000000001010007000000000000000006040000") + getmodel + layerID + scantime + selfangle + selfx + selfy + type + size;
		QString BCC = GetBCCXorCode(qsget);
		qsget = tr("FFAA") + qsget + BCC + tr("EEEE");
		zl_GetLM = QString2Hex(qsget);
		tcpSocket->write(zl_GetLM);
		initialtablewidget();
		info = QString::fromLocal8Bit("�б��ȡģʽ���óɹ���");
		ui.btn_MarkDetect->setEnabled(false);
		ui.btn_LMGet->setEnabled(false);
		ui.btn_nav->setEnabled(false);
	}
	else
	{
		QString str = QString::fromLocal8Bit("����ʧ�ܣ�����δ���ӻ��ѶϿ���");
		Infoview(2, "", str);
	}
}
#pragma endregion

#pragma region tableview��tablewidget�аб���Ϣˢ����ʾ
void KI::RefreshTableView(float *LandmarkData, int LMCount, int view_Kind, float *LaserPose)
{
	if (view_Kind == 1)//�б�̽��
	{
		if (LandmarkData != NULL&&LMCount > 0)
		{
			QModelIndexList selected = ui.TB_LMDetectInfo->selectionModel()->selectedIndexes();
			if (model->rowCount() > 0)
				ui.txt_DetNum->setText(QString::number(LMCount));
			if (LaserPose != NULL)
			{
				ui.le_effmark->setText(QString::number(LaserPose[0]));
				ui.le_ang->setText(QString::number(LaserPose[1]));
				ui.le_px->setText(QString::number(LaserPose[2]));
				ui.le_py->setText(QString::number(LaserPose[3]));
			}
			for (int i = 0; i < LMCount; i++)
			{
				model->setItem(i, 0, new QStandardItem(QString::number(i)));
				model->setItem(i, 1, new QStandardItem(QString::number(*(LandmarkData + 1 + 3 * i) * 1000)));
				model->setItem(i, 2, new QStandardItem(QString::number(*(LandmarkData + 2 + 3 * i) * 1000)));
			}
			if (!selected.isEmpty())
			{
				foreach(const QModelIndex&index, selected)
				{
					int cur_row = index.row();
					ui.TB_LMDetectInfo->selectionModel()->select(index, QItemSelectionModel::Select);
					QModelIndex index1 = model->index(index.row(), 0, QModelIndex());
					slotRowClicked(index1);
				}
			}
		}
	}
	if (view_Kind == 2)
	{
		int lastRowNUm = ui.TBW_lmkget->rowCount();
		if (lastRowNUm > 0)
		{
			for (int i = 0; i < lastRowNUm; i++)
			{
				ui.TBW_lmkget->removeRow(0);
			}
		}
		if (LandmarkData != NULL&&LMCount > 0)
		{
			ui.TBW_lmkget->setRowCount(LMCount + 1);
			ui.txt_GetNum->setText(QString::number(LMCount));
			for (int pi = 0; pi < LMCount + 1; pi++)
			{
				ui.TBW_lmkget->setItem(pi, 0, new QTableWidgetItem(QString::number(*(LandmarkData + 6 * pi))));//ID
				ui.TBW_lmkget->setItem(pi, 1, new QTableWidgetItem(QString::number(*(LandmarkData + 1 + 6 * pi))));//X
				ui.TBW_lmkget->setItem(pi, 2, new QTableWidgetItem(QString::number(*(LandmarkData + 2 + 6 * pi))));//Y
				ui.TBW_lmkget->setItem(pi, 3, new QTableWidgetItem(QString::number(*(LandmarkData + 5 + 6 * pi))));//��id
				checkbox = new QCheckBox();
				if (*(LandmarkData + 4 + 6 * pi) == 0)//��״
					checkbox->setChecked(true);
				else
					checkbox->setChecked(false);
				ui.TBW_lmkget->setCellWidget(pi, 4, checkbox);


				ui.TBW_lmkget->setItem(pi, 5, new QTableWidgetItem(QString::number(*(LandmarkData + 3 + 6 * pi))));//�ߴ�
				checkbox = new QCheckBox();
				checkbox->setChecked(true);
				ui.TBW_lmkget->setCellWidget(pi, 6, checkbox);
				PushButton = new QPushButton();
				PushButton->setText(QString::fromLocal8Bit("ɾ��"));
				connect(PushButton, SIGNAL(clicked()), this, SLOT(slot_delete()));//����ɾ����ť���źźͲ�
				ui.TBW_lmkget->setCellWidget(pi, 7, PushButton);
				if (pi == LMCount)
				{
					InsertRow(pi);
				}
				for (size_t j = 0; j < 7; j++)
				{
					if (j != 4 && j != 6)
					{
						ui.TBW_lmkget->item(pi, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
					}
				}
			}
		}

	}
}
#pragma endregion

#pragma region tableview��ѡ�����л���Ӧ����(ѡ�аб�����Ϊ��ɫ)
void KI::slotRowClicked(const QModelIndex &index)//�����б�̽��ĳ��
{
	if (index.isValid())
	{
		QVariant data = index.data();
		pclfunc->RedLMDisplay(1, data.toInt());
	}
}
#pragma endregion

#pragma region ����tablewidgetĳ�У����б�����Ϊ��ɫ��ʾ
void KI::slot_CellClick(int row, int column)
{
	QTableWidgetItem * item1 = new QTableWidgetItem;
	item1 = ui.TBW_lmkget->item(row, 0);
	int LMNO = item1->text().toInt() + 1;
	pclfunc->RedSetLMDisplay(LMNO);
}
#pragma endregion

#pragma region ɾ��tablewidget�е�ĳ��
void KI::slot_delete()
{
	int row = ui.TBW_lmkget->rowCount();
	get_cur_row = ui.TBW_lmkget->currentRow();
	if (get_cur_row != -1)
	{
		QTableWidgetItem * item1 = new QTableWidgetItem;
		item1 = ui.TBW_lmkget->item(get_cur_row, 0);
		int LMNO = item1->text().toInt() + 1;
		pclfunc->DeleteOneSetLMDisplay(LMNO);
		ui.TBW_lmkget->removeRow(get_cur_row);
	}
	ui.txt_GetNum->setText(QString::number(row - 1));
}
#pragma endregion

bool f_FirstChecked = true;
#pragma region ��ѡ��checked������(��Ҫѡ�����һ�У�Ȼ��ѡ�и�ѡ��)
void KI::slot_addrow(int state)
{
	try
	{
		if (state == Qt::Checked) // "ѡ��"
		{
			int row = ui.TBW_lmkget->rowCount();
			int cur_row = ui.TBW_lmkget->currentRow();
			ui.TBW_lmkget->setRowCount(row + 1);
			InsertRow(row);
			ui.txt_GetNum->setText(QString::number(row + 1));
			if (cur_row == row - 1 && (cur_row != 0 || f_FirstChecked))
			{
				f_FirstChecked = false;
				if (row > 0)
				{
					PushButton->setEnabled(true);
				}
			}
		}
	}
	catch (exception&e)
	{
		QString err = e.what();
		Infoview(2, "", err);
	}
}
#pragma endregion 

#pragma region ��ʾ��Ϣ
void KI::Infoview(int info_Kind, QString slotinfo, QString qstr)
{
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("hh:mm:ss.zzz") + tr(":");
	if (info_Kind == 1)//�ۺ�������
	{
		if (info.length() > 5)//̽�⡢��ȡ���������÷�����Ϣ��ʾ
		{
			ui.tbr_Info->append(current_date + info);//insertPlainTextʹ������ӵ����ݲ��������޷���ʾ
			if (info.contains(QString::fromLocal8Bit("�б�̽��"), Qt::CaseSensitive) || info.contains(QString::fromLocal8Bit("����ģʽ"), Qt::CaseSensitive))
			{
				ui.btn_Continue->setEnabled(true);
				ui.tabWidget->setCurrentIndex(1);
			}
			else if (info.contains(QString::fromLocal8Bit("�б��ȡ"), Qt::CaseSensitive))
			{
				ui.tabWidget->setCurrentIndex(0);
				setbtnenable(false);
			}
			info = "";
		}
		else
		{
			if (slotinfo.length() > 0)
			{
				ui.tbr_Info->append(current_date + slotinfo);
			}
		}
	}
	if (info_Kind == 2)
	{
		ui.tbr_Info->append(current_date + qstr);
	}
}
#pragma endregion

#pragma region ��ʼ��tablewidget
void KI::initialtablewidget()
{
	QStringList header;
	header << "ID" << QString::fromLocal8Bit("X����(mm)") << QString::fromLocal8Bit("Y����(mm)")
		<< QString::fromLocal8Bit("��ID") << QString::fromLocal8Bit("Բ��") << QString::fromLocal8Bit("�ߴ�(mm)")
		<< QString::fromLocal8Bit("ѡ��") << QString::fromLocal8Bit("����");
	ui.TBW_lmkget->setColumnCount(8); //��������Ϊ8
	ui.TBW_lmkget->setHorizontalHeaderLabels(header);
	ui.TBW_lmkget->horizontalHeader()->setStretchLastSection(true);
	QHeaderView *wheader = ui.TBW_lmkget->verticalHeader();
	wheader->setHidden(true);
	ui.TBW_lmkget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
	ui.TBW_lmkget->setSelectionMode(QAbstractItemView::SingleSelection);//����ѡ������
	ui.TBW_lmkget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.TBW_lmkget->setRowCount(1);//��仰�����Ҫ��Ҫ��Ȼû����һ����ʾ
	InsertRow(0);
	ui.txt_GetNum->setText("1");
	connect(ui.TBW_lmkget, SIGNAL(cellClicked(int, int)), this, SLOT(slot_CellClick(int, int)));
	connect(ui.TBW_lmkget, SIGNAL(cellChanged(int, int)), this, SLOT(slot_cellChanged(int, int)));
}
#pragma endregion

#pragma region �ı�tabwidget������ʾ�б�
void KI::slot_cellChanged(int row, int column)
{
	if (column == 1 || column == 2)
	{
		QTableWidgetItem * itemx = new QTableWidgetItem;
		int add_x = 0, add_y = 0, add_lmid = 0;
		QTableWidgetItem * itemy = new QTableWidgetItem;
		if (column == 1)
		{
			itemx = ui.TBW_lmkget->item(row, column);
			if (itemx != NULL)
				add_x = itemx->text().toInt();
			itemy = ui.TBW_lmkget->item(row, column + 1);
			if (itemy != NULL)
				add_y = itemy->text().toInt();
		}
		else
		{
			itemy = ui.TBW_lmkget->item(row, column);
			if (itemy != NULL)
			{
				QString sy = itemx->text();
				add_y = itemy->text().toInt();
			}
			itemx = ui.TBW_lmkget->item(row, column - 1);
			if (itemx != NULL)
			{
				QString sx = itemy->text();
				add_x = itemx->text().toInt();
			}
		}
		QTableWidgetItem *itemid = new QTableWidgetItem;
		itemid = ui.TBW_lmkget->item(row, 0);
		if (itemid != NULL)
			add_lmid = itemid->text().toInt() + 1;
		if (add_x != 0 || add_y != 0)
			pclfunc->AddOneSetLMDisplay(add_x, add_y, add_lmid);
	}
}
#pragma endregion

#pragma region ����tablewidget���һ�п�����
void KI::InsertRow(int row)
{
	ui.TBW_lmkget->setItem(row, 0, new QTableWidgetItem(QString::number(row)));//ID
	ui.TBW_lmkget->setItem(row, 1, new QTableWidgetItem(tr("0")));//X
	ui.TBW_lmkget->setItem(row, 2, new QTableWidgetItem(tr("0")));//Y
	ui.TBW_lmkget->setItem(row, 3, new QTableWidgetItem(tr("0")));//��id
	checkbox = new QCheckBox();//Ҫ�õĵط�����Ҫ��ʵ����
	checkbox->setChecked(true);
	ui.TBW_lmkget->setCellWidget(row, 4, checkbox);
	ui.TBW_lmkget->setItem(row, 5, new QTableWidgetItem(tr("80")));//�ߴ�
	QCheckBox* chec = new QCheckBox();
	chec->setChecked(false);
	ui.TBW_lmkget->setCellWidget(row, 6, chec);
	connect(chec, SIGNAL(stateChanged(int)), this, SLOT(slot_addrow(int)));
	PushButton = new QPushButton();
	PushButton->setText(QString::fromLocal8Bit("ɾ��"));
	PushButton->setEnabled(false);
	connect(PushButton, SIGNAL(clicked()), this, SLOT(slot_delete()));
	ui.TBW_lmkget->setCellWidget(row, 7, PushButton);
	/*for (size_t j = 0; j < 7; j++)
	{
	if (j != 4 && j != 6)
	{
	ui.TBW_lmkget->item(row,j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	}
	}*/
}
#pragma endregion 

#pragma region �б�����
void KI::LMSettingClick()
{
	try
	{
		int RowNum = ui.TBW_lmkget->rowCount() - 1;//Ҫ���õİб�����
		QString SGet_TargetNum = QString("%1").arg(RowNum, 4, 16, QLatin1Char('0'));
		n_setpackage = RowNum / Everypack_Setlm + 1;//1����ຬ��100���б�
		QString Ord_First = tr("0020000000000000010100070000000000000000060300000000") + SGet_TargetNum + tr("0002");
		Ord_First = tr("FFAA") + Ord_First + GetBCCXorCode(Ord_First) + tr("EEEE");
		QByteArray send_GetMess = QString2Hex(Ord_First);
		tcpSocket->write(send_GetMess);
		int L_Frame = 0;
		for (int i = 0; i < n_setpackage; i++)
		{
			QString Ord_GetTarget = "";
			if (RowNum>(i + 1)*Everypack_Setlm)
				L_Frame = Everypack_Setlm * 14 + 28;//֡��(ÿһ���б�ռ14���ֽ�)
			else
				L_Frame = (RowNum - i*Everypack_Setlm) * 14 + 28;
			QString SL_Frame = QString("%1").arg(L_Frame, 4, 16, QLatin1Char('0'));
			for (int j = i*Everypack_Setlm; j < (i + 1)*Everypack_Setlm; j++)
			{
				int SerialNum = ui.TBW_lmkget->item(j, 0)->text().toInt();//ID
				QString S_SerialNum = QString("%1").arg(SerialNum, 4, 16, QLatin1Char('0'));
				int X_Pos = ui.TBW_lmkget->item(j, 1)->text().toInt();
				int Y_Pos = ui.TBW_lmkget->item(j, 2)->text().toInt();
				QString SX_Pos, SY_Pos;
				if (X_Pos >= 0)
					SX_Pos = QString("%1").arg(X_Pos, 8, 16, QLatin1Char('0'));
				else
					SX_Pos = QString("%1").arg(X_Pos, 8, 16, QLatin1Char('0')).right(8);
				if (Y_Pos >= 0)
					SY_Pos = QString("%1").arg(Y_Pos, 8, 16, QLatin1Char('0'));
				else
					SY_Pos = QString("%1").arg(Y_Pos, 8, 16, QLatin1Char('0')).right(8);
				Ord_GetTarget += S_SerialNum + SX_Pos + SY_Pos + "00000050";
				if (j == RowNum - 1)
				{
					Ord_GetTarget = SL_Frame + "000000000000010100070000000000000000060300000001" + Ord_GetTarget;
					Ord_GetTarget = "ffaa" + Ord_GetTarget + GetBCCXorCode(Ord_GetTarget) + "EEEE";
					send_GetMess = QString2Hex(Ord_GetTarget);
					tcpSocket->write(send_GetMess);
					break;
				}
				if (j == (i + 1)*Everypack_Setlm - 1)
				{
					Ord_GetTarget = SL_Frame + "000000000000010100070000000000000000060300000001" + Ord_GetTarget;
					Ord_GetTarget = "ffaa" + Ord_GetTarget + GetBCCXorCode(Ord_GetTarget) + "EEEE";
					send_GetMess = QString2Hex(Ord_GetTarget);
					tcpSocket->write(send_GetMess);
				}
			}
		}
	}
	catch (exception&e)
	{
		QString err = e.what();
		Infoview(2, "", err);
	}
}
#pragma endregion

#pragma region �б��ѯ
void KI::LMQueryClick()
{
	QByteArray send_GetMess = QString2Hex(tr("FFAA001E00000000000001010007000000000000000006060000000000000019EEEE"));
	tcpSocket->write(send_GetMess);
	initialtablewidget();
}
#pragma endregion

#pragma region ����
void KI::NavClick()
{
	if (f_tcpNetConn)
	{
		navpar = new navpara(this);
		navpar->setWindowModality(Qt::ApplicationModal);
		navpar->show();
		if (model != NULL)
		{
			delete model;
			model = NULL;
		}
		model = new QStandardItemModel();
		model->setColumnCount(3);
		model->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("���"));
		model->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("X����(mm)"));
		model->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("Y����(mm)"));
		ui.TB_LMDetectInfo->setModel(model);
		///������Դ����������
		QHeaderView *theader = ui.TB_LMDetectInfo->verticalHeader();
		theader->setHidden(true);
		///
		ui.TB_LMDetectInfo->setSelectionBehavior(QAbstractItemView::SelectRows);//����ѡ��ģʽΪѡ����
		ui.TB_LMDetectInfo->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
		ui.TB_LMDetectInfo->setColumnWidth(0, 40);
		ui.TB_LMDetectInfo->setColumnWidth(0, 60);
		ui.TB_LMDetectInfo->setColumnWidth(0, 60);
		info = QString::fromLocal8Bit("����ģʽ���óɹ���");
		//if (lmid) delete[] lmid; lmid = NULL;
	}
	else
	{
		QString str = QString::fromLocal8Bit("����ʧ�ܣ�����δ���ӻ��ѶϿ���");
		Infoview(2, "", str);
	}
}
void KI::NavWay(QString layerId, QString datakind)
{
	LMQueryClick();
	QString qsnav = tr("00200000000000000101000700000000000000000604000000040100") + layerId;
	QString BCC = GetBCCXorCode(qsnav);
	qsnav = tr("FFAA") + qsnav + BCC + tr("EEEE");
	QByteArray zl_Nav = QString2Hex(qsnav);
	tcpSocket->write(zl_Nav);
}
#pragma endregion 

#pragma region ����btn�ܷ�ʹ��
void KI::setbtnenable(bool f_enable)
{
	ui.btn_MarkDetect->setEnabled(f_enable);
	ui.btn_LMGet->setEnabled(f_enable);
	ui.btn_nav->setEnabled(f_enable);
	ui.tabWidget->setEnabled(f_enable);
}
#pragma endregion

KI::~KI()
{
}