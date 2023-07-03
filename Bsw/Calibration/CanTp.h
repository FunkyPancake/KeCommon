//
// Created by PC on 02.07.2023.
//

#ifndef CANTP_H
#define CANTP_H


#include "ICan.h"

namespace KeCommon::Bsw::Cal {

    class CanTp {
    public:
        void ProcessFrame(const KeCommon::Bsw::Can::ICanFrame &frame);
        explicit CanTp(uint32_t rxId, uint32_t txId, Can::ICan& can) ;
    private:
        std::array<uint8_t, 128> rxBuffer{};
        std::array<uint8_t, 128> txBuffer{};

        void ProcessSingleFrame(const Can::ICanFrame &frame);

        void ProcessFirstFrame(const Can::ICanFrame &frame);

        void ProcessConsecutiveFreame(const Can::ICanFrame &frame);

        void ProcessFlowControlFrame(const Can::ICanFrame &frame);
    };
}
#endif //CANTP_H
