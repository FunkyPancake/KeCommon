/*
 * tle9461.h
 *
 *  Created on: 5 gru 2020
 *      Author: Mati
 */

#pragma once

#include "ISpi.h"
#include <stdint.h>

class Tle9461
{
    
    typedef enum
    {
        WgTimer10ms,
        WgTimer20ms,
        WgTimer50ms,
        WgTimer100ms,
        WgTimer200ms,
        WgTimer500ms,
        WgTimer1000ms,
        WgTimer10000ms
    } WgPeriodEnum;
    
    typedef enum
    {
        StateNormal,
        StateSleep,
        StateStop,
        StateReset
    } StateEnum;
    
    typedef union
    {
        struct
        {
            uint8_t Sup0: 1;
            uint8_t Therm: 1;
            uint8_t Dev: 1;
            uint8_t Bus: 1;
            uint8_t Wk: 1;
            uint8_t Sup1: 1;
            uint8_t GpioOc: 1;
            uint8_t GpioOl: 1;
        } B;
        uint8_t R;
    } DeviceStatusType;

private:
    uint8_t *txBuf;
    uint8_t *rxBuf;
    DeviceStatusType status;
    uint8_t wdReg;
    ISpi *spi;
    
    void GetStatus(uint8_t data);
    
    bool ReadRegister(uint8_t regAddr, uint8_t *data, bool clearFlag);
    
    bool WriteRegister(uint8_t regAddr, uint8_t data);

public:
    
    explicit Tle9461(ISpi *spi);
    
    ~Tle9461();
    
    void Init();
    
    void RefreshWatchdog();
    
};
