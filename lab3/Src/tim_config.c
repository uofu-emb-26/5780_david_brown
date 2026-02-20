#include "tim.h"
#include "main.h"
#include "stm32f0xx_hal.h"
#include "hal_gpio_L3.h"

void tim2Config(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Configure timer to trigger an update event at 4 Hz
    TIM2->PSC = 7999;  // Set Prescaler to milliseconds
    TIM2->ARR = 250;   // Set Auto relode at 250 ticks.

    // Congire the timer to generate an interrupt on the UEV event.
    TIM2->DIER |= TIM_DIER_UIE;    // Configure the DIER control register to enable the update interrupt.

    TIM2->CR1 &= ~TIM_CR1_DIR;  // Clear bit 4. Set as upcounter.
    //TIM2->CR1 |= TIM_CR1_ARPE;     
    TIM2->CR1 |= TIM_CR1_CEN;   // Counter enabled.

    //TIM2->CR2 |= 0x10000; // Enable Counter enable signal in Master Mode Selection. pg 448.

    // NVIC Enable
    __NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler(void) {

    My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8 | GPIO_PIN_9);    
    TIM2->SR &= ~TIM_SR_UIF;
}

void tim3Config(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    
    // Configure timer to trigger an update event at 4 Hz
    TIM3->PSC = 71;  // Set Prescaler to milliseconds
    TIM3->ARR = 499;   // Set Auto relode at 1.25 ticks.

    TIM3->CCMR1 &= ~TIM_CCMR1_CC1S; // Configure channel 1 t0 output mode
    TIM3->CCMR1 &= ~TIM_CCMR1_CC2S; // Configure Channel 2 to output mode

    //TIM3->CCMR2 &= ~TIM_CCMR2_CC3S; // Configure Channel 3 to output mode
    //TIM3->CCMR2 &= ~TIM_CCMR2_CC4S; // Configure Channel 4 to output mode

    TIM3->CCMR1 &= ~(TIM_CCMR1_OC1M | TIM_CCMR1_OC2M);
    TIM3->CCMR1 = TIM_CCMR1_OC1M;  // Configure Channel 1 to PWM Mode 2
    TIM3->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2; // Configure Channel 2 to PWM Mode 1

    TIM3->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE; // Enable output compare preload for channel 1
    //TIM3->CR1 |= TIM_CR1_ARPE;

    //TIM3->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);
    TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E; // Capture compare 1 and 2 output enable

    // Set capture compare registers to 20% of ARR value
    TIM3->CCR1 = (TIM3->ARR) * 20/100; 
    TIM3->CCR2 = (TIM3->ARR) * 20/100;

    //TIM3->EGR = TIM_EGR_UG;

    TIM3->CR1 |= TIM_CR1_CEN;
}