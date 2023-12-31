//
// Created by PC on 19.04.2021.
//

#pragma once
#include <cstdint>
#include <vector>

namespace Communication
{
    class ICom
    {
    public:
        virtual ~ICom() = default;
        virtual bool writeBytes(const std::vector<uint8_t>& data) =0;
        virtual bool readBytes(uint16_t size, std::vector<uint8_t>& data) =0;
    };
}
