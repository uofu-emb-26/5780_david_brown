#include "main.h"
#include "stm32f0xx_hal.h"

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

  setGPIO();
  //configADC();
  configDAC();
  //uint8_t data = 0;

  // Sawtooth Wave: 8-bit, 32 samples/cycle
  const uint8_t sawtooth_table[32] = {0,7,15,23,31,39,47,55,63,71,79,87,95,103,
  111,119,127,134,142,150,158,166,174,182,190,198,206,214,222,230,238,246};

  uint8_t idx = 0;
  while (1)
  {
    // Checkoff 6.1
    //while(!(ADC1->ISR & ADC_ISR_EOC)){}
    // data = ADC1->DR;

    // if(data >= 50) {
    //   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);   // Set blue
    // }
    // else
    //   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);   // Reset blue
    
    // if(data >= 100) {
    //   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);   // Set orange
    // }
    // else
    //   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);   // Reset orange
    
    // if(data >= 150) {
    //   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); // Set red
    // }
    // else
    //   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET); // Reset red

    // if(data >= 200) {
    //   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET); // Set Green
    // }
    // else
    //   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET); // Reset Green 

    // Checkoff 6.2
    if(idx == 32) {
      idx = 0;
    }
    DAC->DHR8R1 = sawtooth_table[idx];
    idx++;
    HAL_Delay(1);
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

void setGPIO(void) {

  // Enable GPIOB and GPIOC in RCC
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

  // LED Initialization
  GPIO_InitTypeDef initCStr = {GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, GPIO_MODE_OUTPUT_PP};
  HAL_GPIO_Init(GPIOC, &initCStr);
}

void configADC(void) {
  // ADC
  GPIO_InitTypeDef initADC = {GPIO_PIN_0, GPIO_MODE_ANALOG, GPIO_NOPULL};
  HAL_GPIO_Init(GPIOC, &initADC);

  // Enable RCC
  RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
  
  // Continuous conversion mode, hardware triggers disabled, 8-bit resolution
  ADC1->CFGR1 |= ADC_CFGR1_CONT | ADC_CFGR1_RES_1;

  // Select/enable input pin's channel for ADC conversion. 
  ADC1->CHSELR |= (1 << 10);    // Channel 10, Connected to PC0

  // Self Calibration
  ADC1->CR &= ~(ADC_CR_ADEN);   // ENSURE ADEN = 0 and DMAEN = 0
  ADC1->CFGR1 &= ~(ADC_CFGR1_DMAEN);

  ADC1->CR |= ADC_CR_ADCAL;

  while (ADC1->CR & ADC_CR_ADCAL) {
    // Wait until ADCAL is 0 before moving on
  }

  ADC1->CR |= ADC_CR_ADEN;
  while (!(ADC1->ISR & ADC_ISR_ADRDY)) {
    // Wait until ADC is ready before starting
  } 
  ADC1->CR |= ADC_CR_ADSTART;
}

void configDAC(void) {
  // DAC Channel 1
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  RCC->APB1ENR|= RCC_APB1ENR_DACEN;
  GPIO_InitTypeDef initDAC = {GPIO_PIN_4, GPIO_MODE_ANALOG, GPIO_NOPULL};
  HAL_GPIO_Init(GPIOA, &initDAC);
 

  // Set to software trigger mode. Enable channel 1
  DAC->CR |= DAC_CR_TSEL1 | DAC_CR_EN1;// | DAC_CR_TEN1;
  //DAC->SWTRIGR |= 1;

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
