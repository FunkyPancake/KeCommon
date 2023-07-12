//
// Created by PC on 12.07.2023.
//

#ifndef UDSERRORCODE_H
#define UDSERRORCODE_H
namespace KeCommon::Bsw::Diag::Internal{
    enum class ResponseCode{
        Ok,
        GeneralReject = 0x10,
        ServiceNotSupported = 0x11,
        SubFunctionNotSupported = 0x12,
        IncorrectMessageLenOrFormat = 0x13
    };
}
#endif//UDSERRORCODE_H
