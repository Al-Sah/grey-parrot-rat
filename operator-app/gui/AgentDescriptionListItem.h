#ifndef GREY_PARROT_RAT_OPERATOR_GUI_AGENT_DESCRIPTION_LIST_ITEM_H
#define GREY_PARROT_RAT_OPERATOR_GUI_AGENT_DESCRIPTION_LIST_ITEM_H

#include <QWidget>
#include "core-data.pb.h"

namespace Ui {
class AgentDescriptionListItem;
}

class AgentDescriptionListItem : public QWidget
{
    Q_OBJECT

public:
    AgentDescriptionListItem(QWidget *parent, msgs::AgentDescription  agent);
    ~AgentDescriptionListItem() override;

    void update(msgs::AgentDescription agent);

    QString getAgentId() const;

    const msgs::AgentDescription &getAgentDescription() const;

private:
    Ui::AgentDescriptionListItem *ui;
    msgs::AgentDescription agentDescription;

    void updateUI();
};

#endif // GREY_PARROT_RAT_OPERATOR_GUI_AGENT_DESCRIPTION_LIST_ITEM_H
