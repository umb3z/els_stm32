/*
 * menuPrincipale.c
 *
 *  Created on: Feb 18, 2022
 *      Author: umbez
 */
#include "Impostazioni.h"
#include <stdbool.h>

static char *MenuImpostazioni[] = { "*** IMPOSTAZIONI ***",
		"passi enc. mandrino", "passi stepper vite", "passo vite madre",
		"rit. accelerazione", "rit. decelerazione", "rotazione standard",
		"avanzamento carro 1", "avanzamento trasv 1", "avanzamento carro 2",
		"avanzamento trasv 2", "avanzamento carro 3", "avanzamento trasv 3",
		"avanzamento carro 4", "avanzamento trasv 4", "offset filetto",
		"NORTON", "Vel max stepper", "ripristina valori", "ORARIA",
		"ANTI-ORARIA", "RESET:OK" };

extern int menuPosition;
extern int one_turn_mandrel_steps;
extern unsigned int one_turn_screw_steps;
extern float screw_pitch;
extern unsigned int AccelerationDelay;
extern unsigned int DecelerationDelay;
extern bool CW;
extern bool CCW;
extern unsigned int av_carro[];
extern unsigned int av_trasv[];
extern int thread_offset_steps;
extern unsigned long MaxStepperSpeed;
extern bool SHOW_Impostazioni;
extern unsigned char NORTON_gearbox;

/*
 * Menu Impostazione
 *
 * CubeIDE
 * nb. per abilitare sprintf abilitare questo flag :
 * -u _printf_float in Properties -> C/C++ Build -> Settings -> MCU GCC Linker -> Miscellaneous -> Linker flags
 *
 * */

void Impostazioni() {
	unsigned char current = 1;
	unsigned char previous = 0;
	unsigned char next = 2;
	bool stay = true;
	bool innerstay = true;

	lcd_clear();
	lcd_cursor_pos(0, 2);
	lcd_send_string(">");
	lcd_cursor_pos(19, 2);
	lcd_send_char('<');
	lcd_cursor_pos(0, 0);
	lcd_send_string(MenuImpostazioni[0]);

	int oldMenuPosition = menuPosition;

	menuPosition = 1;

	while (stay) {

		switch (current) {
		case 0:
			current = 18;
			break;
		case 19:
			current = 1;
			break;
		}
		previous = current - 1;
		next = current + 1;
		if (previous == 0)
			previous = 18;
		if (next == 19)
			next = 1;

		lcd_cursor_pos(1, 1);
		lcd_send_string("                   ");
		lcd_cursor_pos(1, 1);
		lcd_send_string(MenuImpostazioni[previous]);

		lcd_cursor_pos(1, 2);
		lcd_send_string("                   ");
		lcd_cursor_pos(1, 2);
		lcd_send_string(MenuImpostazioni[current]);

		lcd_cursor_pos(1, 3);
		lcd_send_string("                   ");
		lcd_cursor_pos(1, 3);
		lcd_send_string(MenuImpostazioni[next]);

		innerstay = true;
		while (innerstay) {
			if (HAL_GPIO_ReadPin(MENU_BUTTON_GPIO_Port, MENU_BUTTON_Pin)
					== GPIO_PIN_RESET) {
				HAL_Delay(300);
				ImpostazioniCambiaValore(current);
				lcd_clear();
				lcd_cursor_pos(0, 2);
				lcd_send_string(">");
				lcd_cursor_pos(19, 2);
				lcd_send_char('<');
				lcd_cursor_pos(0, 0);
				lcd_send_string(MenuImpostazioni[0]);
				innerstay = false;

			}
			if (HAL_GPIO_ReadPin(MENU_ESC_GPIO_Port, MENU_ESC_Pin)
					== GPIO_PIN_RESET) {
				HAL_Delay(300);
				stay = false;
				innerstay = false;
				menuPosition = oldMenuPosition;
			}
			if (current != menuPosition) {
				if (menuPosition > 18)
					menuPosition = 1;
				if (menuPosition < 1)
					menuPosition = 18;
				current = menuPosition;
				innerstay = false;

			}
		}
		HAL_Delay(300);
	}
}

