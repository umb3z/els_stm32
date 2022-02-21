/*
 * VirtualEeprom.h
 *
 *  Created on: Feb 21, 2022
 *      Author: umbez
 */

#ifndef INC_VIRTUALEEPROM_H_
#define INC_VIRTUALEEPROM_H_
#include "main.h"
#include "ee.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "main.h"
#include <stdbool.h>



void WriteToEEPROM();
void LoadFromEEPROM();

unsigned int value_quattro_int(int begin, uint8_t elements[]);
float value_quattro_float(int begin, uint8_t elements[]);
unsigned int value_otto_int(int begin, uint8_t elements[]);
int value_due_int(int begin, uint8_t elements[]);
#endif /* INC_VIRTUALEEPROM_H_ */
