//
// Created by alsah on 09.06.23.
//

#ifndef GREY_PARROT_RAT_CORE_PEER_DATA_H
#define GREY_PARROT_RAT_CORE_PEER_DATA_H

#include <memory>
#include "rtc/peerconnection.hpp"

struct PeerData {
    std::shared_ptr<rtc::PeerConnection> connection;
    std::shared_ptr<rtc::DataChannel> ctrlChannel;
    std::string remoteId;
};

#endif //GREY_PARROT_RAT_CORE_PEER_DATA_H
