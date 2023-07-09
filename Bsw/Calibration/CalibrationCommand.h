//
// Created by PC on 27.06.2023.
//

#ifndef CALIBRATIONCOMMAND_H
#define CALIBRATIONCOMMAND_H

#include "SId.h"

namespace KeCommon::Bsw::Cal::Internal {
    class CalibrationCommand {

    public:
        SId sid;
        std::vector<uint8_t> payload;
    };
}
#endif //CALIBRATIONCOMMAND_H
