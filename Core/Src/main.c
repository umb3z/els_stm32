/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

volatile int menuPosition;
volatile int lastEncodedMenu;
volatile float rpm;


uint32_t counterMandrino;
int16_t countMandrino;
int16_t positionMandrino;
int speedMandrino;




char buff[21];                                    //buffer per memorizzare le stringhe lette dalla flash e visualizzate sul display

bool mm_min = false;                              //variabile che determina la modalita' di avanzamento: true = mm/min; false = cent/giro

/* VALORI EEPROM (che non c'è lol)*/
unsigned int av_carro[] = {400,400,400,400};                      //valore di avanzamento in centesimi del carro (valore caricato da EEPROM)
unsigned int av_trasv[] = {400,400,400,400};                      //valore di avanzamento in centesimi del carro (valore caricato da EEPROM)

unsigned char NORTON_gearbox;              //valore che memorizza la posizione corrente della scatola norton (usato come indice per gli array qui sopra)

/* variabili relative ad encoder e motore stepper*/

int one_turn_mandrel_steps = 0;                //numero di step/giro dell'encoder mandrino (x4) (valore caricato da EEPROM)

float screw_pitch = 4.0;                          //passo della vite madre in mm (valore caricato da EEPROM)

unsigned int one_turn_screw_steps = 800;          //numero di step/giro del motore stepper sulla vite (valore caricato da EEPROM)
												  //nel mio caso 200*2*2 (step motore * half-step driver * demoltiplica hardware)

float single_step_pitch = 4.0/800; 						    //valore in micron di movimento del carro ad ogni step della vite madre
															//calcolato al termine della funzione LoadFromEEPROM()
															//calculates the linear movement (in mm) of the carriage for a single step of the stepper motor

unsigned int AccelerationDelay=100;           //ritardo accelerazione del motore stepper - varibile letta da EEPROM
unsigned int DecelerationDelay=100;           //ritardo decelerazione del motore stepper - varibile letta da EEPROM

bool CW = true;                     //TRUE se la rotazione standard della vite e' in senso orario (orario trascina il carro verso il madnrino). variabile letta da EEPROM
bool CCW = false;                   //TRUE se la rotazione standard della vite e' in senso anti-orario (anti-orario trascina il carro verso il madnrino) variabile letta da EEPROM

int thread_offset_steps=400;                  //offset per operazione di filettatura. Indica quanti passi prima dell'inizio del filetto si deve posizionare la vite
unsigned long MaxStepperSpeed = 100; //massima velocit di rotazione ammessa per il motore stepper. Variabile letta da EEPROM
/* FINE VALORI EEPROM */
float SingleStepFeed;                       //valore in millimetri di movimento del carro ad ogni step della barra
                                            //calcolato nelle funzioni di avanzamento dove richiesto ed in base alla posizione del cambio norton
float distance = 0.00;                      //distanza in mm usata nella funzione FilettaturaToPosition()
// fine variabili relative ad encoder e motore stepper

// mixed variables
long OldSpeedTimer;
long OldPos = 0;
long NewPos = 0;
//end mixed variables

//variabili usate per il calcolo della progressione di filettatura
char sequenza [MaxSteps];              //array usato per i calcolo della sequenza passi encoder/stepper. la lunghezza di questo array limita il massimo passo eseguibile in filettatura
                                       //max_passo = passo_vite*(800/passi_stepper_giro)
int numero_passi = 0;                  //variabile che memorizza il numero dei passi stepper per ogni giro mandrino (usata in filettatura e avanzamento)
int pointer = 0;                       //puntatore per array "sequenza"
bool Metric = true;                 //se vero il passo impostato in filettatura e' metrico, se Falso e' imperial
float thread_pitch = 1.00;                  //passo impostato per la filettatura in mm (standard 1.00mm)
char TPInch = 20;                              //passo impostato per la filettatura in pollici (standard 20 TPI)
//fine variabili usate per il calcolo della progressione di filettatura

//variabili relative alla lettura dell'encoder
int steps = 0;                             //passi encoder relativi - variabile usata come appoggio in filettatura e avanzamento
volatile long absolute_encoder_steps = 0;  //passi encoder assoluti
bool step_flag;                         //Flag per detarminare se il passo encode  avvenuto (usata nella rountine di interrupt di lettura dell'nencoder)
int passi_sequenza = 0;
char encoder[] = {0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, -1, 0, -1, 1, 0};  //array da 16 valori usato per "muovere" i passi enoder nella rountine interrupt di filettatura
bool sviluppo_filetto = true;      //Flag che detrmina il verso id filettatura: TRUE = DESTRO; FALSE = SINISTRO
//fine variabili relative alla lettura dell'encoder

// Stepper motor variables
//bool Direction = 1;        //variabile usata per determinare il senso di rotazione momentaneo del motore stepper e incrementare o decrementare i passi
int passi_stepper = 0;
unsigned long Speed;           //memorizza la velocit corrente del motore stepper in giri al minuto - se = 0 lo stepper e' fermo


volatile unsigned int TOP = 65535;        //valore usato per scrivere il registro ICR1 che determina la frequenza del PWM che regola la velocit di rotazione del motore stepper
volatile long absolute_steps = 0;         //passi stepper assoluti

int feed = 1;                             //valore corrente avanzamneto usato nella funzione di avanzamneto
bool SHOW_Impostazioni = false;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  if (HAL_TIM_Encoder_Start_IT(&htim1, TIM_CHANNEL_ALL) != HAL_OK)
	  Error_Handler();
  if (HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1)!= HAL_OK)
	  Error_Handler();
  if (!ee_init())
	  Error_Handler();
  //WriteToEEPROM();
  LoadFromEEPROM();
	lcd_init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
		char scelta;
				  while (true)
				  {
				    scelta = Principale();
				    switch (scelta) {
				    case 1: lcd_clear(); Impostazioni(); break;
				    case 6: lcd_clear(); PosizioneAngolare(); break;
				    case 5: lcd_clear(); MandrelSpeed(); break;
				      /*case 2: lcd.clear(); Avanzamento(); break;
				      case 3: lcd.clear(); Filettatura(); break;
				      case 4: lcd.clear(); MovimentoLibero(); break;
				      */
				    }
				  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_FALLING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_FALLING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC1;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 72-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
  if (HAL_TIM_SlaveConfigSynchro(&htim2, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_TRC;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : MENU_RESET_Pin MENU_BUTTON_Pin */
  GPIO_InitStruct.Pin = MENU_RESET_Pin|MENU_BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : MENU_ESC_Pin */
  GPIO_InitStruct.Pin = MENU_ESC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(MENU_ESC_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : MENU_ENCODER_A_Pin MENU_ENCODER_B_Pin */
  GPIO_InitStruct.Pin = MENU_ENCODER_A_Pin|MENU_ENCODER_B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

