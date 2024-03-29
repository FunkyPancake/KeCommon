//
// Created by PC on 12.07.2023.
//

#include "Uds.h"
extern "C" {
#include "../Bootloader/Bootloader.h"
};

#include "UdsErrorCode.h"
using namespace Diag;
using namespace Diag::Internal;

Uds::Uds(const IDoXTp *iDoxTp)
    : _doXTp(const_cast<IDoXTp *>(iDoxTp)), _connected(false), _busy(false),
      _commandMap(
          {{Internal::SId::SessionControl, &Uds::SessionControl},
           {Internal::SId::ReadMemoryByAddr, &Uds::ReadMemoryByAddress},
           {Internal::SId::ReadDataById, &Uds::ReadDataById}})
{
}

bool Uds::SessionControl(const std::vector<uint8_t> &command)
{
    if (command.size() != 2) {
        SendNegativeResponse(static_cast<SId>(command[0]), Internal::ResponseCode::IncorrectMessageLenOrFormat);
        return false;
    }
    switch (const auto subFun = command[1]) {
        case 0x01:
            //TODO: Set session to basic session
            SendPosRespShortSubFunc(static_cast<SId>(command[0]), subFun);
            break;
        case 0x02:
            //ToDo JumpToFbl
            JumpToBootloader();
            break;
        case 0x03:
        default:
            SendNegativeResponse(static_cast<SId>(command[0]), Internal::ResponseCode::SubFunctionNotSupported);
            break;
    }
    return true;
}

bool Uds::ReadMemoryByAddress(const std::vector<uint8_t> &command)
{
    SendNegativeResponse(static_cast<SId>(command[0]), Internal::ResponseCode::ServiceNotSupported);
    return false;
}

void Uds::SendNegativeResponse(Internal::SId sid, Internal::ResponseCode errorCode)
{
    std::vector<uint8_t> nrc{0x7F, static_cast<uint8_t>(sid), static_cast<uint8_t>(errorCode)};
    _doXTp->Write(nrc);
}
//
//UdsCommand Uds::ParseCommand(const std::vector<int8_t> &vector1) {
//    return UdsCommand();
//}

void Uds::MainFunction()
{
    if (!_busy) {
        if (_doXTp->RxRdy()) {
            auto command = _doXTp->Read();
            const auto sid = static_cast<SId>(command[0]);
            if (const auto search = _commandMap.find(sid); search != _commandMap.end()) {
                const auto handler = search->second;
                (this->*handler)(command);
            }
            else {
                SendNegativeResponse(sid, Internal::ResponseCode::ServiceNotSupported);
            }
        }
    }
}

void Uds::SendPosRespShortSubFunc(Internal::SId sid, uint8_t subFunc)
{
    std::vector<uint8_t> response{
        static_cast<uint8_t>(static_cast<uint8_t>(sid) + PositiveResponseSidOffset), subFunc};
    _doXTp->Write(response);
}

bool Uds::ReadDataById(const std::vector<uint8_t> &command)
{
    if (command.size() != 3) {
        SendNegativeResponse(static_cast<SId>(command[0]), Internal::ResponseCode::IncorrectMessageLenOrFormat);
        return false;
    }
    auto id = static_cast<ReadMemoryByIdIdents>((command[1] << 8) + command[2]);
    std::vector<uint8_t> response{50};
    switch (id) {
        case ReadMemoryByIdIdents::SoftwareVersion:
            response = {static_cast<uint8_t>(command[0] + PositiveResponseSidOffset), command[1], command[2], 1, 0, 0};
            _doXTp->Write(response);
            break;
        default:
            SendNegativeResponse(static_cast<SId>(command[0]), Internal::ResponseCode::SubFunctionNotSupported);
            break;
    }
    return false;
}
