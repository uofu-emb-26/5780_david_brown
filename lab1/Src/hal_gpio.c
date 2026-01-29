#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

void My_HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    GPIO_InitTypeDef initStrPC69 = {GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, 
                                    GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 
                                GPIO_SPEED_FREQ_LOW};
    HAL_GPIO_Init(GPIOC, &initStrPC69);

    GPIO_InitTypeDef initStrPA0 = {GPIO_PIN_0, GPIO_MODE_INPUT,
                                    GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW};
    HAL_GPIO_Init(GPIOA, &initStrPA0);

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

/*
void My_HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
}
*/

/*
void My_HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
}
*/
