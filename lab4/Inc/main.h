#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

void Error_Handler(void);

// Configure usart peripherals for lab4.
void usartConfig(void);
void transmitChar(char sc);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
