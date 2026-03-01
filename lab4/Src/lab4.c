#include "main.h"
#include "stm32f0xx_hal.h"
#include "string.h"

void SystemClock_Config(void);

char data[4];
int dataIndex;
int newDataFlag;

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
  
  // usartConfigP1();
  usartConfigP2();

  // UserButton Initialization
  GPIO_InitTypeDef pinA0Init = {GPIO_PIN_0, GPIO_MODE_INPUT,
                                GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW};
  HAL_GPIO_Init(GPIOA, &pinA0Init);

  // Transmit/Reciever Initialization
  GPIO_InitTypeDef initBStr = {GPIO_PIN_10 | GPIO_PIN_11, GPIO_MODE_AF_PP,
                                GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF4_USART3};

  HAL_GPIO_Init(GPIOB, &initBStr);

  // LED's Initialization
  GPIO_InitTypeDef initCStr = {GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, GPIO_MODE_OUTPUT_PP,
                                GPIO_NOPULL, GPIO_SPEED_FREQ_LOW};

  HAL_GPIO_Init(GPIOC, &initCStr);

  __NVIC_EnableIRQ(USART3_4_IRQn);
  __NVIC_SetPriority(USART3_4_IRQn, 1);

  uint8_t prevState = 0;
  uint8_t currState = 1;
  while (1)
  {
    // CheckOff 1
    // currState = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
    // if(currState && !prevState) {
    //   transmitChar('A');
    //   transmitString(" string is an array of chars in c. ");
    // }
    // prevState = currState;

    // if(USART3->ISR & USART_ISR_RXNE) {
    //   if (USART3->RDR == 'r') {    // RED LED
    //     HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
    //   }
    //   else if (USART3->RDR == 'b') {   // BLUE LED
    //     HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
    //   }
    //   else if (USART3->RDR == 'o') {   // ORANGE LED
    //     HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
    //   }
    //   else if (USART3->RDR == 'g') {   // GREEN LED
    //     HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);
    //   }
    //   else {
    //     transmitString("Error: Enter a valid input. ");
    //   }
    // }
    //HAL_Delay(30); // Delay 30ms 

    // prevState = currState;
    // Checkoff 2
    
    if(currState && !prevState) {
       transmitString("CMD? ");
       currState = 0;
    }
    if(newDataFlag) {
      char cmd[4];

      __disable_irq();
      if (dataIndex >= sizeof(data)) 
        dataIndex = sizeof(data)-1;
      memcpy(cmd, (const void*)data, sizeof(cmd));
      dataIndex = 0;
      newDataFlag = 0;
      __enable_irq();

      // RED LED
      if (strcmp(cmd, "r0\r") == 0) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
        transmitString("Turn off Red. ");
        transmitString("CMD? ");
      }
      else if (strcmp(cmd, "r1\r") == 0) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
        transmitString("Turn on Red. ");
        transmitString("CMD? ");
      }
      else if (strcmp(cmd, "r2\r") == 0) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
        transmitString("Toggle Red. ");
        transmitString("CMD? ");
      }

      // BLUE LED
      else if (strcmp(cmd, "b0\r") == 0) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
        transmitString("Turn off Blue. ");
        transmitString("CMD? ");
      }
      else if (strcmp(cmd, "b1\r") == 0) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
        transmitString("Turn on Blue. ");
        transmitString("CMD? ");
      }
      else if (strcmp(cmd, "b2\r") == 0) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
        transmitString("Toggle Blue. ");
        transmitString("CMD? ");
      }

      // ORANGE LED
      else if (strcmp(cmd, "o0\r") == 0) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
        transmitString("Turn off Orange. ");
        transmitString("CMD? ");
      }
      else if (strcmp(cmd, "o1\r") == 0) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
        transmitString("Turn on Orange. ");
        transmitString("CMD? ");
      }
      else if (strcmp(cmd, "o2\r") == 0) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
        transmitString("Toggle Orange. ");
        transmitString("CMD? ");
      }

      // GREEN LED
      else if (strcmp(cmd, "g0\r") == 0) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
        transmitString("Turn off Green. ");
        transmitString("CMD? ");
      }
      else if (strcmp(cmd, "g1\r") == 0) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
        transmitString("Turn on Green. ");
        transmitString("CMD? ");
      }
      else if (strcmp(cmd, "g2\r") == 0) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);
        transmitString("Toggle Green. ");
        transmitString("CMD? ");
      }
      else {
        transmitString("Error: Enter a valid input. \n");
      }
    }

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

void usartConfigP1(void) {
  RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN ;

  USART3->BRR = HAL_RCC_GetHCLKFreq() / 115200;

  USART3->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
}

void usartConfigP2(void) {
  RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN ;

  USART3->BRR = HAL_RCC_GetHCLKFreq() / 115200;

  USART3->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE | USART_CR1_RXNEIE;
}

void USART3_4_IRQHandler(void) {
  if(USART3->ISR & USART_ISR_RXNE) {
    char ch = USART3->RDR;
    if(dataIndex < sizeof(data)-1) 
      data[dataIndex++] = ch;
    else {
      dataIndex = 0;
    }

    if(ch == '\r') {
      if (dataIndex >= 1) {
        if (dataIndex <= sizeof(data)-1) {
          data[dataIndex] = '\0';
        } else {
          data[sizeof(data)-1] = '\0';
        }
      } else {
        data[0] = '\0';
      }
      newDataFlag = 1;
    } 
  }
  
}

void transmitChar(char sc) {
  while(!(USART3->ISR & USART_ISR_TXE)) {
    // wait until transmit data register is empty
  }
  USART3->TDR = sc;
}

void transmitString(char* str){
  while(*str) {
    transmitChar(*str);
    str++;
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
