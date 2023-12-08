/*
 * can_il.cpp
 *
 *  Created on: 14 gru 2020
 *      Author: Mati
 */
#include "FlexCan.h"
#include <functional>

using namespace KeCommon::Bsw::Can;

FlexCan::FlexCan(CAN_Type* canBase, const uint32_t mainFunctionPeriodMs, const int mailboxCount):canBase_(canBase),
    mailboxCount_(mailboxCount), mainFunctionPeriodMs_(mainFunctionPeriodMs)
{
    mutex_ = xSemaphoreCreateMutex();
}

bool
FlexCan::RegisterRxFrame(const uint32_t id, const std::function<void(const CanFrame& frame)>& handler)
{
    int mbId;
    _flexcan_rx_mb_config config = {.type = kFLEXCAN_FrameTypeData};
    if (xSemaphoreTake(mutex_, 10) == pdTRUE)
    {
        mbId = static_cast<uint8_t>(registeredRxMb_.size()) + 1;
        xSemaphoreGive(mutex_);
    }
    else
    {
        return false;
    }

    if ((id & CanIdExtBit) == CanIdExtBit)
    {
        config.id = FLEXCAN_ID_EXT(id);
        config.format = kFLEXCAN_FrameFormatExtend;
    }
    else
    {
        config.id = FLEXCAN_ID_STD(id);
        config.format = kFLEXCAN_FrameFormatExtend;
    }
    FLEXCAN_SetRxMbConfig(canBase_, mbId, &config, true);
    registeredRxMb_[mbId] = handler;
    return true;
}

bool
FlexCan::RegisterRxFrame(const uint32_t id)
{
    int mbId;
    _flexcan_rx_mb_config config = {.type = kFLEXCAN_FrameTypeData};
    if (xSemaphoreTake(mutex_, 10) == pdTRUE)
    {
        mbId = static_cast<uint8_t>(registeredRxMb_.size()) + 1;
        xSemaphoreGive(mutex_);
    }
    else
    {
        return false;
    }

    if ((id & CanIdExtBit) == CanIdExtBit)
    {
        config.id = FLEXCAN_ID_EXT(id);
        config.format = kFLEXCAN_FrameFormatExtend;
    }
    else
    {
        config.id = FLEXCAN_ID_STD(id);
        config.format = kFLEXCAN_FrameFormatExtend;
    }
    FLEXCAN_SetRxMbConfig(canBase_, mbId, &config, true);
    registeredRxMb_[mbId] = nullptr;
    return true;
}

void FlexCan::RegisterCyclicTxFrame(const uint32_t id, const uint8_t dlc, const uint32_t cycleTime)
{
    const uint16_t cycles = cycleTime >= mainFunctionPeriodMs_ ? cycleTime / mainFunctionPeriodMs_ - 1 : 0;
    cyclicTxList_[id] = {.cnt = cycles, .reloadCnt = cycles, .frame = CanFrame{.id = id, .dlc = dlc}};
}

bool FlexCan::Send(const uint32_t id, const Payload& data, const uint8_t dlc)
{
    flexcan_frame_t frame = {.type = kFLEXCAN_FrameTypeData};
    frame.length = dlc;

    if ((id & CanIdExtBit) == CanIdExtBit)
    {
        frame.id = id & 0x7fffffff;
        frame.format = kFLEXCAN_FrameFormatExtend;
    }
    else
    {
        frame.id = (id & 0x7ff) << 18;
        frame.format = kFLEXCAN_FrameFormatStandard;
    }

    WritePayloadRegisters(&frame, data.b, dlc);
    for (; _lastUsedTxMb < mailboxCount_; _lastUsedTxMb++)
    {
        //        if (xSemaphoreTake(_mutex, (TickType_t) 10) == pdTRUE) {
        const auto result = FLEXCAN_WriteTxMb(canBase_, _lastUsedTxMb, &frame);

        //            xSemaphoreGive(_mutex);
        if (result == kStatus_Success)
            return true;
        //        }
    }

    if (_lastUsedTxMb >= mailboxCount_)
    {
        _lastUsedTxMb = registeredRxMb_.size();
    }
    return false;
}

bool FlexCan::Send(const CanFrame& frame)
{
    return Send(frame.id, frame.payload, frame.dlc);
}

bool FlexCan::ReadFrame(uint32_t* id, Payload* data, uint8_t dlc)
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

bool FlexCan::ReadFrame(CanFrame& frame)
{
    return ReadFrame(&frame.id, &frame.payload, frame.dlc);
}

bool FlexCan::UpdateCyclicFrame(const uint32_t id, const Payload& data)
{
    if (cyclicTxList_.find(id) != cyclicTxList_.end())
    {
        cyclicTxList_[id].frame.payload = data;
        return true;
    }
    return false;
}

void FlexCan::UpdateCyclicFrame(const CanFrame& frame)
{
}

void FlexCan::RxTask()
{
    for (const auto& [id, callback] : registeredRxMb_)
    {
        _flexcan_frame frame{};
        if (FLEXCAN_ReadRxMb(canBase_, id, &frame) != kStatus_Fail)
        {
            canBase_->MB[id].CS = (~CAN_CS_CODE_MASK & canBase_->MB[id].CS) | CAN_CS_CODE(0x4);
            const auto iCanFrame = ToICanFrame(frame);
            callback(iCanFrame);
        }
    }
}

void FlexCan::TxTask()
{
    for (auto& [id, frameEntry] : cyclicTxList_)
    {
        if (frameEntry.cnt <= 0)
        {
            if (Send(frameEntry.frame))
            {
                //reload only if sending was successful
                frameEntry.cnt = frameEntry.reloadCnt;
            }
        }
        else
        {
            frameEntry.cnt--;
        }
    }
}

void FlexCan::WritePayloadRegisters(flexcan_frame_t* frame, const uint8_t* data, const uint8_t dlc)
{
    constexpr uint8_t dataOffsets[] = {3, 2, 1, 0, 7, 6, 5, 4};
    const auto firstDataPtr = &frame->dataByte3;
    for (int i = 0; i < dlc; i++)
    {
        firstDataPtr[dataOffsets[i]] = data[i];
    }
}

CanFrame FlexCan::ToICanFrame(const _flexcan_frame& frame)
{
    auto iFrame = CanFrame{.id = frame.id, .timestamp = frame.timestamp, .dlc = frame.length};
    iFrame.payload.w[0] = SwapBytes<uint32_t>(frame.dataWord0);
    iFrame.payload.w[1] = SwapBytes<uint32_t>(frame.dataWord1);
    return iFrame;
}
