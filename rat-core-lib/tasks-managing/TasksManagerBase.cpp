//
// Created by alsah on 08.05.23.
//

#include <sstream>
#include <fstream>
#include "TasksManagerBase.h"


void TasksManagerBase::setMessagesSender(const std::shared_ptr<IControlPacketSender> &aMessagesSender) {
    this->messagesSender = aMessagesSender;
}

void TasksManagerBase::setTaskHandler(const std::shared_ptr<ITaskHandler> &aTaskHandler) {
    this->taskHandler = aTaskHandler;
}

TasksMap &TasksManagerBase::getTasksMap() {
    return tasks;
}

void TasksManagerBase::setOnTasksCountChange(const std::function<void(std::uint32_t)> &onTasksCountChange) {
    TasksManagerBase::onTasksCountChange = onTasksCountChange;
}

msgs::FileData TasksManagerBase::loadFile(const std::filesystem::path& path) {

    std::ifstream file(path, std::ios::in | std::ios::binary );
    std::ostringstream strStream;
    strStream << file.rdbuf();

    msgs::FileData fd{};
    fd.set_data(strStream.str());
    fd.set_filename(path.filename());
    return fd;
}
