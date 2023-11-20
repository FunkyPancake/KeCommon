//
// Created by PC on 19.04.2021.
//

#pragma once
#include <cstdint>
#include <vector>
class ICom
{
  public:
    virtual bool writeBytes(const std::vector<uint8_t>& data)=0;
    virtual std::vector<uint8_t> readBytes(uint16_t size)=0;
    virtual void setBaudrate(uint32_t baudrate)=0;
};
