#include "StreamerWidget.h"
#include "ui_StreamerWidget.h"

StreamerWidget::StreamerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StreamerWidget)
{
    ui->setupUi(this);
}

StreamerWidget::~StreamerWidget()
{
    delete ui;
}
