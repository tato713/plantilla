/* Copyright 2017, Esteban Volentini - Facet UNT, Fi UNER
 * Copyright 2014, 2015 Mariano Cerdeiro
 * Copyright 2014, Pablo Ridolfi
 * Copyright 2014, Juan Cecconi
 * Copyright 2014, Gustavo Muro
 *
 * All rights reserved.
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
 */

/** @file blinking.c
 **
 ** @brief Ejemplo de un led parpadeando
 **
 ** Robot movil tipo reptante con 5 servomotores SG90.
 ** 
 ** | RV | YYYY.MM.DD | Autor       | Descripción de los cambios              |
 ** |----|------------|-------------|-----------------------------------------|
 ** |  2 | 2019.05.17 | evolentini  | Proyecto                                |
 ** 
 ** @defgroup ejemplos Proyectos de Tato
 ** @brief Bla bla bla robot movil bla bla.
 ** @{ 
 */

/* === Inclusiones de cabeceras ============================================ */
#include "FreeRTOS.h"
#include "task.h"
// #include "sapi.h"
// #include "board.h"
// #include "unt.h"
#include "switch.h"
#include "led.h"
#include "servo.h"
#include "uart.h"
// #include "event_groups.h"
#include "semphr.h"
// #include "pwm_simple.h"
// #include "soc.h"

/* === Definicion y Macros ================================================= */

// #define SERVO_PER_US          20
#define SERVO_FREQ      50
#define POS_REPOSO      90

#define ANG_TORSO       30
#define ANG_GIRO        30

#define POS_PASO_MIN    10
#define POS_PASO_MAX    80

#define FACT_VEL        15
#define MAX_VEL         45

// #define EVENTO_COMPLETO   (1 << 0)

typedef enum {
   MOV_RE,     // reposo
   MOV_PA,     // parado
   MOV_DI,     // adelante izquierda
   MOV_DC,     // adelante centrado
   MOV_DD,     // adelante derecha
   MOV_TI,     // atras izquierda
   MOV_TC,     // atras centrado
   MOV_TD
} mov_direccion;

// typedef struct {
//    const char * datos;   /** < Puntero al bloque de datos a enviar */
//    uint8_t cantidad;     /** < Cantidad de datos a enviar */
//    uint8_t enviados;     /** < Cantidad de datos enviados */
// } cola_t;


/* === Declaraciones de tipos de datos internos ============================ */

// };

static uint8_t pos_torso;
static uint8_t pos_brazo_I;    // delantera izquierda
static uint8_t pos_brazo_D;    // delantera derecha
static uint8_t pos_pierna_I;    // trasera izquierda
static uint8_t pos_pierna_D;    // trasera derecha
static uint16_t velocidad;        // frecuencia en que cambia de posición
static uint8_t pos_parado=60;
static mov_direccion direccion;
static uint8_t secuencia;

static uint8_t secuencia_torso[16]={0,10,38,80,128,176,218,245,255,245,218,176,128,80,38,10};
static uint8_t secuencia_brazo_I[16]={128,176,218,245,255,245,218,176,128,80,38,10,0,10,38,80};
static uint8_t secuencia_brazo_D[16]={128,80,38,10,0,10,38,80,128,176,218,245,255,245,218,176};
static uint8_t secuencia_pierna_I[16]={128,80,38,10,0,10,38,80,128,176,218,245,255,245,218,176};
static uint8_t secuencia_pierna_D[16]={128,176,218,245,255,245,218,176,128,80,38,10,0,10,38,80};


/* === Declaraciones de funciones internas ================================= */

/** @brief Movimiento de torso
 ** 
 ** @parameter[in] parametros Puntero a una estructura
 */ 
void Mov_Torso(void * parametros);

/** @brief Movimiento de brazo izquierdo
 ** 
 ** @parameter[in] parametros Puntero a una estructura
 */ 
void Mov_Brazo_Izquierdo(void * parametros);

/** @brief Movimiento de brazo derecho
 ** 
 ** @parameter[in] parametros Puntero a una estructura
 */ 
void Mov_Brazo_Derecho(void * parametros);

/** @brief Movimiento de pierna izquierda
 ** 
 ** @parameter[in] parametros Puntero a una estructura
 */ 
