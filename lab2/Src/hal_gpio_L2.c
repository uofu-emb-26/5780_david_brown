#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>
#include "main.h"

void My_HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    uint32_t pinPos = 0;
    uint32_t pinMask = GPIO_Init->Pin;

    /* Find bit position (0–15) */
    while (((pinMask >> pinPos) & 1U) == 0)
    {
        pinPos++;
    }

    // Initialize LED Registers
    uint32_t mdrMsk = (3U << (pinPos * 2));
    
    // MODER 
    GPIOx->MODER &= ~(mdrMsk);
    GPIOx->MODER |=  (GPIO_Init->Mode << (pinPos * 2));

    // OTYPER
    if (GPIO_Init->Mode == GPIO_MODE_OUTPUT_PP)
        GPIOx->OTYPER &= ~(1U << pinPos);
    else
        GPIOx->OTYPER |=  (1U << pinPos);

    // OSPEEDR
    GPIOx->OSPEEDR &= ~(3U << (pinPos * 2));
    GPIOx->OSPEEDR |=  (GPIO_Init->Speed << (pinPos * 2));

    // PUPDR
    GPIOx->PUPDR &= ~(3U << (pinPos * 2));
    GPIOx->PUPDR |=  (GPIO_Init->Pull << (pinPos * 2));
}


/*
void My_HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin)
{
}
*/


GPIO_PinState My_HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    return GPIOA->IDR & GPIO_Pin;
}



void My_HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    if (PinState == GPIO_PIN_SET)
        GPIOx->ODR |= GPIO_Pin;
    else
        GPIOx->ODR &= ~GPIO_Pin;
}


void My_HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    GPIOx-> ODR ^= GPIO_Pin;
}

void Config_EXTI0() {
    EXTI->IMR |=EXTI_IMR_IM0;
    EXTI->RTSR |= EXTI_RTSR_RT0; 
}

