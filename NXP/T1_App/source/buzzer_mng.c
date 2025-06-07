/*
 * buzzer_mng.c
 *
 *  Created on: 07-Jun-2025
 *      Author: xpress_embedo
 */

#include "buzzer_mng.h"
#include "pin_mux.h"
#include "fsl_port.h"
#include "fsl_clock.h"
#include "fsl_ftm.h"

/*--------------------------------- Macros -----------------------------------*/
#define BUZZ_FTM_CLOCK                                  (kCLOCK_CoreSysClk)
#define BUZZ_FTM_CLOCK_SRC                              (kFTM_SystemClock)
#define BUZZ_FTM                                        (FTM1)      // FTM1 Channel-3 is used for Buzzer
#define BUZZ_FTM_CHANNEL                                (kFTM_Chnl_3)
#define BUZZ_FTM_PWM_FREQ                               (4000u)     // 4KHz
#define BUZZ_FTM_DUTY_CYCLE                             (50u)       // 50% duty cycle

#define BUZZ_PIN_CLOCK_IP_NAME                          (kCLOCK_PortC)
#define BUZZ_PIN_HW_PORT                                (BOARD_INITPINS_BUZZER_PORT)
#define BUZZ_PIN_HW_PIN                                 (BOARD_INITPINS_BUZZER_PIN)
#define BUZZ_PIN_HW_PIN_MASK                            (BOARD_INITPINS_BUZZER_PIN_MASK)


/*--------------------------- Private Variables ------------------------------*/

/*------------------------- Private Function Prototypes ----------------------*/

/*-------------------------- Public Function Definitions ---------------------*/
void Buzzer_Init( void )
{
  uint32_t clock;
  ftm_config_t ftm_config;
  ftm_chnl_pwm_signal_param_t ftm_param;
  
  // enable the buzzer pin clock
  CLOCK_EnableClock( BUZZ_PIN_CLOCK_IP_NAME );
  // configure the port multiplexer to configure the PTC15 as FTM1 CH3
  // this can be checked in 4.1 KE1xZ Signal Multiplexing and Pin Assignments of datasheet
  PORT_SetPinMux( BUZZ_PIN_HW_PORT, BUZZ_PIN_HW_PIN, kPORT_MuxAlt2 );
  
  // get the default FTM configuration
  FTM_GetDefaultConfig( &ftm_config );
  
  clock = CLOCK_GetFreq( BUZZ_FTM_CLOCK );
  // calculate the clock division based on the PWM frequency to be obtained
  ftm_config.prescale = FTM_CalculateCounterClkDiv( BUZZ_FTM, BUZZ_FTM_PWM_FREQ, clock );

  // Initialize the FTM module
  FTM_Init( BUZZ_FTM, &ftm_config );

  // configure the FTM parameter for buzzer PWM frequency
  ftm_param.chnlNumber            = BUZZ_FTM_CHANNEL;
  ftm_param.dutyCyclePercent      = BUZZ_FTM_DUTY_CYCLE;
  ftm_param.level                 = kFTM_HighTrue;
  ftm_param.firstEdgeDelayPercent = 0;
  ftm_param.enableComplementary   = false;
  ftm_param.enableDeadtime        = false;

  // setup PWM for the FTM channel
  if( FTM_SetupPwm( BUZZ_FTM, &ftm_param, 1u, kFTM_CenterAlignedPwm, BUZZ_FTM_PWM_FREQ, clock) != kStatus_Success )
  {
    // PWM setup fail, stay here
    while(1);
  }

  // stop timer
  Buzzer_PwmStop();
}

void Buzzer_PwmStart( void )
{
  // Start the timer
  // Note: this is bit confusion BUZZ_FTM_CLOCK and BUZZ_FTM_CLOCK_SRC
  // initially I used the same at both places and things were not working
  FTM_StartTimer( BUZZ_FTM, BUZZ_FTM_CLOCK_SRC );
}

void Buzzer_PwmStop( void )
{
  // Stop the timer
  FTM_StopTimer( BUZZ_FTM );
}
