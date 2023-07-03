//
// Created by PC on 27.06.2023.
//

#ifndef CALIBRATIONCOMMAND_H
#define CALIBRATIONCOMMAND_H

#include "CommandId.h"

namespace KeCommon::Bsw::Calibration::Internal {
    class CalibrationCommand {

    public:
        CommandId sid;
        std::vector<uint8_t> payload;
    };
}
#endif //CALIBRATIONCOMMAND_H
