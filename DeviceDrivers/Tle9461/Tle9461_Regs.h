//
// Created by PC on 09.06.2021.
//
#pragma once

typedef union
{
    uint8_t R;
    struct
    {
        uint8_t Vcc1_Rt: 2;
        uint8_t Vcc1_Ov_Rst: 1;
        uint8_t Vcc2_On: 2;
        uint8_t _Reserved: 1;
        uint8_t Mode: 2;
    } B;
} MSCtrl;

typedef union
{
    uint8_t R;
    struct
    {
        uint8_t Can: 3;
        uint8_t _Reserved: 5;
    } B;
} BusCtrl0;

typedef union
{
    uint8_t R;
    struct
    {
        uint8_t WdTimer: 3;
        uint8_t Reserved: 1;
        uint8_t WdEnWkBus: 1;
        uint8_t WdWin: 1;
        uint8_t WdStmEn0: 1;
        uint8_t Checksum: 1;
    } B;
} WdCtrl;
typedef enum
{
    StateNormal, StateSleep, StateStop, StateReset
} StateEnum;
