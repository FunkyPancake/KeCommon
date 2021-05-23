/*
 * spi_il.h
 *
 *  Created on: 14 gru 2020
 *      Author: Mati
 */

#ifndef CDD_SPI_IL_H_
#define CDD_SPI_IL_H_

#include "ISpi.h"
#include "fsl_lpspi_freertos.h"

class LpSpiRtos : public ISpi
{

  private:
    lpspi_rtos_handle_t *handle;
    uint32_t flags;
  public:
    enum LpSpiFlags{
        Pcs0 = kLPSPI_MasterPcs0,
        Pcs1 = kLPSPI_MasterPcs1,
        Pcs2 = kLPSPI_MasterPcs2,
        Pcs3 = kLPSPI_MasterPcs3,
        CsContinuous = kLPSPI_MasterPcsContinuous,
        ByteSwap = kLPSPI_MasterByteSwap
    };
    bool Transfer(uint8_t *tx_data, uint8_t *rx_data, uint8_t len);
    LpSpiRtos(lpspi_rtos_handle_t *handle);
    void SetFlags(uint32_t flags);
    ~LpSpiRtos();
};

#endif /* CDD_SPI_IL_H_ */
