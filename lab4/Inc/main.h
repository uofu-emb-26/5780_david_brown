#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

void Error_Handler(void);

// Configure usart peripherals for lab4.
void usartConfigP1(void);
void usartConfigP2(void);
void transmitChar(char sc);
void transmitString(char* str);

void USART3_4_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
