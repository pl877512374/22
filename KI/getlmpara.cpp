#include "getlmpara.h"
int n_scantime = 0;//靶标获取次数
GetLmPara::GetLmPara(KI *ki, QWidget *parent)
: QWidget(parent)
{
	ui.setupUi(this);
	this->GKI = ki;
	this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
	QPalette palette(this->palette());
	palette.setColor(QPalette::Background, Qt::lightGray);
	this->setPalette(palette);
	ui.cmb_GetModel->insertItem(0, QString::fromLocal8Bit("普通模式"));
	ui.cmb_GetModel->insertItem(1, QString::fromLocal8Bit("添加模式"));
	ui.cmb_LMType->insertItem(0, QString::fromLocal8Bit("圆柱"));
	ui.cmb_LMType->insertItem(1, QString::fromLocal8Bit("平面"));
}
void GetLmPara::CanClick()
{
	this->close();
}
void GetLmPara::SureClick()
{
	n_scantime = ui.le_ScanTime->text().toInt();
	QString model,layerID,scantime,selfangle,selfx,selfy,type,size;
	model = ui.cmb_GetModel->currentIndex() == 0 ? "0002" : "0003";
	layerID = QString("%1").arg(ui.le_LayerID->text().toInt(), 4, 16, QChar('0'));
	scantime = QString("%1").arg(n_scantime, 4, 16, QChar('0'));
	//QString("1%").arg(ui.le_ScanTime->text().toInt(), 4, 16, QLatin1Char('0'));
	selfangle = QString("%1").arg(ui.le_CurAngle->text().toInt(), 8, 16, QChar('0'));
	selfx = QString("%1").arg(ui.le_CurX->text().toInt(), 8, 16, QChar('0'));
	selfy = QString("%1").arg(ui.le_CurY->text().toInt(), 8, 16, QChar('0'));
	type = ui.cmb_LMType->currentIndex() == 0 ? "00" : "02";
	size = QString("%1").arg(ui.le_LMSize->text().toInt(), 2, 16, QChar('0'));
	GKI->GetLMWay(model,layerID,scantime,selfangle,selfx,selfy,type,size);
	this->close();
}
GetLmPara::~GetLmPara()
{

}
