//
// Created by alsah on 08.05.23.
//

#include <iostream>
#include "TasksManager.h"

void TasksManager::addTask(ControlMessageHeader header, std::vector<std::byte> payload) {
    std::cout << "Received new task: " << header.getModuleId().value() << "; payload: ";
    for (auto &item: payload){
        std::cout << (char)item;
    }
}
