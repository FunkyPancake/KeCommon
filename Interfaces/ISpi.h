/*
 * spi_abstract.h
 *
 *  Created on: 14 gru 2020
 *      Author: Mati
 */

#pragma once

#include "ICom.h"
#include <cstdint>

class ISpi :public ICom
{
public:
    virtual bool Transfer(uint8_t *tx_data, uint8_t *rx_data, uint8_t len) = 0;
};
