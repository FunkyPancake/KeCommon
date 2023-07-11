//
// Created by PC on 27.06.2023.
//

#include <cstdint>
#include "Calibration.h"
#include "../Bootloader/Bootloader.h"

uint8_t StartByte = 0x5a;
using namespace KeCommon::Bsw::Cal;
using namespace KeCommon::Bsw::Cal::Internal;


Calibration::Calibration(const IDoxTp *iDoxTp) : doxTp(const_cast<IDoxTp *>(iDoxTp)), _connected(false), _busy(false),
                                                 _commandMap(
                                                         {{Internal::SId::Connect,    &Calibration::SessionControl},
                                                          {Internal::SId::Disconnect, &Calibration::ReadMemoryByAddress}
                                                         }) {

}

bool Calibration::SessionControl(const std::vector<uint8_t> &command) {
    auto subFun = command[0];
    if (subFun == 0x01) {
        _connected = true;
    }
    if (subFun == 0x02) {
//        JumpToFbl();
    }
    return false;
}


bool Calibration::ReadMemoryByAddress(const std::vector<uint8_t> &command) {
    _connected = false;
    return false;
}


void Calibration::SendNegativeResponse() {

}
//
//CalibrationCommand Calibration::ParseCommand(const std::vector<int8_t> &vector1) {
//    return CalibrationCommand();
//}

void Calibration::CalTask() {
    if (!_busy) {
        if (doxTp->RxCmdAvailable()) {
            auto command = doxTp->ReadCommand();
            auto sid = (Internal::SId) command[0];
            if (auto search = _commandMap.find(sid); search != _commandMap.end()) {
                auto handler = search->second;
                auto result = (this->*handler)(command);
            }
        }
    }

}


