//
// Created by PC on 02.07.2023.
//

#ifndef CANTP_H
#define CANTP_H


#include "ICan.h"
#include "IDoxTp.h"

namespace KeCommon::Bsw::Cal {

    class CanTp : public IDoxTp {
    public:
        void ProcessFrame(const KeCommon::Bsw::Can::ICanFrame &frame);

        explicit CanTp(Can::ICan &can, uint32_t rxId, uint32_t txId);

        void SendCmd();

        std::vector<uint8_t> GetCmd();

        bool CommandAvailable();

        void TxTask();

        bool TxRdy() override;

        bool RxCmdAvailable() override;

        bool Write(std::vector<uint8_t> &data) override;

        std::vector<uint8_t> ReadCommand();

    private:
        static constexpr uint8_t FrameDlc = 8;
        static constexpr uint32_t MaxPayloadSizeFrame = FrameDlc - 1;
        static constexpr uint32_t MaxPayloadSizeFirstFrame = FrameDlc - 2;
        static constexpr uint32_t bufSize = 128;
        static constexpr uint8_t FillByte = 0x55;
        KeCommon::Bsw::Can::ICan &_can;
        std::array<uint8_t, bufSize> _rxBuf{};
        std::array<uint8_t, bufSize> _txBuf{};
        uint8_t _txCnt{0};
        uint32_t _txId;
        uint32_t _txBufPtr{0};
        uint32_t _txMsgLen{0};
        uint32_t _rxBufPtr{0};
        bool _txRdy{true};
        bool _commandAvailable{false};

        void ProcessSingleFrame(const Can::ICanFrame &frame);

        void ProcessFirstFrame(const Can::ICanFrame &frame);

        void ProcessConsecutiveFreame(const Can::ICanFrame &frame);

        void ProcessFlowControlFrame(const Can::ICanFrame &frame);
    };
}
#endif //CANTP_H
