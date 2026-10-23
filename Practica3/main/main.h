#ifndef MAIN_H
#define MAIN_H
 
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "sdkconfig.h"
#include "driver/uart.h"
 
// Funciones para operar UART
void UART0_Init(void);
void UART0_ClrScr(void);
void UART0_SetTheme(void);
void UART0_RemoveCursor(void);
void UART0_AddCursor(void);
bool UART0_Kbhit(void);
void UART0_Putchar(char c);
void UART0_Puts(const char *str);
char UART0_GetcharAuto(void);
 
// Funcion para el delay
void delayMs(uint16_t ms);
 
// DEFINICIONES PARA EL JUEGO PONG
#define PONG_SCREEN_ROWS 25
#define PONG_SCREEN_COLS   80
#define RAQUETA_LEN 6         
#define CHAR_RAQUETA 219
#define CHAR_PELOTA 254
#define POS_ROW_RAQUETA_ESP  0          // Raqueta de la computadora
#define POS_ROW_RAQUETA_PC 23           // Raqueta del usuario
#define POS_ROW_PUNTO 24                // Renglon del Punto
#define LEFT 'a'                        // Tecla para ir a la izquierda
#define RIGHT 'd'                       // Tecla para ir a la derecha

unsigned char pongScreen[PONG_SCREEN_ROWS][PONG_SCREEN_COLS];

// FUNCIONES PARA EL JUEGO PONG
void initPongScreen(void);                          // Funcion para inicializar la pantalla del pong 25 renglones x 80 columnas
void printPongScreen(void);                         // Funcion para imprimir la pantalla del pong 25 renglones x 80 columnas
void updatePongState(void);                         // Funcion para actualizar el estado del juego
void moveRaqueta(int raqueta, int *posX, int newX); // Funcion para mover las raquetas
void movePC(void);                                  // Funcion para mover la raqueta del jugador
void moveESP(void);                                 // Funcion para mover la raqueta del ESP
void moveBall(void);                                // Funcion para mover la pelota

// Variables para el juego Pong
int raquetaEspX;       // columna donde empieza la raqueta de la computadora
int raquetaPcX;        // columna donde empieza la raqueta del usuario
int ballX, ballY;      // posicion de la pelota (columna, renglon)
int ballDx, ballDy;    // pendiente de la recta: m = ballDy / ballDx
bool gameOver = false;
 
#endif /* MAIN_H */
