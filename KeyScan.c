
/**
******************************************************************************
* @FileName		    KeyScan.cpp
* @Description      detect key mode
* @author           xz
* @note
******************************************************************************
**/

#define __KEYSCAN_C

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_pit.h"
#include "fsl_gpio.h"
#include "KeyScan.h"

/*** key initialization  ***/
struct key_obj KEY1_obj ={GPIO_PIN_SET,BOARD_INITPINS_KEY1_GPIO,BOARD_INITPINS_KEY1_PIN,0,FUNCTION_WAIT,*general_key_state_judge,*key_value_measurement_function_for_KEY1};
struct key_obj KEY2_obj ={GPIO_PIN_SET,BOARD_INITPINS_KEY2_GPIO,BOARD_INITPINS_KEY2_PIN,0,FUNCTION_WAIT,*general_key_state_judge,*key_value_measurement_function_for_KEY2};
struct key_obj SW1_obj  ={GPIO_PIN_SET,BOARD_INITPINS_SW1_GPIO, BOARD_INITPINS_SW1_PIN, 0,FUNCTION_WAIT,*general_key_state_judge,*key_value_measurement_function_for_SW1};
struct key_obj SW2_obj  ={GPIO_PIN_SET,BOARD_INITPINS_SW2_GPIO, BOARD_INITPINS_SW2_PIN, 0,FUNCTION_WAIT,*general_key_state_judge,*key_value_measurement_function_for_SW2};
struct key_obj SW3_obj  ={GPIO_PIN_SET,BOARD_INITPINS_SW3_GPIO, BOARD_INITPINS_SW3_PIN, 0,FUNCTION_WAIT,*general_key_state_judge,*key_value_measurement_function_for_SW3};
struct key_obj SW4_obj  ={GPIO_PIN_SET,BOARD_INITPINS_SW4_GPIO, BOARD_INITPINS_SW4_PIN, 0,FUNCTION_WAIT,*general_key_state_judge,*key_value_measurement_function_for_SW4};

/*** KEY MEASUREMENT FUNCTION  ***/
static void key_value_measurement_function_for_KEY1(struct key_obj* key_obj){
           //test code
	    	GPIO_PinWrite(BOARD_INITPINS_LED1_GPIO,BOARD_INITPINS_LED1_PIN,0);

}
static void key_value_measurement_function_for_KEY2(struct key_obj* key_obj){
   }
static void key_value_measurement_function_for_SW1(struct key_obj* key_obj){
   }
static void key_value_measurement_function_for_SW2(struct key_obj* key_obj){
   }
static void key_value_measurement_function_for_SW3(struct key_obj* key_obj){
   }
static void key_value_measurement_function_for_SW4(struct key_obj* key_obj){
   }

/*** key list declaration ***/
struct key_obj *list_key[] = {&KEY1_obj,&KEY2_obj,&SW1_obj,&SW2_obj,&SW3_obj,&SW4_obj};
uint8_t total_num = (sizeof(list_key)) / (sizeof(struct key_obj*));

/*** Key value judgment ***/
static void general_key_state_judge(struct key_obj* KEY_obj){
	KEY_obj->trig_condition= GPIO_PinRead(KEY_obj->GPIO_key, KEY_obj->GPIO_PIN_KEY);
			 if(KEY_obj->trig_condition == 0){
				 KEY_obj->detect_buf++;
			     if(KEY_obj->detect_buf++> 7){
			    	   KEY_obj->measurement_flag=FUNCTION_BEGIN;
			    	   KEY_obj->detect_buf = 0;
			           }
			    }
			 else
				 KEY_obj->detect_buf=0;
}

/*** key refreshment in period  ***/
void Key_refresh_key_state(){
	for(int i = 0; i < total_num; i++){
		list_key[i]->State_judge_function(list_key[i]);
	    if(list_key[i]->measurement_flag==FUNCTION_BEGIN)
	    	list_key[i]->Key_value_measurement_function(list_key[i]);
	}
}

/*** PIT INITIALIZATION ***/
void InitPITs() {
    pit_config_t pitConfig;

    PIT_GetDefaultConfig(&pitConfig);
    PIT_Init(PIT, &pitConfig);
    /* Set timer period for channel 0: 5000us = 5ms */
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(1250U, APPTICK_PIT_SOURCE_CLOCK));
    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
    /* Enable at the NVIC */
    EnableIRQ(APPTICK_PIT_IRQn);
    /* Start channel 0 */
    PIT_StartTimer(PIT, kPIT_Chnl_0);

}

/*** PIT HANDLER ***/
void APPTICK_PIT_HANDLER(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
    Key_refresh_key_state();

#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}


