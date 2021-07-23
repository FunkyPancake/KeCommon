//
// Created by PC on 19.04.2021.
//

#pragma once
#include "stdint.h"
#include "vector"
class ICom
{
  public:
    virtual bool WriteBytes(std::vector<uint8_t> data)=0;
    virtual std::vector<uint8_t> ReadBytes(uint16_t size)=0;
    virtual void SetBaudrate(uint32_t baudrate)=0;
};
