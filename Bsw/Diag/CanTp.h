//
// Created by PC on 02.07.2023.
//

#ifndef CANTP_H
#define CANTP_H

#include <CanFrame.h>
#include <ICan.h>

#include "IDoXTp.h"

namespace Diag
{

    class CanTp : public IDoXTp
    {
    public:
        void ProcessFrame(const Communication::Can::CanFrame &frame);

        explicit CanTp(Communication::Can::ICan &can);

        void TxMainFunction();

        bool TxRdy() override;

        bool RxRdy() override;

        bool Write(std::vector<uint8_t> &data) override;

        std::vector<uint8_t> Read() override;

    private:
        static constexpr uint8_t FrameDlc = 8;
        static constexpr uint32_t MaxPayloadSizeFrame = FrameDlc - 1;
        static constexpr uint32_t MaxPayloadSizeFirstFrame = FrameDlc - 2;
        static constexpr uint32_t bufSize = 512;
        static constexpr uint8_t FillByte = 0x55;
        Communication::Can::ICan &_can;
        uint32_t _txId;
        std::array<uint8_t, bufSize> _txBuf{};
        uint8_t _txCnt{0};
        uint32_t _txBufPtr{0};
        uint32_t _txMsgLen{0};
        bool _txRdy{true};

        std::array<uint8_t, bufSize> _rxBuf{};
        bool _rxRdy{false};
        uint32_t _rxBufPtr{0};
        uint32_t _rxId;

        void ProcessSingleFrame(const Communication::Can::CanFrame &frame);

        void ProcessFirstFrame(const Communication::Can::CanFrame &frame);

        void ProcessConsecutiveFreame(const Communication::Can::CanFrame &frame);

        void ProcessFlowControlFrame(const Communication::Can::CanFrame &frame);
    };
}// namespace Diag
#endif//CANTP_H
