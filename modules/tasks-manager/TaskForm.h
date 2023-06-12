#ifndef TASKFORM_H
#define TASKFORM_H

#include <QWidget>
#include "TaskDescription.h"

namespace Ui {
class TaskForm;
}

class TaskForm : public QWidget
{
    Q_OBJECT

public:
    TaskForm(QWidget *parent, TaskDescription  tsd);
    ~TaskForm() override;

private:
    void setUI();

    TaskDescription taskDescription;
    Ui::TaskForm *ui;

};

#endif // TASKFORM_H
