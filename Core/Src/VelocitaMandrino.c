/*
 * VelocitaMandrino.h
 *
 *  Created on: Feb 20, 2022
 *      Author: umbez
 */
#include "VelocitaMandrino.h"
extern volatile int menuPosition;
extern int steps;
extern char buff[];
extern TIM_HandleTypeDef htim3;
extern volatile int giri;
void MandrelSpeed() {
	uint32_t tick = 0;
	giri = 0;
	int giri_old = giri;
	int oldMenuPosition = menuPosition;
	bool stay = true; //boolean to determine if stay or not in the reading cycle

	lcd_clear();
	lcd_cursor_pos(0, 0);
	lcd_send_string("*VELOCITA' MANDRINO*");
	tick = HAL_GetTick();

	while (stay) {
		if (HAL_GPIO_ReadPin(MENU_ESC_GPIO_Port, MENU_ESC_Pin)
				== GPIO_PIN_RESET) {
			HAL_Delay(300);
			stay = false;
			menuPosition = oldMenuPosition;
		}
		if (HAL_GetTick() - tick > 1000L) {
			if (giri != giri_old) {
				lcd_cursor_pos(1, 2);
				int diff = giri-giri_old;
				sprintf(buff, "RPM:%d", diff * 60 );
				lcd_send_string("                    ");
				lcd_send_string(buff);
				giri_old = giri;
			}


		}


	}

}
