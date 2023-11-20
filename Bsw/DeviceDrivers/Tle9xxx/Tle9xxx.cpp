/*
 * Tle9xxx.cpp
 *
 *  Created on: 5 gru 2020
 *      Author: Mati
 */
#include "Tle9xxx.h"
#include <cstdlib>
#include <memory>

#define SPI_REG_WRITE(x) (x | 0x80)
#define SPI_REG_READ_CLEAR(x) (x | 0x80)
#define SPI_REG_READ(x) (x & 0xef)

Tle9xxx::Tle9xxx(const ISpi &spi, const Tle9xxx::SbcConfig &config) : _txBuf(std::vector<uint8_t>(SpiCommandLength)),
                                                                      _rxBuf(std::vector<uint8_t>(SpiCommandLength)),
                                                                      _spi(spi)
{
    WriteRegister(static_cast<uint8_t>(ControlRegisters::M_S_CTRL), MSCtrl{.B{.Vcc2_On=1, .Mode=0,}}.R);
    WriteRegister(static_cast<uint8_t>(ControlRegisters::BUS_CTRL_0), BusCtrl0{.B{.Can=3}}.R);

    ConfigWatchdog(config.WatchdogPeriod, config.WatchdogWindowMode);
}


Tle9xxx::~Tle9xxx()
{
}

void Tle9xxx::GetStatus(uint8_t data)
{
    _status.R = data;
}

bool Tle9xxx::WriteRegister(uint8_t regAddr, uint8_t data)
{
    bool ret = false;
    _txBuf[0] = SPI_REG_WRITE(regAddr);
    _txBuf[1] = data;
    if (_spi.Transfer(_txBuf.data(), _rxBuf.data(), SpiCommandLength))
    {
        GetStatus(_rxBuf[0]);
        ret = true;
    }
    return ret;
}

bool Tle9xxx::ReadRegister(uint8_t regAddr, uint8_t *data, bool clearFlag)
{
    bool ret = false;
    _txBuf[0] = clearFlag ? SPI_REG_READ(regAddr) : SPI_REG_READ_CLEAR(regAddr);
    if (_spi.Transfer(_txBuf.data(), _rxBuf.data(), SpiCommandLength))
    {
        GetStatus(_rxBuf[0]);
        *data = _rxBuf[1];
        ret = true;
    }
    return ret;
}

void Tle9xxx::RefreshWatchdog()
{
    WdCtrl reg;
    reg.R = wdReg;
    auto sum = 0;
    for (int i = 0; i < 8; i++)
    {
        sum += (reg.R >> i) & 0x01;
    }
    reg.B.Checksum = sum & 0x01;
    WriteRegister(static_cast<uint8_t>(ControlRegisters::WD_CTRL), reg.R);
}

void Tle9xxx::ConfigWatchdog(Tle9xxx::WdTimerPeriod timeoutMs, bool windowWatchdog)
{
    WdCtrl reg{.B{.WdTimer = static_cast<uint8_t>(timeoutMs), .WdWin = windowWatchdog, .Checksum=0}};
    wdReg = reg.R;
    RefreshWatchdog();
}

