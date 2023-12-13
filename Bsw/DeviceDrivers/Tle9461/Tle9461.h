/*
 * tle9461.h
 *
 *  Created on: 5 gru 2020
 *      Author: Mati
 */

#pragma once

#include <array>

#include "../SPI/ISpi.h"
#include <cstdint>

namespace DeviceDrivers::TleSbc
{
    class Tle9461
    {
    public:
        enum class WdTimerPeriod
        {
            WgTimer10ms = 0,
            WgTimer20ms,
            WgTimer50ms,
            WgTimer100ms,
            WgTimer200ms,
            WgTimer500ms,
            WgTimer1000ms,
            WgTimer10000ms
        };

    private:
        enum class ControlRegisters
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
        };

        enum class StatusRegisters
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
        };

        typedef union
        {
            uint8_t R;

            struct
            {
                uint8_t Vcc1_Rt : 2;
                uint8_t Vcc1_Ov_Rst : 1;
                uint8_t Vcc2_On : 2;
                uint8_t Reserved : 1;
                uint8_t Mode : 2;
            } B;
        } MSCtrl;

        typedef union
        {
            uint8_t R;

            struct
            {
                uint8_t Can : 3;
                uint8_t Reserved : 5;
            } B;
        } BusCtrl0;

        typedef union
        {
            uint8_t R;

            struct
            {
                uint8_t WdTimer : 3;
                uint8_t Reserved : 1;
                uint8_t WdEnWkBus : 1;
                uint8_t WdWin : 1;
                uint8_t WdStmEn0 : 1;
                uint8_t Checksum : 1;
            } B;
        } WdCtrl;

        typedef union
        {
            struct
            {
                uint8_t Sup0 : 1;
                uint8_t Therm : 1;
                uint8_t Dev : 1;
                uint8_t Bus : 1;
                uint8_t Wk : 1;
                uint8_t Sup1 : 1;
                uint8_t GpioOc : 1;
                uint8_t GpioOl : 1;
            } B;

            uint8_t R;
        } DeviceStatus;

        enum class WakeRegisters
        {
            SWK_STAT = 0x70,
            SWK_ECNT_STAT = 0x71,
            SWK_CDR_STAT1 = 0x72,
            SWK_CDR_STAT2 = 0x73,
        };

        const Communication::Spi::ISpi& spi_;
        std::array<uint8_t, 2> txBuf_{};
        std::array<uint8_t, 2> rxBuf_{};
        DeviceStatus status_{};
        uint8_t wdReg_{};

        void getStatus(uint8_t data);

        bool readRegister(ControlRegisters reg, uint8_t& data, bool clearFlag);
        bool readRegister(StatusRegisters reg, uint8_t& data, bool clearFlag);
        bool readRegister(uint8_t reg, uint8_t& data, bool clearFlag);

        bool writeRegister(ControlRegisters reg, uint8_t data);
        bool writeRegister(StatusRegisters reg, uint8_t data);
        bool writeRegister(uint8_t reg, uint8_t data);

        void ConfigWatchdog(WdTimerPeriod timeoutMs, bool windowWatchdog = false);

    public:
        struct SbcConfig
        {
            bool Vcc2Enable;
            bool WakeupEnable;
            bool CanEnable;
            bool WatchdogWindowMode;
            WdTimerPeriod WatchdogPeriod;
        };

        Tle9461(const Communication::Spi::ISpi& spi, const SbcConfig& config);

        ~Tle9461() = default;

        void RefreshWatchdog();
    };
}
