/*
 * display_mng.c
 *
 *  Created on: 06-Jun-2025
 *      Author: xpress_embedo
 */

#include "display_mng.h"

/*------------------------------Private Macros -------------------------------*/
#define MAX_ROWS                                  (6u)
#define MAX_COLUMNS                               (21u)   // this includes direct + shift register ones

// Indexes for Rows and Columns
// Rows
#define HD_ROW1_IDX                               (0u)
#define HD_ROW2_IDX                               (1u)
#define HD_ROW3_IDX                               (2u)
#define HD_ROW4_IDX                               (3u)
#define HD_ROW5_IDX                               (4u)
#define HD_ROW6_IDX                               (5u)
// Columns
#define HD_COL01_IDX                              (0u)
#define HD_COL02_IDX                              (1u)
#define HD_COL03_IDX                              (2u)
#define HD_COL04_IDX                              (3u)
#define HD_COL05_IDX                              (4u)
// the above indexes are for direct LEDs and the below are shift register related columns
#define HD_COL06_IDX                              (5u)
#define HD_COL07_IDX                              (6u)
#define HD_COL08_IDX                              (7u)
#define HD_COL09_IDX                              (8u)
#define HD_COL10_IDX                              (9u)
#define HD_COL11_IDX                              (10u)
#define HD_COL12_IDX                              (11u)
#define HD_COL13_IDX                              (12u)
#define HD_COL14_IDX                              (13u)
#define HD_COL15_IDX                              (14u)
#define HD_COL16_IDX                              (15u)
#define HD_COL17_IDX                              (16u)
#define HD_COL18_IDX                              (17u)
#define HD_COL19_IDX                              (18u)
#define HD_COL20_IDX                              (19u)
#define HD_COL21_IDX                              (20u)

/*--------------------------- Private Variables ------------------------------*/
static const uint8_t TAB_LED_KEY_STATUS_ROW[TOTAL_NUM_SW_LEDS][TOTAL_NUM_LEDS_PER_KEY] = 
{
    { HD_ROW3_IDX,    HD_ROW2_IDX,    HD_ROW4_IDX },      // SW01
    { HD_ROW2_IDX,    HD_ROW2_IDX,    HD_ROW2_IDX },      // SW02
    { HD_ROW3_IDX,    HD_ROW3_IDX,    HD_ROW3_IDX },      // SW03
    { HD_ROW1_IDX,    HD_ROW1_IDX,    HD_ROW1_IDX },      // SW04
    { HD_ROW1_IDX,    HD_ROW1_IDX,    HD_ROW1_IDX },      // SW05
    { HD_ROW4_IDX,    HD_ROW4_IDX,    HD_ROW4_IDX },      // SW06
    { HD_ROW3_IDX,    HD_ROW3_IDX,    HD_ROW3_IDX },      // SW07
    { HD_ROW1_IDX,    HD_ROW1_IDX,    HD_ROW1_IDX },      // SW12
    { HD_ROW1_IDX,    HD_ROW1_IDX,    HD_ROW1_IDX },      // SW13
    { HD_ROW4_IDX,    HD_ROW4_IDX,    HD_ROW4_IDX },      // SW14
    { HD_ROW3_IDX,    HD_ROW3_IDX,    HD_ROW3_IDX },      // SW15
    { HD_ROW2_IDX,    HD_ROW2_IDX,    HD_ROW2_IDX },      // SW16
    { HD_ROW4_IDX,    HD_ROW4_IDX,    HD_ROW4_IDX },      // SW17
    { HD_ROW4_IDX,    HD_ROW4_IDX,    HD_ROW4_IDX },      // SW18
    { HD_ROW2_IDX,    HD_ROW2_IDX,    HD_ROW2_IDX },      // SW19
    { HD_ROW1_IDX,    HD_ROW1_IDX,    HD_ROW1_IDX },      // SW20
    { HD_ROW3_IDX,    HD_ROW3_IDX,    HD_ROW3_IDX },      // SW21
    { HD_ROW2_IDX,    HD_ROW2_IDX,    HD_ROW2_IDX },      // SW22
};

