/**
 * \file            ShrinkCommunicationController.c
 * \brief           ShrinkCommunicationController source file
 */

/*
 * Copyright (c) 2022 DURULSAN R&D
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of ShrinkCommunicationController.
 *
 * Author:          DURULSAN R&D <info@durulsan.com>
 * Version:         v1.0.0
 */


#include <xc.h>
#include "main.h"


shrinkCommunicationTypeDef_t shrinkCommunicationTypeDef;

/*---------------------------------------------------------------------------------------------------------
 * \fn              CalculateCRC
 * \brief           Calculating CRC with given rx or tx buffer.
 * \param[in]       a: Rx or Tx Buffer
 * \param[in]       b: none
 * \return          Calculated CRC.
 ---------------------------------------------------------------------------------------------------------*/
uint8_t CalculateCRC(uint8_t *pBuffer)
{
    // CRC = (dataH - dataL) + cmd + prm;

    uint8_t tmp = 0;   // Local variable for CRC calculation.

    // Was compared of two data variable for find which one is bigger.
    if(*(pBuffer + DATAH_BUFFER_ADR) > *(pBuffer + DATAL_BUFFER_ADR))
    {
        tmp = *(pBuffer + DATAH_BUFFER_ADR) - *(pBuffer + DATAL_BUFFER_ADR);
    }
    else
    {
        tmp = *(pBuffer + DATAL_BUFFER_ADR) - *(pBuffer + DATAH_BUFFER_ADR);
    }

    tmp += *(pBuffer + CMD_BUFFER_ADR);

    tmp += *(pBuffer + PRM_BUFFER_ADR);

    return tmp;
}


/*---------------------------------------------------------------------------------------------------------
 * \fn              ShrinkCommunicationRead
 * \brief           Read datas with ESUART read function and fill rx buffer. Control of matching calculated CRC 
 *                  and received CRC.
 * \param[in]       a: none
 * \param[in]       b: none
 * \return          OK or NOK. If calculated CRC and received CRC is matched it should be return OK, otherwise
 *                  return NOK.
 ---------------------------------------------------------------------------------------------------------*/
uint8_t ShrinkCommunicationRead(void)
{
    if(shrinkCommunicationTypeDef.rxIsOk == RESET)          // When buffer is filled rxIsOk flag will set.
    {
        shrinkCommunicationTypeDef.rxCommandBuffer[shrinkCommunicationTypeDef.rxCommandBufferIndex] = EUSART1_Read();

        if(IncreaseRxCommandBufferIndex() == OK)
        {
            shrinkCommunicationTypeDef.rxIsOk = SET;
        }
    }
    
    else
    {
        if(shrinkCommunicationTypeDef.rxCommandBuffer[CMD_BUFFER_ADR] < sh_cmd_ACK)
        {
            // CRC = (dataH - dataL) + cmd + prm

            shrinkCommunicationTypeDef.crc = CalculateCRC(&shrinkCommunicationTypeDef.rxCommandBuffer[0]);

            if(shrinkCommunicationTypeDef.crc == shrinkCommunicationTypeDef.rxCommandBuffer[CRC_BUFFER_ADR])
            {
                return OK;
            }

            else
            {
                return NOK;
            }
        }
        
        else
        {
            return OK;
        }
    }
}


/*---------------------------------------------------------------------------------------------------------
 * \fn              ShrinkCommunicationSendACK
 * \brief           If data received, send a OK message. 
 * \param[in]       a: none
 * \param[in]       b: none
 * \return          none.
 ---------------------------------------------------------------------------------------------------------*/
void ShrinkCommunicationSendACK(void)
{
    shrinkCommunicationTypeDef.txCommandBuffer[0] = START_BIT;
    shrinkCommunicationTypeDef.txCommandBuffer[CMD_BUFFER_ADR] = sh_cmd_ACK;
    shrinkCommunicationTypeDef.txCommandBuffer[PRM_BUFFER_ADR] = 65;    //'A';
    shrinkCommunicationTypeDef.txCommandBuffer[DATAL_BUFFER_ADR] = 67;  //'C';
    shrinkCommunicationTypeDef.txCommandBuffer[DATAH_BUFFER_ADR] = 75;  //'K';
    shrinkCommunicationTypeDef.txCommandBuffer[CRC_BUFFER_ADR] = 0;
    shrinkCommunicationTypeDef.txCommandBuffer[COMMAND_BUFFER_LENGTH - 1] =  STOP_BIT;

    for(uint8_t i = 0; i < COMMAND_BUFFER_LENGTH; i++)
    {
        EUSART1_Write(shrinkCommunicationTypeDef.txCommandBuffer[i]);
        __delay_ms(1);
    }
}


