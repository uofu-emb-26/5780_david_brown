#include "main.h"
#include "stm32f0xx_hal.h"
#include "hal_gpio_L3.h"
#include "assert.h"
#include "tim.h"

void SystemClock_Config(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();

  HAL_RCC_GPIOC_CLK_Enable();
  

  // LED Pins INIT string
  GPIO_InitTypeDef pin6Init = {GPIO_PIN_6, GPIO_MODE_AF_PP,
                                GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF0_TIM3};
  GPIO_InitTypeDef pin7Init = {GPIO_PIN_7, GPIO_MODE_AF_PP,
                                GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF0_TIM3};
  GPIO_InitTypeDef pin8Init = {GPIO_PIN_8, GPIO_MODE_OUTPUT_PP,
                                GPIO_NOPULL, GPIO_SPEED_FREQ_LOW};
  GPIO_InitTypeDef pin9Init = {GPIO_PIN_9, GPIO_MODE_OUTPUT_PP,
                                GPIO_NOPULL, GPIO_SPEED_FREQ_LOW}; 
  
  // Initialize LED's                             
  My_HAL_GPIO_Init(GPIOC, &pin6Init); // Red
  My_HAL_GPIO_Init(GPIOC, &pin7Init); // Blue
  My_HAL_GPIO_Init(GPIOC, &pin8Init); // Orange
  My_HAL_GPIO_Init(GPIOC, &pin9Init); // Green
  //assert((GPIOC->MODER & ((0x3 << (6*2)) | (0x3 << (7*2)) | (0x3 << (8*2)) | (0x3 << (9*2)))) == 0x55000);

  My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_SET);
  //assert( (GPIOC->ODR & (1 << 9)) && !(GPIOC->ODR & (1 << 8)) && !(GPIOC->ODR & (1 << 6)));

  tim2Config(); // Configure the TIM 2 timer.
  tim3Config();
  
  while (1)
  {
    for (int p = 0; p <= 100; p++)
    {
        setLedDuty(p);
        HAL_Delay(15);
    }

    for (int p = 100; p >= 0; p--)
    {
        setLedDuty(p);
        HAL_Delay(15);
    }
  }
  return -1;
}

void setLedDuty(uint8_t percent) {
  if (percent > 100) percent = 100;
  TIM3->CCR1 = (TIM3->ARR * percent) / 100;
  TIM3->CCR2 = (TIM3->ARR * percent) / 100;
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add their own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add their own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */
