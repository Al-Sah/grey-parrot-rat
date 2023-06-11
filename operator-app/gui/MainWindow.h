#ifndef GREY_PARROT_RAT_OPERATOR_GUI_MAIN_WINDOW_H
#define GREY_PARROT_RAT_OPERATOR_GUI_MAIN_WINDOW_H

#include <QMainWindow>
#include "../OperatorApp.h"
#include "../CoreCtrlBridge.h"
#include "ItemsListDialog.h"

#include <QTimer>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    static QString TimeStr(std::uint64_t timestamp);
    ~MainWindow() override;

public slots:
    void resetAgentsList(const msgs::ActiveAgents& agents);
    void updateC2ServerConnectionState(const ConnectionChange& change);
    void updatePeerConnectionState(const ConnectionChange& change);
    void updateRunningTasksCount(int count);
    void addNewAgent(const msgs::AgentDescription& agentDescription);
    void removeAgent(const std::string& agentId);

private:

    void handleConnectBtnClick();
    void handleTasksDetailsBtnClick();
    void handleNodulesDetailsBtnClick();
    void handleAgentItemClicked(QListWidgetItem *item);
    void handleReturnBtnClick();

    void setAgentPageUI(const msgs::AgentDescription & agentDescription);
    void setAgentModulesTabs(const std::vector<std::shared_ptr<TaskGenerator>> &taskGenerators);

    std::uint32_t secondsLeft;
    bool timerSet;
    std::int32_t timerId;

    bool isConnected;
    std::shared_ptr<OperatorApp> operatorApp;
    Ui::MainWindow *ui;

    int agentsCount;

    ItemsListDialog activeTasksDialog;
    ItemsListDialog installedModulesDialog;

protected:
    void timerEvent(QTimerEvent *event) override;
};
#endif // GREY_PARROT_RAT_OPERATOR_GUI_MAIN_WINDOW_H