/*---------------------------------------------------------------------------------------------------------
 * \fn              ShrinkCommunicationTransmit
 * \brief           Transmit datas 
 * \param[in]       a: Command     @CommunicationCommand
 * \param[in]       b: Parameter   @CommunicationParameters or @CommunicationTimingParameters
 * \param[in]       c: Value
 * \return          OK or NOK. If transmit is occured return OK, otherwise NOK.
 ---------------------------------------------------------------------------------------------------------*/
uint8_t ShrinkCommunicationTransmit(uint8_t cmd, uint8_t prm, unsigned int value)
{
    TUInt tmp;
    
    tmp.Int = value;
    
    shrinkCommunicationTypeDef.txCommandBuffer[0] = START_BIT;
    shrinkCommunicationTypeDef.txCommandBuffer[CMD_BUFFER_ADR] = cmd;
    shrinkCommunicationTypeDef.txCommandBuffer[PRM_BUFFER_ADR] = prm;
    shrinkCommunicationTypeDef.txCommandBuffer[DATAL_BUFFER_ADR] = tmp.Char[0];
    shrinkCommunicationTypeDef.txCommandBuffer[DATAH_BUFFER_ADR] = tmp.Char[1];
    shrinkCommunicationTypeDef.txCommandBuffer[CRC_BUFFER_ADR] = CalculateCRC(&shrinkCommunicationTypeDef.txCommandBuffer[0]);
    shrinkCommunicationTypeDef.txCommandBuffer[COMMAND_BUFFER_LENGTH - 1] =  STOP_BIT;
    
    if(shrinkCommunicationTypeDef.txIsOk == RESET)
    {
        for(uint8_t i = 0; i < COMMAND_BUFFER_LENGTH; i++)
        {
            EUSART1_Write(shrinkCommunicationTypeDef.txCommandBuffer[i]);
            __delay_ms(1);
        }
        
        shrinkCommunicationTypeDef.txIsOk = SET;
    }
    
    return shrinkCommunicationTypeDef.txIsOk;
}


/*---------------------------------------------------------------------------------------------------------
 * \fn              IncreaseRxCommandBufferIndex
 * \brief           Increase buffer index.
 * \param[in]       a: none
 * \param[in]       b: none
 * \return          OK or NOK. If index reach final value it should be return OK, otherwise return NOK.
 ---------------------------------------------------------------------------------------------------------*/
uint8_t IncreaseRxCommandBufferIndex(void)
{
    if((++shrinkCommunicationTypeDef.rxCommandBufferIndex) >= COMMAND_BUFFER_LENGTH)
    {
        shrinkCommunicationTypeDef.rxCommandBufferIndex = 0;
        
        return OK;
    }
    else
    {
        return NOK;
    }
}


/*---------------------------------------------------------------------------------------------------------
 * \fn              IncreaseTxCommandBufferIndex
 * \brief           Increase buffer index.
 * \param[in]       a: none
 * \param[in]       b: none
 * \return          OK or NOK. If index reach final value it should be return OK, otherwise return NOK.
 ---------------------------------------------------------------------------------------------------------*/
uint8_t IncreaseTxCommandBufferIndex(void)
{
    if((++shrinkCommunicationTypeDef.txCommandBufferIndex) >= COMMAND_BUFFER_LENGTH)
    {
        shrinkCommunicationTypeDef.txCommandBufferIndex = 0;
        
        return OK;
    }
    else
    {
        return NOK;
    }
}

/*---------------------------------------------------------------------------------------------------------
 * \fn              ParamSendMessage
 * \brief           -
 * \param[in]       a: none
 * \param[in]       b: none
 * \return          none
 ---------------------------------------------------------------------------------------------------------*/
void ParamSendMessage (void)
{
    GLCD.Font= GLCD_FONT5x7;
    GLCD.Invert= 0;
    //GotoXYGLCD (2,6);
    putrsXYGLCD (2,6,Texts[Parameters.LanguageSel].ParamSendMessage);
    __delay_ms(1000);
}