void Mov_Pierna_Izquierda(void * parametros);

/** @brief Movimiento de pierna derecha
 ** 
 ** @parameter[in] parametros Puntero a una estructura
 */ 
void Mov_Pierna_Derecha(void * parametros);

/** @brief Teclado
 ** 
 ** @parameter[in] parametros Puntero a una estructura
 */ 
void Teclado(void * parametros);

/** @brief Bluetooth
 ** 
 ** @parameter[in] parametros Puntero a una estructura
 */ 
void Bluetooth(void * parametros);
void Moverse(void * parametros);


/* === Definiciones de variables internas ================================== */
/** @brief Información para el envio de datos por la uart */
// cola_t cola;



/* === Definiciones de variables externas ================================== */
SemaphoreHandle_t semaforo = NULL;

/* === Definiciones de funciones internas ================================== */

void Moverse(void * parametros){
   while (1){
      switch(direccion){
         case MOV_DI:
         case MOV_DD:
         case MOV_DC:
         secuencia=secuencia+1;
         break;
         case MOV_TI:
         case MOV_TD:
         case MOV_TC:
         secuencia=secuencia-1;
         break;
         case MOV_PA:
         case MOV_RE:
         secuencia=secuencia;
         break;
      }
      secuencia=secuencia%16;
      vTaskDelay(velocidad / portTICK_PERIOD_MS);
   }
}

void Mov_Torso(void * parametros) {
   while(1) { 
      if (xSemaphoreTake(semaforo, 0) == pdPASS)
      {
         switch(direccion){
            case MOV_DI:
            case MOV_TI:
            pos_torso=POS_REPOSO-ANG_TORSO+(uint8_t)(((ANG_TORSO*2+ANG_GIRO)*(uint16_t)secuencia_torso[secuencia])/256);
            break;
            case MOV_DD:
            case MOV_TD:
            pos_torso=POS_REPOSO-ANG_TORSO-ANG_GIRO+(uint8_t)(((ANG_TORSO*2+ANG_GIRO)*(uint16_t)secuencia_torso[secuencia])/256);
            break;
            case MOV_DC:
            case MOV_TC:
            pos_torso=POS_REPOSO-ANG_TORSO+(uint8_t)(((ANG_TORSO*2)*(uint16_t)secuencia_torso[secuencia])/256);
            break;
            case MOV_PA:
            case MOV_RE:
            pos_torso=POS_REPOSO;
            break;
         }
         xSemaphoreGive(semaforo);
      }
      ServoMove(SERVO_0,pos_torso);
      vTaskDelay(velocidad/ portTICK_PERIOD_MS);
   }
}

void Mov_Brazo_Izquierdo(void * parametros) {
   while(1) { 
      if (xSemaphoreTake(semaforo, 0) == pdPASS)
      {
         switch(direccion){
            case MOV_DI:
            case MOV_TI:
            case MOV_DD:
            case MOV_TD:
            case MOV_DC:
            case MOV_TC:
            pos_brazo_I=POS_REPOSO+POS_PASO_MIN+(uint8_t)(((POS_PASO_MAX-POS_PASO_MIN)*(uint16_t)secuencia_brazo_I[secuencia])/256);
            break;
            case MOV_PA:
            pos_brazo_I=POS_REPOSO+pos_parado;
            break;
            case MOV_RE:
            pos_brazo_I=POS_REPOSO;
            break;
         }
         xSemaphoreGive(semaforo);
      }
      ServoMove(SERVO_1,pos_brazo_I);
      vTaskDelay(velocidad / portTICK_PERIOD_MS);
   }
}

void Mov_Brazo_Derecho(void * parametros) {
   while(1) { 
      if (xSemaphoreTake(semaforo, 0) == pdPASS)
      {
         switch(direccion){
            case MOV_DI:
            case MOV_TI:
            case MOV_DD:
            case MOV_TD:
            case MOV_DC:
            case MOV_TC:
            pos_brazo_D=POS_REPOSO+POS_PASO_MIN+(uint8_t)(((POS_PASO_MAX-POS_PASO_MIN)*(uint16_t)secuencia_brazo_D[secuencia])/256);
            break;
            case MOV_PA:
            pos_brazo_D=POS_REPOSO+pos_parado;
            break;
            case MOV_RE:
            pos_brazo_D=POS_REPOSO;
            break;
         }
         xSemaphoreGive(semaforo);
      }
      ServoMove(SERVO_2,180-pos_brazo_D);
      vTaskDelay( velocidad / portTICK_PERIOD_MS);
   }
}

