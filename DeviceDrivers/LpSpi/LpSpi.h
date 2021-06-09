/*
 * spi_il.h
 *
 *  Created on: 14 gru 2020
 *      Author: Mati
 */

#pragma once

#include "ISpi.h"
#include "fsl_lpspi_freertos.h"

class LpSpiRtos : public ISpi
{
private:
    lpspi_rtos_handle_t *handle;
    uint32_t flags;
public:
    enum LpSpiFlags
    {
        Pcs0 = kLPSPI_MasterPcs0,
        Pcs1 = kLPSPI_MasterPcs1,
        Pcs2 = kLPSPI_MasterPcs2,
        Pcs3 = kLPSPI_MasterPcs3,
        CsContinuous = kLPSPI_MasterPcsContinuous,
        ByteSwap = kLPSPI_MasterByteSwap
    };
    
    bool Transfer(uint8_t *tx_data, uint8_t *rx_data, uint8_t len) override;
    std::vector<uint8_t> ReadBytes(uint16_t size) override;
    bool WriteBytes(std::vector<uint8_t> data) override;
    explicit LpSpiRtos(lpspi_rtos_handle_t *handle);
    void SetFlags(uint32_t flags);
    ~LpSpiRtos();
};

