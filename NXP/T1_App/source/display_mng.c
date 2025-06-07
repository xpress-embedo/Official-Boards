/*
 * display_mng.c
 *
 *  Created on: 06-Jun-2025
 *      Author: xpress_embedo
 */

#include "display_mng.h"
#include "led_driver.h"

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

// Mask Definitions for 7 segments
/*

   -----------------
  |       0         |
  |   -----------   |
  |  |           |  |
  | 5|           |1 |
  |  |     6     |  |
  |   -----------   |
  |  |           |  |
  | 4|           |2 |
  |  |     3     |  |
  |   -----------   |
  |                 |
   -----------------
        A
      F   B
        G 
      E   C
        D
*/
// Basic 7-segment masks
#define SEG7_A_MSK                                (0x01)
#define SEG7_B_MSK                                (0x02)
#define SEG7_C_MSK                                (0x04)
#define SEG7_D_MSK                                (0x08)
#define SEG7_E_MSK                                (0x10)
#define SEG7_F_MSK                                (0x20)
#define SEG7_G_MSK                                (0x40)

// Character and Symbol Mask Definition for 7 segment
#define SEG7_DIG_NONE_MSK   ( 0)
#define SEG7_DIG_0_MSK      ( SEG7_A_MSK | SEG7_B_MSK | SEG7_C_MSK | SEG7_D_MSK | SEG7_E_MSK | SEG7_F_MSK )
#define SEG7_DIG_1_MSK      ( SEG7_B_MSK | SEG7_C_MSK )
#define SEG7_DIG_2_MSK      ( SEG7_A_MSK | SEG7_B_MSK | SEG7_D_MSK | SEG7_E_MSK | SEG7_G_MSK )
#define SEG7_DIG_3_MSK      ( SEG7_A_MSK | SEG7_B_MSK | SEG7_C_MSK | SEG7_D_MSK | SEG7_G_MSK )
#define SEG7_DIG_4_MSK      ( SEG7_B_MSK | SEG7_C_MSK | SEG7_F_MSK | SEG7_G_MSK )
#define SEG7_DIG_5_MSK      ( SEG7_A_MSK | SEG7_C_MSK | SEG7_D_MSK | SEG7_F_MSK | SEG7_G_MSK )
#define SEG7_DIG_6_MSK      ( SEG7_A_MSK | SEG7_C_MSK | SEG7_D_MSK | SEG7_E_MSK | SEG7_F_MSK | SEG7_G_MSK )
#define SEG7_DIG_7_MSK      ( SEG7_A_MSK | SEG7_B_MSK | SEG7_C_MSK )
#define SEG7_DIG_8_MSK      ( SEG7_A_MSK | SEG7_B_MSK | SEG7_C_MSK | SEG7_D_MSK | SEG7_E_MSK | SEG7_F_MSK | SEG7_G_MSK )
#define SEG7_DIG_9_MSK      ( SEG7_A_MSK | SEG7_B_MSK | SEG7_C_MSK | SEG7_D_MSK | SEG7_F_MSK | SEG7_G_MSK )
#define SEG7_DIG_A_MSK      ( SEG7_A_MSK | SEG7_B_MSK | SEG7_C_MSK | SEG7_E_MSK | SEG7_F_MSK | SEG7_G_MSK )
#define SEG7_DIG_b_MSK      ( SEG7_C_MSK | SEG7_D_MSK | SEG7_E_MSK | SEG7_F_MSK | SEG7_G_MSK )
#define SEG7_DIG_c_MSK      ( SEG7_D_MSK | SEG7_E_MSK | SEG7_G_MSK )
#define SEG7_DIG_C_MSK      ( SEG7_A_MSK | SEG7_D_MSK | SEG7_E_MSK | SEG7_F_MSK )
#define SEG7_DIG_d_MSK      ( SEG7_B_MSK | SEG7_C_MSK | SEG7_D_MSK | SEG7_E_MSK | SEG7_G_MSK )
#define SEG7_DIG_E_MSK      ( SEG7_A_MSK | SEG7_D_MSK | SEG7_E_MSK | SEG7_F_MSK | SEG7_G_MSK )
#define SEG7_DIG_F_MSK      ( SEG7_A_MSK | SEG7_E_MSK | SEG7_F_MSK | SEG7_G_MSK )
#define SEG7_DIG_h_MSK      ( SEG7_C_MSK | SEG7_E_MSK | SEG7_F_MSK | SEG7_G_MSK )
#define SEG7_DIG_H_MSK      ( SEG7_B_MSK | SEG7_C_MSK | SEG7_E_MSK | SEG7_F_MSK | SEG7_G_MSK )
#define SEG7_DIG_I_MSK      ( SEG7_DIG_1_MSK )
#define SEG7_DIG_L_MSK      ( SEG7_D_MSK | SEG7_E_MSK | SEG7_F_MSK )
#define SEG7_DIG_n_MSK      ( SEG7_C_MSK | SEG7_E_MSK | SEG7_G_MSK )
#define SEG7_DIG_o_MSK      ( SEG7_C_MSK | SEG7_D_MSK | SEG7_E_MSK | SEG7_G_MSK )
#define SEG7_DIG_P_MSK      ( SEG7_A_MSK | SEG7_B_MSK | SEG7_E_MSK | SEG7_F_MSK | SEG7_G_MSK )
#define SEG7_DIG_r_MSK      ( SEG7_E_MSK | SEG7_G_MSK )
#define SEG7_DIG_S_MSK      ( SEG7_DIG_5_MSK )
#define SEG7_DIG_t_MSK      ( SEG7_D_MSK | SEG7_E_MSK | SEG7_F_MSK | SEG7_G_MSK )
#define SEG7_DIG_u_MSK      ( SEG7_C_MSK | SEG7_D_MSK | SEG7_E_MSK )
#define SEG7_DIG_U_MSK      ( SEG7_B_MSK | SEG7_C_MSK | SEG7_D_MSK | SEG7_E_MSK | SEG7_F_MSK )

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

