/*
 * display_mng.h
 *
 *  Created on: 06-Jun-2025
 *      Author: xpress_embedo
 */

#ifndef DISPLAY_MNG_H_
#define DISPLAY_MNG_H_

#include "base_ref.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "pin_mux.h"

/*--------------------------------- Macros -----------------------------------*/
#define OUT_LINE(f)                     Display_OutLine(f##_GPIO, f##_PORT, f##_PIN)
#define SET_LINE(f)                     GPIO_PinWrite(f##_GPIO, f##_PIN, true)
#define CLR_LINE(f)                     GPIO_PinWrite(f##_GPIO, f##_PIN, false)

// LEDs Row Configuration
#define HD_ROW1_GPIO                    (BOARD_INITPINS_HD_ROW1_GPIO)
#define HD_ROW1_PORT                    (BOARD_INITPINS_HD_ROW1_PORT)
#define HD_ROW1_PIN                     (BOARD_INITPINS_HD_ROW1_PIN)
#define HD_ROW1_PIN_MASK                (BOARD_INITPINS_HD_ROW1_GPIO_PIN_MASK)

#define HD_ROW2_GPIO                    (BOARD_INITPINS_HD_ROW2_GPIO)
#define HD_ROW2_PORT                    (BOARD_INITPINS_HD_ROW2_PORT)
#define HD_ROW2_PIN                     (BOARD_INITPINS_HD_ROW2_PIN)
#define HD_ROW2_PIN_MASK                (BOARD_INITPINS_HD_ROW2_GPIO_PIN_MASK)

#define HD_ROW3_GPIO                    (BOARD_INITPINS_HD_ROW3_GPIO)
#define HD_ROW3_PORT                    (BOARD_INITPINS_HD_ROW3_PORT)
#define HD_ROW3_PIN                     (BOARD_INITPINS_HD_ROW3_PIN)
#define HD_ROW3_PIN_MASK                (BOARD_INITPINS_HD_ROW3_GPIO_PIN_MASK)

#define HD_ROW4_GPIO                    (BOARD_INITPINS_HD_ROW4_GPIO)
#define HD_ROW4_PORT                    (BOARD_INITPINS_HD_ROW4_PORT)
#define HD_ROW4_PIN                     (BOARD_INITPINS_HD_ROW4_PIN)
#define HD_ROW4_PIN_MASK                (BOARD_INITPINS_HD_ROW4_GPIO_PIN_MASK)

#define HD_ROW5_GPIO                    (BOARD_INITPINS_HD_ROW5_GPIO)
#define HD_ROW5_PORT                    (BOARD_INITPINS_HD_ROW5_PORT)
#define HD_ROW5_PIN                     (BOARD_INITPINS_HD_ROW5_PIN)
#define HD_ROW5_PIN_MASK                (BOARD_INITPINS_HD_ROW5_GPIO_PIN_MASK)

#define HD_ROW6_GPIO                    (BOARD_INITPINS_HD_ROW6_GPIO)
#define HD_ROW6_PORT                    (BOARD_INITPINS_HD_ROW6_PORT)
#define HD_ROW6_PIN                     (BOARD_INITPINS_HD_ROW6_PIN)
#define HD_ROW6_PIN_MASK                (BOARD_INITPINS_HD_ROW6_GPIO_PIN_MASK)

// LEDs Column Configuration
#define HD_COL1_GPIO                    (BOARD_INITPINS_HD_COL1_GPIO)
#define HD_COL1_PORT                    (BOARD_INITPINS_HD_COL1_PORT)
#define HD_COL1_PIN                     (BOARD_INITPINS_HD_COL1_PIN)
#define HD_COL1_PIN_MASK                (BOARD_INITPINS_HD_COL1_GPIO_PIN_MASK)

#define HD_COL2_GPIO                    (BOARD_INITPINS_HD_COL2_GPIO)
#define HD_COL2_PORT                    (BOARD_INITPINS_HD_COL2_PORT)
#define HD_COL2_PIN                     (BOARD_INITPINS_HD_COL2_PIN)
#define HD_COL2_PIN_MASK                (BOARD_INITPINS_HD_COL2_GPIO_PIN_MASK)

#define HD_COL3_GPIO                    (BOARD_INITPINS_HD_COL3_GPIO)
#define HD_COL3_PORT                    (BOARD_INITPINS_HD_COL3_PORT)
#define HD_COL3_PIN                     (BOARD_INITPINS_HD_COL3_PIN)
#define HD_COL3_PIN_MASK                (BOARD_INITPINS_HD_COL3_GPIO_PIN_MASK)

#define HD_COL4_GPIO                    (BOARD_INITPINS_HD_COL4_GPIO)
#define HD_COL4_PORT                    (BOARD_INITPINS_HD_COL4_PORT)
#define HD_COL4_PIN                     (BOARD_INITPINS_HD_COL4_PIN)
#define HD_COL4_PIN_MASK                (BOARD_INITPINS_HD_COL4_GPIO_PIN_MASK)

#define HD_COL5_GPIO                    (BOARD_INITPINS_HD_COL5_GPIO)
#define HD_COL5_PORT                    (BOARD_INITPINS_HD_COL5_PORT)
#define HD_COL5_PIN                     (BOARD_INITPINS_HD_COL5_PIN)
#define HD_COL5_PIN_MASK                (BOARD_INITPINS_HD_COL5_GPIO_PIN_MASK)

// SW01, SW02, SW03, SW04, SW05, SW06, SW07, SW12, SW13, SW14, SW15, SW16, SW17
// SW18, SW19, SW20, SW21, SW22, these keys has three LEDs (direct and shift register)
// on top of it. (these are in total 18 keys)
#define TOTAL_NUM_SW_LEDS               (18u)
#define TOTAL_NUM_LEDS_PER_KEY          (3u)

/*-------------------------Public Function Prototypes ------------------------*/
void Display_Init( void );
void Display_Mng( void );
void Display_Update( void );
void Display_OutLine( GPIO_Type *gpio, PORT_Type *port, uint32_t pin );

#endif /* DISPLAY_MNG_H_ */
