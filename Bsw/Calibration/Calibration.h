//
// Created by PC on 27.06.2023.
//

#ifndef CALIBRATION_H
#define CALIBRATION_H


#include <map>
#include <vector>
#include <memory>
#include "CalibrationCommand.h"
#include "CalibrationCommand.h"
#include "SId.h"
#include "IDoxTp.h"

namespace KeCommon::Bsw::Cal {
    class Calibration {
    public:
        explicit Calibration(const IDoxTp* iDoxTp);

        void CalTask();


    private:
        IDoxTp* doxTp;
        bool _connected;
        bool _busy;
        const std::map<Internal::SId, bool (Calibration::*)(const std::vector<uint8_t> &)> _commandMap;

        bool SessionControl(const std::vector<uint8_t> &command);

        void SendNegativeResponse();


//        Internal::CalibrationCommand ParseCommand(const std::vector<uint8_t> &vector);

        bool ReadMemoryByAddress(const std::vector<uint8_t> &command);

//        void ProcessCommand(const std::vector<uint8_t> &command);
    };
}


#endif //CALIBRATION_H