// LED_ASSIST_MAX and TOTAL_NUM_ASSIST_LEDS should be equal and can be used interchangeably
static const uint8_t TAB_LED_ASSIST_ROW[TOTAL_NUM_ASSIST_LEDS] = 
{
  HD_ROW5_IDX,        // LED ASSIST_FOOD PROBE        (SY1-1)
  HD_ROW6_IDX,        // LED ASSIST CONNECTIVITY      (SY1-2)
  HD_ROW6_IDX,        // LED ASSIST FILTER            (SY1-3)
  HD_ROW5_IDX,        // LED ASSIST_TIMER_Z2          (ST1-1)
  HD_ROW6_IDX,        // LED ASSIST TIMER Z1          (ST1-2)
  HD_ROW5_IDX,        // LED ASSIST COLON             (ST6-1)
  HD_ROW6_IDX,        // LED ASSIST HOUR              (ST6-2)
  HD_ROW1_IDX,        // LED ASSIST DEG CELSIUS       (SY4-1)   This led is not visible due to the light guide
  HD_ROW5_IDX,        // LED ASSIST MIN               (SY4-2)
  HD_ROW5_IDX,        // LED_ASSIST TIMER Z4          (ST3-1)
  HD_ROW6_IDX,        // LED ASSIST TIMER Z3          (ST3-2)
  HD_ROW5_IDX,        // LED ASSIST HOOD FAN          (SY3-1)
  HD_ROW5_IDX,        // LED ASSIST HOOD BREEZE       (SY3-2)
  HD_ROW6_IDX,        // LED_ASSIST_HOOD AUTO         (SY3-3)
};

