/*
 * can_abstract.h
 *
 *  Created on: 14 gru 2020
 *      Author: Mati
 */

#pragma once

#include "ICanFrame.h"
#include <cstdint>
#include <functional>

namespace KeCommon::Bsw::Can
{

    class ICan
    {
    public:
        virtual ~ICan() = default;
        virtual bool Send(uint32_t id, Payload &data, uint8_t dlc) = 0;

        virtual bool Receive(uint32_t *id, Payload *data, uint8_t dlc) = 0;

        virtual void RegisterRxFrame(uint32_t id,
                                     const std::function<void(KeCommon::Bsw::Can::ICanFrame frame)> &handler) = 0;
    };
}// namespace KeCommon::Bsw::Can