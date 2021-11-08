
#include <stdlib.h>
#include "User_Slip.h"

#define USER_SLIP_BUFFERSIZE     (USER_UART_MAX_MESSAGE_LENGTH * 2U + 2)

static User_Slip_Buffer User_Slip_TxBuffer[USER_UART_NUMBEROFCHANNEL_USED];
static User_Slip_Buffer User_Slip_RxBuffer[USER_UART_NUMBEROFCHANNEL_USED];

void User_Slip_Init(uint8_t channelID)
{
    if(channelID < USER_UART_NUMBEROFCHANNEL_USED)
    {
        User_Slip_RxBuffer[channelID].DataLen = 0U;
        User_Slip_TxBuffer[channelID].DataLen = 0U;
        User_Slip_RxBuffer[channelID].DataSlipPtr =  (uint8_t *)calloc(USER_SLIP_BUFFERSIZE,sizeof(uint8_t));
        User_Slip_TxBuffer[channelID].DataSlipPtr =  (uint8_t *)calloc(USER_SLIP_BUFFERSIZE,sizeof(uint8_t));
    }
}

Std_ReturnType User_Slip_SendFrame(uint8_t u8ChannelId, uint8_t *u8DataSlipPtr, uint16_t length)
{
    uint8_t u8Val;
    uint16_t u16CountIdx;
    uint16_t u16LenCount;
    Std_ReturnType ret = E_NOT_OK;
    User_Slip_Buffer *TxBufPtr = &User_Slip_TxBuffer[u8ChannelId];

    if (USER_SLIP_BUFFERSIZE > length)
    {
        u16LenCount = 0;
        TxBufPtr->DataSlipPtr[u16LenCount] = USER_SLIP_END;
        u16LenCount++;

        for(u16CountIdx = 0; u16CountIdx < length; u16CountIdx ++)
        {
            u8Val = u8DataSlipPtr[u16CountIdx];
            switch(u8Val)
            {
                case USER_SLIP_END:
                    TxBufPtr->DataSlipPtr[u16LenCount] = USER_SLIP_ESC;
                    u16LenCount++;
                    TxBufPtr->DataSlipPtr[u16LenCount] = USER_SLIP_ESC_END;
                    u16LenCount++;
                    break;
                case USER_SLIP_ESC:
                    TxBufPtr->DataSlipPtr[u16LenCount] = USER_SLIP_ESC;
                    u16LenCount++;
                    TxBufPtr->DataSlipPtr[u16LenCount] = USER_SLIP_ESC_ESC;
                    u16LenCount++;
                    break;
                default:
                    TxBufPtr->DataSlipPtr[u16LenCount] = u8Val;
                    u16LenCount++;
                    break;
            }
        }
        TxBufPtr->DataSlipPtr[u16LenCount] = USER_SLIP_END;
        u16LenCount++;
        TxBufPtr->DataLen =  u16LenCount;

    	ret = User_Uart_Ipw_AsyncTransmit(u8ChannelId,TxBufPtr->DataSlipPtr,TxBufPtr->DataLen);
        return ret;
    }

    return ret;
}

uint8_t User_Slip_CreateSlipBuffer(User_Slip_Buffer *RxBufPtr, uint8_t data)
{
    static uint8_t u8Data_Last = 0U;
    
    switch(data)
    {
        case USER_SLIP_ESC:
            u8Data_Last = USER_SLIP_ESC;
            break;
        case USER_SLIP_END:
            return USER_SLIP_END_FRAME;
            break;
        default:
            if(u8Data_Last == USER_SLIP_ESC)
            {
                u8Data_Last = 0U;
                if(data == USER_SLIP_ESC_END)
                {
                    RxBufPtr->DataSlipPtr[RxBufPtr->DataLen] = USER_SLIP_END;
                    RxBufPtr->DataLen++;
                }
                else if(data == USER_SLIP_ESC_ESC)
                {
                    RxBufPtr->DataSlipPtr[RxBufPtr->DataLen] = USER_SLIP_ESC;
                    RxBufPtr->DataLen++;
                }
                else
                {
                    RxBufPtr->DataLen = 0U;
                    return USER_SLIP_ERROR_FRAME;
                }
            }
            else
            {
                RxBufPtr->DataSlipPtr[RxBufPtr->DataLen] = data;
                RxBufPtr->DataLen++;
            }
            break;
    }

    if (RxBufPtr->DataLen > USER_SLIP_BUFFERSIZE)
    {
        RxBufPtr->DataLen = 0U;
    }

    return USER_SLIP_NOT_END_FRAME;
}

Std_ReturnType User_Slip_ReceivedFrame (uint8_t u8ChannelId, uint8_t* DataSlipPtr, uint16_t* DataLenPtr)
{
    uint16_t u16Count;
    uint8_t u8TmpData;
    uint8_t u8SlipReturn;
    uint16_t u16ProtectCount = 0U;
    Std_ReturnType RetVal = E_NOT_OK;
    *DataLenPtr = 0U;

    /* Check frame Slip */
    if(E_NOT_OK == User_Ip_Ipw_FindByte(u8ChannelId,USER_SLIP_END,&u16ProtectCount))
    {
    	return E_NOT_OK;
    }

    if(u16ProtectCount > USER_SLIP_BUFFERSIZE)
    {
    	User_Ip_Ipw_DeleteRxData(u8ChannelId,u16ProtectCount);
    	return E_NOT_OK;
    }

    while(E_OK == User_Ip_Ipw_ReadByte(u8ChannelId, &u8TmpData))
    {
    	u8SlipReturn = User_Slip_CreateSlipBuffer(&User_Slip_RxBuffer[u8ChannelId], u8TmpData);
        if (USER_SLIP_END_FRAME == u8SlipReturn)
        {
            if (0U < User_Slip_RxBuffer[u8ChannelId].DataLen)
            {
                for (u16Count = 0U; u16Count < User_Slip_RxBuffer[u8ChannelId].DataLen; u16Count++)
                {
                    DataSlipPtr[u16Count] = User_Slip_RxBuffer[u8ChannelId].DataSlipPtr[u16Count];
                }
                *DataLenPtr = User_Slip_RxBuffer[u8ChannelId].DataLen;
                User_Slip_RxBuffer[u8ChannelId].DataLen = 0U;
                RetVal = E_OK;
                break;
            }
        }
        else if(USER_SLIP_ERROR_FRAME == u8SlipReturn)
        {
        	User_Slip_RxBuffer[u8ChannelId].DataLen = 0U;
        	return E_NOT_OK;
        }
        else
        {

        }
    }

    return(RetVal);
}
