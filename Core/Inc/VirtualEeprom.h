/*
 * VirtualEeprom.h
 *
 *  Created on: Feb 21, 2022
 *      Author: umbez
 */

#ifndef INC_VIRTUALEEPROM_H_
#define INC_VIRTUALEEPROM_H_
#include "main.h"
#include "FLASH_PAGE_F1.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "main.h"
#include <stdbool.h>



void WriteToEEPROM();
void LoadFromEEPROM();

#endif /* INC_VIRTUALEEPROM_H_ */
