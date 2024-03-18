//
// Created by PC on 02.07.2023.
//

#include "CanTp.h"
#include "../Bootloader/Bootloader.h"
#include <array>
#include <cstring>

using namespace Communication::Can;
using namespace Diag;

CanTp::CanTp(ICan *can)
    : _can(can), _txId(DiagCanTxId), _txBuf(), _rxBuf(),_rxId(DiagCanRxId)
{
    auto f = [this](const CanFrame& canFrame) { ProcessFrame(canFrame); };
    can->RegisterRxFrame(_rxId, f);
}

void CanTp::ProcessFrame(const CanFrame &frame)
{
    const auto frameType = frame.payload.b[0] >> 4;
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

void CanTp::ProcessSingleFrame(const CanFrame &frame)
{
    const auto len = frame.payload.b[0];
    std::memcpy(_rxBuf.data(), frame.payload.b + 1, len);
    _rxBufPtr = len;
    _rxRdy = true;
}

void CanTp::ProcessFirstFrame(const CanFrame &frame)
{
}

void CanTp::ProcessConsecutiveFreame(const CanFrame &frame)
{
}

void CanTp::ProcessFlowControlFrame(const CanFrame &frame)
{
}

void CanTp::TxMainFunction()
{
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
        }
        else {
            //Send First frame packet
            payload.b[0] = 0x10 + (_txMsgLen >> 8);
            payload.b[1] = 0xff & _txMsgLen;
            std::memcpy(&(payload.b[2]), _txBuf.data(), MaxPayloadSizeFirstFrame);
            _txCnt = 0;
            _txBufPtr += MaxPayloadSizeFirstFrame;
        }
        _txRdy = false;
    }
    else {
        //Transmission already in progress
        //TODO: Check if flowControl frame was received and sending is allowed
        _txCnt += 1;
        _txCnt &= 0x0f;
        payload.b[0] = 0x20 + _txCnt;
        auto size = std::min((_txMsgLen - _txBufPtr), MaxPayloadSizeFrame);
        memcpy(&(payload.b[1]), _txBuf.data() + _txBufPtr, size);
        _txBufPtr += size;
    }
    _can->Send(_txId, payload, FrameDlc);
    if (_txBufPtr >= _txMsgLen) {
        _txMsgLen = 0;
        _txBufPtr = 0;
        _txRdy = true;
    }
}

bool CanTp::TxRdy()
{
    return _txRdy;
}

bool CanTp::RxRdy()
{
    return _rxRdy;
}

bool CanTp::Write(std::vector<uint8_t> &data)
{
    if (data.size() > _txBuf.size() || !_txRdy) {
        return false;
    }
    memcpy(_txBuf.data(), data.data(), data.size());
    _txMsgLen = data.size();
    _txBufPtr = 0;
    _txRdy = false;
    return true;
}

std::vector<uint8_t> CanTp::Read()
{
    auto response = std::vector(_rxBuf.begin(), _rxBuf.begin() + _rxBufPtr);
    _rxBufPtr = 0;
    _rxRdy = false;
    return response;
}
