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

namespace Communication::Can
{
    class ICan
    {
    public:
        template<typename T>
        static T SwapBytes(T data) {
            std::array<uint8_t, sizeof(T)> tmp{};
            *reinterpret_cast<T*>(tmp.data()) = data;
            std::reverse(tmp.begin(), tmp.end());
            return *reinterpret_cast<T*>(tmp.data());
        }

        virtual ~ICan() = default;

        virtual bool Send(uint32_t id, const Payload &data, uint8_t dlc) = 0;
        virtual bool Send(const CanFrame &frame) = 0;

        virtual bool ReadFrame(uint32_t& id, Payload& data, uint8_t& dlc) = 0;

        virtual bool ReadFrame(CanFrame &frame) = 0;

        virtual bool
        RegisterRxFrame(uint32_t id, const std::function<void(const CanFrame&frame)> &handler) = 0;

        virtual void RegisterCyclicTxFrame(uint32_t id, uint8_t dlc, uint32_t cycleTime) = 0;

        virtual bool UpdateCyclicFrame(uint32_t id, const Payload &data) = 0;
        virtual void UpdateCyclicFrame(const CanFrame &frame) = 0;
        virtual void RxTask() = 0;

        virtual void TxTask() = 0;
    };
}// namespace KeCommon::Bsw::Can