void Mov_Pierna_Izquierda(void * parametros) {
   while(1) { 
      if (xSemaphoreTake(semaforo, 0) == pdPASS)
      {
         switch(direccion){
            case MOV_DI:
            case MOV_TI:
            case MOV_DD:
            case MOV_TD:
            case MOV_DC:
            case MOV_TC:
            pos_pierna_I=POS_REPOSO+POS_PASO_MIN+(uint8_t)(((POS_PASO_MAX-POS_PASO_MIN)*(uint16_t)secuencia_pierna_I[secuencia])/256);
            break;
            case MOV_PA:
            pos_pierna_I=POS_REPOSO+pos_parado;
            break;
            case MOV_RE:
            pos_pierna_I=POS_REPOSO;
            break;
         }
         xSemaphoreGive(semaforo);
      }
      ServoMove(SERVO_3,180-pos_pierna_I);
      vTaskDelay(velocidad / portTICK_PERIOD_MS);
   }
}

void Mov_Pierna_Derecha(void * parametros) {
   while(1) { 
      if (xSemaphoreTake(semaforo, 0) == pdPASS)
      {
         switch(direccion){
            case MOV_DI:
            case MOV_TI:
            case MOV_DD:
            case MOV_TD:
            case MOV_DC:
            case MOV_TC:
            pos_pierna_D=POS_REPOSO+POS_PASO_MIN+(uint8_t)(((POS_PASO_MAX-POS_PASO_MIN)*(uint16_t)secuencia_pierna_D[secuencia])/256);
            break;
            case MOV_PA:
            pos_pierna_D=POS_REPOSO+pos_parado;
            break;
            case MOV_RE:
            pos_pierna_D=POS_REPOSO;
            break;
         }
         xSemaphoreGive(semaforo);
      }
      ServoMove(SERVO_4,pos_pierna_D);
      vTaskDelay(velocidad / portTICK_PERIOD_MS);
   }
}

void Bluetooth(void * parametros){
   uint8_t comando;
   while(1) {
      
      Led_Off(RED_LED);
      Led_Off(GREEN_LED);
      Led_Off(YELLOW_LED);
      Led_Off(RGB_R_LED);
      Led_Off(RGB_G_LED);
      Led_Off(RGB_B_LED);
      if (ReadByte_Uart_Rs232(&comando))
      {
         // if (EnviarTexto(&comando)) {
         //    xEventGroupWaitBits(eventos, EVENTO_COMPLETO, TRUE, FALSE, portMAX_DELAY);
         // }  
         if (xSemaphoreTake(semaforo, 0) == pdPASS)
			{
            switch (comando)
            {
            case 'F':
               direccion=MOV_DC;
               Led_On(RED_LED);
               break;
            case 'B':
               direccion=MOV_TC;
               Led_On(YELLOW_LED);
               break;
            case 'L':
               direccion=MOV_PA;
               Led_On(GREEN_LED);
               break;
            case 'R':
               direccion=MOV_PA;
               Led_On(RGB_B_LED);
               break;
            case 'G':
               direccion=MOV_DI;
               Led_On(RGB_G_LED);
               break;
            case 'I':
               direccion=MOV_DD;
               Led_On(RGB_R_LED);
               break;
            case 'H':
               direccion=MOV_TI;
               Led_On(RGB_G_LED);
               Led_On(RGB_R_LED);
               break;
            case 'J':
               direccion=MOV_TD;
               Led_On(RGB_B_LED);
               Led_On(RGB_R_LED);
               break;
            case 'S':
               direccion=MOV_PA;
               Led_On(RGB_B_LED);
               Led_On(RGB_G_LED);
               Led_On(RGB_R_LED);
               break;
            case '0':
               direccion=MOV_PA;
               break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
               velocidad=MAX_VEL+(FACT_VEL*(10-(comando-'0')));
               Led_On(RGB_B_LED);
               Led_On(RGB_G_LED);
               Led_On(RGB_R_LED);
               Led_On(RGB_R_LED);
               Led_On(GREEN_LED);
               Led_On(YELLOW_LED);
               break;
            case 'q':
               velocidad=MAX_VEL;
               Led_On(RGB_B_LED);
               Led_On(RGB_G_LED);
               Led_On(RGB_R_LED);
               Led_On(RED_LED);
               Led_On(GREEN_LED);
               Led_On(YELLOW_LED);
               break;
            default:
               direccion=MOV_RE;
               break;
            }
				xSemaphoreGive(semaforo);
			}
         else
         {
            direccion=MOV_RE;
         }
         
      }
      vTaskDelay(50/ (portTICK_PERIOD_MS));
   }
}

