/*
 * can_il.h
 *
 *  Created on: 14 gru 2020
 *      Author: Mati
 */

#ifndef CDD_CAN_IL_H_
#define CDD_CAN_IL_H_

#include "../ICan.h"
#include "FreeRTOS.h"
#include "fsl_flexcan.h"
#include "semphr.h"
#include <map>
#include <set>

namespace KeCommon::Bsw::Can {
    class FlexCan : public ICan {
    private:
        struct TxFrameEntry {
            uint16_t cnt;
            uint16_t reloadCnt;
            CanFrame frame;
        };
        static constexpr int MutexWaitTime = 100;
        uint32_t _mailboxCount;
        CAN_Type *_canBase;
        SemaphoreHandle_t _mutex;

        static void WritePayloadRegisters(flexcan_frame_t *frame, const uint8_t *data, uint8_t dlc);

        std::unordered_map<uint8_t, std::function<void(KeCommon::Bsw::Can::CanFrame frame)>> _registeredRxMb;
        std::unordered_map<uint32_t, TxFrameEntry> _cyclicTxList;

        static CanFrame ToICanFrame(const _flexcan_frame &frame);

    public:

        explicit FlexCan(CAN_Type *canBase, int mailboxCount);

        bool RegisterRxFrame(uint32_t id,
                             const std::function<void(const KeCommon::Bsw::Can::CanFrame &frame)> &handler) override;

        void RegisterCyclicTxFrame(uint32_t id, uint8_t dlc, uint32_t cycleTime) override;

        bool Send(uint32_t id, const Payload &data, uint8_t dlc) override;

        bool Send(const CanFrame &frame) override;

        bool ReadFrame(uint32_t *id, Payload *data, uint8_t dlc) override;

        bool ReadFrame(CanFrame &frame) override;

        bool UpdateCyclicFrame(uint32_t id, const Payload &data) override;

        void UpdateCyclicFrame(const CanFrame &frame) override;

        void RxTask() override;

        void TxTask() override;

        bool RegisterRxFrame(uint32_t id);

        uint32_t _lastUsedTxMb{0};
    };
}// namespace KeCommon::Bsw::Can

#endif /* CDD_CAN_IL_H_ */
