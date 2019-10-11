#ifndef GETLMPARA_H
#define GETLMPARA_H
#include "ki.h"
#include <QWidget>
#include "ui_getlmpara.h"
class GetLmPara : public QWidget
{
	Q_OBJECT

public:
	GetLmPara(KI *ki, QWidget *parent = 0);
	~GetLmPara();

private:
	Ui::GetLmPara ui;
	KI *GKI;
private slots:
	void CanClick();
	void SureClick();
};

#endif // GETLMPARA_H