static const uint8_t TAB_LED_ASSIST_COL[TOTAL_NUM_ASSIST_LEDS] = 
{
  HD_COL04_IDX,       // LED ASSIST FOOD PROBE        (SY1-1)
  HD_COL04_IDX,       // LED ASSIST CONNECTIVITY      (SY1-2)
  HD_COL03_IDX,       // LED ASSIST FILTER            (SY1-3)
  HD_COL06_IDX,       // LED ASSIST TIMER Z2          (ST1-1)
  HD_COL06_IDX,       // LED ASSIST TIMER Z1          (ST1-2)
  HD_COL05_IDX,       // LED ASSIST COLON             (ST6-1)
  HD_COL05_IDX,       // LED ASSIST HOUR              (ST6-2)
  HD_COL18_IDX,       // LED ASSIST DEG CELSIUS       (SY4-1)
  HD_COL03_IDX,       // LED ASSIST MIN               (SY4-2)
  HD_COL07_IDX,       // LED ASSIST TIMER Z4          (ST3-1)
  HD_COL07_IDX,       // LED ASSIST TIMER Z3          (ST3-2)
  HD_COL01_IDX,       // LED ASSIST HOOD FAN          (SY3-1)
  HD_COL02_IDX,       // LED ASSIST HOOD BREEZE       (SY3-2)
  HD_COL01_IDX,       // LED ASSIST HOOD AUTO         (SY3-3)
};

static const uint8_t TAB_7SEG_ZONE_ROW[TOTAL_NUM_ZONE_SEGMENTS][TOTAL_NUM_LEDS_PER_ZONE] = 
{
  /*Led-1(A)      Led-2(B)      Led-3(C)      Led-4(D)      Led-5(E)      Led-6(F)      Led-7(G)  */
  { HD_ROW5_IDX,  HD_ROW5_IDX,  HD_ROW5_IDX,  HD_ROW5_IDX,  HD_ROW6_IDX,  HD_ROW6_IDX,  HD_ROW6_IDX   },  // ZONE-1
  { HD_ROW6_IDX,  HD_ROW6_IDX,  HD_ROW6_IDX,  HD_ROW6_IDX,  HD_ROW5_IDX,  HD_ROW5_IDX,  HD_ROW5_IDX   },  // ZONE-2
  { HD_ROW5_IDX,  HD_ROW5_IDX,  HD_ROW5_IDX,  HD_ROW5_IDX,  HD_ROW6_IDX,  HD_ROW6_IDX,  HD_ROW6_IDX   },  // ZONE-3
  { HD_ROW6_IDX,  HD_ROW6_IDX,  HD_ROW6_IDX,  HD_ROW6_IDX,  HD_ROW5_IDX,  HD_ROW5_IDX,  HD_ROW5_IDX   },  // ZONE-4
};

static const uint8_t TAB_7SEG_ZONE_COL[TOTAL_NUM_ZONE_SEGMENTS][TOTAL_NUM_LEDS_PER_ZONE] = 
{
  /*Led-1(A)      Led-2(B)      Led-3(C)      Led-4(D)      Led-5(E)      Led-6(F)      Led-7(G)  */
  { HD_COL08_IDX, HD_COL09_IDX, HD_COL10_IDX, HD_COL11_IDX, HD_COL08_IDX, HD_COL09_IDX, HD_COL10_IDX  },  // ZONE-1
  { HD_COL11_IDX, HD_COL12_IDX, HD_COL13_IDX, HD_COL14_IDX, HD_COL12_IDX, HD_COL13_IDX, HD_COL14_IDX  },  // ZONE-2
  { HD_COL15_IDX, HD_COL16_IDX, HD_COL17_IDX, HD_COL18_IDX, HD_COL15_IDX, HD_COL16_IDX, HD_COL17_IDX  },  // ZONE-3
  { HD_COL18_IDX, HD_COL19_IDX, HD_COL20_IDX, HD_COL21_IDX, HD_COL19_IDX, HD_COL20_IDX, HD_COL21_IDX  },  // ZONE-4
};

