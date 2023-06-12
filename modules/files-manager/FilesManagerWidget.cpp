#include "FilesManagerWidget.h"
#include "ui_FilesManagerWidget.h"

FilesManagerWidget::FilesManagerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilesManagerWidget)
{
    ui->setupUi(this);
}

FilesManagerWidget::~FilesManagerWidget()
{
    delete ui;
}
