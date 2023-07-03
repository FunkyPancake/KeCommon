/*
 * can_abstract.h
 *
 *  Created on: 14 gru 2020
 *      Author: Mati
 */

#pragma once

#include <cstdint>
#include "ICanFrame.h"
#include <functional>

namespace KeCommon::Bsw::Can {

    class ICan {
    public:

        virtual bool Send(uint32_t id, Payload &data, uint8_t dlc) = 0;

        virtual bool Receive(uint32_t *id, Payload *data, uint8_t dlc) = 0;

        virtual void RegisterRxFrame(uint32_t id,
                                     const std::function<void(KeCommon::Bsw::Can::ICanFrame frame)> &handler) = 0;

    };
}