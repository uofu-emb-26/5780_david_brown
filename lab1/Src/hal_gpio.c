#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

void My_HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    // GPIO_InitTypeDef initStrPC69 = {GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, 
    //                                 GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 
    //                             GPIO_SPEED_FREQ_LOW};
    // HAL_GPIO_Init(GPIOC, &initStrPC69);

    // GPIO_InitTypeDef initStrPA0 = {GPIO_PIN_0, GPIO_MODE_INPUT,
    //                                 GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW};
    // HAL_GPIO_Init(GPIOA, &initStrPA0);
    
    // Set Blue and Red LED's registers
    GPIOC->MODER &= ~((3 << 12) | (3 << 14));  // clear both pairs
    GPIOC->MODER |=  ((1 << 12) | (1 << 14)); // set to output // Set moder to general purpose output mode
    GPIOC->OTYPER &= ~((1 << 6) | (1 << 7)); // Set OTYPER to push-pull output mode
    GPIOC->OSPEEDR &= ~((1 << 12) | (1 << 14)); // Set pins to low speed in the OSPEEDR register
    GPIOC->PUPDR &= ~((1 << 12) | (1 << 13) | (1 << 14) | (1 << 15)); // Sets to no pull-up/down resistors in PUPDR register
    
    // else if(GPIOx == GPIOA) {

    // }
}


/*
void My_HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin)
{
}
*/

/*
GPIO_PinState My_HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    return -1;
}
*/


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

