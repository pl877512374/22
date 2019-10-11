#ifndef DETECTLMPARA_H
#define DETECTLMPARA_H

#include <QWidget>
#include "ui_detectlmpara.h"

class detectlmpara : public QWidget
{
	Q_OBJECT

public:
	detectlmpara(QWidget *parent = 0);
	~detectlmpara();

private:
	Ui::detectlmpara ui;
};

#endif // DETECTLMPARA_H
