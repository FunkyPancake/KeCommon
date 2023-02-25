#include "Calibration.h"

using Utils::Calibration;
Calibration::Calibration()
    : _commandMapping{
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
                {Calibration::Commands::Sync, &Calibration::ProcessSync}
    }
{
}

std::vector<uint8_t> Calibration::ProcessConnect(std::vector<uint8_t> &command)
{
    return {};
}
std::vector<uint8_t> Calibration::ProcessCommand(std::vector<uint8_t> &command)
{
    if(_commandMapping.find(static_cast<Calibration::Commands>(command[0])) == _commandMapping.end()){
        ErrorHandler();
    }

    return {};
}
void Calibration::ErrorHandler()
{
}
std::vector<uint8_t> Utils::Calibration::ProcessDisconnect(std::vector<uint8_t> &command)
{
    return std::vector<uint8_t>();
}
std::vector<uint8_t> Utils::Calibration::ProcessReset(std::vector<uint8_t> &command)
{
    return std::vector<uint8_t>();
}
std::vector<uint8_t> Utils::Calibration::ProcessGetControlBlock(std::vector<uint8_t> &command)
{
    return std::vector<uint8_t>();
}
std::vector<uint8_t> Utils::Calibration::ProcessReadMemory(std::vector<uint8_t> &command)
{
    return std::vector<uint8_t>();
}
std::vector<uint8_t> Utils::Calibration::ProcessWriteMemory(std::vector<uint8_t> &command)
{
    return std::vector<uint8_t>();
}
std::vector<uint8_t> Utils::Calibration::ProcessProgram(std::vector<uint8_t> &command)
{
    return std::vector<uint8_t>();
}
std::vector<uint8_t> Utils::Calibration::ProcessConfigureCyclicRead(std::vector<uint8_t> &command)
{
    return std::vector<uint8_t>();
}
std::vector<uint8_t> Utils::Calibration::ProcessClearCyclicRead(std::vector<uint8_t> &command)
{
    return std::vector<uint8_t>();
}
std::vector<uint8_t> Utils::Calibration::ProcessJumpToFbl(std::vector<uint8_t> &command)
{
    return std::vector<uint8_t>();
}
std::vector<uint8_t> Utils::Calibration::ProcessEraseNvm(std::vector<uint8_t> &command)
{
    return std::vector<uint8_t>();
}
std::vector<uint8_t> Utils::Calibration::ProcessStartCyclicRead(std::vector<uint8_t> &command)
{
    return std::vector<uint8_t>();
}
std::vector<uint8_t> Utils::Calibration::ProcessStopCyclicRead(std::vector<uint8_t> &command)
{
    return std::vector<uint8_t>();
}
std::vector<uint8_t> Utils::Calibration::ProcessSync(std::vector<uint8_t> &command)
{
    return std::vector<uint8_t>();
}
