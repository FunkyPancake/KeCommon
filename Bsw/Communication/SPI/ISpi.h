/*
 * spi_abstract.h
 *
 *  Created on: 14 gru 2020
 *      Author: Mati
 */

#pragma once

#include "../ICom.h"
#include <cstdint>

namespace Communication::Spi
{
    class ISpi : public ICom
    {
    public:
        enum class CsPin
        {
            Cs0,
            Cs1,
            Cs2,
            NoCs,
        };

        virtual bool Transfer(const uint8_t* txData, uint8_t* rxData, uint8_t len) const = 0;
    };
}
