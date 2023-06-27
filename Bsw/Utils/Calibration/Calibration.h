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
        inline static constexpr uint16_t BufferLength=256;
        std::vector<uint8_t> responseBuffer;
        enum class Resposnse{
            Positive = 0xFF,
            Negative = 0xFE
        };
        enum class Commands
        {
            Connect = 0xff,
            Disconnect =0xfe,
            Sync = 0xfc,
            Reset,
            GetControlBlock,
            ReadMemory = 0xf5,
            WriteMemory=0xf0,
            Program,
            ConfigureCyclicRead,
            StartCyclicRead,
            StopCyclicRead,
            ClearCyclicRead,
            JumpToFbl,
            EraseNvm
        };
        typedef void (Calibration::*CommandProcessor)(std::vector<uint8_t> &command);
        const std::unordered_map<Commands, CommandProcessor> _commandMapping;
        void ProcessDisconnect(std::vector<uint8_t> &command);
        void ProcessReset(std::vector<uint8_t> &command);
        void ProcessGetControlBlock(std::vector<uint8_t> &command);
        void ProcessReadMemory(std::vector<uint8_t> &command);
        void ProcessWriteMemory(std::vector<uint8_t> &command);
        void ProcessProgram(std::vector<uint8_t> &command);
        void ProcessConfigureCyclicRead(std::vector<uint8_t> &command);
        void ProcessClearCyclicRead(std::vector<uint8_t> &command);
        void ProcessJumpToFbl(std::vector<uint8_t> &command);
        void ProcessEraseNvm(std::vector<uint8_t> &command);
        void ProcessStartCyclicRead(std::vector<uint8_t> &command);
        void ProcessStopCyclicRead(std::vector<uint8_t> &command);
        void ProcessConnect(std::vector<uint8_t> &command);
        void ProcessSync(std::vector<uint8_t> &command);
        void ErrorHandler();

    public:
        std::vector<uint8_t>& ProcessCommand(std::vector<uint8_t> &command);
        Calibration();
    };
}// namespace Utils
#endif//CALIBRATION_H
