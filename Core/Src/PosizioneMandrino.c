#include "PosizioneMandrino.h"

static char *MenuPosizioneAngolare[] = { "*POSIZIONE ANGOLARE*",
		"RESET            ESC" };

extern int one_turn_mandrel_steps;
extern char buff[];
extern uint32_t steps;
extern uint32_t steps2;
extern volatile int giri;
extern TIM_HandleTypeDef htim4;

void PosizioneAngolare() {
	float Degrees = 0.00;                       //variable used to store degrees
	float old_Degrees = 0.01; //variable to check if the position is changed. Set different from Degrees to print LCD output on the first cycle
	float AngularRatio = 360.0/(float)one_turn_mandrel_steps; //calculates the degrees per single mandrel encoder step

	lcd_clear();
	lcd_cursor_pos(0, 0);
	lcd_send_string(MenuPosizioneAngolare[0]);
	lcd_cursor_pos(0, 3);
	lcd_send_string(MenuPosizioneAngolare[1]);



	while (HAL_GPIO_ReadPin(MENU_ESC_GPIO_Port, MENU_ESC_Pin) != GPIO_PIN_RESET) {
		if (HAL_GPIO_ReadPin(MENU_RESET_GPIO_Port, MENU_RESET_Pin) == GPIO_PIN_RESET){
			HAL_Delay(300);
			steps = 0;
			Degrees = 0;
		}

		Degrees = ((float) ((abs(steps) % one_turn_mandrel_steps))
				* AngularRatio);
		steps = TIM3->CNT;
		steps2 = __HAL_TIM_GET_COUNTER(&htim4);

		if (steps < 0)
			Degrees = 360 - Degrees;
		if (Degrees == 360)
			Degrees = 0;
		if (Degrees != old_Degrees) {
			lcd_cursor_pos(1, 1);
			sprintf(buff,"gradi:%.3f" , Degrees);
			lcd_send_string(buff);
			old_Degrees = Degrees;

			lcd_cursor_pos(1, 2);
			sprintf(buff,"steps2:%lu" , steps2);
			lcd_send_string(buff);

			HAL_Delay(300);

		}

		HAL_Delay(300);
	}
	HAL_Delay(300);
}
