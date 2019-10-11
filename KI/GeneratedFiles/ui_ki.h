/********************************************************************************
** Form generated from reading UI file 'ki.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KI_H
#define UI_KI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KIClass
{
public:
    QAction *actionOpen;
    QWidget *centralWidget;
    QGroupBox *groupBox1;
    QLineEdit *hostLineEdit;
    QLineEdit *portLineEdit;
    QLabel *label;
    QLabel *label_2;
    QPushButton *btn_BroadCast;
    QTextBrowser *tbr_Info;
    QPushButton *btn_Conn;
    QTabWidget *tabWidget;
    QWidget *tab_Get;
    QGroupBox *groupBox_3;
    QLabel *label_4;
    QLineEdit *txt_GetNum;
    QTableWidget *TBW_lmkget;
    QPushButton *pushButton;
    QPushButton *btn_outdata;
    QPushButton *btn_query;
    QPushButton *btn_setting;
    QWidget *tab_Nav;
    QGroupBox *groupBox_2;
    QTableView *TB_LMDetectInfo;
    QLabel *label_3;
    QLineEdit *txt_DetNum;
    QGroupBox *groupBox_4;
    QGroupBox *groupBox_5;
    QPushButton *btn_Continue;
    QGroupBox *groupBox_6;
    QGroupBox *groupBox_7;
    QLabel *label_5;
    QLineEdit *le_effmark;
    QLineEdit *le_ang;
    QLabel *label_6;
    QLineEdit *le_px;
    QLabel *label_7;
    QLineEdit *le_py;
    QLabel *label_8;
    QGroupBox *groupBox2;
    QPushButton *btn_nav;
    QPushButton *btn_chibiao;
    QPushButton *btn_XDown;
    QPushButton *btn_Rotate;
    QPushButton *btn_YUP;
    QPushButton *btn_ZUP;
    QPushButton *btn_PointTest;
    QPushButton *btn_MarkDetect;
    QPushButton *btn_ZDOWN;
    QPushButton *btn_YDown;
    QPushButton *btn_LMGet;
    QPushButton *btn_XUP;
    QGroupBox *groupBox;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *KIClass)
    {
        if (KIClass->objectName().isEmpty())
            KIClass->setObjectName(QStringLiteral("KIClass"));
        KIClass->setWindowModality(Qt::ApplicationModal);
        KIClass->resize(1136, 677);
        actionOpen = new QAction(KIClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        centralWidget = new QWidget(KIClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupBox1 = new QGroupBox(centralWidget);
        groupBox1->setObjectName(QStringLiteral("groupBox1"));
        groupBox1->setGeometry(QRect(10, 0, 241, 641));
        hostLineEdit = new QLineEdit(groupBox1);
        hostLineEdit->setObjectName(QStringLiteral("hostLineEdit"));
        hostLineEdit->setGeometry(QRect(100, 30, 113, 20));
        portLineEdit = new QLineEdit(groupBox1);
        portLineEdit->setObjectName(QStringLiteral("portLineEdit"));
        portLineEdit->setGeometry(QRect(100, 70, 113, 20));
        label = new QLabel(groupBox1);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 30, 54, 12));
        label_2 = new QLabel(groupBox1);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 70, 54, 12));
        btn_BroadCast = new QPushButton(groupBox1);
        btn_BroadCast->setObjectName(QStringLiteral("btn_BroadCast"));
        btn_BroadCast->setGeometry(QRect(30, 120, 75, 23));
        tbr_Info = new QTextBrowser(groupBox1);
        tbr_Info->setObjectName(QStringLiteral("tbr_Info"));
        tbr_Info->setGeometry(QRect(10, 170, 221, 461));
        btn_Conn = new QPushButton(groupBox1);
        btn_Conn->setObjectName(QStringLiteral("btn_Conn"));
        btn_Conn->setGeometry(QRect(130, 120, 75, 23));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(260, 330, 871, 311));
        tab_Get = new QWidget();
        tab_Get->setObjectName(QStringLiteral("tab_Get"));
        groupBox_3 = new QGroupBox(tab_Get);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(0, 0, 861, 251));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(170, 10, 54, 12));
        txt_GetNum = new QLineEdit(groupBox_3);
        txt_GetNum->setObjectName(QStringLiteral("txt_GetNum"));
        txt_GetNum->setGeometry(QRect(270, 10, 51, 21));
        TBW_lmkget = new QTableWidget(groupBox_3);
        TBW_lmkget->setObjectName(QStringLiteral("TBW_lmkget"));
        TBW_lmkget->setGeometry(QRect(10, 40, 731, 171));
        pushButton = new QPushButton(groupBox_3);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(750, 50, 111, 23));
        btn_outdata = new QPushButton(groupBox_3);
        btn_outdata->setObjectName(QStringLiteral("btn_outdata"));
        btn_outdata->setGeometry(QRect(750, 90, 111, 23));
        btn_query = new QPushButton(groupBox_3);
        btn_query->setObjectName(QStringLiteral("btn_query"));
        btn_query->setGeometry(QRect(750, 130, 111, 23));
        btn_setting = new QPushButton(groupBox_3);
        btn_setting->setObjectName(QStringLiteral("btn_setting"));
        btn_setting->setGeometry(QRect(750, 170, 111, 23));
        tabWidget->addTab(tab_Get, QString());
        tab_Nav = new QWidget();
        tab_Nav->setObjectName(QStringLiteral("tab_Nav"));
        groupBox_2 = new QGroupBox(tab_Nav);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(370, 20, 271, 221));
        TB_LMDetectInfo = new QTableView(groupBox_2);
        TB_LMDetectInfo->setObjectName(QStringLiteral("TB_LMDetectInfo"));
        TB_LMDetectInfo->setGeometry(QRect(20, 40, 231, 151));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(50, 20, 54, 12));
        txt_DetNum = new QLineEdit(groupBox_2);
        txt_DetNum->setObjectName(QStringLiteral("txt_DetNum"));
        txt_DetNum->setGeometry(QRect(110, 10, 51, 21));
        groupBox_4 = new QGroupBox(tab_Nav);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 20, 141, 241));
        groupBox_5 = new QGroupBox(groupBox_4);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(190, 90, 191, 201));
        btn_Continue = new QPushButton(tab_Nav);
        btn_Continue->setObjectName(QStringLiteral("btn_Continue"));
        btn_Continue->setEnabled(false);
        btn_Continue->setGeometry(QRect(250, 0, 131, 21));
        groupBox_6 = new QGroupBox(tab_Nav);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setGeometry(QRect(160, 20, 191, 241));
        groupBox_7 = new QGroupBox(groupBox_6);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        groupBox_7->setGeometry(QRect(190, 90, 191, 201));
        label_5 = new QLabel(groupBox_6);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 30, 51, 16));
        le_effmark = new QLineEdit(groupBox_6);
        le_effmark->setObjectName(QStringLiteral("le_effmark"));
        le_effmark->setGeometry(QRect(90, 30, 71, 20));
        le_ang = new QLineEdit(groupBox_6);
        le_ang->setObjectName(QStringLiteral("le_ang"));
        le_ang->setGeometry(QRect(90, 70, 71, 20));
        label_6 = new QLabel(groupBox_6);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 70, 71, 16));
        le_px = new QLineEdit(groupBox_6);
        le_px->setObjectName(QStringLiteral("le_px"));
        le_px->setGeometry(QRect(90, 110, 71, 20));
        label_7 = new QLabel(groupBox_6);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 110, 61, 16));
        le_py = new QLineEdit(groupBox_6);
        le_py->setObjectName(QStringLiteral("le_py"));
        le_py->setGeometry(QRect(90, 150, 71, 20));
        label_8 = new QLabel(groupBox_6);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 150, 61, 16));
        tabWidget->addTab(tab_Nav, QString());
        groupBox2 = new QGroupBox(centralWidget);
        groupBox2->setObjectName(QStringLiteral("groupBox2"));
        groupBox2->setGeometry(QRect(270, 0, 861, 51));
        btn_nav = new QPushButton(groupBox2);
        btn_nav->setObjectName(QStringLiteral("btn_nav"));
        btn_nav->setGeometry(QRect(580, 20, 75, 23));
        btn_chibiao = new QPushButton(groupBox2);
        btn_chibiao->setObjectName(QStringLiteral("btn_chibiao"));
        btn_chibiao->setGeometry(QRect(350, 20, 41, 23));
        btn_XDown = new QPushButton(groupBox2);
        btn_XDown->setObjectName(QStringLiteral("btn_XDown"));
        btn_XDown->setGeometry(QRect(150, 20, 41, 23));
        btn_Rotate = new QPushButton(groupBox2);
        btn_Rotate->setObjectName(QStringLiteral("btn_Rotate"));
        btn_Rotate->setGeometry(QRect(270, 20, 41, 23));
        btn_YUP = new QPushButton(groupBox2);
        btn_YUP->setObjectName(QStringLiteral("btn_YUP"));
        btn_YUP->setGeometry(QRect(190, 20, 41, 23));
        btn_ZUP = new QPushButton(groupBox2);
        btn_ZUP->setObjectName(QStringLiteral("btn_ZUP"));
        btn_ZUP->setGeometry(QRect(30, 20, 41, 23));
        btn_PointTest = new QPushButton(groupBox2);
        btn_PointTest->setObjectName(QStringLiteral("btn_PointTest"));
        btn_PointTest->setGeometry(QRect(310, 20, 41, 23));
        btn_MarkDetect = new QPushButton(groupBox2);
        btn_MarkDetect->setObjectName(QStringLiteral("btn_MarkDetect"));
        btn_MarkDetect->setGeometry(QRect(420, 20, 75, 23));
        btn_ZDOWN = new QPushButton(groupBox2);
        btn_ZDOWN->setObjectName(QStringLiteral("btn_ZDOWN"));
        btn_ZDOWN->setGeometry(QRect(70, 20, 41, 23));
        btn_YDown = new QPushButton(groupBox2);
        btn_YDown->setObjectName(QStringLiteral("btn_YDown"));
        btn_YDown->setGeometry(QRect(230, 20, 41, 23));
        btn_LMGet = new QPushButton(groupBox2);
        btn_LMGet->setObjectName(QStringLiteral("btn_LMGet"));
        btn_LMGet->setGeometry(QRect(500, 20, 75, 23));
        btn_XUP = new QPushButton(groupBox2);
        btn_XUP->setObjectName(QStringLiteral("btn_XUP"));
        btn_XUP->setGeometry(QRect(110, 20, 41, 23));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(270, 50, 861, 271));
        KIClass->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(KIClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        KIClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(KIClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        KIClass->setStatusBar(statusBar);

        retranslateUi(KIClass);
        QObject::connect(btn_ZDOWN, SIGNAL(clicked()), KIClass, SLOT(MoveZDOWN()));
        QObject::connect(btn_YUP, SIGNAL(clicked()), KIClass, SLOT(MoveYUP()));
        QObject::connect(btn_XUP, SIGNAL(clicked()), KIClass, SLOT(MoveXUP()));
        QObject::connect(btn_XDown, SIGNAL(clicked()), KIClass, SLOT(MoveXDOWN()));
        QObject::connect(btn_YDown, SIGNAL(clicked()), KIClass, SLOT(MoveYDOWN()));
        QObject::connect(btn_PointTest, SIGNAL(clicked()), KIClass, SLOT(PointTest()));
        QObject::connect(pushButton, SIGNAL(clicked()), KIClass, SLOT(Cylinder()));
        QObject::connect(btn_BroadCast, SIGNAL(clicked()), KIClass, SLOT(Broadcast()));
        QObject::connect(btn_chibiao, SIGNAL(clicked()), KIClass, SLOT(CBClick()));
        QObject::connect(btn_MarkDetect, SIGNAL(clicked()), KIClass, SLOT(LandMarkDetect_Click()));
        QObject::connect(btn_Continue, SIGNAL(clicked()), KIClass, SLOT(Con_Click()));
        QObject::connect(btn_LMGet, SIGNAL(clicked()), KIClass, SLOT(LMGetClick()));
        QObject::connect(btn_ZUP, SIGNAL(clicked()), KIClass, SLOT(MoveZUP()));
        QObject::connect(btn_setting, SIGNAL(clicked()), KIClass, SLOT(LMSettingClick()));
        QObject::connect(btn_query, SIGNAL(clicked()), KIClass, SLOT(LMQueryClick()));
        QObject::connect(btn_nav, SIGNAL(clicked()), KIClass, SLOT(NavClick()));
        QObject::connect(btn_Conn, SIGNAL(clicked()), KIClass, SLOT(ConnClick()));
        QObject::connect(btn_Rotate, SIGNAL(clicked()), KIClass, SLOT(RotateClick()));

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(KIClass);
    } // setupUi

    void retranslateUi(QMainWindow *KIClass)
    {
        KIClass->setWindowTitle(QApplication::translate("KIClass", "KI", 0));
        actionOpen->setText(QApplication::translate("KIClass", "Open", 0));
        groupBox1->setTitle(QApplication::translate("KIClass", "\347\275\221\347\273\234\350\256\276\347\275\256", 0));
        label->setText(QApplication::translate("KIClass", "IP\345\234\260\345\235\200\357\274\232", 0));
        label_2->setText(QApplication::translate("KIClass", "\347\253\257\345\217\243\345\217\267\357\274\232", 0));
        btn_BroadCast->setText(QApplication::translate("KIClass", "\345\271\277\346\222\255", 0));
        btn_Conn->setText(QApplication::translate("KIClass", "\350\277\236\346\216\245", 0));
        groupBox_3->setTitle(QApplication::translate("KIClass", "\351\235\266\346\240\207\350\216\267\345\217\226\344\277\241\346\201\257", 0));
        label_4->setText(QApplication::translate("KIClass", "\351\235\266\346\240\207\346\200\273\346\225\260\357\274\232", 0));
        pushButton->setText(QApplication::translate("KIClass", "\345\257\274\345\205\245\346\226\207\344\273\266\351\235\266\346\240\207", 0));
        btn_outdata->setText(QApplication::translate("KIClass", "\345\257\274\345\207\272\351\200\211\344\270\255\351\235\266\346\240\207\344\277\241\346\201\257", 0));
        btn_query->setText(QApplication::translate("KIClass", "\346\237\245\350\257\242\344\277\241\346\201\257", 0));
        btn_setting->setText(QApplication::translate("KIClass", "\350\256\276\347\275\256\344\277\241\346\201\257", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_Get), QApplication::translate("KIClass", "\351\235\266\346\240\207\350\216\267\345\217\226/\350\256\276\347\275\256", 0));
        groupBox_2->setTitle(QApplication::translate("KIClass", "\351\235\266\346\240\207\344\277\241\346\201\257", 0));
        label_3->setText(QApplication::translate("KIClass", "\351\235\266\346\240\207\346\200\273\346\225\260\357\274\232", 0));
        groupBox_4->setTitle(QApplication::translate("KIClass", "\346\211\253\346\217\217\346\225\260\346\215\256\344\277\241\346\201\257", 0));
        groupBox_5->setTitle(QApplication::translate("KIClass", "GroupBox", 0));
        btn_Continue->setText(QApplication::translate("KIClass", "\350\216\267\345\217\226\350\277\236\347\273\255\346\263\242\345\275\242", 0));
        groupBox_6->setTitle(QApplication::translate("KIClass", "\346\277\200\345\205\211\345\231\250\344\275\215\347\275\256", 0));
        groupBox_7->setTitle(QApplication::translate("KIClass", "GroupBox", 0));
        label_5->setText(QApplication::translate("KIClass", "\346\234\211\346\225\210\346\240\207\345\277\227", 0));
        label_6->setText(QApplication::translate("KIClass", "\346\226\271\345\220\221\350\247\222(deg)", 0));
        label_7->setText(QApplication::translate("KIClass", "X\345\235\220\346\240\207(mm)", 0));
        label_8->setText(QApplication::translate("KIClass", "Y\345\235\220\346\240\207(mm)", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_Nav), QApplication::translate("KIClass", "\345\257\274\350\210\252/\346\216\242\346\265\213", 0));
        groupBox2->setTitle(QApplication::translate("KIClass", "\346\230\276\347\244\272\350\256\276\347\275\256", 0));
        btn_nav->setText(QApplication::translate("KIClass", "\345\257\274\350\210\252", 0));
        btn_chibiao->setText(QApplication::translate("KIClass", "\345\260\272\346\240\207", 0));
        btn_XDown->setText(QApplication::translate("KIClass", "X-", 0));
        btn_Rotate->setText(QApplication::translate("KIClass", "90Z", 0));
        btn_YUP->setText(QApplication::translate("KIClass", "Y+", 0));
        btn_ZUP->setText(QApplication::translate("KIClass", "Z+", 0));
        btn_PointTest->setText(QApplication::translate("KIClass", "\347\272\277", 0));
        btn_MarkDetect->setText(QApplication::translate("KIClass", "\351\235\266\346\240\207\346\216\242\346\265\213", 0));
        btn_ZDOWN->setText(QApplication::translate("KIClass", "Z-", 0));
        btn_YDown->setText(QApplication::translate("KIClass", "Y-", 0));
        btn_LMGet->setText(QApplication::translate("KIClass", "\351\235\266\346\240\207\350\216\267\345\217\226", 0));
        btn_XUP->setText(QApplication::translate("KIClass", "X+", 0));
        groupBox->setTitle(QApplication::translate("KIClass", "GroupBox", 0));
    } // retranslateUi

};

namespace Ui {
    class KIClass: public Ui_KIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KI_H
