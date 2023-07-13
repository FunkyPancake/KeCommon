#include "Calibration.h"
#include <cstring>

using Utils::Calibration;
Calibration::Calibration()
    : responseBuffer(BufferLength),
      _commandMapping{
          {Calibration::Commands::Connect, &Calibration::ProcessConnect},
          {Calibration::Commands::Disconnect, &Calibration::ProcessDisconnect},
          {Calibration::Commands::Reset, &Calibration::ProcessReset},
          {Calibration::Commands::GetControlBlock, &Calibration::ProcessGetControlBlock},
          {Calibration::Commands::ReadMemory, &Calibration::ProcessReadMemory},
          {Calibration::Commands::WriteMemory, &Calibration::ProcessWriteMemory},
          {Calibration::Commands::Program, &Calibration::ProcessProgram},
          {Calibration::Commands::ConfigureCyclicRead, &Calibration::ProcessConfigureCyclicRead},
          {Calibration::Commands::StartCyclicRead, &Calibration::ProcessStartCyclicRead},
          {Calibration::Commands::StopCyclicRead, &Calibration::ProcessStopCyclicRead},
          {Calibration::Commands::ClearCyclicRead, &Calibration::ProcessClearCyclicRead},
          {Calibration::Commands::JumpToFbl, &Calibration::ProcessJumpToFbl},
          {Calibration::Commands::EraseNvm, &Calibration::ProcessEraseNvm},
          {Calibration::Commands::Sync, &Calibration::ProcessSync}}
{
}

void Calibration::ProcessConnect(std::vector<uint8_t> &command)
{

    ErrorHandler();
}
std::vector<uint8_t> &Calibration::ProcessCommand(std::vector<uint8_t> &command)
{
    auto iter = _commandMapping.find(static_cast<Calibration::Commands>(command[0]));
    if (iter == _commandMapping.end()) {
        ErrorHandler();
    }
    else {
        (*this.*(iter->second))(command);
    }
    return responseBuffer;
}
void Calibration::ErrorHandler()
{
    responseBuffer.resize(1);
    responseBuffer[0] = static_cast<uint8_t>(Resposnse::Negative);
}
void Utils::Calibration::ProcessDisconnect(std::vector<uint8_t> &command)
{
    ErrorHandler();
}
void Utils::Calibration::ProcessReset(std::vector<uint8_t> &command)
{
    ErrorHandler();
}
void Utils::Calibration::ProcessGetControlBlock(std::vector<uint8_t> &command)
{
    ErrorHandler();
}

void Utils::Calibration::ProcessReadMemory(std::vector<uint8_t> &command)
{
    if (command.size() != 7)
    {
        ErrorHandler();
        return;
    }
    uintptr_t address;
    uint16_t length;
    std::memcpy(&address, command.data() + 1, sizeof(uint8_t *));
    std::memcpy(&length, command.data() + 5, sizeof(uint16_t));
    if(length > BufferLength-1){
        ErrorHandler();
        return;
    }
    responseBuffer.resize(length + 1);
    responseBuffer[0] = static_cast<uint8_t>(Resposnse::Positive);
    std::memcpy(responseBuffer.data() + 1, reinterpret_cast<uint8_t *>(address), length);
}

void Utils::Calibration::ProcessWriteMemory(std::vector<uint8_t> &command)
{
    if (command.size() < 8)
    {
        ErrorHandler();
        return;
    }
    uintptr_t address;
    uint16_t length;
    std::memcpy(&address, command.data() + 1, sizeof(uint8_t *));
    std::memcpy(&length, command.data() + 5, sizeof(uint16_t));
    std::memcpy(reinterpret_cast<uint8_t *>(address),command.data() + 7,length);
    responseBuffer.resize(1);
    responseBuffer[0] = static_cast<uint8_t>(Resposnse::Positive);
}
void Utils::Calibration::ProcessProgram(std::vector<uint8_t> &command)
{
    ErrorHandler();
}
void Utils::Calibration::ProcessConfigureCyclicRead(std::vector<uint8_t> &command)
{
    ErrorHandler();
}
void Utils::Calibration::ProcessClearCyclicRead(std::vector<uint8_t> &command)
{
    ErrorHandler();
}
void Utils::Calibration::ProcessJumpToFbl(std::vector<uint8_t> &command)
{
    ErrorHandler();
}
void Utils::Calibration::ProcessEraseNvm(std::vector<uint8_t> &command)
{
    ErrorHandler();
}
void Utils::Calibration::ProcessStartCyclicRead(std::vector<uint8_t> &command)
{
    ErrorHandler();
}
void Utils::Calibration::ProcessStopCyclicRead(std::vector<uint8_t> &command)
{
    ErrorHandler();
}
void Utils::Calibration::ProcessSync(std::vector<uint8_t> &command)
{
    ErrorHandler();
}
