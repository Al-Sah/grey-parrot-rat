#ifndef FILESMANAGERWIDGET_H
#define FILESMANAGERWIDGET_H

#include <QWidget>

namespace Ui {
class FilesManagerWidget;
}

class FilesManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FilesManagerWidget(QWidget *parent = nullptr);
    ~FilesManagerWidget();

private:
    Ui::FilesManagerWidget *ui;
};

#endif // FILESMANAGERWIDGET_H
