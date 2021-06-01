#pragma once

#include <QtWidgets/QWidget>
#include "ui_ZipTool.h"

class ZipTool : public QWidget
{
    Q_OBJECT

public:
    ZipTool(QWidget *parent = Q_NULLPTR);

private:
    Ui::ZipToolClass ui;
};
