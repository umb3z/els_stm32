/*
 * menuPrincipale.c
 *
 *  Created on: Feb 16, 2022
 *      Author: umbez
 */
#include "menuPrincipale.h"
#include <stdbool.h>

static char *MenuPrincipale[] = { "**MENU  PRINCIPALE**", "Impostazioni","Avanzamento",
		"Filettatura", "Movimento Libero",  "Velocita' mandrino","Posizione angolare" };

extern int timeToggle;
extern int menuPosition;

char Principale() {
	unsigned char current = 1;
	unsigned char previous = 6;
	unsigned char next = 2;
	bool stay = true;
	bool innerstay = true;

	lcd_clear();
	lcd_cursor_pos(0, 2);
	lcd_send_string(">");
	lcd_cursor_pos(19, 2);
	lcd_send_char('<');
	lcd_cursor_pos(0, 0);
	lcd_send_string(MenuPrincipale[0]);
	menuPosition = 1;

	while (stay) {
		// LED ON
		//HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		//HAL_Delay(timeToggle);

		switch (current) {
		case 0:
			current = 6;
			break;
		case 7:
			current = 1;
			break;
		}
		previous = current - 1;
		next = current + 1;
		if (previous == 0)
			previous = 6;
		if (next == 7)
			next = 1;

		lcd_cursor_pos(1, 1);
		lcd_send_string("                  ");
		lcd_cursor_pos(1, 1);
		lcd_send_string(MenuPrincipale[previous]);

		lcd_cursor_pos(1, 2);
		lcd_send_string("                  ");
		lcd_cursor_pos(1, 2);
		lcd_send_string(MenuPrincipale[current]);

		lcd_cursor_pos(1, 3);
		lcd_send_string("                  ");
		lcd_cursor_pos(1, 3);
		lcd_send_string(MenuPrincipale[next]);

		innerstay = true;
		while (innerstay) {
			if (HAL_GPIO_ReadPin(MENU_BUTTON_GPIO_Port, MENU_BUTTON_Pin)
					== GPIO_PIN_RESET) {
				stay = false;
				innerstay = false;
				HAL_Delay(300);
			}
			if (current != menuPosition) {
				if (menuPosition >6) menuPosition = 1;
				if (menuPosition < 1) menuPosition = 6;
				current = menuPosition;
				innerstay = false;
			}
			/*  HAL_GPIO_WritePin(ONBOARD_LED_GPIO_Port, ONBOARD_LED_Pin, GPIO_PIN_RESET);
							 	  HAL_Delay(1000);
							 	  HAL_GPIO_WritePin(ONBOARD_LED_GPIO_Port, ONBOARD_LED_Pin, GPIO_PIN_SET);
							 	 HAL_Delay(1000);*/
		}
		HAL_Delay(300);
	}

	return current;
}

