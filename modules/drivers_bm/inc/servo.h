


   /* Copyright 2018, Osvaldo Marcos Zanet
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

#ifndef INC_SERVO_H_
#define INC_SERVO_H_

/** \brief Header for Stepper Drivers
 **
 ** The header for the d_stepper.c file
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup Baremetal Bare Metal example header file
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * TATO			Osvaldo Marcos Zanet
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20190517 v0.0.1 first documented version
 */

/*==================[inclusions]=============================================*/
#include "stdint.h"

/*==================[macros and definitions]=================================*/


   
#define PWM_0_MUX_GROUP	    4	// T_FIL1
#define PWM_0_MUX_PIN 		1
#define PWM_0_INDEX			1
#define PWM_0_FUNC 		    1

#define PWM_1_MUX_GROUP		1	// T_COL0
#define PWM_1_MUX_PIN 		5
#define PWM_1_INDEX			10
#define PWM_1_FUNC 			1

#define PWM_2_MUX_GROUP		4	// T_FIL2
#define PWM_2_MUX_PIN 		2
#define PWM_2_INDEX			0
#define PWM_2_FUNC 			1

#define PWM_3_MUX_GROUP		4	// T_FIL3
#define PWM_3_MUX_PIN 		3
#define PWM_3_INDEX			3
#define PWM_3_FUNC 			1

#define PWM_4_MUX_GROUP		6	// GPIO8
#define PWM_4_MUX_PIN 		12
#define PWM_4_INDEX			7
#define PWM_4_FUNC 			1

#define PWM_5_MUX_GROUP		4	// LCD1
#define PWM_5_MUX_PIN 		4
#define PWM_5_INDEX			2
#define PWM_5_FUNC 			1

#define PWM_6_MUX_GROUP		4	// LCD2
#define PWM_6_MUX_PIN 		5
#define PWM_6_INDEX			5
#define PWM_6_FUNC 			1

#define PWM_7_MUX_GROUP		4	// LCD3
#define PWM_7_MUX_PIN 		6
#define PWM_7_INDEX			4
#define PWM_7_FUNC 			1

#define PWM_UP				1
#define PWM_DOWN			0

#define SERVO_0             1
#define SERVO_1             2
#define SERVO_2             4
#define SERVO_3             8
#define SERVO_4             16
#define SERVO_5             32
#define SERVO_6             64
#define SERVO_7             128


 /*==================[typedef]================================================*/

 typedef struct
 {
	 uint8_t hwPort ;
	 uint8_t hwPin ;
	 uint8_t index;
	 uint8_t gpioFunc;
 } servo_t;





/*==================[external functions declaration]=========================*/

/*! \brief Change servos angle
 * Change servos angle. Can be changed multiple servos by a masc.
 * \param[in] servos : The servos that will be moved.
 * \param[in] angle : Angle the servos will be moved.
 */
void ServoMove(uint8_t servos,uint16_t angle);

/*! \brief Initialize the servo output
 * Initialize the ports designed for the pwm output.
 * \param[in] st The pointer to the pwm output.
 */
void ServoInit(uint8_t servos,uint32_t freq);

#endif /* INC_SERVO_H_ */