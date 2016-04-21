/********************************************************************************
** Form generated from reading UI file 'GridItem.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRIDITEM_H
#define UI_GRIDITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GridItem
{
public:

    void setupUi(QWidget *GridItem)
    {
        if (GridItem->objectName().isEmpty())
            GridItem->setObjectName(QStringLiteral("GridItem"));
        GridItem->resize(208, 153);

        retranslateUi(GridItem);

        QMetaObject::connectSlotsByName(GridItem);
    } // setupUi

    void retranslateUi(QWidget *GridItem)
    {
        GridItem->setWindowTitle(QApplication::translate("GridItem", "GridItem", 0));
    } // retranslateUi

};

namespace Ui {
    class GridItem: public Ui_GridItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRIDITEM_H
