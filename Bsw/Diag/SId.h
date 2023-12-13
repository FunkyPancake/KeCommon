//
// Created by PC on 27.06.2023.
//

#ifndef COMMANDID_H
#define COMMANDID_H
namespace Diag::Internal
{
    enum class SId
    {
        SessionControl = 0x10,
        EcuReset = 0x11,
        SecurityAccess = 0x27,
        CommunicationControl = 0x28,
        TesterPresent = 0x3e,
        ReadDataById = 0x22,
        ReadMemoryByAddr = 0x23,
        WriteDataById = 0x2E,
        WriteDataByAddr = 0x3D,
        RoutineControl = 0x31,
    };
    const uint8_t PositiveResponseSidOffset = 0x40;

    enum class ReadMemoryByIdIdents{
        SoftwareVersion = 0x1010,
    };
}// namespace Diag::Internal
#endif//COMMANDID_H
