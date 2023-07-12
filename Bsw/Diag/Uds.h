//
// Created by PC on 12.07.2023.
//

#ifndef UDS_H
#define UDS_H
#include "IDoXTp.h"
#include "SId.h"
#include "UdsErrorCode.h"
#include <map>
namespace KeCommon::Bsw::Diag
{

    class Uds
    {
    public:
        explicit Uds(const IDoXTp *iDoxTp);

        void MainFunction();

    private:
        IDoXTp *_doXTp;
        bool _connected;
        bool _busy;
        const std::map<Internal::SId, bool (Uds::*)(const std::vector<uint8_t> &)> _commandMap;

        bool SessionControl(const std::vector<uint8_t> &command);

        void SendNegativeResponse(Internal::SId sid, Internal::ResponseCode errorCode);
        void SendPosRespShortSubFunc(Internal::SId sid,uint8_t subFunc);
        //        Internal::CalibrationCommand ParseCommand(const std::vector<uint8_t> &vector);

        bool ReadMemoryByAddress(const std::vector<uint8_t> &command);
        bool ReadDataById(const std::vector<uint8_t> &command);

        //        void ProcessCommand(const std::vector<uint8_t> &command);
    };

}

#endif//UDS_H
