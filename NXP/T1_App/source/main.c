/*
 * Copyright 2016-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    T1_App.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "main.h"
#include "display_mng.h"
#include "debug_mng.h"
/* TODO: insert other definitions and declarations here. */


/*------------------------------Private Macros -------------------------------*/
#define SYSTICK_TIMER_PERIOD_MS                       (1000u)
#define TASK_10_MSEC                                  (10u)
#define TASK_100_MSEC                                 (100u)
#define TASK_1_SEC                                    (1000u)
#define TASK_10_SEC                                   (10000u)

// This macro allow helper variables to turn on and off the LEDs, comment this
// macro to disable the helper variables and function, and save some memory space
#define SIMULATE_LED_ON_OFF
// comment this line to switch off all switch keys LEDs animation
#define KEY_LED_ON_OFF_ANIMATION

/*--------------------------- Private Variables ------------------------------*/
static volatile uint32_t milliseconds = 0;
static uint32_t task_10ms_time;
static uint32_t task_100ms_time;
static uint32_t task_1sec_time;
static uint32_t task_10sec_time;

#ifdef SIMULATE_LED_ON_OFF
static volatile uint8_t set_clear = 0u;
static volatile uint8_t custom_key_idx = TOTAL_NUM_SW_LEDS;
static volatile uint8_t custom_led_idx = TOTAL_NUM_LEDS_PER_KEY;
#endif

/*------------------------- Private Function Prototypes ----------------------*/
static void SysTick_Init( void );
#ifdef SIMULATE_LED_ON_OFF
static void Simulate_LedOnOff( void );
#endif
#ifdef KEY_LED_ON_OFF_ANIMATION
static void Simulate_KeyLed_Animation( void );
#endif

/*
 * @brief   Application entry point.
 */
int main(void)
{
  uint32_t temp_time = 0;

  /* Init board hardware. */
  BOARD_InitBootPins();
  BOARD_InitBootClocks();
  BOARD_InitBootPeripherals();

  DBG_INIT();
  DBG_PRINTF_GREEN("Program Starting\n");

  SysTick_Init();
  Display_Init();

  temp_time = Millis();
  task_10ms_time = temp_time;
  task_100ms_time = temp_time;
  task_1sec_time = temp_time;
  task_10sec_time = temp_time;

  while(1)
  {
    temp_time = Millis();
    // 10 ms task
    if( temp_time - task_10ms_time >= TASK_10_MSEC )
    {
      task_10ms_time = temp_time;
      Display_Mng();
    }

    // 100 ms task
    if( temp_time - task_100ms_time >= TASK_100_MSEC )
    {
      task_100ms_time = temp_time;
      #ifdef SIMULATE_LED_ON_OFF
      Simulate_LedOnOff();
      #endif
      #ifdef KEY_LED_ON_OFF_ANIMATION
      Simulate_KeyLed_Animation();
      #endif
    }

    // 1 sec task
    if( temp_time - task_1sec_time >= TASK_1_SEC )
    {
      task_1sec_time = temp_time;
      // DBG_PRINTF_GREEN("1sec Task Executing\n");
    }

    // 10 sec task
    if( temp_time - task_10sec_time >= TASK_10_SEC )
    {
      task_10sec_time = temp_time;
      // DBG_PRINTF_MAGENTA("10sec Task Executing\n");
    }
  }

  return 0 ;
}

/*-------------------------- Public Function Definitions ---------------------*/
void SysTick_Handler( void )
{
  DIS_INT();
  milliseconds++;
  EN_INT();
  Display_Update();
}

uint32_t Millis( void )
{
  uint32_t temp = 0;
  DIS_INT();
  temp = milliseconds;
  EN_INT();
  return temp;
}

/*------------------------- Private Function Definitions ---------------------*/
static void SysTick_Init( void )
{
  if( SysTick_Config( SystemCoreClock/SYSTICK_TIMER_PERIOD_MS) )
  {
    // Problem with SysTick Initialization
    DBG_PRINTF_RED("SysTick_Init: Failed\n");
    while(1);
  }
}

#ifdef SIMULATE_LED_ON_OFF
static void Simulate_LedOnOff( void )
{
  if( set_clear == 1 )
  {
    set_clear = 0;
    Display_SetKeyLed(custom_key_idx, custom_led_idx);
    custom_key_idx = TOTAL_NUM_SW_LEDS;
    custom_led_idx = TOTAL_NUM_LEDS_PER_KEY;
  }
  else if( set_clear == 2 )
  {
    set_clear = 0;
    Display_ClearKeyLed(custom_key_idx, custom_led_idx);
    custom_key_idx = TOTAL_NUM_SW_LEDS;
    custom_led_idx = TOTAL_NUM_LEDS_PER_KEY;
  }
}
#endif

#ifdef KEY_LED_ON_OFF_ANIMATION
static void Simulate_KeyLed_Animation( void )
{
  static uint8_t on_off = 0;
  static uint8_t key_idx = 0;
  static uint8_t led_idx = 0;

  // time to on all leds sequentially
  if( on_off == 0 )
  {
    DBG_PRINTF_GREEN("Turning on LED = %d, for Key = %d\n", led_idx, key_idx );
    Display_SetKeyLed( key_idx, led_idx );
    led_idx++;
    // here we have 3 leds per key i.e. TOTAL_NUM_LEDS_PER_KEY
    if( led_idx >= TOTAL_NUM_LEDS_PER_KEY )
    {
      led_idx = 0;
      key_idx++;
      // here we have 18 switches which has leds on top of them i.e. TOTAL_NUM_SW_LEDS
      if( key_idx >= TOTAL_NUM_SW_LEDS )
      {
        key_idx = 0;
        on_off = 1;
      }
    }
  }
  else if( on_off )
  {
    DBG_PRINTF_GREEN("Turning off LED = %d, for Key = %d\n", led_idx, key_idx );
    Display_ClearKeyLed( key_idx, led_idx );
    led_idx++;
    // here we have 3 leds per key i.e. TOTAL_NUM_LEDS_PER_KEY
    if( led_idx >= TOTAL_NUM_LEDS_PER_KEY )
    {
      led_idx = 0;
      key_idx++;
      // here we have 18 switches which has leds on top of them i.e. TOTAL_NUM_SW_LEDS
      if( key_idx >= TOTAL_NUM_SW_LEDS )
      {
        key_idx = 0;
        on_off = 0;
      }
    }
  }
}
#endif
