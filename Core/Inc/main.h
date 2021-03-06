/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FLASH_PAGE_F1.h"
#include "i2c-lcd.h"
#include "menuPrincipale.h"
#include "Impostazioni.h"
#include "PosizioneMandrino.h"
#include "VelocitaMandrino.h"
#include "VirtualEeprom.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ONBOARD_LED_Pin GPIO_PIN_13
#define ONBOARD_LED_GPIO_Port GPIOC
#define CHANNEL_A_Pin GPIO_PIN_6
#define CHANNEL_A_GPIO_Port GPIOA
#define CHANNEL_B_Pin GPIO_PIN_7
#define CHANNEL_B_GPIO_Port GPIOA
#define MENU_RESET_Pin GPIO_PIN_15
#define MENU_RESET_GPIO_Port GPIOB
#define MENU_ESC_Pin GPIO_PIN_15
#define MENU_ESC_GPIO_Port GPIOA
#define MENU_ENCODER_A_Pin GPIO_PIN_3
#define MENU_ENCODER_A_GPIO_Port GPIOB
#define MENU_ENCODER_A_EXTI_IRQn EXTI3_IRQn
#define MENU_ENCODER_B_Pin GPIO_PIN_4
#define MENU_ENCODER_B_GPIO_Port GPIOB
#define MENU_ENCODER_B_EXTI_IRQn EXTI4_IRQn
#define MENU_BUTTON_Pin GPIO_PIN_5
#define MENU_BUTTON_GPIO_Port GPIOB
#define ENCODER2_A_Pin GPIO_PIN_6
#define ENCODER2_A_GPIO_Port GPIOB
#define ENCODER2_B_Pin GPIO_PIN_7
#define ENCODER2_B_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define MaxSteps 800   //dimension of the step array - this can be adjusted to increase the maximum pitch according to the available RAM
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
