/*
 * buzzer_mng.h
 *
 *  Created on: 07-Jun-2025
 *      Author: xpress_embedo
 */

#ifndef BUZZER_MNG_H_
#define BUZZER_MNG_H_

#include "base_ref.h"

/*--------------------------------- Macros -----------------------------------*/


/*-------------------------Public Function Prototypes ------------------------*/
void Buzzer_Init( void );
void Buzzer_PwmStart( void );
void Buzzer_PwmStop( void );

#endif /* BUZZER_MNG_H_ */
