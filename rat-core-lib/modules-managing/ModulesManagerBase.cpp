//
// Created by alsah on 10.06.23.
//

#include "ModulesManagerBase.h"

ModulesManagerBase::ModulesManagerBase(
        const std::shared_ptr<ITaskHandler> &requestsHandler) : taskHandler(requestsHandler){}
