//
// Created by PC on 05.07.2023.
//

#ifndef ICALTP_H
#define ICALTP_H
namespace KeCommon::Bsw::Cal {
    class IDoxTp {
    public:
        virtual ~IDoxTp() = default;

        virtual bool TxRdy() = 0;

        virtual bool RxCmdAvailable() = 0;

        virtual bool WriteCommand(std::vector<uint8_t> data) = 0;

        virtual std::vector<uint8_t> ReadCommand() = 0;
    };
}
#endif //ICALTP_H
