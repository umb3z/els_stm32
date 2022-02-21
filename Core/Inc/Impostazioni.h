/*
 * Impostazioni.h
 *
 *  Created on: Feb 18, 2022
 *      Author: umbez
 */

#ifndef INC_IMPOSTAZIONI_H_
#define INC_IMPOSTAZIONI_H_
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "i2c-lcd.h"
#include "main.h"


void Impostazioni();
void ImpostazioniCambiaValore(unsigned char current);
char* ImpostazioniGetStringValueFromMenuIndex(unsigned char current);

#endif /* INC_IMPOSTAZIONI_H_ */
