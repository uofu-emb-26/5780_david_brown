#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

void Error_Handler(void);
void setGPIO(void);
void initI2C(void);
void initGyro(void);

#include <stdint.h>

uint8_t readRegisterI2C(uint8_t devAddr, uint8_t regAddr);
void writeRegisterI2C(uint8_t devAddr, uint8_t regAddr, uint8_t value);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
