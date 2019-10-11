#include "navpara.h"

navpara::navpara(KI* ki,QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->NKI = ki;
	this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);//���˳������С��������
	QPalette palette(this->palette());//Palette ����
	palette.setColor(QPalette::Background, Qt::lightGray);
	this->setPalette(palette);
}

navpara::~navpara()
{

}
void navpara::sureclick()
{
	int layerid = ui.le_layerid->text().toInt();
	QString qslayerid = QString("%1").arg(layerid, 4, 16, QLatin1Char('0'));
	NKI->NavWay(qslayerid);
	this->close();
}
void navpara::cancelclick()
{
	this->close();
}