static const uint8_t TAB_7SEG_TIMER_ROW[TOTAL_NUM_TIMER_SEGMENTS][TOTAL_NUM_LEDS_PER_TIMER] = 
{
  /*Led-1(A)      Led-2(B)      Led-3(C)      Led-4(D)      Led-5(E)      Led-6(F)      Led-7(G)  */
  { HD_ROW2_IDX,  HD_ROW3_IDX,  HD_ROW4_IDX,  HD_ROW1_IDX,  HD_ROW2_IDX,  HD_ROW3_IDX,  HD_ROW4_IDX   },  // T1
  { HD_ROW2_IDX,  HD_ROW3_IDX,  HD_ROW4_IDX,  HD_ROW1_IDX,  HD_ROW2_IDX,  HD_ROW3_IDX,  HD_ROW4_IDX   },  // T2
  { HD_ROW2_IDX,  HD_ROW3_IDX,  HD_ROW4_IDX,  HD_ROW1_IDX,  HD_ROW2_IDX,  HD_ROW3_IDX,  HD_ROW4_IDX   },  // T3
};

static const uint8_t TAB_7SEG_TIMER_COL[TOTAL_NUM_TIMER_SEGMENTS][TOTAL_NUM_LEDS_PER_TIMER] = 
{
  /*Led-1(A)      Led-2(B)      Led-3(C)      Led-4(D)      Led-5(E)      Led-6(F)      Led-7(G)  */
  { HD_COL16_IDX, HD_COL16_IDX, HD_COL16_IDX, HD_COL17_IDX, HD_COL17_IDX, HD_COL17_IDX, HD_COL17_IDX  },  // T1
  { HD_COL18_IDX, HD_COL18_IDX, HD_COL18_IDX, HD_COL19_IDX, HD_COL19_IDX, HD_COL19_IDX, HD_COL19_IDX  },  // T2
  { HD_COL20_IDX, HD_COL20_IDX, HD_COL20_IDX, HD_COL21_IDX, HD_COL21_IDX, HD_COL21_IDX, HD_COL21_IDX  },  // T3
};

// Seven Segment Character/Symbol Table
static const char TAB_CHAR_MSK[SEG7_DIG_TAB_MAX] = 
{
    SEG7_DIG_NONE_MSK,
    SEG7_DIG_0_MSK,
    SEG7_DIG_1_MSK,
    SEG7_DIG_2_MSK,
    SEG7_DIG_3_MSK,
    SEG7_DIG_4_MSK,
    SEG7_DIG_5_MSK,
    SEG7_DIG_6_MSK,
    SEG7_DIG_7_MSK,
    SEG7_DIG_8_MSK,
    SEG7_DIG_9_MSK,
    SEG7_DIG_A_MSK,
    SEG7_DIG_b_MSK,
    SEG7_DIG_c_MSK,
    SEG7_DIG_C_MSK,
    SEG7_DIG_d_MSK,
    SEG7_DIG_E_MSK,
    SEG7_DIG_F_MSK,
    SEG7_DIG_h_MSK,
    SEG7_DIG_H_MSK,
    SEG7_DIG_I_MSK,
    SEG7_DIG_L_MSK,
    SEG7_DIG_n_MSK,
    SEG7_DIG_o_MSK,
    SEG7_DIG_P_MSK,
    SEG7_DIG_r_MSK,
    SEG7_DIG_S_MSK,
    SEG7_DIG_t_MSK,
    SEG7_DIG_u_MSK,
    SEG7_DIG_U_MSK,
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
static void _Display_Blank( void );
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

 // initialize latch enable signal
 OUT_LINE( HD_SHIFT_LE );
 CLR_LINE( HD_SHIFT_LE );

 // initialize the shift register enable pin, this will turn on the shift register supply
 OUT_LINE( HD_SHIFT_EN );
 CLR_LINE( HD_SHIFT_EN );   // low means supply on

 // output enable is initialized and set to high to display the output at init.
 OUT_LINE( HD_SHIFT_OE );
 SET_LINE( HD_SHIFT_OE );

 // initialize the led driver to send data over SPI bus using FLEXIO module
 LedDriver_Init();

 _Display_Blank();

 row_num = 0;
 column_data = 0;

 // Test Code starts
//  SET_LINE( HD_COL5 );
//  CLR_LINE( HD_ROW1 );
//  CLR_LINE( HD_ROW2 );
//  CLR_LINE( HD_ROW3 );
//  CLR_LINE( HD_ROW4 );
//  CLR_LINE( HD_ROW5 );
//  CLR_LINE( HD_ROW6 );
 // Test Code ends
}

