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
  setGPIO();
  initI2C();
  initGyro();
  uint8_t who = readRegisterI2C(0x69, 0x0f);

    if(who == 0xD3)
    {
        for(int i = 0; i < 9; i++) {
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
            HAL_Delay(500);
        }
    }
    else
    {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
        HAL_Delay(500);
    }

  while (1)
  {
    uint8_t xLow  = readRegisterI2C(0x69, 0x28);
    uint8_t xHigh = readRegisterI2C(0x69, 0x29);
    uint8_t yLow  = readRegisterI2C(0x69, 0x2A);
    uint8_t yHigh = readRegisterI2C(0x69, 0x2B);

    int16_t yValue = (yHigh << 8) | yLow;
    int16_t xValue = (xHigh << 8) | xLow;

    if(xValue > 1000) {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET); // Set Green
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET); // Reset Orange
    }
    else if(xValue < -1000) {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);   // set orange
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET); // reset green
    }
    else {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET); // reset orange
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET); // reset green
    }

    if(yValue > 1000) {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); // Set red
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET); // Reset blue
    }
    else if(yValue < -1000) {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);   // set blue
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET); // reset red
    }
    else {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET); // reset red
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET); // reset blue
    }

    HAL_Delay(100);
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

  // LED Initialization
  GPIO_InitTypeDef initCStr = {GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, GPIO_MODE_OUTPUT_PP,
                                GPIO_NOPULL, GPIO_SPEED_FREQ_LOW};

  HAL_GPIO_Init(GPIOC, &initCStr);
}

void initI2C(void) {
  // Exercise 5.3
  RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
  // Set the parameters in the TIMING register to use 100kHz standard-mode I2C
                    // PRESC     // SCLL  // SCLH       // SDADEL     // SCLDEL
  I2C2->TIMINGR |= (0x1 << 28) | (0x13) | (0xF << 8) | (0x2 << 16) | (0x4 << 20); 

  // Enable the I2C Peripheral using the PE bit in the CR1 register.
  I2C2->CR1 |= I2C_CR1_PE;

  // Exercise 5.4
  // Set RD_WRN bit to indicate write operation.
  I2C2->CR2 &= ~I2C_CR2_RD_WRN;
              // SADD = 0x69  // Transmit 1 byte. // Set Start bit
  I2C2->CR2 = (0x69 << 1) |  (0x1 << 16) |       I2C_CR2_START;
}

void initGyro(void) {
  writeRegisterI2C(0x69, 0x20, 0x1 | (0x1 << 1) | (0x1 << 3));
}

uint8_t readRegisterI2C(uint8_t devAddr, uint8_t regAddr)
{
    int data = 0;

    // Configure CR2: Transmit 1 byte (register address), Write mode
    I2C2->CR2 = (devAddr << 1) | (0x1 << 16) | I2C_CR2_START;

    // Wait until either the TXIS flag is set or the NACKF flag is set
    while(!((I2C2->ISR & I2C_ISR_TXIS) | (I2C2->ISR & I2C_ISR_NACKF))) {

    }

    if(I2C2->ISR & I2C_ISR_NACKF) {
        // send debug message
        return 0;
    }

    // SET TXDR to the address of teh WHO AM I register
    if(I2C2->ISR & I2C_ISR_TXIS) {
        I2C2->TXDR = regAddr;   // Address of requested register
    }

    // Wait until the TC flag is set
    while(!(I2C2->ISR & I2C_ISR_TC)) {

    }

    // Reload CR2 register
    if(I2C2->ISR & I2C_ISR_TC) {
        // SADD = device address    // Transmit 1 byte. // Set Start bit    // Read Operation
        I2C2->CR2 = (devAddr << 1) | (0x1 << 16) | I2C_CR2_START | I2C_CR2_RD_WRN;
    }

    // Wait until either of the RXNE or NACKF flags are set
    while(!((I2C2->ISR & I2C_ISR_RXNE) | (I2C2->ISR & I2C_ISR_NACKF))) {

    }

    if(I2C2->ISR & I2C_ISR_NACKF) {
        // send debug message
        return 0;
    }

    while(!(I2C2->ISR & I2C_ISR_TC)) {

    }

    data = I2C2->RXDR;

    I2C2->CR2 |= I2C_CR2_STOP;

    return data;
}

void writeRegisterI2C(uint8_t devAddr, uint8_t regAddr, uint8_t value)
{
    // Configure CR2: Transmit 2 bytes (register + data), Write mode
    I2C2->CR2 = (devAddr << 1) | (0x2 << 16) | I2C_CR2_START;

    // Wait until either the TXIS flag is set or the NACKF flag is set
    while(!((I2C2->ISR & I2C_ISR_TXIS) | (I2C2->ISR & I2C_ISR_NACKF))) {

    }

    if(I2C2->ISR & I2C_ISR_NACKF) {
        // send debug message
        while(1) {
          HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
          HAL_Delay(500);
        }
    }

    // SET TXDR to the address of the WHO AM I register
    if(I2C2->ISR & I2C_ISR_TXIS) {
        I2C2->TXDR = regAddr;   // Address of requested register
    }

    // Wait until either the TXIS flag is set or the NACKF flag is set again
    while(!((I2C2->ISR & I2C_ISR_TXIS) | (I2C2->ISR & I2C_ISR_NACKF))) {

    }

    if(I2C2->ISR & I2C_ISR_NACKF) {
        // send debug message
        while(1) {
          HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
          HAL_Delay(500);
        }
    }

    // SET TXDR to the address of the WHO AM I register
    if(I2C2->ISR & I2C_ISR_TXIS) {
        I2C2->TXDR = value;   // Address of requested register
    }

    // Wait until the TC flag is set

    if(I2C2->ISR & I2C_ISR_NACKF) {
        // send debug message
        while(1) {
          HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
          HAL_Delay(500);
        }
        
    }

    // Wait until the TC flag is set
    while(!(I2C2->ISR & I2C_ISR_TC)) {

    }

    I2C2->CR2 |= I2C_CR2_STOP;
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
