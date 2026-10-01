#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "sdkconfig.h"

bool fast = false;

#define LED1_GPIO 2
#define LED2_GPIO 4

/*** funciones para operar recursos */
void Led1_On(void);
void Led1_Off(void);
void Led2_On(void);
void Led2_Off(void);
void UART0_putchar(char data);
void UART_clrscr(void);

/**** Tareas ******/
void Tarea1 (void *pvParameters);
void Tarea2 (void *pvParameters);
void Tarea3 (void *pvParameters);

#endif /* MAIN_H */
