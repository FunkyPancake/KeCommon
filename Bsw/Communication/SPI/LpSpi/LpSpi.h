/*
 * spi_il.h
 *
 *  Created on: 14 gru 2020
 *      Author: Mati
 */

#pragma once

#include "../ISpi.h"
#include "fsl_lpspi_freertos.h"
#include <vector>

namespace Communication::Spi
{
    class LpSpiRtos final : public ISpi
    {
    private:
        lpspi_rtos_handle_t* _handle;
        uint32_t _flags;
        static uint32_t MapCs(CsPin cs);

        enum LpSpiFlags
        {
            Pcs0 = kLPSPI_MasterPcs0,
            Pcs1 = kLPSPI_MasterPcs1,
            Pcs2 = kLPSPI_MasterPcs2,
            Pcs3 = kLPSPI_MasterPcs3,
            CsContinuous = kLPSPI_MasterPcsContinuous,
            ByteSwap = kLPSPI_MasterByteSwap
        };

    public:
        explicit LpSpiRtos(lpspi_rtos_handle_t* handle, CsPin csPin, bool reverseByteOrder);
        ~LpSpiRtos() override;
        bool Transfer(const uint8_t* txData, uint8_t* rxData, uint8_t len) const override;
        bool readBytes(uint16_t size, std::vector<uint8_t>& data) override;
        bool writeBytes(const std::vector<uint8_t>& data) override;
    };
}
