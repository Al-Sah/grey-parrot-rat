#include "TaskForm.h"

#include <utility>
#include "ui_TaskForm.h"

TaskForm::TaskForm(QWidget *parent, TaskDescription  tsd) :
    QWidget(parent),
    taskDescription(std::move(tsd)),
    ui(new Ui::TaskForm)
{
    ui->setupUi(this);
    setUI();
}

TaskForm::~TaskForm()
{
    delete ui;
}

void TaskForm::setUI() {
    ui->labelName->setText(QString::fromStdString(taskDescription.programName));
}