void Teclado(void * parametros) {
   uint8_t tecla;
   uint8_t anterior = 0;
   

   while(1) {
      tecla = Read_Switches();
      if (tecla != anterior) {
         switch(tecla) {
            case TEC1:
               if(pos_torso!=0){
                  pos_torso=pos_torso-2;  
               }
               else
               {
                  pos_torso=0;
               }
               break;
            case TEC2:
                  pos_torso=90;
               break;
            case TEC3:
               break;
            case TEC4:
               if(pos_torso<180){
                  pos_torso=pos_torso+2;  
               }
               else
               {
                  pos_torso=180;
               }
               
               break;
         }
         anterior = tecla;
      }
      if(pos_torso==180){
         Led_On(RGB_B_LED);
      }
      else if (pos_torso==0){
         Led_On(RGB_G_LED);
         Led_On(RGB_R_LED);
      }
      else
      {
         Led_Off(RED_LED);
         Led_Off(GREEN_LED);
         Led_Off(YELLOW_LED);
         Led_Off(RGB_R_LED);
         Led_Off(RGB_G_LED);
         Led_Off(RGB_B_LED);
      }
      
      ServoMove(SERVO_0,180-pos_torso);
      ServoMove(SERVO_1,90);
      ServoMove(SERVO_2,90);
      ServoMove(SERVO_3,90);
      ServoMove(SERVO_4,90);
      vTaskDelay(100/ (portTICK_PERIOD_MS));
      // Led_Toggle(GREEN_LED); 
   }
}
/* === Definiciones de funciones externas ================================== */

/** @brief Función principal del programa
 **
 ** @returns 0 La función nunca debería termina
 **
 ** @remarks En un sistema embebido la función main() nunca debe terminar.
 **          El valor de retorno 0 es para evitar un error en el compilador.
 */
int main(void) {
   /* Inicializaciones y configuraciones de dispositivos */
   SisTick_Init();
   // Init_PonchoUNT();
   Init_Leds();
   Init_Switches();
   Init_Uart_Rs232();
   // Init_Uart_Ftdi();
   velocidad=MAX_VEL;
   secuencia=0;
   // eventos = xEventGroupCreate();
	semaforo = xSemaphoreCreateMutex();
   ServoInit(SERVO_0|SERVO_1|SERVO_2|SERVO_3|SERVO_4,SERVO_FREQ);
   pos_torso=90;
   /* Creación de las tareas */
   xTaskCreate(Bluetooth, "Bluetooth", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
   xTaskCreate(Moverse, "Moverse", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL);
   xTaskCreate(Mov_Torso, "Mov_Torso", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
   xTaskCreate(Mov_Brazo_Izquierdo, "Mov_Brazo_Izquierdo", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
   xTaskCreate(Mov_Brazo_Derecho, "Mov_Brazo_Derecho", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
   xTaskCreate(Mov_Pierna_Izquierda, "Mov_Pierna_Izquierda", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
   xTaskCreate(Mov_Pierna_Derecha, "Mov_Pierna_Derecha", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
   // xTaskCreate(Teclado, "Teclado", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 4, NULL);

   /* Arranque del sistema operativo */
   vTaskStartScheduler();

   /* vTaskStartScheduler solo retorna si se detiene el sistema operativo */
   while(1);

   /* El valor de retorno es solo para evitar errores en el compilador*/
   return 0;
}
/* === Ciere de documentacion ============================================== */
/** @} Final de la definición del modulo para doxygen */
