//
// Created by PC on 09.06.2021.
//

#pragma once

#include <IUart.h>

class LpUart : public IUart
{
public:
    std::vector<uint8_t> ReadBytes(uint16_t size) override;
    bool WriteBytes(std::vector<uint8_t> data) override;
};

