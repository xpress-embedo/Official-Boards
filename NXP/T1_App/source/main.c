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
/* TODO: insert other definitions and declarations here. */


/*------------------------------Private Macros -------------------------------*/
#define SYSTICK_TIMER_PERIOD_MS                       (1000u)
#define TASK_10_MSEC                                  (10u)
#define TASK_100_MSEC                                 (100u)
#define TASK_1_SEC                                    (1000u)
#define TASK_10_SEC                                   (10000u)

/*--------------------------- Private Variables ------------------------------*/
static volatile uint32_t milliseconds = 0;
static uint32_t task_10ms_time;
static uint32_t task_100ms_time;
static uint32_t task_1sec_time;
static uint32_t task_10sec_time;

/*------------------------- Private Function Prototypes ----------------------*/
static void SysTick_Init( void );

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

  SysTick_Init();
  Display_Init();

  temp_time = Millis();
  task_10ms_time = temp_time;
  task_100ms_time = temp_time;
  task_1sec_time = temp_time;
  task_10sec_time = temp_time;

  while(1)

  temp_time = Millis();
  // 10 ms task
  if( temp_time - task_10ms_time >= TASK_10_MSEC )
  {
    task_10ms_time = temp_time;
  }

  // 100 ms task
  if( temp_time - task_100ms_time >= TASK_100_MSEC )
  {
    task_100ms_time = temp_time;
  }

  // 1 sec task
  if( temp_time - task_1sec_time >= TASK_1_SEC )
  {
    task_1sec_time = temp_time;
  }

  // 10 sec task
  if( temp_time - task_10sec_time >= TASK_10_SEC )
  {
    task_10sec_time = temp_time;
  }

  return 0 ;
}

/*-------------------------- Public Function Definitions ---------------------*/
void SysTick_Handler( void )
{
  DIS_INT();
  milliseconds++;
  EN_INT();
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
    while(1);
  }
}
