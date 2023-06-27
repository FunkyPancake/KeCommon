//
// Created by PC on 27.06.2023.
//

#include <cstdint>
#include "Calibration.h"

uint8_t StartByte=0x5a;
using namespace KeCommon::Bsw::Calibration::Internal;

void Calibration::ProcessCommand(CalibrationCommand command) {
    if (auto search = _commandMap.find(command.Id); search != _commandMap.end()) {
        auto handler = search->second;
        auto result = (this->*handler)(command);
    }

}

bool Calibration::TryDequeueCmd(CalibrationCommand &command) {
    if (_tp.RxQueue.Peek(1) == StartByte && _tp.RxQueue.Count > 6) {
        auto header = _tp.RxQueue.Dequeue(6);
        auto commandLen = header[3];
        auto payload = _tp.RxQueue.WaitDequeue(commandLen);
        auto calcCrc = CalcCrc(header, payload);
        auto crc = header[6];
        if (calcCrc != crc) {
            SendNegativeResponse();
            return false;
        }
        command = ParseCommand(payload);
        return true;
    }
    return false;
}

Calibration::Calibration(ICalTp tp) : _tp(tp), _commandMap({{CommandId::Connect,    &Calibration::ProcessConnect},
                                                            {CommandId::Disconnect, &Calibration::ProcessDisconnect}
                                                           }), _connected(false) {

}

bool Calibration::ProcessConnect(const Internal::CalibrationCommand &command) {
    _connected = true;
    return false;
}

bool Calibration::ProcessDisconnect(const CalibrationCommand &command) {
    _connected = false;
    return false;
}

void Calibration::SendNegativeResponse() {

}

uint16_t Calibration::CalcCrc(std::vector<uint8_t> vector1, std::vector<int8_t> vector2) {
    return 0;
}

CalibrationCommand Calibration::ParseCommand(const std::vector<int8_t> &vector1) {
    return CalibrationCommand();
}
