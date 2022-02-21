/*
 * Avanzamento.h
 *
 *  Created on: Feb 20, 2022
 *      Author: umbez
 */

#ifndef INC_AVANZAMENTO_H_
#define INC_AVANZAMENTO_H_

#include <stdio.h>
#include "i2c-lcd.h"
#include "main.h"


void ImpostaModalitaAvanzamento();
char AvanzamentoMenu();
void ImpostaNORTON();
void LongTrasv();
void AvanzamentoLibero();
void AvanzamentoVincolato();
void Avanzamento();
unsigned int GetSpeed (unsigned long frame);




#endif /* INC_AVANZAMENTO_H_ */
