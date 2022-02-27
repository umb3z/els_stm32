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
extern volatile float rpm;
void MandrelSpeed() {
	int oldMenuPosition = menuPosition;
	bool stay = true; //boolean to determine if stay or not in the reading cycle

	lcd_clear();
	lcd_cursor_pos(0, 0);
	lcd_send_string("*VELOCITA' MANDRINO*");

	while (stay) {
		if (HAL_GPIO_ReadPin(MENU_ESC_GPIO_Port, MENU_ESC_Pin)
				== GPIO_PIN_RESET) {
			HAL_Delay(300);
			stay = false;
			menuPosition = oldMenuPosition;
		}
		sprintf(buff, "RPM:%.2f", rpm);
		lcd_cursor_pos(1, 1);
		lcd_send_string("                    ");
		lcd_cursor_pos(1, 1);
		lcd_send_string(buff);
		HAL_Delay(300);
	}
}
