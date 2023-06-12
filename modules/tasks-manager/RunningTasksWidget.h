#ifndef RUNNINGTASKSWIDGET_H
#define RUNNINGTASKSWIDGET_H

#include <QWidget>
#include "TaskDescription.h"

namespace Ui {
class RunningTasksWidget;
}

class RunningTasksWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RunningTasksWidget(QWidget *parent = nullptr);
    ~RunningTasksWidget();

private:

    void handleRefreshBtnClick();
    std::list<TaskDescription> readProc();

    Ui::RunningTasksWidget *ui;
};

#endif // RUNNINGTASKSWIDGET_H
