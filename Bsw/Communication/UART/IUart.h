//
// Created by PC on 09.06.2021.
//
#pragma once
#include "../ICom.h"

namespace Communication::Uart
{
    class IUart : public ICom
    {
        virtual void setBaudrate(uint32_t baudrate) =0;
    };
}
