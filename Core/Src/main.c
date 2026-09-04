/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Melodia Star Wars usando las funciones del profesor
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* Frecuencias exactas en Hz para cada nota musical */
/* USER CODE BEGIN PD */
/* Frecuencias exactas en Hz para cada nota musical */
#define NOTE_A3  220
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_C4  262
#define NOTE_C5  523
#define NOTE_D4  294
#define NOTE_D5  587
#define NOTE_E4  330
#define NOTE_E5  659
#define NOTE_F4  349
#define NOTE_F5  698
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A5  880
/* USER CODE END PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t pasos = 0;

/* Partitura de Star Wars (Frecuencia de cada nota) */
int melodia[] = {
  NOTE_A4, NOTE_A4, NOTE_A4, 
  NOTE_F4, NOTE_C5, 
  NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4,
  NOTE_E5, NOTE_E5, NOTE_E5, 
  NOTE_F5, NOTE_C5, 
  NOTE_GS4, NOTE_F4, NOTE_C5, NOTE_A4
};

/* Duracion de cada nota en milisegundos */
int duracion[] = {
  500, 500, 500, 
  350, 150, 
  500, 350, 150, 650,
  500, 500, 500, 
  350, 150, 
  500, 350, 150, 650
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void delay_us_dwt_init(void);
void delay_us_dwt(uint32_t reta);
void Sound_play(uint32_t frec, uint32_t dura);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void delay_us_dwt_init(void)
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  pasos = (HAL_RCC_GetSysClockFreq() / 1000000);
}

void delay_us_dwt(uint32_t reta)
{
  DWT->CYCCNT = 0;
  while (DWT->CYCCNT <= pasos * reta);
}

void Sound_play(uint32_t frec, uint32_t dura)
{
  if (frec == 0)
  {
    HAL_Delay(dura);
    return;
  }
  uint32_t dela = 500000 / frec;
  uint32_t repe = frec * dura / 1000;

  while (repe--)
  {
    HAL_GPIO_WritePin(parlante_GPIO_Port, parlante_Pin, 1);
    delay_us_dwt(dela);
    HAL_GPIO_WritePin(parlante_GPIO_Port, parlante_Pin, 0);
    delay_us_dwt(dela);
  }
}
/* USER CODE END 0 */

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */
  delay_us_dwt_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    int total_notas = sizeof(melodia) / sizeof(melodia[0]);

    for (int i = 0; i < total_notas; i++)
    {
      Sound_play(melodia[i], duracion[i]);
      HAL_Delay(30); /* Breve separacion para articular bien las notas */
    }

    HAL_Delay(3000); /* Pausa de 3 segundos antes de volver a empezar */
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE();

  HAL_GPIO_WritePin(parlante_GPIO_Port, parlante_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = parlante_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(parlante_GPIO_Port, &GPIO_InitStruct);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif