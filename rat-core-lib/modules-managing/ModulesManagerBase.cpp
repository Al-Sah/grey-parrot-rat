//
// Created by alsah on 10.06.23.
//

#include "ModulesManagerBase.h"
#include "BS_thread_pool.hpp"

ModulesManagerBase::ModulesManagerBase(
        const std::shared_ptr<ITaskHandler> &requestsHandler) : taskHandler(requestsHandler){}

void ModulesManagerBase::setPool(BS::thread_pool *pPool) {
    this->pool = pPool;
}
