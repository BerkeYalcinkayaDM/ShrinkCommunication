/**
 * \file            ShrinkCommunicationController.h
 * \brief           ShrinkCommunicationController header file
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

#ifndef SHRINKCOMMUNICATIONCONTROLLER_H
#define	SHRINKCOMMUNICATIONCONTROLLER_H

#ifdef	__cplusplus
extern "C" {
#endif

#define COMMAND_BUFFER_LENGTH  7
    
/*-------------------------------- ENUMARATORS --------------------------------*/    
// @CommunicationCommand    
typedef enum
{
    sh_cmd_IDLE = 0,
    sh_cmd_READ,
    sh_cmd_SET,
    sh_cmd_TIMINGS,
    sh_cmd_READ_TIMINGS,
    sh_cmd_ACK,
}shrinkCommunicationCommand_t;

// @CommunicationTimingParameters
typedef enum
{
    sh_prm_T1 = 0,
    sh_prm_T2,
    sh_prm_T3,
    sh_prm_T4,
    sh_prm_T5,
    sh_prm_T6,
    sh_prm_T7,
    sh_prm_T8,
    sh_prm_T9,
    sh_prm_T10,
    sh_prm_T11,
    sh_prm_T12,
    sh_prm_T13,
    sh_prm_T14,
    sh_prm_T15,
    sh_prm_T16,        
}shrinkCommunicationTimingParameters_t;

//------------------------------------------------------------------------------
// @CommunicationParameters
typedef enum
{
    sh_prm_IDLE = 0,
    sh_prm_COUNTERS,
    sh_prm_TUBETYPE,
    sh_prm_HEAT_SET,
    sh_prm_PRESSURE_SET, 
    sh_prm_MAX_HEAT,
    sh_prm_MAX_PRESSURE,
    sh_prm_HEAT_LIMIT,
    sh_prm_HEAT_HIST,   
    sh_prm_TEMPERATURE,
    sh_prm_PRESSURE,
}shrinkCommunicationParameters_t;


typedef enum
{
    sh_st_READ = 1,
    sh_st_WAIT_ACK,
}shrinkCommunicationStatus_t;
   

/*-------------------------------- STRUCTURES --------------------------------*/ 

typedef struct
{
    shrinkCommunicationCommand_t shrinkCommunicationCommand;
    shrinkCommunicationParameters_t shrinkCommunicationParameters;
    shrinkCommunicationTimingParameters_t shrinkCommunicationTimingParameters;
    shrinkCommunicationStatus_t shrinkCommunicationStatus;
    
    uint8_t rxCommandBuffer[COMMAND_BUFFER_LENGTH];
    uint8_t txCommandBuffer[COMMAND_BUFFER_LENGTH];
    
    uint8_t crc;
    
    uint8_t rxIsOk;
    uint8_t txIsOk;
    
    uint8_t rxCommandBufferIndex;
    uint8_t txCommandBufferIndex;
    
    uint8_t shrinkCommunicationTimeout;
}shrinkCommunicationTypeDef_t;

extern shrinkCommunicationTypeDef_t shrinkCommunicationTypeDef;


/*---------------------------- User Defined Macros ----------------------------*/

#define START_BIT              26
#define STOP_BIT               06

#define CMD_BUFFER_ADR         1
#define PRM_BUFFER_ADR         2
#define DATAL_BUFFER_ADR       3
#define DATAH_BUFFER_ADR       4
#define CRC_BUFFER_ADR         5

#define COMMUNICATION_TIMEOUT  40   // 50ms * 40 = 2000ms 


/*---------------------------- Function Prototypes ----------------------------*/
void ParamSendMessage (void);
void MessageIsOk (void);
uint8_t CalculateCRC(uint8_t *pBuffer);
uint8_t IncreaseRxCommandBufferIndex(void);
uint8_t ShrinkCommunicationRead(void);
uint8_t ShrinkCommunicationTransmit(uint8_t cmd, uint8_t prm, unsigned int value);
void ShrinkCommunicationSendACK(void);
void ShrinkCommunicationProtocol(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SHRINKCOMMUNICATIONCONTROLLER_H */

