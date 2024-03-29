//
// Created by PC on 09.06.2021.
//

#include "LpUartDma.h"
#include <fsl_lpuart.h>
using namespace Communication::Uart;

void LpUartDma::callback(LPUART_Type* base, lpuart_edma_handle_t* handle, status_t status, void* userData)
{
    auto objPointer = static_cast<LpUartDma*>(userData);
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t xResult = pdFAIL;
    if (status == kStatus_LPUART_RxIdle)
    {
        xResult = xEventGroupSetBitsFromISR(objPointer->rxEvent_, TransferState::Complete, &xHigherPriorityTaskWoken);
    }
    if (status == kStatus_LPUART_TxIdle)
    {
        xResult = xEventGroupSetBitsFromISR(objPointer->txEvent_, TransferState::Complete, &xHigherPriorityTaskWoken);
    }
    if (xResult != pdFAIL)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

LpUartDma::LpUartDma(LPUART_Type* base, lpuart_edma_handle_t* handle, uint32_t timeout): base_(base), handle_(handle),
    timeout_(pdMS_TO_TICKS(timeout))
{
    handle_->callback = LpUartDma::callback;
    handle_->userData = this;
    rxEvent_ = xEventGroupCreate();
    txEvent_ = xEventGroupCreate();
}


LpUartDma::~LpUartDma()
{
    vEventGroupDelete(rxEvent_);
    vEventGroupDelete(txEvent_);
}

bool LpUartDma::readBytes(uint16_t size, std::vector<uint8_t>& data)
{
    std::vector<uint8_t> buf(size);
    lpuart_transfer_t xfer{
        .data = const_cast<uint8_t*>(buf.data()),
        .dataSize = buf.size()
    };
    base_->FIFO |= LPUART_FIFO_RXFLUSH(1);
    base_->STAT |= LPUART_STAT_OR(1);
    LPUART_TransferAbortReceiveEDMA(const_cast<LPUART_Type*>(base_), const_cast<lpuart_edma_handle_t*>(handle_));
    if (LPUART_ReceiveEDMA(base_, handle_,
                           &xfer) != kStatus_Success)
    {
        return false;
    }
    auto ev = xEventGroupWaitBits(rxEvent_, TransferState::Complete | TransferState::Error, pdTRUE, pdFALSE,
                                  timeout_);
    if (ev & TransferState::Error)
    {
        xEventGroupClearBits(rxEvent_, TransferState::Complete);
        return false;
    }
    if (ev & TransferState::Complete)
    {
        data = buf;
        return true;
    }
    return false;
}


bool LpUartDma::writeBytes(const std::vector<uint8_t>& data)
{
    lpuart_transfer_t xfer{
        .data = const_cast<uint8_t*>(data.data()),
        .dataSize = data.size()
    };
    if (LPUART_SendEDMA(const_cast<LPUART_Type*>(base_), const_cast<lpuart_edma_handle_t*>(handle_), &xfer) !=
        kStatus_Success)
    {
        return false;
    }
    auto ev = xEventGroupWaitBits(txEvent_, TransferState::Complete | TransferState::Error, pdTRUE, pdFALSE,
                                  timeout_);
    if (ev & TransferState::Error)
    {
        xEventGroupClearBits(txEvent_, TransferState::Complete);
        return false;
    }
    if (ev & TransferState::Complete)
    {
        return true;
    }
    return false;
}

void LpUartDma::setBaudrate(uint32_t baudrate)
{
    LPUART_SetBaudRate(const_cast<LPUART_Type*>(base_), baudrate, Clock);
}

