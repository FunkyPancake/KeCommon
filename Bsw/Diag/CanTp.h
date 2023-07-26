//
// Created by PC on 02.07.2023.
//

#ifndef CANTP_H
#define CANTP_H

#include "../DeviceDrivers/CAN/ICan.h"
#include "IDoXTp.h"

namespace KeCommon::Bsw::Diag
{

    class CanTp : public IDoXTp
    {
    public:
        void ProcessFrame(const KeCommon::Bsw::Can::CanFrame &frame);

        explicit CanTp(Can::ICan &can);

        void TxMainFunction();

        bool TxRdy() override;

        bool RxRdy() override;

        bool Write(std::vector<uint8_t> &data) override;

        std::vector<uint8_t> Read() override;

    private:
        static constexpr uint8_t FrameDlc = 8;
        static constexpr uint32_t MaxPayloadSizeFrame = FrameDlc - 1;
        static constexpr uint32_t MaxPayloadSizeFirstFrame = FrameDlc - 2;
        static constexpr uint32_t bufSize = 128;
        static constexpr uint8_t FillByte = 0x55;
        KeCommon::Bsw::Can::ICan &_can;
        uint32_t _txId;
        std::array<uint8_t, bufSize> _txBuf{};
        uint8_t _txCnt{0};
        uint32_t _txBufPtr{0};
        uint32_t _txMsgLen{0};
        bool _txRdy{true};

        std::array<uint8_t, bufSize> _rxBuf{};
        bool _rxRdy{false};
        uint32_t _rxBufPtr{0};

        void ProcessSingleFrame(const Can::CanFrame &frame);

        void ProcessFirstFrame(const Can::CanFrame &frame);

        void ProcessConsecutiveFreame(const Can::CanFrame &frame);

        void ProcessFlowControlFrame(const Can::CanFrame &frame);
    };
}// namespace KeCommon::Bsw::Diag
#endif//CANTP_H
