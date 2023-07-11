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
    std::memcpy((void *) (frame.payload.b), (void *) _txBuf.data(), frame.dlc);
    _commandAvailable = true;
}

void CanTp::ProcessFirstFrame(const ICanFrame &frame) {

}

void CanTp::ProcessConsecutiveFreame(const ICanFrame &frame) {

}

void CanTp::ProcessFlowControlFrame(const ICanFrame &frame) {

}

CanTp::CanTp(ICan &can, uint32_t rxId, uint32_t txId) : _can(can), _txId(txId) {
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
    if (_txMsgLen == 0) {
        return;
    }
    Payload payload;
    memset(&(payload.b[0]), FillByte, FrameDlc);
    if (_txBufPtr == 0) {
        //No transmission in progress - can start new tx
        if (_txMsgLen <= MaxPayloadSizeFrame) {
            //Send response in a single frame
            payload.b[0] = _txMsgLen;
            std::memcpy(&(payload.b[1]), _txBuf.data(), _txMsgLen);
            _txBufPtr += _txMsgLen;
        } else {
            //Send First frame packet
            payload.b[0] = 0x10 + (_txMsgLen >> 8);
            payload.b[1] = 0xff & _txMsgLen;
            std::memcpy(&(payload.b[2]), _txBuf.data(), MaxPayloadSizeFirstFrame);
            _txCnt = 0;
            _txBufPtr += MaxPayloadSizeFirstFrame;
        }
        _txRdy = false;
    } else {
        //Transmission already in progress
        _txCnt += 1;
        _txCnt &= 0x0f;
        payload.b[0] = 0x20 + _txCnt;
        auto size = std::min((_txMsgLen - _txBufPtr), MaxPayloadSizeFrame);
        memcpy(&(payload.b[1]), _txBuf.data() + _txBufPtr, size);
        _txBufPtr += size;

    }
    _can.Send(_txId, payload, FrameDlc);
    if (_txBufPtr >= _txMsgLen) {
        _txMsgLen = 0;
        _txBufPtr = 0;
        _txRdy = true;
    }
}

bool CanTp::TxRdy() {
    return _txRdy;
}

bool CanTp::RxCmdAvailable() {
    return false;
}

bool CanTp::Write(std::vector<uint8_t> &data) {
    if (data.size() > _txBuf.size() || !_txRdy) {
        return false;
    }
    memcpy(_txBuf.data(), data.data(), data.size());
    _txMsgLen = data.size();
    _txBufPtr = 0;
    _txRdy = false;
    return true;
}

std::vector<uint8_t> CanTp::ReadCommand() {
    return std::vector<uint8_t>();
}
