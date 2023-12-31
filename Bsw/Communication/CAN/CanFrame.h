//
// Created by PC on 02.07.2023.
//

#ifndef CANGPS_ICANFRAME_H
#define CANGPS_ICANFRAME_H

#include <cstdint>

namespace Communication::Can {
    constexpr uint32_t CanIdExtBit = 0x80000000;

    typedef union {
        uint8_t b[8];
        uint16_t s[4];
        uint32_t w[2];
    } Payload;

    typedef struct {
        uint32_t id;
        uint32_t timestamp;
        uint32_t dlc;
        Payload payload;
    } CanFrame;
}


#endif //CANGPS_ICANFRAME_H
