#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <cstdint>
#include <unordered_map>
#include <vector>
namespace Utils
{
    class Calibration
    {
    private:
        std::vector<uint8_t> responseBuffer;
        enum class Commands
        {
            Connect,
            Disconnect,
            Reset,
            GetControlBlock,
            ReadMemory,
            WriteMemory,
            Program,
            ConfigureCyclicRead,
            StartCyclicRead,
            StopCyclicRead,
            ClearCyclicRead,
            JumpToFbl,
            Sync,
            EraseNvm
        };
        typedef std::vector<uint8_t> (Calibration::*CommandProcessor)(std::vector<uint8_t> &command);
        std::vector<uint8_t> ProcessDisconnect(std::vector<uint8_t> &command);
        std::vector<uint8_t> ProcessReset(std::vector<uint8_t> &command);
        std::vector<uint8_t> ProcessGetControlBlock(std::vector<uint8_t> &command);
        std::vector<uint8_t> ProcessReadMemory(std::vector<uint8_t> &command);
        std::vector<uint8_t> ProcessWriteMemory(std::vector<uint8_t> &command);
        std::vector<uint8_t> ProcessProgram(std::vector<uint8_t> &command);
        std::vector<uint8_t> ProcessConfigureCyclicRead(std::vector<uint8_t> &command);
        std::vector<uint8_t> ProcessClearCyclicRead(std::vector<uint8_t> &command);
        std::vector<uint8_t> ProcessJumpToFbl(std::vector<uint8_t> &command);
        std::vector<uint8_t> ProcessEraseNvm(std::vector<uint8_t> &command);
        std::vector<uint8_t> ProcessStartCyclicRead(std::vector<uint8_t> &command);
        std::vector<uint8_t> ProcessStopCyclicRead(std::vector<uint8_t> &command);
        std::vector<uint8_t> ProcessConnect(std::vector<uint8_t> &command);
        std::vector<uint8_t> ProcessSync(std::vector<uint8_t> &command);
        const std::unordered_map<Commands, CommandProcessor> _commandMapping;
        void ErrorHandler();

    public:
        std::vector<uint8_t> ProcessCommand(std::vector<uint8_t> &command);
        Calibration();
    };
}// namespace Utils
#endif//CALIBRATION_H