void Display_Mng( void )
{
  uint8_t idx;

  DIS_INT();
  for( idx=0; idx<MAX_ROWS; idx++ )
  {
    int_display_buffer[idx] = display_buffer[idx];
  }
  EN_INT();
}

// This function should be called @1ms hence called from 1ms interrupt ISR
// if this function at the rate > 1ms, let's say @10ms we will see fluctuation
void Display_Update( void )
{
  uint8_t old_row_num = 0;

  #define DISPLAY_UPDATE_RATE               (2u)
  static uint8_t update_counter = 0u;
  update_counter++;

  if( update_counter >= DISPLAY_UPDATE_RATE )
  {
    update_counter = 0;

    // Display Update Logic Starts from Here
    old_row_num = row_num;
    row_num += 1;
    if( row_num >= MAX_ROWS )
    {
      row_num = 0;
    }

    // enable the shift register power supply
    // note: it is already enabled & not disabled, but still displaying again
    CLR_LINE( HD_SHIFT_EN );

    // disable the output of the shift register
    SET_LINE( HD_SHIFT_OE );
    
    // display all rows and columns
    _Display_Blank();
    // this will be used for SPI data for Shift Register and extract direct LED Data
    _Display_Refresh( int_display_buffer[old_row_num] );

    // display refresh function above sen the data to shift register
    // now we need to give the latch enable signal high to low
    // when low signal is received, the data input reaches the shift register output
    SET_LINE( HD_SHIFT_LE );

    // enable row after transmitting the column data
    switch ( old_row_num )
    {
      case 0: CLR_LINE( HD_ROW1 );  break;
      case 1: CLR_LINE( HD_ROW2 );  break;
      case 2: CLR_LINE( HD_ROW3 );  break;
      case 3: CLR_LINE( HD_ROW4 );  break;
      case 4: CLR_LINE( HD_ROW5 );  break;
      case 5: CLR_LINE( HD_ROW6 );  break;
      default:                      break;
    }

    // enable column data (not the shift register data)
    if( column_data & 0x01 )          SET_LINE( HD_COL1 );
    if( column_data & 0x02 )          SET_LINE( HD_COL2 );
    if( column_data & 0x04 )          SET_LINE( HD_COL3 );
    if( column_data & 0x08 )          SET_LINE( HD_COL4 );
    if( column_data & 0x10 )          SET_LINE( HD_COL5 );

    // disabling the latch pulse now
    CLR_LINE( HD_SHIFT_LE );
    // enabling the output enable of the shift register
    CLR_LINE( HD_SHIFT_OE );
    
    // Display Update Logic Ends Here
  }
}

void Display_SetKeyLed( uint8_t key_idx, uint8_t led_idx )
{
  uint32_t temp_row, temp_col;

  // function should return if invalid led_idx or key_idx is provided
  if( led_idx >= TOTAL_NUM_LEDS_PER_KEY )
    return;
  if( key_idx >= TOTAL_NUM_SW_LEDS )
    return;

  temp_row = TAB_LED_KEY_STATUS_ROW[key_idx][led_idx];
  temp_col = TAB_LED_KEY_STATUS_COL[key_idx][led_idx];

  display_buffer[ temp_row ] |= (0x01 << temp_col);
}

void Display_ClearKeyLed( uint8_t key_idx, uint8_t led_idx )
{
  uint32_t temp_row, temp_col;

  // function should return if invalid led_idx or key_idx is provided
  if( led_idx >= TOTAL_NUM_LEDS_PER_KEY )
    return;
  if( key_idx >= TOTAL_NUM_SW_LEDS )
    return;

  temp_row = TAB_LED_KEY_STATUS_ROW[key_idx][led_idx];
  temp_col = TAB_LED_KEY_STATUS_COL[key_idx][led_idx];

  display_buffer[ temp_row ] &= ~(0x01 << temp_col);
}