static const uint8_t TAB_LED_KEY_STATUS_COL[TOTAL_NUM_SW_LEDS] [TOTAL_NUM_LEDS_PER_KEY] =
{
    { HD_COL05_IDX,   HD_COL05_IDX,   HD_COL05_IDX },     // SW01
    { HD_COL06_IDX,   HD_COL01_IDX,   HD_COL07_IDX },     // SW02
    { HD_COL06_IDX,   HD_COL01_IDX,   HD_COL07_IDX },     // SW03
    { HD_COL14_IDX,   HD_COL05_IDX,   HD_COL15_IDX },     // SW04
    { HD_COL06_IDX,   HD_COL01_IDX,   HD_COL07_IDX },     // SW05
    { HD_COL06_IDX,   HD_COL01_IDX,   HD_COL07_IDX },     // SW06
    { HD_COL12_IDX,   HD_COL04_IDX,   HD_COL13_IDX },     // SW07
    { HD_COL12_IDX,   HD_COL04_IDX,   HD_COL13_IDX },     // SW12
    { HD_COL08_IDX,   HD_COL02_IDX,   HD_COL09_IDX },     // SW13
    { HD_COL08_IDX,   HD_COL02_IDX,   HD_COL09_IDX },     // SW14
    { HD_COL08_IDX,   HD_COL02_IDX,   HD_COL09_IDX },     // SW15
    { HD_COL08_IDX,   HD_COL02_IDX,   HD_COL09_IDX },     // SW16
    { HD_COL12_IDX,   HD_COL04_IDX,   HD_COL13_IDX },     // SW17
    { HD_COL10_IDX,   HD_COL03_IDX,   HD_COL11_IDX },     // SW18
    { HD_COL10_IDX,   HD_COL03_IDX,   HD_COL11_IDX },     // SW19
    { HD_COL10_IDX,   HD_COL03_IDX,   HD_COL11_IDX },     // SW20
    { HD_COL10_IDX,   HD_COL03_IDX,   HD_COL11_IDX },     // SW21
    { HD_COL12_IDX,   HD_COL04_IDX,   HD_COL13_IDX },     // SW22
};

// This buffer contains the data which needs to be updated on display
// Here important thing is only 5 columns are used directly rest of them are
// used with the help of a shift register, so two separate logic needs to be
// integrated together to make things work properly
static uint32_t display_buffer[MAX_ROWS] = { 0 };
// Same buffer, but will be transferred to display in interrupt
static uint32_t int_display_buffer[MAX_ROWS] = { 0 };
static uint8_t row_num = 0;                           // used to track row no while updating on display
static uint8_t column_data = 0;                       // column data to be transmitted

/*------------------------- Private Function Prototypes ----------------------*/
static void_Display_Blank(void);
static void _Display_Refresh( uint32_t complete_column_data );

/*-------------------------- Public Function Definitions ---------------------*/
void Display_Init( void )
{
  /* Display related pins are already initialized, but here doing that again, 
  the reason for this is to remove the dependency from the peripheral.c and .h
  file generated automatically using the MCUXpresso Config tool.
  Apart from this helper macros are created i.e. OUT_LINE, SET_LINE, CLEAR_LINE
  We are also testing them here.
  Logic is Inverted, i.e. LED will be turned on when column is high & row is low
  */
 OUT_LINE( HD_ROW1 );
 OUT_LINE( HD_ROW2 );
 OUT_LINE( HD_ROW3 );
 OUT_LINE( HD_ROW4 );
 OUT_LINE( HD_ROW5 );
 OUT_LINE( HD_ROW6 );
 
 OUT_LINE( HD_COL1 );
 OUT_LINE( HD_COL2 );
 OUT_LINE( HD_COL3 );
 OUT_LINE( HD_COL4 );
 OUT_LINE( HD_COL5 );

 _Display_Blank();

 // Test Code
 CLR_LINE( HD_ROW1 );
 SET_LINE( HD_COL1 );
 SET_LINE( HD_COL2 );
 SET_LINE( HD_COL3 );
 SET_LINE( HD_COL4 );
 SET_LINE( HD_COL5 );
}

void Display_OutLine( GPIO_Type *gpio, PORT_Type *port, uint32_t pin )
{
  const gpio_pin_config_t output_config = 
  {
    .pinDirection = kGPIO_DigitalOutput,
    .outputLogic  = 0u
  };
  GPIO_PinInit( gpio, pin, &output_config );
  PORT_SetPinMux( port, pin, kPORT_MuxAsGpio );
}

/*------------------------- Private Function Definitions ---------------------*/
void _Display_Blank( void )
{
  SET_LINE( HD_ROW1 );
  SET_LINE( HD_ROW2 );
  SET_LINE( HD_ROW3 );
  SET_LINE( HD_ROW4 );
  SET_LINE( HD_ROW5 );
  SET_LINE( HD_ROW6 );
 
  CLR_LINE( HD_COL1 );
  CLR_LINE( HD_COL2 );
  CLR_LINE( HD_COL3 );
  CLR_LINE( HD_COL4 );
  CLR_LINE( HD_COL5 );
}