/*---------------------------------------------------------------------------------------------------------
 * \fn              MessageIsOk
 * \brief           -
 * \param[in]       a: none
 * \param[in]       b: none
 * \return          none
 ---------------------------------------------------------------------------------------------------------*/
void MessageIsOk (void)
{
    GLCD.Font= GLCD_FONT5x7;
    GLCD.Invert= 0;
    //GotoXYGLCD (2,6);
    putrsXYGLCD (40,6,Texts[Parameters.LanguageSel].MessageIsOk);
    __delay_ms(1000);
}


/*---------------------------------------------------------------------------------------------------------
 * \fn              MessageIsNok
 * \brief           -
 * \param[in]       a: none
 * \param[in]       b: none
 * \return          none
 ---------------------------------------------------------------------------------------------------------*/
void MessageIsNok (void)
{
    GLCD.Font= GLCD_FONT5x7;
    GLCD.Invert= 0;
    //GotoXYGLCD (2,6);
    putrsXYGLCD (40,6,Texts[Parameters.LanguageSel].MessageIsNok);
    __delay_ms(1000);
}



/*---------------------------------------------------------------------------------------------------------
 * \fn              ShrinkCommunicationProtocol
 * \brief           Enter the system initial values. 
 * \param[in]       a: none
 * \param[in]       b: none
 * \return          none
 ---------------------------------------------------------------------------------------------------------*/
void ShrinkCommunicationProtocol(void)
{
    TUInt value;
    
    if(ShrinkCommunicationRead() == OK)
    {
        if((shrinkCommunicationTypeDef.rxCommandBuffer[0] == START_BIT) && (shrinkCommunicationTypeDef.rxCommandBuffer[COMMAND_BUFFER_LENGTH - 1] == STOP_BIT))
        {
            if(shrinkCommunicationTypeDef.shrinkCommunicationStatus == sh_st_READ)
            {
                shrinkCommunicationTypeDef.shrinkCommunicationCommand = shrinkCommunicationTypeDef.rxCommandBuffer[CMD_BUFFER_ADR];

    /*-------------------------------- cmd_TIMINGS --------------------------------*/

                if(shrinkCommunicationTypeDef.shrinkCommunicationCommand == sh_cmd_TIMINGS)
                {
                    shrinkCommunicationTypeDef.shrinkCommunicationTimingParameters = shrinkCommunicationTypeDef.rxCommandBuffer[PRM_BUFFER_ADR];

                    switch(shrinkCommunicationTypeDef.shrinkCommunicationTimingParameters)
                    {
                        case sh_prm_T1:
                            value.Char[0] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAL_BUFFER_ADR];
                            value.Char[1] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAH_BUFFER_ADR];

                            Parameters.T1 = value.Int;

                            shrinkCommunicationTypeDef.rxIsOk = RESET;

                            ShrinkCommunicationSendACK();

                            break;

                        case sh_prm_T2:
                            value.Char[0] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAL_BUFFER_ADR];
                            value.Char[1] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAH_BUFFER_ADR];

                            Parameters.T2 = value.Int;

                            shrinkCommunicationTypeDef.rxIsOk = RESET;

                            ShrinkCommunicationSendACK();

                            break;

                        case sh_prm_T3:
                            value.Char[0] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAL_BUFFER_ADR];
                            value.Char[1] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAH_BUFFER_ADR];

                            Parameters.T3 = value.Int;

                            shrinkCommunicationTypeDef.rxIsOk = RESET;

                            ShrinkCommunicationSendACK();

                            break;

                        case sh_prm_T4:
                            value.Char[0] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAL_BUFFER_ADR];
                            value.Char[1] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAH_BUFFER_ADR];

                            Parameters.T4 = value.Int;

                            shrinkCommunicationTypeDef.rxIsOk = RESET;

                            ShrinkCommunicationSendACK();

                            break;

                        case sh_prm_T5:
                            value.Char[0] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAL_BUFFER_ADR];
                            value.Char[1] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAH_BUFFER_ADR];

                            Parameters.T5 = value.Int;

                            shrinkCommunicationTypeDef.rxIsOk = RESET;

                            ShrinkCommunicationSendACK();

                            break;

                        case sh_prm_T6:
                            value.Char[0] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAL_BUFFER_ADR];
                            value.Char[1] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAH_BUFFER_ADR];

                            Parameters.T6 = value.Int;

                            shrinkCommunicationTypeDef.rxIsOk = RESET;

                            ShrinkCommunicationSendACK();

                            break;

                        case sh_prm_T7:
                            value.Char[0] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAL_BUFFER_ADR];
                            value.Char[1] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAH_BUFFER_ADR];

                            Parameters.T7 = value.Int;

                            shrinkCommunicationTypeDef.rxIsOk = RESET;

                            ShrinkCommunicationSendACK();

                            break;

                        case sh_prm_T8:
                            value.Char[0] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAL_BUFFER_ADR];
                            value.Char[1] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAH_BUFFER_ADR];

                            Parameters.T8 = value.Int;

                            shrinkCommunicationTypeDef.rxIsOk = RESET;

                            ShrinkCommunicationSendACK();

                            break;

                        case sh_prm_T9:
                            value.Char[0] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAL_BUFFER_ADR];
                            value.Char[1] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAH_BUFFER_ADR];

                            Parameters.T9 = value.Int;

                            shrinkCommunicationTypeDef.rxIsOk = RESET;

                            ShrinkCommunicationSendACK();

                            break;

                        case sh_prm_T10:
                            value.Char[0] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAL_BUFFER_ADR];
                            value.Char[1] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAH_BUFFER_ADR];

                            Parameters.T10 = value.Int;

                            shrinkCommunicationTypeDef.rxIsOk = RESET;

                            ShrinkCommunicationSendACK();

                            break;

                        default:
                            break;
                    }
                }

    /*-------------------------------- cmd_SET --------------------------------*/

                if(shrinkCommunicationTypeDef.shrinkCommunicationCommand == sh_cmd_SET)
                {
                    shrinkCommunicationTypeDef.shrinkCommunicationParameters = shrinkCommunicationTypeDef.rxCommandBuffer[PRM_BUFFER_ADR];

                    switch(shrinkCommunicationTypeDef.shrinkCommunicationParameters)
                    {
                        case sh_prm_COUNTERS:
                            value.Char[0] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAL_BUFFER_ADR];
                            value.Char[1] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAH_BUFFER_ADR];

                            Parameters.TodayCount = value.Int;

                            shrinkCommunicationTypeDef.rxIsOk = RESET;

                            ShrinkCommunicationSendACK();

                            break;                    

                        case sh_prm_TUBETYPE:
                            value.Char[0] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAL_BUFFER_ADR];
                            value.Char[1] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAH_BUFFER_ADR];

