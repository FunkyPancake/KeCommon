//
// Created by PC on 27.06.2023.
//

#include "CalibrationTask.h"

void KeCommon::Bsw::Calibration::CalibrationTask::Task() {
    auto _cal = Calibration::Internal::Calibration(_tp);
    Internal::CalibrationCommand command{};
    if (_cal.TryDequeueCmd(command)) {
        _cal.ProcessCommand(command);
    }


}
