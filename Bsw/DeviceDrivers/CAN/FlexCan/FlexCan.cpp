/*
 * can_il.cpp
 *
 *  Created on: 14 gru 2020
 *      Author: Mati
 */
#include "FlexCan.h"
#include <functional>

using namespace KeCommon::Bsw::Can;

FlexCan::FlexCan(CAN_Type *canBase, int mailboxCount) {

    this->_canBase = canBase;
    this->_mailboxCount = mailboxCount;
    _mutex = xSemaphoreCreateMutex();
}

bool
FlexCan::RegisterRxFrame(uint32_t id, const std::function<void(const KeCommon::Bsw::Can::CanFrame &frame)> &handler) {
    int mbId;
    _flexcan_rx_mb_config config = {.type = kFLEXCAN_FrameTypeData};
    if (xSemaphoreTake(_mutex, (TickType_t) 10) == pdTRUE) {
        mbId = (uint8_t) _registeredRxMb.size() + 1;
        xSemaphoreGive(_mutex);
    } else {
        return false;
    }

    if ((id & CanIdExtBit) == CanIdExtBit) {
        config.id = FLEXCAN_ID_EXT(id);
        config.format = kFLEXCAN_FrameFormatExtend;
    } else {
        config.id = FLEXCAN_ID_STD(id);
        config.format = kFLEXCAN_FrameFormatExtend;
    }
    FLEXCAN_SetRxMbConfig(_canBase, mbId, &config, true);
    _registeredRxMb[mbId] = handler;
    return true;
}

bool
FlexCan::RegisterRxFrame(uint32_t id) {
    int mbId;
    _flexcan_rx_mb_config config = {.type = kFLEXCAN_FrameTypeData};
    if (xSemaphoreTake(_mutex, (TickType_t) 10) == pdTRUE) {
        mbId = (uint8_t) _registeredRxMb.size() + 1;
        xSemaphoreGive(_mutex);
    } else {
        return false;
    }

    if ((id & CanIdExtBit) == CanIdExtBit) {
        config.id = FLEXCAN_ID_EXT(id);
        config.format = kFLEXCAN_FrameFormatExtend;
    } else {
        config.id = FLEXCAN_ID_STD(id);
        config.format = kFLEXCAN_FrameFormatExtend;
    }
    FLEXCAN_SetRxMbConfig(_canBase, mbId, &config, true);
    _registeredRxMb[mbId] = nullptr;
    return true;
}

void FlexCan::RegisterCyclicTxFrame(uint32_t id, uint8_t dlc, uint32_t cycleTime) {
    uint16_t cycles = (cycleTime / 10) - 1;
    _cyclicTxList[id] = {.cnt = cycles, .reloadCnt = cycles, .frame = CanFrame{.id=id, .dlc=dlc}};
}

bool FlexCan::Send(uint32_t id, const Payload &data, uint8_t dlc) {
    flexcan_frame_t frame = {.type = kFLEXCAN_FrameTypeData};
    frame.length = dlc;

    if ((id & CanIdExtBit) == CanIdExtBit) {
        frame.id = id & 0x7fffffff;
        frame.format = kFLEXCAN_FrameFormatExtend;
    } else {
        frame.id = (id & 0x7ff) << 18;
        frame.format = kFLEXCAN_FrameFormatStandard;
    }

    WritePayloadRegisters(&frame, (uint8_t *) data.b, dlc);
    for (; _lastUsedTxMb < _mailboxCount; _lastUsedTxMb++)
    {
//        if (xSemaphoreTake(_mutex, (TickType_t) 10) == pdTRUE) {
        auto result = FLEXCAN_WriteTxMb(_canBase, _lastUsedTxMb, &frame);

//            xSemaphoreGive(_mutex);
        if (result == kStatus_Success)
            return true;
//        }
    }

    if (_lastUsedTxMb >= _mailboxCount)
    {
        _lastUsedTxMb = _registeredRxMb.size();
    }
    return false;
}

bool FlexCan::Send(const CanFrame &frame) {
    return Send(frame.id, frame.payload, frame.dlc);
}

bool FlexCan::ReadFrame(uint32_t *id, Payload *data, uint8_t dlc) {
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

bool FlexCan::ReadFrame(CanFrame &frame) {
    return ReadFrame(&frame.id, &frame.payload, frame.dlc);
}

bool FlexCan::UpdateCyclicFrame(uint32_t id, const Payload &data) {
    if (_cyclicTxList.find(id) != _cyclicTxList.end()) {
        _cyclicTxList[id].frame.payload = data;
        return true;
    }
    return false;
}

void FlexCan::UpdateCyclicFrame(const CanFrame &frame) {
}

void FlexCan::RxTask() {
    for (const auto &id: _registeredRxMb) {
        _flexcan_frame frame{};
        if (FLEXCAN_ReadRxMb(_canBase, id.first, &frame) != kStatus_Fail) {
            _canBase->MB[id.first].CS = (~CAN_CS_CODE_MASK & _canBase->MB[id.first].CS) | CAN_CS_CODE(0x4);
            auto iCanFrame = ToICanFrame(frame);
            id.second(iCanFrame);
        }
    }
}

void FlexCan::TxTask() {
    for (auto &entry: _cyclicTxList) {
        if (entry.second.cnt <= 0) {
            if (Send(entry.second.frame))
            {
                //reload only if sending was successful
                entry.second.cnt = entry.second.reloadCnt;
            }
        } else {
            entry.second.cnt--;
        }

    }
}

void FlexCan::WritePayloadRegisters(flexcan_frame_t *frame, const uint8_t *data, uint8_t dlc) {
    constexpr uint8_t dataOffsets[] = {3, 2, 1, 0, 7, 6, 5, 4};
    auto firstDataPtr = (static_cast<uint8_t *>(&frame->dataByte3));
    for (int i = 0; i < dlc; i++) {
        firstDataPtr[dataOffsets[i]] = data[i];
    }
}

CanFrame FlexCan::ToICanFrame(const _flexcan_frame &frame) {
    auto iFrame = CanFrame{.id = frame.id, .timestamp = frame.timestamp, .dlc = frame.length};
    iFrame.payload.w[0] = SwapBytes<uint32_t>(frame.dataWord0);
    iFrame.payload.w[1] = SwapBytes<uint32_t>(frame.dataWord1);
    return iFrame;
}
