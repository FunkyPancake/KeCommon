//
// Created by PC on 27.06.2023.
//

#ifndef CANGPS_CALIBRATIONTASK_H
#define CANGPS_CALIBRATIONTASK_H

#include <utility>

#include "Calibration.h"
#include "CalibrationCommand.h"
#include "ICalTp.h"

namespace KeCommon::Bsw::Calibration {
    class CalibrationTask {
    private:
        ICalTp _tp;
    public:
        explicit CalibrationTask(ICalTp calTp): _tp(std::move(calTp))   {
        }

        void Task();

    };
}

#endif //CANGPS_CALIBRATIONTASK_H
