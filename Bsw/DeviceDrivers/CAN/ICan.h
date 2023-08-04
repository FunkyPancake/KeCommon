/*
 * can_abstract.h
 *
 *  Created on: 14 gru 2020
 *      Author: Mati
 */

#pragma once

#include "CanFrame.h"
#include <cstdint>
#include <functional>
#include <memory>

namespace KeCommon::Bsw::Can
{
    class ICan
    {
    public:
        virtual ~ICan() = default;

        virtual bool Send(uint32_t id, const Payload &data, uint8_t dlc) = 0;
        virtual bool Send(const CanFrame &frame) = 0;

        virtual bool Receive(uint32_t *id, Payload *data, uint8_t dlc) = 0;
        virtual bool Receive(CanFrame &frame) = 0;

        virtual bool
        RegisterRxFrame(uint32_t id, const std::function<void(const KeCommon::Bsw::Can::CanFrame &frame)> &handler) = 0;

        virtual void RegisterCyclicTxFrame(uint32_t id, uint32_t cycleTime) = 0;

        virtual void UpdateCyclicFrame(uint32_t id, const Payload &data) = 0;
        virtual void UpdateCyclicFrame(const CanFrame &frame) = 0;
        virtual void RxTask() = 0;

        virtual void TxTask() = 0;
    };
}// namespace KeCommon::Bsw::Can