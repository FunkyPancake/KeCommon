/*
 * can_il.cpp
 *
 *  Created on: 14 gru 2020
 *      Author: Mati
 */
#include "FlexCan.h"
#include "../ICanFrame.h"
#include <functional>
#include <utility>

using namespace KeCommon::Bsw::Can;

FlexCan::FlexCan(CAN_Type *canBase, int mailboxCount)
{

    this->_canBase = canBase;
    this->_mailboxCount = mailboxCount;
    _mutex = xSemaphoreCreateMutex();
}

bool FlexCan::RegisterRxFrame(uint32_t id, const std::function<void(KeCommon::Bsw::Can::ICanFrame frame)> &handler)
{
    int mbId;
    _flexcan_rx_mb_config config = {.type = kFLEXCAN_FrameTypeData};
    if (xSemaphoreTake(_mutex, (TickType_t) 10) == pdTRUE) {
        mbId = (uint8_t) _registeredRxMb.size() + 1;
        xSemaphoreGive(_mutex);
    }
    else {
        return false;
    }

    if ((id & CanIdExtBit) == CanIdExtBit) {
        config.id = FLEXCAN_ID_EXT(id);
        config.format = kFLEXCAN_FrameFormatExtend;
    }
    else {
        config.id = FLEXCAN_ID_STD(id);
        config.format = kFLEXCAN_FrameFormatExtend;
    }
    FLEXCAN_SetRxMbConfig(_canBase, mbId, &config, true);
    _registeredRxMb[mbId] = handler;
    return true;
}

void FlexCan::RegisterCyclicTxFrame(uint32_t id, uint32_t cycleTime)
{
}

bool FlexCan::Send(uint32_t id, const Payload &data, uint8_t dlc)
{
    flexcan_frame_t frame = {.type = kFLEXCAN_FrameTypeData};
    frame.length = dlc;

    if ((id & CanIdExtBit) == CanIdExtBit) {
        frame.id = id & 0x7fffffff;
        frame.format = kFLEXCAN_FrameFormatExtend;
    }
    else {
        frame.id = (id & 0x7ff) << 18;
        frame.format = kFLEXCAN_FrameFormatStandard;
    }
    WritePayloadRegisters(&frame, (uint8_t *) data.b, dlc);
    for (int i = 0; i < _mailboxCount; i++) {
        if (xSemaphoreTake(_mutex, (TickType_t) 100) == pdTRUE) {
            auto result = FLEXCAN_WriteTxMb(_canBase, i, &frame) == kStatus_Success;
            xSemaphoreGive(_mutex);
            return result;
        }
    }
    return false;
}

bool FlexCan::Send(const ICanFrame &frame)
{
    return Send(frame.id, frame.payload, frame.dlc);
}

bool FlexCan::Receive(uint32_t *id, Payload *data, uint8_t dlc)
{
    //    flexcan_frame_t frame;
    //    // FLEXCAN_ReadRxMb();
    //    if(frame.format == kFLEXCAN_FrameFormatExtend)
    //    {
    //    }
    //    else
    //    {
    //    }
    return false;
}

bool FlexCan::Receive(ICanFrame &frame)
{
    return Receive(&frame.id, &frame.payload, frame.dlc);
}

void FlexCan::UpdateCyclicFrame(uint32_t id, const Payload &data, uint8_t dlc)
{
}

void FlexCan::UpdateCyclicFrame(const ICanFrame &frame)
{
}

void FlexCan::RxTask()
{
    for (const auto &id : _registeredRxMb) {
        _flexcan_frame frame{};
        if (FLEXCAN_ReadRxMb(_canBase, id.first, &frame) != kStatus_Fail)
        {
            _canBase->MB[id.first].CS = (~CAN_CS_CODE_MASK & _canBase->MB[id.first].CS) | CAN_CS_CODE(0x4);
            auto iCanFrame = ToICanFrame(frame);
            id.second(iCanFrame);
        }
    }
}

void FlexCan::TxTask()
{
}

void FlexCan::WritePayloadRegisters(flexcan_frame_t *frame, const uint8_t *data, uint8_t dlc)
{
    uint8_t *reg[8] = {
        &frame->dataByte0,
        &frame->dataByte1,
        &frame->dataByte2,
        &frame->dataByte3,
        &frame->dataByte4,
        &frame->dataByte5,
        &frame->dataByte6,
        &frame->dataByte7};

    for (int i = 0; i < dlc; i++) {
        *reg[i] = data[i];
    }
}

ICanFrame FlexCan::ToICanFrame(const _flexcan_frame &frame)
{
    auto iFrame = ICanFrame{.id = frame.id, .timestamp = frame.timestamp, .dlc = frame.length};
    iFrame.payload.w[0] = SwapBytes<uint32_t>(frame.dataWord0);
    iFrame.payload.w[1] = SwapBytes<uint32_t>(frame.dataWord1);
    return iFrame;
}
