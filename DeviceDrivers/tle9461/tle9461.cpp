/*
 * tle9461.cpp
 *
 *  Created on: 5 gru 2020
 *      Author: Mati
 */
#include "tle9461.h"
#include <stdlib.h>

#define SPI_COMMAND_LEN       2
#define SPI_REG_WRITE(x)      (x | 0x01)
#define SPI_REG_READ_CLEAR(x) (x | 0x01)
#define SPI_REG_READ(x)       (x & 0xfe)


// C O N T R O L   R E G I S T E R S
#define M_S_CTRL        0x01
#define HW_CTRL_0       0x02
#define WD_CTRL         0x03
#define BUS_CTRL_0      0x04
#define WK_CTRL_0       0x06
#define WK_CTRL_1       0x07
#define WK_PUPD_CTRL    0x08
#define BUS_CTRL_3      0x0B
#define TIMER_CTRL      0x0C
#define HW_CTRL_1       0x0E
#define HW_CTRL_2       0x0F
#define GPIO_CTRL       0x17
#define PWM_CTRL        0x18
#define PWM_FREQ_CTRL   0x1C
#define HW_CTRL_3       0x1D
#define SYS_STAT_CTRL_0 0x1E
#define SYS_STAT_CTRL_1 0x1F
// S E L E C T I V E   W A K E   R E G I S T E R S

// S T A T U S   R E G I S T E R S
#define SUP_STAT_1   0x40
#define SUP_STAT_0   0x41
#define THERM_STAT   0x42
#define DEV_STAT     0x43
#define BUS_STAT     0x44
#define WK_STAT_0    0x46
#define WK_STAT_1    0x47
#define WK_LVL_STAT  0x48
#define GPIO_OC_STAT 0x54
#define GPIO_OL_STAT 0x55

// S E L E C T I V E   W A K E   S T A T U S   R E G I S T E R S
#define SWK_STAT      0x70
#define SWK_ECNT_STAT 0x71
#define SWK_CDR_STAT1 0x72
#define SWK_CDR_STAT2 0x73

// F A M I LY   A N D   P R O D U C T    R E G I S T E R S
#define FAM_PROD_STAT       0x7E
#define TLE9461_SPI_MSG_LEN 2


Tle9461::Tle9461(ISpi *spi) {
    this->spi = spi;
    this->txBuf = (uint8_t *) malloc(SPI_COMMAND_LEN);
    this->rxBuf = (uint8_t *) malloc(SPI_COMMAND_LEN);
}

void Tle9461::Init() {
//    uint8_t data;
//    WriteRegister(M_S_CTRL,data);
}

Tle9461::~Tle9461() {
    free(this->rxBuf);
    free(this->txBuf);
}

void Tle9461::GetStatus(uint8_t data) {
    status.R = data;
}


bool Tle9461::WriteRegister(uint8_t regAddr, uint8_t data) {
    bool ret = false;
    txBuf[0] = SPI_REG_WRITE(regAddr);
    txBuf[1] = data;
    if (spi->Transfer(this->txBuf, this->rxBuf, SPI_COMMAND_LEN)) {
        GetStatus(rxBuf[0]);
        ret = true;
    }
    return ret;
}


bool Tle9461::ReadRegister(uint8_t regAddr, uint8_t *data, bool clearFlag) {
    bool ret = false;
    txBuf[0] = clearFlag ? SPI_REG_READ(regAddr) : SPI_REG_READ_CLEAR(regAddr);
    if (spi->Transfer(this->txBuf, this->rxBuf, SPI_COMMAND_LEN)) {
        GetStatus(rxBuf[0]);
        *data = rxBuf[1];
        ret = true;
    }
    return ret;
}

void Tle9461::RefreshWatchdog() {
    WriteRegister(WD_CTRL, wdReg);
}