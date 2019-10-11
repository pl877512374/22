/********************************************************************************
** Form generated from reading UI file 'getlmpara.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GETLMPARA_H
#define UI_GETLMPARA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GetLmPara
{
public:
    QLineEdit *le_LayerID;
    QPushButton *btn_Sure;
    QLabel *label;
    QComboBox *cmb_GetModel;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *le_ScanTime;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QComboBox *cmb_LMType;
    QLabel *label_8;
    QLineEdit *le_CurAngle;
    QLineEdit *le_CurX;
    QLineEdit *le_CurY;
    QLineEdit *le_LMSize;
    QPushButton *btn_Cancel;

    void setupUi(QWidget *GetLmPara)
    {
        if (GetLmPara->objectName().isEmpty())
            GetLmPara->setObjectName(QStringLiteral("GetLmPara"));
        GetLmPara->setWindowModality(Qt::ApplicationModal);
        GetLmPara->resize(275, 423);
        le_LayerID = new QLineEdit(GetLmPara);
        le_LayerID->setObjectName(QStringLiteral("le_LayerID"));
        le_LayerID->setGeometry(QRect(140, 80, 113, 20));
        btn_Sure = new QPushButton(GetLmPara);
        btn_Sure->setObjectName(QStringLiteral("btn_Sure"));
        btn_Sure->setGeometry(QRect(30, 370, 75, 23));
        label = new QLabel(GetLmPara);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(60, 40, 71, 16));
        QFont font;
        font.setPointSize(10);
        label->setFont(font);
        cmb_GetModel = new QComboBox(GetLmPara);
        cmb_GetModel->setObjectName(QStringLiteral("cmb_GetModel"));
        cmb_GetModel->setGeometry(QRect(140, 40, 111, 22));
        label_2 = new QLabel(GetLmPara);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(60, 80, 71, 16));
        label_2->setFont(font);
        label_3 = new QLabel(GetLmPara);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(40, 120, 91, 16));
        label_3->setFont(font);
        le_ScanTime = new QLineEdit(GetLmPara);
        le_ScanTime->setObjectName(QStringLiteral("le_ScanTime"));
        le_ScanTime->setGeometry(QRect(140, 120, 111, 20));
        label_4 = new QLabel(GetLmPara);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 160, 111, 16));
        label_4->setFont(font);
        label_5 = new QLabel(GetLmPara);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(30, 200, 101, 16));
        label_5->setFont(font);
        label_6 = new QLabel(GetLmPara);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(30, 240, 101, 16));
        label_6->setFont(font);
        label_7 = new QLabel(GetLmPara);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(60, 270, 71, 16));
        label_7->setFont(font);
        cmb_LMType = new QComboBox(GetLmPara);
        cmb_LMType->setObjectName(QStringLiteral("cmb_LMType"));
        cmb_LMType->setGeometry(QRect(140, 270, 111, 22));
        label_8 = new QLabel(GetLmPara);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(30, 310, 101, 16));
        label_8->setFont(font);
        le_CurAngle = new QLineEdit(GetLmPara);
        le_CurAngle->setObjectName(QStringLiteral("le_CurAngle"));
        le_CurAngle->setGeometry(QRect(140, 160, 111, 20));
        le_CurX = new QLineEdit(GetLmPara);
        le_CurX->setObjectName(QStringLiteral("le_CurX"));
        le_CurX->setGeometry(QRect(140, 200, 111, 20));
        le_CurY = new QLineEdit(GetLmPara);
        le_CurY->setObjectName(QStringLiteral("le_CurY"));
        le_CurY->setGeometry(QRect(140, 240, 111, 20));
        le_LMSize = new QLineEdit(GetLmPara);
        le_LMSize->setObjectName(QStringLiteral("le_LMSize"));
        le_LMSize->setGeometry(QRect(140, 310, 111, 20));
        btn_Cancel = new QPushButton(GetLmPara);
        btn_Cancel->setObjectName(QStringLiteral("btn_Cancel"));
        btn_Cancel->setGeometry(QRect(150, 370, 75, 23));

        retranslateUi(GetLmPara);
        QObject::connect(btn_Cancel, SIGNAL(clicked()), GetLmPara, SLOT(CanClick()));
        QObject::connect(btn_Sure, SIGNAL(clicked()), GetLmPara, SLOT(SureClick()));

        QMetaObject::connectSlotsByName(GetLmPara);
    } // setupUi

    void retranslateUi(QWidget *GetLmPara)
    {
        GetLmPara->setWindowTitle(QApplication::translate("GetLmPara", "\351\235\266\346\240\207\350\216\267\345\217\226\345\217\202\346\225\260", 0));
        le_LayerID->setText(QApplication::translate("GetLmPara", "0", 0));
        btn_Sure->setText(QApplication::translate("GetLmPara", "\347\241\256\345\256\232", 0));
        label->setText(QApplication::translate("GetLmPara", "\350\216\267\345\217\226\346\250\241\345\274\217\357\274\232", 0));
        label_2->setText(QApplication::translate("GetLmPara", "\345\275\223\345\211\215\345\261\202ID\357\274\232", 0));
        label_3->setText(QApplication::translate("GetLmPara", "\351\235\266\346\240\207\346\211\253\346\217\217\346\254\241\346\225\260\357\274\232", 0));
        le_ScanTime->setText(QApplication::translate("GetLmPara", "20", 0));
        label_4->setText(QApplication::translate("GetLmPara", "\345\275\223\345\211\215\346\226\271\345\220\221\350\247\222(deg)\357\274\232", 0));
        label_5->setText(QApplication::translate("GetLmPara", "\345\275\223\345\211\215X\345\235\220\346\240\207(mm)\357\274\232", 0));
        label_6->setText(QApplication::translate("GetLmPara", "\345\275\223\345\211\215Y\345\235\220\346\240\207(mm)\357\274\232", 0));
        label_7->setText(QApplication::translate("GetLmPara", "\351\235\266\346\240\207\345\275\242\347\212\266\357\274\232", 0));
        label_8->setText(QApplication::translate("GetLmPara", "\351\235\266\346\240\207\345\260\272\345\257\270(mm)\357\274\232", 0));
        le_CurAngle->setText(QApplication::translate("GetLmPara", "0", 0));
        le_CurX->setText(QApplication::translate("GetLmPara", "0", 0));
        le_CurY->setText(QApplication::translate("GetLmPara", "0", 0));
        le_LMSize->setText(QApplication::translate("GetLmPara", "80", 0));
        btn_Cancel->setText(QApplication::translate("GetLmPara", "\345\217\226\346\266\210", 0));
    } // retranslateUi

};

namespace Ui {
    class GetLmPara: public Ui_GetLmPara {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GETLMPARA_H
