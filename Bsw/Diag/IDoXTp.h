//
// Created by PC on 05.07.2023.
//

#ifndef ICALTP_H
#define ICALTP_H
#include "cstdint"
#include "vector"

namespace Diag
{
    class IDoXTp
    {
    public:
        virtual ~IDoXTp() = default;

        virtual bool TxRdy() = 0;

        virtual bool RxRdy() = 0;

        virtual bool Write(std::vector<uint8_t>& data) = 0;

        virtual std::vector<uint8_t> Read() = 0;
    };
}
#endif //ICALTP_H
