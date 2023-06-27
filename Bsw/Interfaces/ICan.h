/*
 * can_abstract.h
 *
 *  Created on: 14 gru 2020
 *      Author: Mati
 */

#pragma once

#include <cstdint>

#define CAN_ID_EX_BIT 0x80000000

class ICan
{
public:
    union Payload
    {
        uint8_t b[8];
        uint16_t w[4];
        uint32_t dw[2];
    };
    virtual bool Send(uint32_t id, Payload &data, uint8_t dlc) = 0;
    virtual bool Receive(uint32_t *id, Payload *data, uint8_t dlc) = 0;
};