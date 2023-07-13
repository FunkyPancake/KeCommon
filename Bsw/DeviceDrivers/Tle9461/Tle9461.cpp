/*
 * tle9461.cpp
 *
 *  Created on: 5 gru 2020
 *      Author: Mati
 */
#include "Tle9461.h"
#include <cstdlib>

#define SPI_REG_WRITE(x) (x | 0x80)
#define SPI_REG_READ_CLEAR(x) (x | 0x80)
#define SPI_REG_READ(x) (x & 0xef)

Tle9461::Tle9461(ISpi *spi)
{
    this->spi = spi;
    this->txBuf = (uint8_t *) malloc(spiCommandLength);
    this->rxBuf = (uint8_t *) malloc(spiCommandLength);
}

void Tle9461::Init()
{
    {
        MSCtrl reg = {0};
        reg.B.Mode = 0;
        reg.B.Vcc2_On = 1;
        WriteRegister(M_S_CTRL, reg.R);
    }
    {
        BusCtrl0 reg = {0};
        reg.B.Can = 3;
        WriteRegister(BUS_CTRL_0, reg.R);
    }
}

Tle9461::~Tle9461()
{
    free(this->rxBuf);
    free(this->txBuf);
}

void Tle9461::GetStatus(uint8_t data)
{
    status.R = data;
}

bool Tle9461::WriteRegister(uint8_t regAddr, uint8_t data)
{
    bool ret = false;
    txBuf[0] = SPI_REG_WRITE(regAddr);
    txBuf[1] = data;
    if (spi->Transfer(this->txBuf, this->rxBuf, spiCommandLength))
    {
        GetStatus(rxBuf[0]);
        ret = true;
    }
    return ret;
}

bool Tle9461::ReadRegister(uint8_t regAddr, uint8_t *data, bool clearFlag)
{
    bool ret = false;
    txBuf[0] = clearFlag ? SPI_REG_READ(regAddr) : SPI_REG_READ_CLEAR(regAddr);
    if (spi->Transfer(this->txBuf, this->rxBuf, spiCommandLength))
    {
        GetStatus(rxBuf[0]);
        *data = rxBuf[1];
        ret = true;
    }
    return ret;
}

void Tle9461::RefreshWatchdog()
{
    WdCtrl reg;
    reg.R = wdReg;
    auto sum = 0;
    for (int i = 0; i < 8; i++)
    {
        sum += (reg.R >> i) & 0x01;
    }
    reg.B.Checksum = sum & 0x01;
    WriteRegister(WD_CTRL, reg.R);
}

void Tle9461::ConfigWatchdog(Tle9461::WdTimerPeriod timeoutMs, bool windowWatchdog)
{
    WdCtrl reg = {0};
    reg.B.Checksum = 0;
    reg.B.WdTimer = static_cast<uint8_t>(timeoutMs);
    reg.B.WdWin = windowWatchdog;
    wdReg = reg.R;
    RefreshWatchdog();
}
