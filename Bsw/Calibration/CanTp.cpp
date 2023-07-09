//
// Created by PC on 02.07.2023.
//

#include <array>
#include <cstring>
#include "CanTp.h"
#include "ICan.h"

using namespace KeCommon::Bsw::Can;
using namespace KeCommon::Bsw::Cal;

void CanTp::ProcessFrame(const ICanFrame &frame) {
    auto frameType = frame.payload.b[0] >> 4;
    if (frameType == 0) {
        ProcessSingleFrame(frame);
    }
    if (frameType == 1) {
        ProcessFirstFrame(frame);
    }
    if (frameType == 2) {
        ProcessConsecutiveFreame(frame);
    }
    if (frameType == 3) {
        ProcessFlowControlFrame(frame);
    }
}

void CanTp::ProcessSingleFrame(const ICanFrame &frame) {
    std::memcpy((void *)(frame.payload.b), (void *)txBuffer.data(), frame.dlc);
    _commandAvailable = true;
}

void CanTp::ProcessFirstFrame(const ICanFrame &frame) {

}

void CanTp::ProcessConsecutiveFreame(const ICanFrame &frame) {

}

void CanTp::ProcessFlowControlFrame(const ICanFrame &frame) {

}

CanTp::CanTp(uint32_t rxId, uint32_t txId, ICan& can) {
    auto f = [this](auto &&PH1) { ProcessFrame(std::forward<decltype(PH1)>(PH1)); };
    can.RegisterRxFrame(rxId, f);
}

bool CanTp::CommandAvailable() {
    return _commandAvailable;
}

std::vector<uint8_t> CanTp::GetCmd() {
    return std::vector<uint8_t>();
}

void CanTp::SendCmd() {

}

void CanTp::TxTask() {

}

bool CanTp::TxRdy() {
    return false;
}

bool CanTp::RxCmdAvailable() {
    return false;
}

bool CanTp::WriteCommand(std::vector<uint8_t> data) {
    return false;
}

std::vector<uint8_t> CanTp::ReadCommand() {
    return std::vector<uint8_t>();
}
