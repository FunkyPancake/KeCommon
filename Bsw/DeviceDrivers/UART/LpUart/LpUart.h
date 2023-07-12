//
// Created by PC on 09.06.2021.
//

#pragma once

#include "../IUart.h"
#include <fsl_lpuart_freertos.h>

class LpUart : public IUart
{
private:
    lpuart_rtos_handle_t* _handle;
    uint32_t _baseClock;
public:
    LpUart(lpuart_rtos_handle_t* handle, lpuart_rtos_config_t *config);
    std::vector<uint8_t> ReadBytes(uint16_t size) override;
    bool WriteBytes(const std::vector<uint8_t>& data)override;
    void SetBaudrate(uint32_t baudrate);
};

