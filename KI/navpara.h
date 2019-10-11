#ifndef NAVPARA_H
#define NAVPARA_H
#include"ki.h"
#include <QWidget>
#include "ui_navpara.h"

class navpara : public QWidget
{
	Q_OBJECT

public:
	navpara(KI *ki,QWidget *parent = 0);
	~navpara();

private:
	Ui::navpara ui;
	KI *NKI;
	private slots:
	void sureclick();
	void cancelclick();
};

#endif // NAVPARA_H
