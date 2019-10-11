/********************************************************************************
** Form generated from reading UI file 'detectlmpara.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DETECTLMPARA_H
#define UI_DETECTLMPARA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_detectlmpara
{
public:

    void setupUi(QWidget *detectlmpara)
    {
        if (detectlmpara->objectName().isEmpty())
            detectlmpara->setObjectName(QStringLiteral("detectlmpara"));
        detectlmpara->resize(400, 300);

        retranslateUi(detectlmpara);

        QMetaObject::connectSlotsByName(detectlmpara);
    } // setupUi

    void retranslateUi(QWidget *detectlmpara)
    {
        detectlmpara->setWindowTitle(QApplication::translate("detectlmpara", "detectlmpara", 0));
    } // retranslateUi

};

namespace Ui {
    class detectlmpara: public Ui_detectlmpara {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DETECTLMPARA_H
