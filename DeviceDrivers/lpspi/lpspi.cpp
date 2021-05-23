/*
 * spi_abstract.c
 *
 *  Created on: 14 gru 2020
 *      Author: Mati
 */
#include "lpspi.h"

bool LpSpiRtos::Transfer(uint8_t *tx_data, uint8_t *rx_data, uint8_t len)
{
    lpspi_transfer_t data = { tx_data, rx_data, len, kLPSPI_MasterPcs0 | kLPSPI_MasterPcsContinuous };
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
