/*
 * can_il.cpp
 *
 *  Created on: 14 gru 2020
 *      Author: Mati
 */
#include "FlexCan.h"

void FlexCan::ConfigRxMailbox(uint32_t id, uint8_t mb_id)
{
    _flexcan_rx_mb_config config = {.type = kFLEXCAN_FrameTypeData};
    if ((id & CAN_ID_EX_BIT) == CAN_ID_EX_BIT)
    {
        config.id = FLEXCAN_ID_EXT(id);
        config.format = kFLEXCAN_FrameFormatExtend;
    }
    else
    {
        config.id = FLEXCAN_ID_STD(id);
        config.format = kFLEXCAN_FrameFormatExtend;
    }
    FLEXCAN_SetRxMbConfig(can_base, mb_id, &config, true);
}

bool FlexCan::Send(uint32_t id, Payload &data, uint8_t dlc)
{
    flexcan_frame_t frame = {.type = kFLEXCAN_FrameTypeData};
    frame.length = dlc;

    if ((id & CAN_ID_EX_BIT) == CAN_ID_EX_BIT)
    {
        frame.id = id & 0x7fffffff;
        frame.format = kFLEXCAN_FrameFormatExtend;
    }
    else
    {
        frame.id = (id & 0x7ff) << 18;
        frame.format = kFLEXCAN_FrameFormatStandard;
    }
    WritePayloadRegisters(&frame, (uint8_t *) data.b, dlc);
    for (int i = 0; i < mailboxCount; i++)
    {
        if (FLEXCAN_WriteTxMb(can_base, i, &frame) == kStatus_Success)
        {
            return true;
        }
    }
    return false;
}

bool FlexCan::Receive(uint32_t *id, Payload *data, uint8_t dlc)
{
    //    flexcan_frame_t frame;
    //    // FLEXCAN_ReadRxMb();
    //    if(frame.format == kFLEXCAN_FrameFormatExtend)
    //    {
    //    }
    //    else
    //    {
    //    }
    return false;
}

void FlexCan::WritePayloadRegisters(flexcan_frame_t *frame, uint8_t *data, uint8_t dlc)
{
    uint8_t *reg[8] = {
        &frame->dataByte0,
        &frame->dataByte1,
        &frame->dataByte2,
        &frame->dataByte3,
        &frame->dataByte4,
        &frame->dataByte5,
        &frame->dataByte6,
        &frame->dataByte7};

    for (int i = 0; i < dlc; i++)
    {
        *reg[i] = data[i];
    }
}

FlexCan::FlexCan(int mailboxCount)
{
    can_base = CAN0;// only single CAN periph is in this MCU family
    this->mailboxCount = mailboxCount;
}
