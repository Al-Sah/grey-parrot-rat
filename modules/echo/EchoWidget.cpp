#include <QMessageBox>
#include <QFileDialog>
#include "EchoWidget.h"
#include "ui_EchoWidget.h"

#include "EchoMaker.h"

EchoWidget::EchoWidget(QWidget *parent, EchoMaker *controlUnit)
    : QWidget(parent),
    ui(new Ui::EchoWidget),
    controlUnit(controlUnit),
    openedDC{false}
{
    ui->setupUi(this);

    connect(ui->btnSendFile, &QPushButton::clicked, this, &EchoWidget::handleSendFileBtnClicked);
    connect(ui->btnSendText, &QPushButton::clicked, this, &EchoWidget::handleSendTextBtnClicked);
    connect(ui->btnSession, &QPushButton::clicked, this, &EchoWidget::handleSessionBtnClicked);
}


EchoWidget::~EchoWidget()
{
    delete ui;
}

void EchoWidget::handleSendTextBtnClicked() {

    QString str = ui->lineEdit->text();
    if(str.size() == 0){
        return;
    }

    ui->textBrowser->append("You: " + str);
    controlUnit->sendText(str.toStdString());
}

void EchoWidget::handleSendFileBtnClicked() {

    auto fileName = QFileDialog::getOpenFileName(this, tr("Select file"));
    if(fileName.isEmpty()){
        return;
    }
    ui->textBrowser->append("You sending file: " + fileName);
    controlUnit->sendFile(fileName.toStdString());
}

void EchoWidget::handleSessionBtnClicked() {

    if(!openedDC){
        ui->labelPeerDCStateValue->setText("Opening DC");
        controlUnit->requestDataChannel();
    } else {
        ui->labelPeerDCStateValue->setText("Closing DC");
        controlUnit->closeDataChanel();
    }
}

void EchoWidget::clearUI() {
    ui->textBrowser->clear();
    ui->lineEdit->clear();
}

void EchoWidget::handleText(const std::string& text) {

    ui->textBrowser->append(QString::fromStdString("Echo: " + text));
}

void EchoWidget::handleDC(bool opened) {
    openedDC = opened;
    ui->labelPeerDCStateValue->setText(opened ? "Using direct session" : "Plain format");
    ui->btnSession->setText(opened ? "Close session" : "Open Session");
}

void EchoWidget::handleDCError(const std::string &error) {
    QMessageBox msgBox(this);
    msgBox.setText("Data channel error");
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setInformativeText(QString::fromStdString(error));
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

void EchoWidget::handleFile(const std::string &fileName) {
    ui->textBrowser->append(QString::fromStdString("Echo sent file: " + fileName));
}


