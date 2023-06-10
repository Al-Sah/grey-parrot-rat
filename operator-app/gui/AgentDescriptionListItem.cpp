#include "AgentDescriptionListItem.h"

#include <utility>
#include "ui_AgentDescriptionListItem.h"

AgentDescriptionListItem::AgentDescriptionListItem(QWidget *parent, msgs::AgentDescription agent) :
    QWidget(parent),
    ui(new Ui::AgentDescriptionListItem),
    agentDescription(std::move(agent))
{
    ui->setupUi(this);
    updateUI();
}

AgentDescriptionListItem::~AgentDescriptionListItem()
{
    delete ui;
}

void AgentDescriptionListItem::updateUI() {
    ui->labelAgentIdValue->setText(QString::fromStdString(agentDescription.device().id()));
    ui->labelComputerNameValue->setText(QString::fromStdString(agentDescription.device().name()));
    ui->labelOSValue->setText(QString::fromStdString(agentDescription.device().os()));

    ui->labelAgentStateValue->setText("Active");
    ui->labelVersionValue->setText(QString::fromStdString(agentDescription.app().version()));
    ui->labelC4ArcValue->setText(QString::fromStdString(agentDescription.app().c4arc()));
    ui->labelC4PlatformValue->setText(QString::fromStdString(agentDescription.app().c4platform()));

    for (const auto &agentModule: agentDescription.app().modules()){
        auto item = new QListWidgetItem();
        auto widget = new QLabel(this);
        widget->setText(QString::fromStdString(agentModule.name() + " - " + agentModule.version()));
        item->setSizeHint(widget->sizeHint());
        ui->modulesList->addItem(item);
        ui->modulesList->setItemWidget(item, widget);
    }
}

void AgentDescriptionListItem::update(msgs::AgentDescription agent) {
    agentDescription = std::move(agent);
    updateUI();
}

QString AgentDescriptionListItem::getAgentId() const {
    return ui->labelAgentIdValue->text();
}

const msgs::AgentDescription &AgentDescriptionListItem::getAgentDescription() const {
    return agentDescription;
}

