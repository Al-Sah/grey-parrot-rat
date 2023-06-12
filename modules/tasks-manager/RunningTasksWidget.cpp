#include "RunningTasksWidget.h"
#include "ui_RunningTasksWidget.h"
#include "TaskForm.h"
#include "TaskDescription.h"


#include <proc/readproc.h>

RunningTasksWidget::RunningTasksWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RunningTasksWidget)
{
    ui->setupUi(this);

    connect(ui->btnRefresh, &QPushButton::clicked, this, &RunningTasksWidget::handleRefreshBtnClick);
}

RunningTasksWidget::~RunningTasksWidget()
{
    delete ui;
}

void RunningTasksWidget::handleRefreshBtnClick() {

    for (const auto &taskDescription: readProc()){
        auto item = new QListWidgetItem();
        auto widget = new TaskForm(this, taskDescription);
        item->setSizeHint(widget->sizeHint());

        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, widget);
    }
}

std::list<TaskDescription> RunningTasksWidget::readProc() {

    PROCTAB* proc = openproc(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS);

    std::list<TaskDescription> tasks;
    proc_t proc_info;
    memset(&proc_info, 0, sizeof(proc_info));
    while (readproc(proc, &proc_info) != nullptr) {
        TaskDescription task{
                .programName = proc_info.cmd,
                .taskId = proc_info.tid,
                .parentProcId = proc_info.ppid,

                .startTime = proc_info.start_time,
                .realPriority = proc_info.rtprio,
                .vmSize = proc_info.vm_size,

                .effectiveUserName = proc_info.euser,
                .realUserName =  proc_info.ruser,
                .savedUserName = proc_info.suser,
                .filesystemUserName =  proc_info.fuser,

                .effectiveGroupName = proc_info.egroup,
                .realGroupName =  proc_info.rgroup,
                .savedGroupName = proc_info.egroup,
                .filesystemGroupName =  proc_info.fgroup,
        };
        tasks.push_back(task);
    }

    closeproc(proc);
    return tasks;
}



