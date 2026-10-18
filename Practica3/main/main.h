#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "sdkconfig.h"
#include "driver/uart.h"

// Funciones para operar UART
void UART0_Init(void);

void UART0_ClrScr();
bool UART0_Kbhit();
void UART0_Putchar(char c);
void UART0_Puts(char *str);
char UART0_Getchar();
void UART0_Gets(char *str);
char UART0_GetcharAuto();
void UART0_GetsAUTO(char *str);
void UART0_Gotoxy(uint8_t x, uint8_t y);

void delayMs(uint16_t ms);

// DEFINICIONES PARA EL JUEGO PONG
#define WhiteColor 0xFFFFFF
#define BlackColor 0x000000
#define PONG_SCREEN_ROWS  25
#define PONG_SCREEN_COLUMS   80
#define POS_ROW_RAQUETA_ESP 1
#define POS_ROW_RAQUETA_PC 24
char pongScreen[PONG_SCREEN_ROWS][PONG_SCREEN_COLUMS];
const char Raqueta[6] = {219, 219, 219, 219, 219, 219};
const char Pelota = 254;
const char Left = 'A';
const char Right = 'D';

// FUNCIONES PARA EL JUEGO PONG
void initPongScreen(); // Funcion para inicializar la pantalla del pong 25 renglones x 80 columnas
void printPongScreen(); // Funcion para imprimir la pantalla del pong 25 renglones x 80 columnas
void updatePongState(); // Funcion para actualizar el estado del juego


#endif /* MAIN_H */
