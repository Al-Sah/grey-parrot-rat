#include "AgentDescriptionListItem.h"
#include "ui_AgentDescriptionListItem.h"

AgentDescriptionListItem::AgentDescriptionListItem(QWidget *parent, const msgs::AgentDescription& agent) :
    QWidget(parent),
    ui(new Ui::AgentDescriptionListItem)
{
    ui->setupUi(this);

    ui->labelAgentIdValue->setText(QString::fromStdString(agent.device().id()));
    ui->labelComputerNameValue->setText(QString::fromStdString(agent.device().name()));
    ui->labelOSValue->setText(QString::fromStdString(agent.device().os()));

    ui->labelAgentStateValue->setText("Active");
    ui->labelVersionValue->setText(QString::fromStdString(agent.app().version()));
    ui->labelC4ArcValue->setText(QString::fromStdString(agent.app().c4arc()));
    ui->labelC4PlatformValue->setText(QString::fromStdString(agent.app().c4platform()));

    for (const auto &agentModule: agent.app().modules()){
        auto item = new QListWidgetItem();
        auto widget = new QLabel(this);
        widget->setText(QString::fromStdString(agentModule.name() + " - " + agentModule.version()));
        item->setSizeHint(widget->sizeHint());
        ui->modulesList->addItem(item);
        ui->modulesList->setItemWidget(item, widget);
    }

}

AgentDescriptionListItem::~AgentDescriptionListItem()
{
    delete ui;
}

