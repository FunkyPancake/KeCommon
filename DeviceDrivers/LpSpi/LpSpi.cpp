/*
 * spi_abstract.c
 *
 *  Created on: 14 gru 2020
 *      Author: Mati
 */
#include "LpSpi.h"

bool LpSpiRtos::Transfer(uint8_t *tx_data, uint8_t *rx_data, uint8_t len)
{
    lpspi_transfer_t data = {tx_data, rx_data, len, kLPSPI_MasterPcs0 | kLPSPI_MasterPcsContinuous};
    return LPSPI_RTOS_Transfer(this->handle, &data) == kStatus_Success;
}

LpSpiRtos::LpSpiRtos(lpspi_rtos_handle_t *handle)
{
    this->flags = 0;
    this->handle = handle;
}

LpSpiRtos::~LpSpiRtos()
{
    LPSPI_RTOS_Deinit(this->handle);
}

void LpSpiRtos::SetFlags(uint32_t flags)
{
    this->flags = flags;
}

std::vector<uint8_t> LpSpiRtos::ReadBytes(uint16_t size)
{
    uint8_t txbuf[size];
    uint8_t rxbuf[size];
    lpspi_transfer_t spiData = {
        reinterpret_cast<uint8_t *>(&txbuf),
        reinterpret_cast<uint8_t *>(&rxbuf),
        static_cast<size_t>(size),
        kLPSPI_MasterPcs0 | kLPSPI_MasterPcsContinuous
    };
    if (LPSPI_RTOS_Transfer(this->handle, &spiData) == kStatus_Success)
    {
        std::vector<uint8_t> rData(size, 0);
        for (int i = 0; i < size; i++)
        {
            rData[i] = rxbuf[i];
        }
        return rData;
    }
    else
    {
        return std::vector<uint8_t>{};
    }
}

bool LpSpiRtos::WriteBytes(std::vector<uint8_t> data)
{
    int len = data.size();
    uint8_t txbuf[len];
    uint8_t rxbuf[len];
    lpspi_transfer_t spiData = {
        reinterpret_cast<uint8_t *>(&txbuf),
        reinterpret_cast<uint8_t *>(&rxbuf),
        static_cast<size_t>(len),
        kLPSPI_MasterPcs0 | kLPSPI_MasterPcsContinuous
    };
    return LPSPI_RTOS_Transfer(this->handle, &spiData) == kStatus_Success;
}
void LpSpiRtos::SetBaudrate(uint32_t baudrate){}