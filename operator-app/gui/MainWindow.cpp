#include <sstream>
#include <iomanip>
#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "AgentDescriptionListItem.h"
#include "ItemsListDialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , activeTasksDialog{this, "Active tasks list"}
    , installedModulesDialog{this, "Installed modules list"}{

    ui->setupUi(this);
    agentsCount = 0;

    operatorApp = OperatorApp::GetInstance();
    operatorApp->setResultsHandler(std::make_shared<CoreCtrlBridge>(this));

    ui->labelCoreCtrlVersionValue->setText(QString::fromStdString(operatorApp->getVersion()));
    ui->stackedWidget->setCurrentIndex(0);

    connect(ui->btnReturn, &QPushButton::clicked, this, &MainWindow::handleReturnBtnClick);
    connect(ui->btnConnect, &QPushButton::clicked, this, &MainWindow::handleConnectBtnClick);
    connect(ui->btnTasksRunning, &QPushButton::clicked, this, &MainWindow::handleTasksDetailsBtnClick);
    connect(ui->btnInstalledModules, &QPushButton::clicked, this, &MainWindow::handleNodulesDetailsBtnClick);
    connect(ui->agentsListWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::handleAgentItemClicked);
}

MainWindow::~MainWindow(){
    delete ui;
    operatorApp->stop();
}

void MainWindow::handleConnectBtnClick() {
    ui->labelConnectionStateValue->setText("Connecting ... ");
    operatorApp->connect(ui->c2serverInput->text().toStdString());
}

void MainWindow::updateC2ServerConnectionState(const ConnectionChange& change) {
    ui->labelConnectionStateValue->setText(QString::fromStdString(change.state));

    isConnected = change.opened;

    if(isConnected){
        operatorApp->requestNotifications();
    }

    if(!isConnected && !timerSet){
        timerSet = true;
        secondsLeft = 10;
        timerId = startTimer(1000);
    }

    if(isConnected && timerSet){
        timerSet = false;
        secondsLeft = 10;
        killTimer(timerId);
    }

    //auto tp =
    // std::chrono::duration_cast<std::chrono::system_clock::time_point::duration>(std::chrono::milliseconds (time));
    ui->labelLastStateChangeValue->setText(TimeStr(change.timestamp));

    QString tmp = isConnected ? "opened" : "closed";

    ui->statusbar->showMessage("c2server channel state - " + tmp);
}

void MainWindow::updatePeerConnectionState(const ConnectionChange &change) {

    ui->agentModulesTabs->setEnabled(change.opened);
    ui->labelPeerStateValue->setText(QString::fromStdString(change.state));
    ui->labelPeerChangeTimeValue->setText(TimeStr(change.timestamp));
}


void MainWindow::timerEvent(QTimerEvent *event) {
    secondsLeft--;
    ui->statusbar->showMessage(
            "c2server channel state - closed; recovering in " + QString::number(secondsLeft) + " seconds");

    if(secondsLeft == 0){
        secondsLeft = 10;
    }
}

void MainWindow::resetAgentsList(const msgs::ActiveAgents& agents) {

    ui->agentsListWidget->clear();
    for (const auto &agent: agents.data()){
        addNewAgent(agent);
    }
    ui->labelLastActionValue->setText("set list of agents");
}

void MainWindow::updateRunningTasksCount(int count) {
    ui->labelTasksRunningValue->setText(QString::number(count));
}


void MainWindow::handleTasksDetailsBtnClick() {
    std::vector<QString> items;
    for (const auto &item: operatorApp->getRunningTasks()){
        std::string str = std::to_string(item.id) + "; created by " + item.module;
        items.push_back(QString::fromStdString(str));
    }
    activeTasksDialog.setItems(items);
    activeTasksDialog.setModal(true);
    activeTasksDialog.show();
}


void MainWindow::handleNodulesDetailsBtnClick() {
    std::vector<QString> items;
    for (const auto &item: operatorApp->getModulesInfo()){
        std::string str = item.id + " - " + item.version;
        items.push_back(QString::fromStdString(str));
    }
    installedModulesDialog.setItems(items);
    installedModulesDialog.setModal(true);
    installedModulesDialog.show();
}

void MainWindow::addNewAgent(const msgs::AgentDescription &agentDescription) {

    auto agentId = agentDescription.device().id();

    auto item = new QListWidgetItem();
    auto widget = new AgentDescriptionListItem(this, agentDescription);
    item->setSizeHint(widget->sizeHint());

    ui->agentsListWidget->addItem(item);
    ui->agentsListWidget->setItemWidget(item, widget);

    agentsCount++;
    ui->labelAgentsNumberValue->setText(QString::number(agentsCount));

    std::string action = "added new agent " + agentId;
    ui->labelLastActionValue->setText(QString::fromStdString(action));
}

void MainWindow::removeAgent(const std::string& agentId) {

    QListWidgetItem* item = nullptr;
    for(int i = 0; i < ui->agentsListWidget->count(); ++i)
    {
        QListWidgetItem* tmp = ui->agentsListWidget->item(i);
        auto widget = dynamic_cast<AgentDescriptionListItem*>(ui->agentsListWidget->itemWidget(tmp));
        if(widget->getAgentId() == QString::fromStdString(agentId))
        {
            item = tmp;
            break;
        }
    }

    if(item != nullptr){
        delete ui->agentsListWidget->takeItem(ui->agentsListWidget->row(item));
        agentsCount--;
        ui->labelAgentsNumberValue->setText(QString::number(agentsCount));

        std::string action = "removed agent " + agentId;
        ui->labelLastActionValue->setText(QString::fromStdString(action));
    }
}

void MainWindow::handleAgentItemClicked(QListWidgetItem* item)
{
    auto widget = dynamic_cast<AgentDescriptionListItem*>(ui->agentsListWidget->itemWidget(item));
    auto agentDescription = widget->getAgentDescription();

    setAgentPageUI(agentDescription);

    std::vector<std::string> names;
    for (const auto &moduleInfo: agentDescription.app().modules()){
        names.push_back(moduleInfo.name());
    }
    setAgentModulesTabs( operatorApp->getTaskGenerators(names));


    ui->stackedWidget->setCurrentIndex(1);
    operatorApp->connectToPeer(agentDescription.device().id());
}

void MainWindow::handleReturnBtnClick() {
    operatorApp->disconnectFromPeer();
    ui->stackedWidget->setCurrentIndex(0);
}

QString MainWindow::TimeStr(std::uint64_t timestamp) {
    auto ts_as_time_t = static_cast<std::time_t>(timestamp);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&ts_as_time_t), "%Y-%m-%d %X");
    return QString::fromStdString(ss.str());
}

void MainWindow::setAgentPageUI(const msgs::AgentDescription & agentDescription) {
    ui->labelAgentIdValue->setText(QString::fromStdString(agentDescription.device().id()));
    ui->labelComputerNameValue->setText(QString::fromStdString(agentDescription.device().name()));
    ui->labelOSValue->setText(QString::fromStdString(agentDescription.device().os()));

    ui->labelVersionValue->setText(QString::fromStdString(agentDescription.app().version()));
    ui->labelC4ArcValue->setText(QString::fromStdString(agentDescription.app().c4arc()));
    ui->labelC4PlatformValue->setText(QString::fromStdString(agentDescription.app().c4platform()));
    ui->labelModulesCountValue->setText(QString::number(agentDescription.app().modules().size()));
}

void MainWindow::setAgentModulesTabs(const std::vector<std::shared_ptr<TaskGenerator>> &taskGenerators) {
    for (const auto &item: taskGenerators){
        void * moduleUI = item->getUI(this);
        if(moduleUI == nullptr){
            continue;
        }
        ui->agentModulesTabs->removeTab(0);
        ui->agentModulesTabs->addTab(static_cast<QWidget*>(moduleUI), QString::fromStdString(item->getModuleInfo().id));
    }

}
