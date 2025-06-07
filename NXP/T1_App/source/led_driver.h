/*
 * led_driver.h
 *
 *  Created on: 07-Jun-2025
 *      Author: xpress_embedo
 */

#ifndef LED_DRIVER_H_
#define LED_DRIVER_H_

#include "base_ref.h"

/*--------------------------------- Macros -----------------------------------*/



/*-------------------------Public Function Prototypes ------------------------*/
void LedDriver_Init( void );
void LedDriver_Write( uint16_t data_to_transmit );

#endif /* LED_DRIVER_H_ */
