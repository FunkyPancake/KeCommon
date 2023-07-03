//
// Created by PC on 27.06.2023.
//

#ifndef CALIBRATION_H
#define CALIBRATION_H


#include <map>
#include <vector>
#include <memory>
#include "CalibrationCommand.h"
#include "CalibrationTask.h"
#include "CalibrationCommand.h"
#include "CommandId.h"

namespace KeCommon::Bsw::Calibration::Internal {
    class Calibration {
    public:
        explicit Calibration();

        bool TryDequeueCmd(Internal::CalibrationCommand &command);

        void ProcessCommand(const std::unique_ptr<Internal::CalibrationCommand>& command);


    private:

        const std::map<Internal::CommandId, bool (Calibration::*)(const Internal::CalibrationCommand &)> _commandMap;

        bool SessionControl(const Internal::CalibrationCommand &command);

        bool _connected;

        void SendNegativeResponse();

        uint16_t CalcCrc(std::vector<uint8_t> header, std::vector<int8_t> payload);

        CalibrationCommand ParseCommand(const std::vector<int8_t> &vector1);

        bool ReadMemoryByAddress(const CalibrationCommand &command);
    };
}


#endif //CALIBRATION_H
