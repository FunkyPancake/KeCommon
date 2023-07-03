//
// Created by PC on 27.06.2023.
//

#include <cstdint>
#include "Calibration.h"
#include "../Bootloader/Bootloader.h"

uint8_t StartByte = 0x5a;
using namespace KeCommon::Bsw::Calibration::Internal;

void Calibration::ProcessCommand(const std::unique_ptr<Internal::CalibrationCommand> &command) {
    if (auto search = _commandMap.find(command->sid); search != _commandMap.end()) {
        auto handler = search->second;
        auto result = (this->*handler)(*command);
    }

}

bool Calibration::TryDequeueCmd(Internal::CalibrationCommand &command) {

}

Calibration::Calibration() : _commandMap({{CommandId::Connect, &Calibration::SessionControl},
                                                            {CommandId::Disconnect, &Calibration::ReadMemoryByAddress}
                                                           }), _connected(false) {

}

bool Calibration::SessionControl(const Internal::CalibrationCommand &command) {
    if(command.payload[0] == 0x01){
        _connected = true;
    }
    if(command.payload[0] == 0x02)
    {
        JumpToFbl();
    }
    return false;
}


bool Calibration::ReadMemoryByAddress(const CalibrationCommand &command) {
    _connected = false;
    return false;
}


void Calibration::SendNegativeResponse() {

}

CalibrationCommand Calibration::ParseCommand(const std::vector<int8_t> &vector1) {
    return CalibrationCommand();
}
