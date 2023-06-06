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
    AgentDescriptionListItem(QWidget *parent, const msgs::AgentDescription& agent);
    ~AgentDescriptionListItem() override;

private:
    Ui::AgentDescriptionListItem *ui;
};

#endif // GREY_PARROT_RAT_OPERATOR_GUI_AGENT_DESCRIPTION_LIST_ITEM_H
