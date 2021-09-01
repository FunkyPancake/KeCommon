/*
 * tle9461.cpp
 *
 *  Created on: 5 gru 2020
 *      Author: Mati
 */
#include "Tle9461.h"
#include "Tle9461_Regs.h"
#include <stdlib.h>

#define SPI_REG_WRITE(x) (x | 0x80)
#define SPI_REG_READ_CLEAR(x) (x | 0x80)
#define SPI_REG_READ(x) (x & 0xef)

enum
{
    M_S_CTRL = 0x01,
    HW_CTRL_0 = 0x02,
    WD_CTRL = 0x03,
    BUS_CTRL_0 = 0x04,
    WK_CTRL_0 = 0x06,
    WK_CTRL_1 = 0x07,
    WK_PUPD_CTRL = 0x08,
    BUS_CTRL_3 = 0x0B,
    TIMER_CTRL = 0x0C,
    HW_CTRL_1 = 0x0E,
    HW_CTRL_2 = 0x0F,
    GPIO_CTRL = 0x17,
    PWM_CTRL = 0x18,
    PWM_FREQ_CTRL = 0x1C,
    HW_CTRL_3 = 0x1D,
    SYS_STAT_CTRL_0 = 0x1E,
    SYS_STAT_CTRL_1 = 0x1F,
} ControlRegisters;

// S T A T U S   R E G I S T E R S
enum
{
    SUP_STAT_1 = 0x40,
    SUP_STAT_0 = 0x41,
    THERM_STAT = 0x42,
    DEV_STAT = 0x43,
    BUS_STAT = 0x44,
    WK_STAT_0 = 0x46,
    WK_STAT_1 = 0x47,
    WK_LVL_STAT = 0x48,
    GPIO_OC_STAT = 0x54,
    GPIO_OL_STAT = 0x55,
} StatusRegisters;
// S E L E C T I V E   W A K E   S T A T U S   R E G I S T E R S

enum
{
    SWK_STAT = 0x70, SWK_ECNT_STAT = 0x71, SWK_CDR_STAT1 = 0x72, SWK_CDR_STAT2 = 0x73,
} WakeRegisters;

Tle9461::Tle9461(ISpi *spi)
{
    this->spi = spi;
    this->txBuf = (uint8_t *) malloc(spiCommandLength);
    this->rxBuf = (uint8_t *) malloc(spiCommandLength);
}

void Tle9461::Init()
{
    {
        MSCtrl reg ={0};
        reg.B.Mode = 0;
        reg.B.Vcc2_On = 1;
        WriteRegister(M_S_CTRL, reg.R);
    }
    {
        BusCtrl0 reg ={0};
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
    wdReg= reg.R;
    RefreshWatchdog();
}