//                            Parameters.UzKs = value.Int;

                            shrinkCommunicationTypeDef.rxIsOk = RESET;

                            ShrinkCommunicationSendACK();

                            break;

                        case sh_prm_HEAT_SET:
                            value.Char[0] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAL_BUFFER_ADR];
                            value.Char[1] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAH_BUFFER_ADR];

                            Parameters.TemperatureSet = value.Int;

                            shrinkCommunicationTypeDef.rxIsOk = RESET;

                            ShrinkCommunicationSendACK();

                            break;    

                        case sh_prm_PRESSURE_SET:
                            value.Char[0] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAL_BUFFER_ADR];
                            value.Char[1] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAH_BUFFER_ADR];

                            Parameters.PressureSet = value.Int;

                            shrinkCommunicationTypeDef.rxIsOk = RESET;

                            ShrinkCommunicationSendACK();

                            break;                    

                        case sh_prm_MAX_HEAT:
                            value.Char[0] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAL_BUFFER_ADR];
                            value.Char[1] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAH_BUFFER_ADR];

                            Temperature.SensorH = value.Int;

                            shrinkCommunicationTypeDef.rxIsOk = RESET;

                            ShrinkCommunicationSendACK();

                            break;

                        case sh_prm_MAX_PRESSURE:
                            value.Char[0] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAL_BUFFER_ADR];
                            value.Char[1] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAH_BUFFER_ADR];

                            Pressure.SensorH = value.Int;

                            shrinkCommunicationTypeDef.rxIsOk = RESET;

                            ShrinkCommunicationSendACK();

                            break;    

                        case sh_prm_HEAT_LIMIT:
                            value.Char[0] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAL_BUFFER_ADR];
                            value.Char[1] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAH_BUFFER_ADR];

                            Parameters.EvacuationTemp = value.Int;

                            shrinkCommunicationTypeDef.rxIsOk = RESET;

                            ShrinkCommunicationSendACK();

                            break;

                        case sh_prm_HEAT_HIST:
                            value.Char[0] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAL_BUFFER_ADR];
                            value.Char[1] = shrinkCommunicationTypeDef.rxCommandBuffer[DATAH_BUFFER_ADR];

                            Parameters.TemperatureLowSet = value.Int;    // Histeresiz.

                            shrinkCommunicationTypeDef.rxIsOk = RESET;

                            ShrinkCommunicationSendACK();

                            break; 
                            
                        default:
                            break;
                    }
                } 


    /*-------------------------------- cmd_READ --------------------------------*/   

                if(shrinkCommunicationTypeDef.shrinkCommunicationCommand == sh_cmd_READ_TIMINGS)
                {            
                    shrinkCommunicationTypeDef.shrinkCommunicationTimingParameters = shrinkCommunicationTypeDef.rxCommandBuffer[PRM_BUFFER_ADR];

                    switch(shrinkCommunicationTypeDef.shrinkCommunicationTimingParameters)
                    {
                        case sh_prm_T1:
                            
                            if(ShrinkCommunicationTransmit(shrinkCommunicationTypeDef.shrinkCommunicationCommand, 
                                                           shrinkCommunicationTypeDef.shrinkCommunicationTimingParameters, 
                                                           Parameters.T1) == OK )   // ACK bekle
                            {
                                shrinkCommunicationTypeDef.txIsOk = RESET;
                                shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                                shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                                // Gönderildi mesaji
                                ParamSendMessage();
                            }

                            break;

                        case sh_prm_T2:

                            if(ShrinkCommunicationTransmit(shrinkCommunicationTypeDef.shrinkCommunicationCommand, 
                                                           shrinkCommunicationTypeDef.shrinkCommunicationTimingParameters, 
                                                           Parameters.T2) == OK)
                            {
                                shrinkCommunicationTypeDef.txIsOk = RESET;
                                shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                                shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                                // Gönderildi mesaji
                                ParamSendMessage();
                            }

                            break;

                        case sh_prm_T3:

                            if(ShrinkCommunicationTransmit(shrinkCommunicationTypeDef.shrinkCommunicationCommand, 
                                                           shrinkCommunicationTypeDef.shrinkCommunicationTimingParameters, 
                                                           Parameters.T3) == OK)
                            {
                                shrinkCommunicationTypeDef.txIsOk = RESET;
                                shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                                shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                                // Gönderildi mesaji
                                ParamSendMessage();
                            }

                            break;

                        case sh_prm_T4:

                            if(ShrinkCommunicationTransmit(shrinkCommunicationTypeDef.shrinkCommunicationCommand, 
                                                           shrinkCommunicationTypeDef.shrinkCommunicationTimingParameters, 
                                                           Parameters.T4) == OK)
                            {
                                shrinkCommunicationTypeDef.txIsOk = RESET;
                                shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                                shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                                // Gönderildi mesaji
                                ParamSendMessage();
                            }

                            break;

                        case sh_prm_T5:

                            if(ShrinkCommunicationTransmit(shrinkCommunicationTypeDef.shrinkCommunicationCommand, 
                                                           shrinkCommunicationTypeDef.shrinkCommunicationTimingParameters, 
                                                           Parameters.T5) == OK)
                            {
                                shrinkCommunicationTypeDef.txIsOk = RESET;
                                shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                                shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                                // Gönderildi mesaji
                                ParamSendMessage();
                            }

                            break;

                        case sh_prm_T6:

                            if(ShrinkCommunicationTransmit(shrinkCommunicationTypeDef.shrinkCommunicationCommand, 
                                                           shrinkCommunicationTypeDef.shrinkCommunicationTimingParameters, 
                                                           Parameters.T6) == OK)
                            {
                                shrinkCommunicationTypeDef.txIsOk = RESET;
                                shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                                shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                                // Gönderildi mesaji
                                ParamSendMessage();
                            }

                            break;

                        case sh_prm_T7:

                            if(ShrinkCommunicationTransmit(shrinkCommunicationTypeDef.shrinkCommunicationCommand, 
                                                           shrinkCommunicationTypeDef.shrinkCommunicationTimingParameters, 
                                                           Parameters.T7) == OK)
                            {
                                shrinkCommunicationTypeDef.txIsOk = RESET;
                                shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                                shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                                // Gönderildi mesaji
                                ParamSendMessage();
                            }

                            break;

                        case sh_prm_T8:

                            if(ShrinkCommunicationTransmit(shrinkCommunicationTypeDef.shrinkCommunicationCommand, 
                                                           shrinkCommunicationTypeDef.shrinkCommunicationTimingParameters, 
                                                           Parameters.T8) == OK)
                            {
                                shrinkCommunicationTypeDef.txIsOk = RESET;
                                shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                                shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                                // Gönderildi mesaji
                                ParamSendMessage();
                            }

                            break;

                        case sh_prm_T9:

                            if(ShrinkCommunicationTransmit(shrinkCommunicationTypeDef.shrinkCommunicationCommand, 
                                                           shrinkCommunicationTypeDef.shrinkCommunicationTimingParameters, 
                                                           Parameters.T9) == OK)
                            {
                                shrinkCommunicationTypeDef.txIsOk = RESET;
                                shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                                shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                                // Gönderildi mesaji
                                ParamSendMessage();
                            }

                            break;

                        case sh_prm_T10:

                            if(ShrinkCommunicationTransmit(shrinkCommunicationTypeDef.shrinkCommunicationCommand, 
                                                           shrinkCommunicationTypeDef.shrinkCommunicationTimingParameters, 
                                                           Parameters.T10) == OK)
                            {
                                shrinkCommunicationTypeDef.txIsOk = RESET;
                                shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                                shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                                // Gönderildi mesaji
                                ParamSendMessage();
                            }

                            break;
                            
                        default:
                            break;
                    }                
                }



    /*-------------------------------- cmd_READ --------------------------------*/   

                if(shrinkCommunicationTypeDef.shrinkCommunicationCommand == sh_cmd_READ)
                {
                    shrinkCommunicationTypeDef.shrinkCommunicationParameters = shrinkCommunicationTypeDef.rxCommandBuffer[PRM_BUFFER_ADR];

                    switch(shrinkCommunicationTypeDef.shrinkCommunicationParameters)
                    {
                        case sh_prm_COUNTERS:

                            if(ShrinkCommunicationTransmit(shrinkCommunicationTypeDef.shrinkCommunicationCommand, 
                                                           shrinkCommunicationTypeDef.shrinkCommunicationParameters, 
                                                           Parameters.TodayCount) == OK)
                            {
                                shrinkCommunicationTypeDef.txIsOk = RESET;
                                shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                                shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                                // Gönderildi mesaji
                                ParamSendMessage();
                            }

                            break;                    

//                        case sh_prm_TUBETYPE:
//
//                            if(ShrinkCommunicationTransmit(shrinkCommunicationTypeDef.shrinkCommunicationCommand, 
//                                                           shrinkCommunicationTypeDef.shrinkCommunicationParameters, 
//                                                           Parameters.UzKs) == OK)
//                            {
//                                shrinkCommunicationTypeDef.txIsOk = RESET;
//                                shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
//                                shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
//                                // Gönderildi mesaji
//                                ParamSendMessage();
//                            }
//
//                            break;

                        case sh_prm_HEAT_SET:

                            if(ShrinkCommunicationTransmit(shrinkCommunicationTypeDef.shrinkCommunicationCommand, 
                                                           shrinkCommunicationTypeDef.shrinkCommunicationParameters, 
                                                           Parameters.TemperatureSet) == OK)
                            {
                                shrinkCommunicationTypeDef.txIsOk = RESET;
                                shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                                shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                                // Gönderildi mesaji
                                ParamSendMessage();
                            }

                            break;    

                        case sh_prm_PRESSURE_SET:

                            if(ShrinkCommunicationTransmit(shrinkCommunicationTypeDef.shrinkCommunicationCommand, 
                                                           shrinkCommunicationTypeDef.shrinkCommunicationParameters, 
                                                           Parameters.PressureSet) == OK)
                            {
                                shrinkCommunicationTypeDef.txIsOk = RESET;
                                shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                                shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                                // Gönderildi mesaji
                                ParamSendMessage();
                            }

                            break;                    

                        case sh_prm_MAX_HEAT:

                            if(ShrinkCommunicationTransmit(shrinkCommunicationTypeDef.shrinkCommunicationCommand, 
                                                           shrinkCommunicationTypeDef.shrinkCommunicationParameters, 
                                                           Temperature.SensorH) == OK)
                            {
                                shrinkCommunicationTypeDef.txIsOk = RESET;
                                shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                                shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                                // Gönderildi mesaji
                                ParamSendMessage();
                            }

                            break;

                        case sh_prm_MAX_PRESSURE:

                            if(ShrinkCommunicationTransmit(shrinkCommunicationTypeDef.shrinkCommunicationCommand, 
                                                           shrinkCommunicationTypeDef.shrinkCommunicationParameters, 
                                                           Pressure.SensorH) == OK)
                            {
                                shrinkCommunicationTypeDef.txIsOk = RESET;
                                shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                                shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                                // Gönderildi mesaji
                                ParamSendMessage();
                            }

                            break;    

                        case sh_prm_HEAT_LIMIT:

                            if(ShrinkCommunicationTransmit(shrinkCommunicationTypeDef.shrinkCommunicationCommand, 
                                                           shrinkCommunicationTypeDef.shrinkCommunicationParameters, 
                                                           Parameters.EvacuationTemp) == OK)
                            {
                                shrinkCommunicationTypeDef.txIsOk = RESET;
                                shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                                shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                                // Gönderildi mesaji
                                ParamSendMessage();
                            }

                            break;

                        case sh_prm_HEAT_HIST:

                            if(ShrinkCommunicationTransmit(shrinkCommunicationTypeDef.shrinkCommunicationCommand, 
                                                           shrinkCommunicationTypeDef.shrinkCommunicationParameters, 
                                                           Parameters.TemperatureLowSet) == OK)
                            {
                                shrinkCommunicationTypeDef.txIsOk = RESET;
                                shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                                shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                                // Gönderildi mesaji
                                ParamSendMessage();
                            }

                            break;   

                        case sh_prm_TEMPERATURE:

                            if(ShrinkCommunicationTransmit(shrinkCommunicationTypeDef.shrinkCommunicationCommand, 
                                                           shrinkCommunicationTypeDef.shrinkCommunicationParameters, 
                                                           Temperature.Value) == OK)
                            {
                                shrinkCommunicationTypeDef.txIsOk = RESET;
                                shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                                shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                                // Gönderildi mesaji
                                ParamSendMessage();
                            }

                            break;  

                        case sh_prm_PRESSURE:

                            if(ShrinkCommunicationTransmit(shrinkCommunicationTypeDef.shrinkCommunicationCommand, 
                                                           shrinkCommunicationTypeDef.shrinkCommunicationParameters, 
                                                           Pressure.Value) == OK)
                            {
                                shrinkCommunicationTypeDef.txIsOk = RESET;
                                shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                                shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                                // Gönderildi mesaji
                                ParamSendMessage();
                            }

                            break;   
                            
                        default:
                            break;
                    }                
                }
            }
            
            else if(shrinkCommunicationTypeDef.shrinkCommunicationStatus == sh_st_WAIT_ACK)
            {
                shrinkCommunicationTypeDef.shrinkCommunicationCommand = shrinkCommunicationTypeDef.rxCommandBuffer[CMD_BUFFER_ADR];
                
                if(shrinkCommunicationTypeDef.shrinkCommunicationCommand == sh_cmd_ACK)
                {
                    if(shrinkCommunicationTypeDef.rxCommandBuffer[PRM_BUFFER_ADR] == 'A' && 
                       shrinkCommunicationTypeDef.rxCommandBuffer[DATAL_BUFFER_ADR] == 'C' &&
                       shrinkCommunicationTypeDef.rxCommandBuffer[DATAH_BUFFER_ADR] == 'K')
                    {
                        shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_READ;
                        // Ekrana OK yazar.
                        MessageIsOk();
                    }
                }
            }
            
            if(shrinkCommunicationTypeDef.shrinkCommunicationTimeout == 0 && shrinkCommunicationTypeDef.shrinkCommunicationStatus == sh_st_WAIT_ACK)
            {
                    shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_READ;
                    // Ekrana NOK yazar.    
                    MessageIsNok();
            }
        }        
    }
}