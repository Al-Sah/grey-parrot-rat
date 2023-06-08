#ifndef ITEMSLISTDIALOG_H
#define ITEMSLISTDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class ItemsListDialog;
}

class ItemsListDialog : public QDialog
{
    Q_OBJECT

public:
    ItemsListDialog(QWidget *parent, const QString &name);
    ~ItemsListDialog() override;

    void setItems(const std::vector<QString>& items);

private:
    Ui::ItemsListDialog *ui;
};

#endif // ITEMSLISTDIALOG_H
