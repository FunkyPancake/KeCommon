/*
 * can_il.h
 *
 *  Created on: 14 gru 2020
 *      Author: Mati
 */

#ifndef CDD_CAN_IL_H_
#define CDD_CAN_IL_H_
#include "ICan.h"
#include "fsl_flexcan.h"

class FlexCan : public ICan
{
    CAN_Type *can_base;

  public:
    FlexCan();
    void ConfigRxMailbox(uint32_t id,uint8_t mb_id);
    bool Send(uint32_t id, Payload &data, uint8_t dlc) override;
    bool Receive(uint32_t *id, Payload *data, uint8_t dlc);

  private:
    void WritePayloadRegisters(flexcan_frame_t* frame, uint8_t * data, uint8_t dlc);
};

#endif /* CDD_CAN_IL_H_ */
