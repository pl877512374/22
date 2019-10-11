/********************************************************************************
** Form generated from reading UI file 'navpara.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NAVPARA_H
#define UI_NAVPARA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_navpara
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *le_layerid;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QWidget *navpara)
    {
        if (navpara->objectName().isEmpty())
            navpara->setObjectName(QStringLiteral("navpara"));
        navpara->resize(400, 300);
        label = new QLabel(navpara);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(60, 40, 81, 16));
        QFont font;
        font.setPointSize(11);
        label->setFont(font);
        label_2 = new QLabel(navpara);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(170, 40, 111, 16));
        label_2->setFont(font);
        label_3 = new QLabel(navpara);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(60, 100, 81, 16));
        label_3->setFont(font);
        le_layerid = new QLineEdit(navpara);
        le_layerid->setObjectName(QStringLiteral("le_layerid"));
        le_layerid->setGeometry(QRect(170, 100, 113, 20));
        pushButton = new QPushButton(navpara);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(70, 180, 75, 23));
        pushButton->setFont(font);
        pushButton_2 = new QPushButton(navpara);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(200, 180, 75, 23));
        pushButton_2->setFont(font);

        retranslateUi(navpara);
        QObject::connect(pushButton_2, SIGNAL(clicked()), navpara, SLOT(cancel()));
        QObject::connect(pushButton, SIGNAL(clicked()), navpara, SLOT(sureclick()));

        QMetaObject::connectSlotsByName(navpara);
    } // setupUi

    void retranslateUi(QWidget *navpara)
    {
        navpara->setWindowTitle(QApplication::translate("navpara", "\345\257\274\350\210\252\345\217\202\346\225\260\350\256\276\347\275\256", 0));
        label->setText(QApplication::translate("navpara", "\346\225\260\346\215\256\345\206\205\345\256\271", 0));
        label_2->setText(QApplication::translate("navpara", "\346\211\253\346\217\217+\345\256\232\344\275\215+\351\235\266\346\240\207", 0));
        label_3->setText(QApplication::translate("navpara", "\345\275\223\345\211\215\345\261\202ID", 0));
        le_layerid->setText(QApplication::translate("navpara", "0", 0));
        pushButton->setText(QApplication::translate("navpara", "\347\241\256\345\256\232", 0));
        pushButton_2->setText(QApplication::translate("navpara", "\345\217\226\346\266\210", 0));
    } // retranslateUi

};

namespace Ui {
    class navpara: public Ui_navpara {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NAVPARA_H
