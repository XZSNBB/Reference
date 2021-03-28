/*
 * KeyScan.h
 *
 *  Created on: 2021年3月27日
 *      Author: 24917
 */

#ifndef KEYSCAN_H_
#define KEYSCAN_H_

typedef enum{
	GPIO_PIN_RESET = 0,
	GPIO_PIN_SET,
} GPIO_PinState;

typedef enum{
	FUNCTION_WAIT =0,
	FUNCTION_BEGIN=1,
} measurement_flag;

struct key_obj{
   GPIO_PinState trig_condition; //Basic Information
   GPIO_Type* GPIO_key; //GPIO Information
   uint32_t GPIO_PIN_KEY;
   uint8_t detect_buf;
   uint8_t measurement_flag;
   void (*State_judge_function)(struct key_obj* key_obj);
   void (*Key_value_measurement_function)(struct key_obj* key_obj);
 };

extern struct key_obj *list_key[];
extern uint8_t total_num;

static void key_value_measurement_function_for_KEY1(struct key_obj* key_obj);
static void key_value_measurement_function_for_KEY2(struct key_obj* key_obj);
static void key_value_measurement_function_for_SW1(struct key_obj* key_obj);
static void key_value_measurement_function_for_SW2(struct key_obj* key_obj);
static void key_value_measurement_function_for_SW3(struct key_obj* key_obj);
static void key_value_measurement_function_for_SW4(struct key_obj* key_obj);

static void general_key_state_judge(struct key_obj* KEY_obj);

void Key_refresh_key_state();

void APPTICK_PIT_HANDLER(void);

#endif

/* KEYSCAN_H_ */
