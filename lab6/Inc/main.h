#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

void Error_Handler(void);
void setGPIO(void);
void configADC(void);
void configDAC(void);

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
