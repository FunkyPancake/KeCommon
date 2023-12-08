/*
 * tle9461.cpp
 *
 *  Created on: 5 gru 2020
 *      Author: Mati
 */
#include "Tle9461.h"
#include <cstdlib>
#include <Tle9xxx.h>


namespace DeviceDriver::TleSbc {
#define SPI_REG_WRITE(x) (x | 0x80)
#define SPI_REG_READ_CLEAR(x) (x | 0x80)
#define SPI_REG_READ(x) (x & 0xef)

    Tle9461::Tle9461(const ISpi&spi, const SbcConfig&config) : spi_(spi) {
        writeRegister(ControlRegisters::M_S_CTRL, MSCtrl{.B{.Vcc2_On = 1, .Mode = 0,}}.R);
        writeRegister(ControlRegisters::BUS_CTRL_0, BusCtrl0{.B{.Can = 3}}.R);
        ConfigWatchdog(config.WatchdogPeriod, config.WatchdogWindowMode);
    }


    void Tle9461::getStatus(const uint8_t data) {
        status_.R = data;
    }

    bool Tle9461::readRegister(ControlRegisters reg, uint8_t&data, const bool clearFlag) {
        return readRegister(static_cast<uint8_t>(reg), data, clearFlag);
    }

    bool Tle9461::readRegister(StatusRegisters reg, uint8_t&data, const bool clearFlag) {
        return readRegister(static_cast<uint8_t>(reg), data, clearFlag);
    }

    bool Tle9461::readRegister(const uint8_t reg, uint8_t&data, const bool clearFlag) {
        bool ret = false;
        txBuf_[0] = clearFlag ? SPI_REG_READ(reg) : SPI_REG_READ_CLEAR(reg);
        if (spi_.Transfer(txBuf_.data(), rxBuf_.data(), txBuf_.size())) {
            getStatus(rxBuf_[0]);
            data = rxBuf_[1];
            ret = true;
        }
        return ret;
    }

    bool Tle9461::writeRegister(StatusRegisters reg, const uint8_t data) {
        return writeRegister(static_cast<uint8_t>(reg), data);
    }

    bool Tle9461::writeRegister(ControlRegisters reg, uint8_t data) {
        return writeRegister(static_cast<uint8_t>(reg), data);
    }

    bool Tle9461::writeRegister(const uint8_t reg, const uint8_t data) {
        bool ret = false;
        txBuf_[0] = SPI_REG_WRITE(reg);
        txBuf_[1] = data;
        if (spi_.Transfer(txBuf_.data(), rxBuf_.data(), txBuf_.size())) {
            getStatus(rxBuf_[0]);
            ret = true;
        }
        return ret;
    }

    void Tle9461::RefreshWatchdog() {
        WdCtrl reg;
        reg.R = wdReg_;
        auto sum = 0;
        for (int i = 0; i < 8; i++) {
            sum += reg.R >> i & 0x01;
        }
        reg.B.Checksum = sum & 0x01;
        writeRegister(ControlRegisters::WD_CTRL, reg.R);
    }

    void Tle9461::ConfigWatchdog(const WdTimerPeriod timeoutMs, const bool windowWatchdog) {
        const WdCtrl reg{.B{.WdTimer = static_cast<uint8_t>(timeoutMs), .WdWin = windowWatchdog, .Checksum = 0}};
        wdReg_ = reg.R;
        RefreshWatchdog();
    }
}
