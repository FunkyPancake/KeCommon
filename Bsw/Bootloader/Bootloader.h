//
// Created by PC on 16.10.2022.
//

#ifndef CANGPS_BOOTLOADER_H
#define CANGPS_BOOTLOADER_H

#include <stdint.h>
const uint16_t DiagCanRxId = 0x7F0;
const uint16_t DiagCanTxId = 0x7F1;
void JumpToBootloader(void);

#endif//CANGPS_BOOTLOADER_H
