/*
 * spi_abstract.h
 *
 *  Created on: 14 gru 2020
 *      Author: Mati
 */

#ifndef CDD_SPI_ABSTRACT_H_
#define CDD_SPI_ABSTRACT_H_
#include <stdint.h>

class ISpi
{
public:
    virtual bool Transfer(uint8_t *tx_data, uint8_t *rx_data, uint8_t len) = 0;
};

#endif /* CDD_SPI_ABSTRACT_H_ */
