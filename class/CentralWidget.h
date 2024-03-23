#pragma once
#include <qwidget.h>


class CentralWidget :
    public QWidget
{
public:
    CentralWidget(QWidget* parent = nullptr);
    void paintEvent(QPaintEvent* event) override;
};
