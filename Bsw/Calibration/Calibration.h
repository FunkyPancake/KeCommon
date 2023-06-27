//
// Created by PC on 27.06.2023.
//

#ifndef CANGPS_CALIBRATION_H
#define CANGPS_CALIBRATION_H


#include <map>
#include <vector>
#include "CalibrationCommand.h"
#include "CalibrationTask.h"
#include "CalibrationCommand.h"
#include "CommandId.h"
#include "ICalTp.h"

namespace KeCommon::Bsw::Calibration::Internal {
    class Calibration {
    public:
        explicit Calibration(ICalTp tp);

        bool TryDequeueCmd(Internal::CalibrationCommand &command);

        void ProcessCommand(Internal::CalibrationCommand command);


    private:
        ICalTp _tp;

        const std::map<Internal::CommandId, bool (Calibration::*)(const Internal::CalibrationCommand &)> _commandMap;

        bool ProcessConnect(const Internal::CalibrationCommand &command);

        bool ProcessDisconnect(const Internal::CalibrationCommand &command);

        bool _connected;

        void SendNegativeResponse();

        uint16_t CalcCrc(std::vector<uint8_t> vector1, std::vector<int8_t> vector2);

        CalibrationCommand ParseCommand(const std::vector<int8_t> &vector1);
    };
}


#endif //CANGPS_CALIBRATION_H