void Display_SetAssistLed( led_assist_map_e assist_led )
{
  uint32_t temp_row, temp_col;

  if( assist_led >= TOTAL_NUM_ASSIST_LEDS )
    return;

  temp_row = TAB_LED_ASSIST_ROW[assist_led];
  temp_col = TAB_LED_ASSIST_COL[assist_led];

  display_buffer[ temp_row ] |= (0x01 << temp_col);
}

void Display_ClearAssistLed( led_assist_map_e assist_led )
{
  uint32_t temp_row, temp_col;

  if( assist_led >= TOTAL_NUM_ASSIST_LEDS )
    return;

  temp_row = TAB_LED_ASSIST_ROW[assist_led];
  temp_col = TAB_LED_ASSIST_COL[assist_led];

  display_buffer[ temp_row ] &= ~(0x01 << temp_col);
}

void Display_SetZone7Segment( uint8_t zone_idx, seg7_digit_e char_idx )
{
  uint8_t segment_mask;
  uint8_t segment;
  uint32_t temp_row, temp_col;

  // checking for valid values (boundary conditions)
  if( ( zone_idx < ZONE_MAX ) && (char_idx < SEG7_DIG_TAB_MAX) )
  {
    segment_mask = TAB_CHAR_MSK[char_idx];
    for( segment=0; segment<7; segment++ )
    {
      temp_row = TAB_7SEG_ZONE_ROW[zone_idx][segment];
      temp_col = TAB_7SEG_ZONE_COL[zone_idx][segment];
      if( segment_mask & (0x01<<segment) )
      {
        display_buffer[temp_row] |= (0x01 << temp_col);
      }
      else
      {
        display_buffer[temp_row] &= ~(0x01 << temp_col);
      }
    }
  }
}

void Display_SetTimer7Segment( uint8_t timer_idx, seg7_digit_e char_idx )
{
  uint8_t segment_mask;
  uint8_t segment;
  uint32_t temp_row, temp_col;

  // checking for valid values (boundary conditions)
  if( ( timer_idx < TIMER_IDX_MAX ) && (char_idx < SEG7_DIG_TAB_MAX) )
  {
    segment_mask = TAB_CHAR_MSK[char_idx];
    for( segment=0; segment<7; segment++ )
    {
      temp_row = TAB_7SEG_TIMER_ROW[timer_idx][segment];
      temp_col = TAB_7SEG_TIMER_COL[timer_idx][segment];
      if( segment_mask & (0x01<<segment) )
      {
        display_buffer[temp_row] |= (0x01 << temp_col);
      }
      else
      {
        display_buffer[temp_row] &= ~(0x01 << temp_col);
      }
    }
  }
}

