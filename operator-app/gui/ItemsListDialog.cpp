#include <QListWidgetItem>
#include <QLabel>
#include "ItemsListDialog.h"
#include "ui_ItemsListDialog.h"

ItemsListDialog::ItemsListDialog(QWidget *parent, const QString &name) :
    QDialog(parent),
    ui(new Ui::ItemsListDialog)
{
    ui->setupUi(this);
    ui->label->setText(name);
}

ItemsListDialog::~ItemsListDialog()
{
    delete ui;
}

void ItemsListDialog::setItems(const std::vector<QString> &items) {

    ui->listWidget->clear();

    for (const auto &inputItem: items){

        auto itemList = new QListWidgetItem();
        auto widget = new QLabel(inputItem);
        itemList->setSizeHint(widget->sizeHint());

        ui->listWidget->addItem(itemList);
        ui->listWidget->setItemWidget(itemList, widget);
    }
}

