/*
 * can_il.cpp
 *
 *  Created on: 14 gru 2020
 *      Author: Mati
 */
#include "FlexCan.h"
#include <functional>

using namespace Communication::Can;

FlexCan::FlexCan(CAN_Type* canBase, const uint32_t mainFunctionPeriodMs, const int mailboxCount): canBase_(canBase),
    mailboxCount_(mailboxCount), mainFunctionPeriodMs_(mainFunctionPeriodMs)
{
    mutex_ = xSemaphoreCreateMutex();
}

bool FlexCan::RegisterRxFrame(const uint32_t id, const std::function<void(const CanFrame& frame)>& handler)
{
    _flexcan_rx_mb_config config = {.type = kFLEXCAN_FrameTypeData};
    if (xSemaphoreTake(mutex_, mutexTimeout) == pdTRUE)
    {
        const int mbId = static_cast<uint8_t>(registeredRxMb_.size()) + 1;
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
        registeredRxMb_[mbId] = {.callback = handler};
        xSemaphoreGive(mutex_);
        return true;
    }
    return false;
}

bool FlexCan::RegisterRxFrame(const uint32_t id)
{
    int mbId;
    _flexcan_rx_mb_config config = {.type = kFLEXCAN_FrameTypeData};
    if (xSemaphoreTake(mutex_, mutexTimeout) == pdTRUE)
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
    registeredRxMb_[mbId] = {.callback = nullptr};
    return true;
}

void FlexCan::RegisterCyclicTxFrame(const uint32_t id, const uint8_t dlc, const uint32_t cycleTime)
{
    const uint16_t cycles = cycleTime >= mainFunctionPeriodMs_ ? cycleTime / mainFunctionPeriodMs_ - 1 : 0;
    cyclicTxList_[id] = {.cnt = cycles, .reloadCnt = cycles, .frame = CanFrame{.id = id, .dlc = dlc}};
}

bool FlexCan::Send(const uint32_t id, const Payload& data, const uint8_t dlc)
{
    bool retVal = false;
    if (xSemaphoreTake(mutex_, mutexTimeout) == pdTRUE)
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
            if (const auto result = FLEXCAN_WriteTxMb(canBase_, _lastUsedTxMb, &frame); result == kStatus_Success)
            {
                retVal = true;
                break;
            }
        }

        if (_lastUsedTxMb >= mailboxCount_)
        {
            _lastUsedTxMb = registeredRxMb_.size();
        }
        xSemaphoreGive(mutex_);
    }
    return retVal;
}

bool FlexCan::Send(const CanFrame& frame)
{
    return Send(frame.id, frame.payload, frame.dlc);
}

bool FlexCan::ReadFrame(uint32_t& id, Payload& data, uint8_t& dlc)
{
    if (CanFrame frame{}; ReadFrame(frame))
    {
        id = frame.id;
        data = frame.payload;
        dlc = frame.dlc;
        return true;
    }
    return false;
}

bool FlexCan::ReadFrame(CanFrame& frame)
{
    if (registeredRxMb_.find(frame.id) != registeredRxMb_.end())
    {
        frame = registeredRxMb_[frame.id].frame;
        return true;
    }
    return false;
}

bool FlexCan::UpdateCyclicFrame(const uint32_t id, const Payload& data)
{
    if (xSemaphoreTake(mutex_, mutexTimeout) == pdTRUE)
    {
        if (cyclicTxList_.find(id) != cyclicTxList_.end())
        {
            cyclicTxList_[id].frame.payload = data;
            return true;
        }
        xSemaphoreGive(mutex_);
    }
    return false;
}

void FlexCan::UpdateCyclicFrame(const CanFrame& frame)
{
}

void FlexCan::RxTask()
{
    for (auto& [id, frameEntry] : registeredRxMb_)
    {
        _flexcan_frame frame{};
        if (FLEXCAN_ReadRxMb(canBase_, id, &frame) != kStatus_Fail)
        {
            canBase_->MB[id].CS = (~CAN_CS_CODE_MASK & canBase_->MB[id].CS) | CAN_CS_CODE(0x4);
            const auto iCanFrame = ToICanFrame(frame);
            frameEntry.frame = iCanFrame;

            if (frameEntry.callback != nullptr)
            {
                frameEntry.callback(iCanFrame);
            }
        }
    }
}

void FlexCan::TxTask()
{
    if (xSemaphoreTake(mutex_, mutexTimeout) == pdTRUE)
    {
        for (auto& [id, frameEntry] : cyclicTxList_)
        {
            CanFrame frame = frameEntry.frame;

            if (frameEntry.cnt <= 0)
            {
                if (Send(frame))
                {
                    //reload only if sending was successful
                    frameEntry.cnt = frameEntry.reloadCnt;
                }
            }
            else
            {
                frameEntry.cnt--;
            }
            xSemaphoreGive(mutex_);
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