void ImpostazioniCambiaValore(unsigned char current) {

	int oldMenuPosition = menuPosition;
	menuPosition = 0;
	int previousMenuPosition = menuPosition;
	int variation = 0;
	bool stay = true;

	lcd_clear();
	lcd_cursor_pos(0, 0);
	lcd_send_string(MenuImpostazioni[current]);
	lcd_cursor_pos(0, 3);
	lcd_send_string("SEL:OK");
	lcd_cursor_pos(0, 1);
	lcd_send_string(ImpostazioniGetStringValueFromMenuIndex(current));

	while (stay) {
		if (current == 18
				&& HAL_GPIO_ReadPin(MENU_RESET_GPIO_Port, MENU_RESET_Pin)
						== GPIO_PIN_RESET) {
			HAL_Delay(300);
			lcd_clear();
			lcd_cursor_pos(0, 0);
			lcd_send_string(MenuImpostazioni[current]);
			lcd_cursor_pos(0, 3);
			lcd_send_string("....................");
			lcd_cursor_pos(0, 1);
			lcd_send_string("Ripristino in corso");
			HAL_Delay(2000);
			menuPosition = oldMenuPosition;
			return;
		}
		if (HAL_GPIO_ReadPin(MENU_BUTTON_GPIO_Port, MENU_BUTTON_Pin)
				== GPIO_PIN_RESET) {
			HAL_Delay(300);
			menuPosition = oldMenuPosition;
			return;

		}
		if (previousMenuPosition != menuPosition) {
			if (menuPosition < previousMenuPosition) {
				variation = -1;
			} else if (menuPosition > previousMenuPosition) {
				variation = 1;
			}
			previousMenuPosition = menuPosition;

			switch (current) {
			case 1:
				if (one_turn_mandrel_steps + variation > 0)
					one_turn_mandrel_steps += variation;
				break;
			case 2:
				if (one_turn_screw_steps + variation > 0)
					one_turn_screw_steps += variation;
				break;
			case 3:
				if ((((screw_pitch * 1000) + (variation)) / 1000) > 0)
					screw_pitch = (((screw_pitch * 1000) + (variation)) / 1000);
				break;
			case 4:
				if (AccelerationDelay + variation * 5 > 0)
					AccelerationDelay += (variation * 5);
				break;
			case 5:
				if (DecelerationDelay + variation * 5 > 0)
					DecelerationDelay += (variation * 5);
				break;
			case 6:
				CW = !(CW);
				CCW = !(CCW);
				break;
			case 7:
				if (av_carro[0] + variation / 5 > 0)
					av_carro[0] += (variation / 5);
				break;
			case 8:
				if (av_trasv[0] + variation / 5 > 0)
					av_trasv[0] += (variation / 5);
				break;
			case 9:
				if (av_carro[1] + variation / 5 > 0)
					av_carro[1] += (variation / 5);
				break;
			case 10:
				if (av_trasv[1] + variation / 5 > 0)
					av_trasv[1] += (variation / 5);
				break;
			case 11:
				if (av_carro[2] + variation / 5 > 0)
					av_carro[2] += (variation / 5);
				break;
			case 12:
				if (av_trasv[2] + variation / 5 > 0)
					av_trasv[2] += (variation / 5);
				break;
			case 13:
				if (av_carro[3] + variation / 5 > 0)
					av_carro[3] += (variation / 5);
				break;
			case 14:
				if (av_trasv[3] + variation / 5 > 0)
					av_trasv[3] += (variation / 5);
				break;
			case 15:
				if (thread_offset_steps + variation > 0)
					thread_offset_steps += variation;
				break;
			case 16:
				if (NORTON_gearbox + variation > 0)
					NORTON_gearbox += variation;
				if (NORTON_gearbox > 3)
					NORTON_gearbox = 3;
				break;
			case 17:
				if (MaxStepperSpeed + variation > 0)
					MaxStepperSpeed += variation;
			}
			lcd_cursor_pos(0, 1);
			lcd_send_string("                    ");
			lcd_cursor_pos(0, 1);
			lcd_send_string(ImpostazioniGetStringValueFromMenuIndex(current));
			HAL_Delay(300);
		}
	}
}

char* ImpostazioniGetStringValueFromMenuIndex(unsigned char current) {
	char *buff;
	buff = malloc(sizeof(char) * 20);

	switch (current) {
	case 1:
		sprintf(buff, "%d", one_turn_mandrel_steps);
		break;
	case 2:
		sprintf(buff, "%d", one_turn_screw_steps);
		break;
	case 3:
		sprintf(buff, "%.3f", screw_pitch);
		break;
	case 4:
		sprintf(buff, "%d", AccelerationDelay);
		break;
	case 5:
		sprintf(buff, "%d", DecelerationDelay);
		break;
	case 6:
		if (CCW)
			sprintf(buff, MenuImpostazioni[19]);
		else
			sprintf(buff, MenuImpostazioni[20]);
		break;

	case 7:
		sprintf(buff, "%d", av_carro[0]);
		break;
	case 8:
		sprintf(buff, "%d", av_trasv[0]);
		break;
	case 9:
		sprintf(buff, "%d", av_carro[1]);
		break;
	case 10:
		sprintf(buff, "%d", av_trasv[1]);
		break;
	case 11:
		sprintf(buff, "%d", av_carro[2]);
		break;
	case 12:
		sprintf(buff, "%d", av_trasv[2]);
		break;
	case 13:
		sprintf(buff, "%d", av_carro[3]);
		break;
	case 14:
		sprintf(buff, "%d", av_trasv[3]);
		break;
	case 15:
		sprintf(buff, "%d", thread_offset_steps);
		break;
	case 16:
		sprintf(buff, "%d", NORTON_gearbox + 1);
		break;
	case 17:
		sprintf(buff, "%lu", MaxStepperSpeed);
		break;
	case 18:
		sprintf(buff, MenuImpostazioni[21]);
		break;
	}

	//ltrim string
	int nhead = 0;

	while (buff[nhead++] == ' ')
		;

	nhead--;
	memmove(buff, &buff[nhead], strlen(buff) - (nhead));
	buff[strlen(buff) - (nhead)] = '\0';

	return buff;
}

