/*
 * can_il.h
 *
 *  Created on: 14 gru 2020
 *      Author: Mati
 */

#ifndef CDD_CAN_IL_H_
#define CDD_CAN_IL_H_

#include <map>
#include "ICan.h"
#include "fsl_flexcan.h"

namespace KeCommon::Bsw::Can {
    class FlexCan : public ICan {
    private:
        int mailboxCount;
        CAN_Type *canBase;

        static void WritePayloadRegisters(flexcan_frame_t *frame, const uint8_t *data, uint8_t dlc);

        std::map<uint8_t,std::function<void(KeCommon::Bsw::Can::ICanFrame frame)>> _registeredRxMb;
        static ICanFrame ToICanFrame(const _flexcan_frame &frame);

    public:
        template<typename T> static
        T SwapBytes(T data) {
            std::array<uint8_t, sizeof(T)> tmp{};
            *(T *) tmp.data() = data;
            std::reverse(tmp.begin(), tmp.end());
            return *(T *) tmp.data();
        }
        explicit FlexCan(int mailboxCount);

        void RegisterRxFrame(uint32_t id, const std::function<void(KeCommon::Bsw::Can::ICanFrame frame)>& handler);

        bool Send(uint32_t id, Payload &data, uint8_t dlc) override;

        bool Receive(uint32_t *id, Payload *data, uint8_t dlc) override;

        bool SendCyclic();

        void RxTask();

    };
}

#endif /* CDD_CAN_IL_H_ */
