//
// Created by alsah on 10.06.23.
//

#include "EchoModuleBridge.h"
#include "EchoWidget.h"

EchoModuleBridge::EchoModuleBridge(QWidget *target) {

    auto* ew = dynamic_cast<EchoWidget*>(target);

    connect(this, &EchoModuleBridge::onText, ew, &EchoWidget::handleText);
    connect(this, &EchoModuleBridge::onFile, ew, &EchoWidget::handleFile);
    connect(this, &EchoModuleBridge::onDataChannel, ew, &EchoWidget::handleDC);
    connect(this, &EchoModuleBridge::onDataChannelError, ew, &EchoWidget::handleDCError);
}
