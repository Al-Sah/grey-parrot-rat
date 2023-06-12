#ifndef STREAMERWIDGET_H
#define STREAMERWIDGET_H

#include <QWidget>

namespace Ui {
class StreamerWidget;
}

class StreamerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StreamerWidget(QWidget *parent);
    ~StreamerWidget() override;

private:
    Ui::StreamerWidget *ui;
};

#endif // STREAMERWIDGET_H
