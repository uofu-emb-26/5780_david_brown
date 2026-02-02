#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

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

    // if(GPIOx == GPIOC) {
    //     // Set Blue and Red LED's registers
    //     GPIOx->MODER &= ~((3 << 12) | (3 << 14));  // clear both pairs
    //     GPIOx->MODER |=  ((1 << 12) | (1 << 14)); // set to output // Set moder to general purpose output mode
    //     GPIOx->OTYPER &= ~((1 << 6) | (1 << 7)); // Set OTYPER to push-pull output mode
    //     GPIOx->OSPEEDR &= ~((1 << 12) | (1 << 14)); // Set pins to low speed in the OSPEEDR register
    //     GPIOx->PUPDR &= ~((1 << 12) | (1 << 13) | (1 << 14) | (1 << 15)); // Sets to no pull-up/down resistors in PUPDR register

    //     // Set Orange and Green LED's registers
    //     GPIOx->MODER &= ~((3 << 16) | (3 << 18));  // clear both pairs
    //     GPIOx->MODER |=  ((1 << 16) | (1 << 18)); // set to output // Set moder to general purpose output mode
    //     GPIOx->OTYPER &= ~((1 << 8) | (1 << 9)); // Set OTYPER to push-pull output mode
    //     GPIOx->OSPEEDR &= ~((1 << 16) | (1 << 18)); // Set pins to low speed in the OSPEEDR register
    //     GPIOx->PUPDR &= ~((1 << 16) | (1 << 17) | (1 << 18) | (1 << 19)); // Sets to no pull-up/down resistors in PUPDR register

    // }
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

