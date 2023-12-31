//
// Created by PC on 09.06.2021.
//

#pragma once

#include "../IUart.h"
#include <fsl_lpuart_edma.h>
#include <FreeRTOS.h>
#include <event_groups.h>


namespace Communication::Uart
{
    class LpUartDma final : public IUart
    {
    private:
        uint32_t Clock = 12000000UL;
        enum TransferState:EventBits_t
        {
            Complete = 0x01,
            Error = 0x02,
        };

        static void callback(LPUART_Type* base,
                             lpuart_edma_handle_t* handle,
                             status_t status,
                             void* userData);

        LPUART_Type *base_;
        lpuart_edma_handle_t* handle_;
        uint32_t timeout_;
        EventGroupHandle_t rxEvent_;
        EventGroupHandle_t txEvent_;

    public:
        explicit LpUartDma(LPUART_Type* base, lpuart_edma_handle_t* handle, uint32_t timeout);
        ~LpUartDma() override;
        bool readBytes(uint16_t size, std::vector<uint8_t>& data) override;
        bool writeBytes(const std::vector<uint8_t>& data) override;
        void setBaudrate(uint32_t baudrate) override;
    };
}
