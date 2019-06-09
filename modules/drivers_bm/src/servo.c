/* Copyright 2018, Osvaldo Marcos Zanet (tato)
 * All rights reserved.
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** \brief Servo PWM Drivers Functions and Ports assignment
 **
 **
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */

/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup Baremetal Servo PWM Driver
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 *  tato		Osvaldo Marcos Zanet
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20190517 v0.0.1 first documented version version
 */

/*==================[inclusions]=============================================*/


#include "chip.h"
#include "servo.h"

/*==================[macros and definitions]=================================*/


#define MAXQSERVOS                   8
#define SERVO_COMPLETECYCLE_PERIOD   20000  /* value in uSec */
#define SERVO_MAXUPTIME_PERIOD       2500   /* value in uSec */
#define SERVO_MINUPTIME_PERIOD       900    /* value in uSec */

/*==================[internal data declaration]==============================*/


uint32_t tickspercycle;
const  servo_t servo_0=
{
	PWM_0_MUX_GROUP,
	PWM_0_MUX_PIN,
	PWM_0_INDEX,
	PWM_0_FUNC
};
const servo_t servo_1=
{
	PWM_1_MUX_GROUP,
	PWM_1_MUX_PIN,
	PWM_1_INDEX,
	PWM_1_FUNC
};
const servo_t servo_2=
{
	PWM_2_MUX_GROUP,
	PWM_2_MUX_PIN,
	PWM_2_INDEX,
	PWM_2_FUNC
};
const servo_t servo_3=
{
	PWM_3_MUX_GROUP,
	PWM_3_MUX_PIN,
	PWM_3_INDEX,
	PWM_3_FUNC
};
const servo_t servo_4=
{
	PWM_4_MUX_GROUP,
	PWM_4_MUX_PIN,
	PWM_4_INDEX,
	PWM_4_FUNC
};
const servo_t servo_5=
{
	PWM_5_MUX_GROUP,
	PWM_5_MUX_PIN,
	PWM_5_INDEX,
	PWM_5_FUNC
};
const servo_t servo_6=
{
	PWM_6_MUX_GROUP,
	PWM_6_MUX_PIN,
	PWM_6_INDEX,
	PWM_6_FUNC
};
const servo_t servo_7=
{
	PWM_7_MUX_GROUP,
	PWM_7_MUX_PIN,
	PWM_7_INDEX,
	PWM_7_FUNC
};


static servo_t *servos_list[MAXQSERVOS]={&servo_0,&servo_1,&servo_2,&servo_3,&servo_4,&servo_5,&servo_6,&servo_7};


/*==================[internal data definition]===============================*/
/*==================[internal functions definition]==========================*/


void ServoMove(uint8_t servos,uint16_t angle){
    uint8_t i=0;
	uint32_t percent;
    while(i<MAXQSERVOS){
        if(((servos&(1<<i))!=0)&&(angle<=180)&&(angle>=0)){
			Chip_SCTPWM_SetDutyCycle(LPC_SCT, servos_list[i]->index +1,(Chip_SCTPWM_GetTicksPerCycle(LPC_SCT)/SERVO_COMPLETECYCLE_PERIOD)*(SERVO_MINUPTIME_PERIOD+((180-angle)*(SERVO_MAXUPTIME_PERIOD-SERVO_MINUPTIME_PERIOD))/180));
        }
        i=i+1;
    }
}



void ServoInit(uint8_t servos, uint32_t freq){
    uint8_t i=0;
	Chip_SCTPWM_Init(LPC_SCT);
	Chip_SCTPWM_SetRate(LPC_SCT,freq);
    while(i<MAXQSERVOS){
        if((servos&(1<<i))!=0){
			Chip_SCU_PinMuxSet(servos_list[i]->hwPort,servos_list[i]->hwPin,SCU_MODE_INACT|servos_list[i]->gpioFunc);
			Chip_SCTPWM_SetOutPin(LPC_SCT ,servos_list[i]->index +1,servos_list[i]->index);
        }
        i=i+1;
    }
	Chip_SCTPWM_Start(LPC_SCT);
}



/*==================[external data definition]===============================*/



/*==================[external functions definition]==========================*/


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

