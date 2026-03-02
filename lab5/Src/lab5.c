#include "main.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_gpio_ex.h"

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

  while (1)
  {
 
  }
  return -1;
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

void setGPIO(void) {

  // Enable GPIOB and GPIOC in RCC
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;

  // Set PB11 to alternate function mode, open-drain output.
  // Set I2C2_SDA as it alternate function
  GPIO_InitTypeDef pinB11Init = {GPIO_PIN_11, GPIO_MODE_AF_OD,
                                GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF1_I2C2};
  HAL_GPIO_Init(GPIOB, &pinB11Init);

  // Set PB13 to alternate function mode, open-drain output.
  // Set I2C2_SDA as it alternate function
  GPIO_InitTypeDef pinB13Init = {GPIO_PIN_13, GPIO_MODE_AF_OD,
                                GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF5_I2C2};
  HAL_GPIO_Init(GPIOB, &pinB13Init);

  // Set PB14 to push pull output type and set high
  GPIO_InitTypeDef pinB14Init = {GPIO_PIN_14, GPIO_MODE_OUTPUT_PP};
  HAL_GPIO_Init(GPIOB, &pinB14Init);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);

  // Set C0 to push pull output type and set high
  GPIO_InitTypeDef pinC0Init = {GPIO_PIN_0, GPIO_MODE_OUTPUT_PP};
  HAL_GPIO_Init(GPIOC, &pinC0Init);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
}

void initI2C(void) {
  RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
  // Set the parameters in the TIMING register to use 100kHz standard-mode I2C
                    // PRESC     // SCLL  // SCLH       // SDADEL     // SCLDEL
  I2C2->TIMINGR |= (0x1 << 28) | (0x13) | (0xF << 8) | (0x2 << 16) | (0x4 << 20); 

  // Enable the I2C Peripheral using the PE bit in the CR1 register.
  I2C2->CR1 |= I2C_CR1_PE;
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
