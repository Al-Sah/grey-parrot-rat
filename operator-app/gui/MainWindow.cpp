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

    operatorApp = OperatorApp::GetInstance();
    operatorApp->setResultsHandler(std::make_shared<CoreCtrlBridge>(this));

    ui->labelCoreCtrlVersionValue->setText(QString::fromStdString(operatorApp->getVersion()));

    connect(ui->btnConnect, &QPushButton::clicked, this, &MainWindow::handleConnectBtnClick);
    connect(ui->btnTasksRunning, &QPushButton::clicked, this, &MainWindow::handleTasksDetailsBtnClick);
    connect(ui->btnInstalledModules, &QPushButton::clicked, this, &MainWindow::handleNodulesDetailsBtnClick);
}

MainWindow::~MainWindow(){
    delete ui;
    operatorApp->stop();
}

void MainWindow::handleConnectBtnClick() {
    ui->labelConnectionStateValue->setText("Connecting ... ");
    operatorApp->connect(ui->c2serverInput->text().toStdString());
}

void MainWindow::updateConnectionStateChange(bool opened, const std::string &state, unsigned long time) {
    ui->labelConnectionStateValue->setText(QString::fromStdString(state));

    if(opened){
        operatorApp->requestNotifications();
    }

    isConnected = opened;

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
    ui->labelLastStateChangeValue->setText(QString::number(time));

    QString tmp = opened ? "opened" : "closed";

    ui->statusbar->showMessage("c2server channel state - " + tmp);
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

        auto item = new QListWidgetItem();
        auto widget = new AgentDescriptionListItem(this, agent);
        item->setSizeHint(widget->sizeHint());

        ui->agentsListWidget->addItem(item);
        ui->agentsListWidget->setItemWidget(item, widget);
    }
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
