//
// Created by PC on 09.06.2021.
//

#include "LpUart.h"

std::vector<uint8_t> LpUart::ReadBytes(uint16_t size)
{
    std::vector<uint8_t> buf(size,0);
    size_t realSize;
    do{
        LPUART_RTOS_Receive(_handle, buf.data(), buf.size(), &realSize);
    }while(realSize != size);
    return {buf.begin(),(buf.begin()+realSize)};
}

bool LpUart::WriteBytes(const std::vector<uint8_t>& data)
{
    return LPUART_RTOS_Send(_handle, data.data(), data.size()) == kStatus_Success;
}

LpUart::LpUart(lpuart_rtos_handle_t *handle, lpuart_rtos_config_t *config)
{
    _handle = handle;
    _baseClock = config->srcclk;
}

void LpUart::SetBaudrate(uint32_t baudrate)
{
    LPUART_SetBaudRate(_handle->base, baudrate, _baseClock);
}
