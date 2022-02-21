/*
 * VelocitaMandrino.h
 *
 *  Created on: Feb 20, 2022
 *      Author: umbez
 */
#include "VelocitaMandrino.h"
extern volatile int menuPosition;
extern volatile float rpm;
extern char buff[];


void MandrelSpeed() {

	int oldMenuPosition = menuPosition;
	bool stay = true; //boolean to determine if stay or not in the reading cycle

	lcd_clear();
	lcd_cursor_pos(0, 0);
	lcd_send_string("*VELOCITA' MANDRINO*");
	float lastRpm = rpm + 0.1;

	while (stay) {
		if (HAL_GPIO_ReadPin(MENU_ESC_GPIO_Port, MENU_ESC_Pin)
							== GPIO_PIN_RESET) {
						HAL_Delay(300);
						stay = false;
						menuPosition = oldMenuPosition;
					}
		if (lastRpm != rpm){
			sprintf(buff,"RPM:%.3f",rpm/4.0);
			lcd_cursor_pos(0, 1);
			lcd_send_string(buff);
			lastRpm = rpm;
			HAL_Delay(3000);
		}

	}

}
