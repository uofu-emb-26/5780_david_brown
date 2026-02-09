#include "tim2.h"
#include "main.h"
#include "stm32f0xx_hal.h"
#include "hal_gpio_L3.h"

void tim2Config(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Configure timer to trigger an update event at 4 Hz
    TIM2->PSC = 7999;  // Set Prescaler to milliseconds
    TIM2->ARR = 250;   // Set Auto relode at 250 ticks.

    //TIM2->EGR = TIM_EGR_UG;

    // Congire the timer to generate an interrupt on the UEV event.
    TIM2->DIER |= TIM_DIER_UIE;    // Configure the DIER control register to enable the update interrupt.

    TIM2->CR1 &= ~TIM_CR1_DIR;  // Clear bit 4. Set as upcounter.
    TIM2->CR1 |= TIM_CR1_ARPE;     // Counter enabled.
    TIM2->CR1 |= TIM_CR1_CEN;

    //TIM2->CR2 |= 0x10000; // Enable Counter enable signal in Master Mode Selection. pg 448.

    // NVIC Enable
    __NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler(void) {

    My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8 | GPIO_PIN_9);    
    TIM2->SR &= ~TIM_SR_UIF;
}