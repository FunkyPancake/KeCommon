/*
 * spi_abstract.c
 *
 *  Created on: 14 gru 2020
 *      Author: Mati
 */
#include "LpSpi.h"
#include  <cstring>
LpSpiRtos::LpSpiRtos(lpspi_rtos_handle_t *handle, const CsPin csPin, const bool reverseByteOrder) : _handle{handle}
{
    _flags = MapCs(csPin);
    if(reverseByteOrder){
        _flags|= kLPSPI_MasterByteSwap;
    }

}

LpSpiRtos::~LpSpiRtos()
{
    LPSPI_RTOS_Deinit(this->_handle);
}

bool LpSpiRtos::Transfer(const uint8_t *txData, uint8_t *rxData, const uint8_t len) const
{
    lpspi_transfer_t data = {const_cast<uint8_t *>(txData), rxData, len,
                             _flags};
    return LPSPI_RTOS_Transfer(this->_handle, &data) == kStatus_Success;
}

std::vector<uint8_t> LpSpiRtos::readBytes(const uint16_t size)
{
    uint8_t txBuf[size];
    uint8_t rxBuf[size];
    lpspi_transfer_t spiData = {
            reinterpret_cast<uint8_t *>(&txBuf),
            reinterpret_cast<uint8_t *>(&rxBuf),
            static_cast<size_t>(size),
            _flags
    };
    if (LPSPI_RTOS_Transfer(this->_handle, &spiData) == kStatus_Success)
    {
        return {rxBuf, rxBuf+size};
    }
    return std::vector<uint8_t>{};
}

bool LpSpiRtos::writeBytes(const std::vector<uint8_t> &data)
{
    const size_t len = data.size();
    uint8_t rxBuf[len];
    lpspi_transfer_t spiData = {
            const_cast<uint8_t*>(data.data()),
            reinterpret_cast<uint8_t *>(&rxBuf),
            len,
            _flags
    };
    return LPSPI_RTOS_Transfer(this->_handle, &spiData) == kStatus_Success;
}

void LpSpiRtos::setBaudrate(const uint32_t baudrate)
{
    (void) baudrate;
}

uint32_t LpSpiRtos::MapCs(const ISpi::CsPin cs)
{
    uint32_t flags = kLPSPI_MasterPcsContinuous;
    switch (cs)
    {
        case CsPin::Cs0:
            flags |= kLPSPI_MasterPcs0;
            break;
        case CsPin::Cs1:
            flags |= kLPSPI_MasterPcs1;
            break;
        case CsPin::Cs2:
            flags |= kLPSPI_MasterPcs2;
            break;
        case CsPin::NoCs:
            break;
    }
    return flags;
}
