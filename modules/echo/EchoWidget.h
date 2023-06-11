#ifndef ECHOWIDGET_H
#define ECHOWIDGET_H

#include <QWidget>
#include "EchoMaker.h"

namespace Ui {
class EchoWidget;
}

class EchoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EchoWidget(QWidget *parent, EchoMaker *controlUnit);
    ~EchoWidget() override;
    void clearUI();

public slots:
    void handleText(const std::string& text);
    void handleFile(const std::string& fileName);
    void handleDC(bool opened);
    void handleDCError(const std::string& error);

private slots:
    void handleSendTextBtnClicked();
    void handleSendFileBtnClicked();
    void handleSessionBtnClicked();

private:
    bool openedDC;
    Ui::EchoWidget *ui;
    EchoMaker *controlUnit;
};

#endif // ECHOWIDGET_H