void Display_SetTime( uint32_t seconds )
{
  /*
  * we have three seven segments, and we have the possibility to either display
  * hour and minutes, or minutes and seconds
  * when displaying hours and minutes, the maximum value that can be displayed 
  * is 9 hours 59 minutes
  * Similarly when displaying minutes and seconds, the maximum value that can
  * be reached is 9 minutes and 59 seconds.
  * This function takes input as seconds, when seconds is less than or equal to
  * 9 minutes 59 seconds, then we display minutes and seconds
  * If greater than 9 minutes 59 seconds, we display in hours and minutes
  */
  #define SECS_IN_MIN                                 (60u)   // seconds in 1 minute
  #define MINS_IN_HOUR                                (60u)   // minutes in 1 hour
  #define THRESHOLD_TIME_MIN_DISP                     (9u*SECS_IN_MIN + 59u)    // threshold time is 9 minutes 59 seconds
 
  static uint8_t col_tog = false;
  uint8_t true_hours = 0;
  uint8_t true_mins = 0;
  uint8_t true_seconds = 0;
  uint8_t t1_char_idx, t2_char_idx, t3_char_idx;

  // colon led should toggle twice in a second, this is handled by calling this
  // function from every 500ms, and is handled in main file
  if( col_tog )
  {
    Display_ClearAssistLed( LED_ASSIST_COLON );
    col_tog = false;
  }
  else
  {
    Display_SetAssistLed( LED_ASSIST_COLON );
    col_tog = true;
  }

  if( seconds <= THRESHOLD_TIME_MIN_DISP )
  {
    // we display in minutes and seconds
    true_seconds = (uint8_t)(seconds % SECS_IN_MIN);
    true_mins = (uint8_t) (seconds / SECS_IN_MIN);
    
    // now displaying this data on timer segments
    // seconds can have maximum two digits i.e. from 0 to 59, and here we need
    // to separate these digits, and then display on t3 and t2, t3 will display
    // the one's part and t2 will display then ten's part
    t3_char_idx = (true_seconds % 10u);     // extracting one's part
    // here added one or SEG7_DIG_0_TAB_IDX because at 0th index we have value
    // to display nothing, hence we have to increment the part to display
    t3_char_idx = t3_char_idx + SEG7_DIG_0_TAB_IDX;
    Display_SetTimer7Segment( TIMER_3_IDX, t3_char_idx );
    
    t2_char_idx = (true_seconds / 10u);     // extracting ten's part
    // here added one or SEG7_DIG_0_TAB_IDX because at 0th index we have value
    // to display nothing, hence we have to increment the part to display
    t2_char_idx = t2_char_idx + SEG7_DIG_0_TAB_IDX;
    Display_SetTimer7Segment( TIMER_2_IDX, t2_char_idx );
    
    // minutes will always remain below 9, and add 1 or SEG7_DIG_0_TAB_IDX due to
    // the same reason
    t1_char_idx = (true_mins + SEG7_DIG_0_TAB_IDX );
    Display_SetTimer7Segment( TIMER_1_IDX, t1_char_idx);
    
    // turn on minute led and clear the hour led
    Display_SetAssistLed( LED_ASSIST_MINUTE );
    Display_ClearAssistLed( LED_ASSIST_HOUR );
  }
  else
  {
    uint32_t remaining_secs;
    // we display in hours and minutes
    true_hours = (uint8_t)( seconds/(SECS_IN_MIN*MINS_IN_HOUR) );
    remaining_secs = (seconds%(SECS_IN_MIN*MINS_IN_HOUR) );
    true_mins = (uint8_t) (remaining_secs / SECS_IN_MIN);

    t3_char_idx = (true_mins % 10u);    // extracting one's part
    t3_char_idx = t3_char_idx + SEG7_DIG_0_TAB_IDX;
    Display_SetTimer7Segment( TIMER_3_IDX, t3_char_idx );

    t2_char_idx = (true_mins / 10u);    // extracting ten's part
    t2_char_idx = t2_char_idx + SEG7_DIG_0_TAB_IDX;
    Display_SetTimer7Segment( TIMER_2_IDX, t2_char_idx );

    t1_char_idx = (true_hours + SEG7_DIG_0_TAB_IDX );
    Display_SetTimer7Segment( TIMER_1_IDX, t1_char_idx );

    // turn off minute led and on the hour led
    Display_SetAssistLed( LED_ASSIST_HOUR );
    Display_ClearAssistLed( LED_ASSIST_MINUTE );
  }
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
static void _Display_Blank( void )
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

static void _Display_Refresh( uint32_t complete_column_data )
{
  uint16_t spi_data;

  // save data in column_data variable which needs to driven directly
  // 0x1F is used because we have only 5 columns which are driven directly
  column_data = (complete_column_data & 0x1F);

  spi_data = ((complete_column_data & 0x001FFFE0) >> 5u);

  // SPI send to shift registers needs to be updated (todo)
  LedDriver_Write(spi_data);
}

