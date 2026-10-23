// Practica 3: RTOS - Ping-Pong
#include "main.h"

/**
 * @brief Inicializacion del UART con valores por defecto
 * nota: es necesario realizar la instalacion del driver por fuera
 */
void UART0_Init(void){
    // Inicializar UART0
    const uart_config_t uart_config = {
        .baud_rate = 115200,                    // Velocidad de transmisión
        .data_bits = UART_DATA_8_BITS,          // Tamaño de palabra: 8 bits
        .parity = UART_PARITY_DISABLE,          // Sin paridad
        .stop_bits = UART_STOP_BITS_1,          // Bit de parada: 1
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE   // Sin control de flujo
    };

    ESP_ERROR_CHECK(uart_param_config(UART_NUM_0, &uart_config));                // Configurar parámetros de UART0
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, 2048, 0, 0, NULL, 0));       // Instalar controlador de UART0 con buffer de recepción de 2KB
}

// Moves the cursor to a specific coordinate
void UART0_Gotoxy(uint8_t x, uint8_t y){
    char buf[10];
    const char GoTo[] = "\e[%d;%dH";
    snprintf(buf, sizeof(buf),GoTo , y, x);
    uart_write_bytes(UART_NUM_0, buf, strlen(buf));
}

//Function Clear
void UART0_ClrScr(void){
    const char caClearScr[] = "\033[2J\033[H";
    uart_write_bytes(UART_NUM_0, caClearScr, sizeof(caClearScr));
}

// Funcion para colocar la pantalla con texto blanco y fondo negro
void UART0_SetTheme(void) {
    const char caTextWhiteBackBlack[] = "\033[37;40m";
    uart_write_bytes(UART_NUM_0, caTextWhiteBackBlack, sizeof(caTextWhiteBackBlack));
}

// Quitar Cursor
void UART0_RemoveCursor(void){
    const char caRemoveCur[] = "\x1b[?25l";
    uart_write_bytes(UART_NUM_0, caRemoveCur, sizeof(caRemoveCur));
}

// Devolver Cursor
void UART0_AddCursor(void){
    const char caAddCur[] = "\x1b[?25h";
    uart_write_bytes(UART_NUM_0, caAddCur, sizeof(caAddCur));
}

//Function Kbhit
bool UART0_Kbhit(void){
    size_t length = 0;
    uart_get_buffered_data_len(UART_NUM_0, &length);
    return (length > 0);
}

//Function Putchar
void UART0_Putchar(char c){
    uart_write_bytes(UART_NUM_0, &c, 1);
}

//Function Puts
void UART0_Puts(const char *str){
    while (*str)
    {
        UART0_Putchar(*str);
        str++;
    }
}

// Function GetcharAuto
char UART0_GetcharAuto(void){
    char c = '\0';
    if(UART0_Kbhit())
    {
        uart_read_bytes(UART_NUM_0, &c, sizeof(c), 0);
    }
    return c;
}

// Delay Function
void delayMs(uint16_t ms){
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

// FUNCIONES PARA EL JUEGO PONG
void initPongScreen(void){
    uint8_t i;
    // Pantalla vacia
    memset(pongScreen, ' ', sizeof(pongScreen));

    // Raquetas centradas
    raquetaEspX = (PONG_SCREEN_COLS - RAQUETA_LEN) / 2;
    raquetaPcX  = raquetaEspX;

    for (i = 0; i < RAQUETA_LEN; i++) { // Colocar las raquetas en la pantalla
        pongScreen[POS_ROW_RAQUETA_ESP][raquetaEspX + i] = CHAR_RAQUETA;   // computadora
        pongScreen[POS_ROW_RAQUETA_PC][raquetaPcX + i]   = CHAR_RAQUETA;   // usuario
    }

    // Pelota al centro, va hacia abajo a la derecha al inicio
    ballX  = PONG_SCREEN_COLS / 2;
    ballY  = PONG_SCREEN_ROWS / 2;
    ballDx = 1;
    ballDy = 1;
    pongScreen[ballY][ballX] = CHAR_PELOTA;
}

// Funcion para imprimir la pantalla
void printPongScreen(void){
    uint8_t row;

    UART0_Gotoxy(1, 1);
    for (row = 0; row < PONG_SCREEN_ROWS; row++) {
        uart_write_bytes(UART_NUM_0, (const char *)pongScreen[row], PONG_SCREEN_COLS);
        if (row < PONG_SCREEN_ROWS - 1) {
            UART0_Puts("\r\n");
        }
    }
}

// Mueve una raqueta dentro del arreglo, con topes al inicio y final de la pantalla
void moveRaqueta(int raqueta, int *posX, int newX){
    uint8_t i;

    if (newX < 0) // Si esta pegado a la izquierda no puede ir mas atras
        newX = 0;

    if (newX > PONG_SCREEN_COLS - RAQUETA_LEN) // Si esta pegado a la derecha no se puede mover
        newX = PONG_SCREEN_COLS - RAQUETA_LEN;

    for (i = 0; i < RAQUETA_LEN; i++){
        pongScreen[raqueta][*posX + i] = ' '; // Borra la raqueta
    }

    *posX = newX;

    for (i = 0; i < RAQUETA_LEN; i++) {
        pongScreen[raqueta][*posX + i] = CHAR_RAQUETA; // Poner nueva raqueta
    } 
}

// Raqueta del usuario: 'a' izquierda, 'd' derecha
void movePC(void){
    char c;

    while (UART0_Kbhit()) { // Entrar si se presiona cualquier tecla
        c = UART0_GetcharAuto();
        if (c == LEFT)  
            moveRaqueta(POS_ROW_RAQUETA_PC, &raquetaPcX, raquetaPcX - 1);
        if (c == RIGHT) 
            moveRaqueta(POS_ROW_RAQUETA_PC, &raquetaPcX, raquetaPcX + 1);
    }
}

// Raqueta de la computadora sigue la pelota
void moveESP(void){
    moveRaqueta(POS_ROW_RAQUETA_ESP, &raquetaEspX, ballX - RAQUETA_LEN / 2);
}

//   Pelota Movimiento
//   Ecuacion de la recta: newX = x + dx , newY = y + dy (pendiente m = dy/dx)
//   Rebote en paredes: dx = -dx
//   Rebote en raqueta: dy = -dy
void moveBall(void){
    // Depende donde pegue la pelota en la raqueta cambia el angulo de dx
    static const int reboteRaqueta[RAQUETA_LEN] = { -2, -1, -1, 1, 1, 2 };

    pongScreen[ballY][ballX] = ' '; //Borrar donde estaba la pelota

    int nx = ballX + ballDx; // Siguiente punto x
    int ny = ballY + ballDy; // Siguiente punto y

    // Rebote con las paredes
    if (nx < 0) {
        nx = 1;
        ballDx = -ballDx; // Cambio de sentido x
    }
    if (nx > PONG_SCREEN_COLS - 1) {
        nx = PONG_SCREEN_COLS - 2;
        ballDx = -ballDx; // Cambio de sentido x
    }

    if (ny == POS_ROW_RAQUETA_ESP) { // Llego a la posicion de la raqueta del ESP
        if (nx >= raquetaEspX && nx < raquetaEspX + RAQUETA_LEN) { // Revisar si toco la raqueta
            ballDy = -ballDy; // Cambio de sentido y
            ballDx = reboteRaqueta[nx - raquetaEspX]; // Cambio del rebote en x
            ny = ballY;
        } else {
            gameOver = true; // El ESP fallo se acaba el juego (No deberia ser posible)
        }
    } else if (ny == POS_ROW_RAQUETA_PC) { // Llego a la posicion de la raqueta del Jugador
        if (nx >= raquetaPcX && nx < raquetaPcX + RAQUETA_LEN) { // Revisar si toco la raqueta
            ballDy = -ballDy; // Cambio de sentido y
            ballDx = reboteRaqueta[nx - raquetaPcX]; // Cambio del rebote en x
            ny = ballY; // rebota se queda en su renglon
        } else {
            gameOver = true; // El Jugador fallo se acaba el juego             
        }
    }

    ballX = nx; // Nueva posicion x de la pelota 
    ballY = ny; // Nueva posicion y de la pelota
    pongScreen[ballY][ballX] = CHAR_PELOTA; // Dibujar la pelota en su nueva posicion
}

// Una pasada del juego
void updatePongState(void){
    movePC();       // Movimiento de la raqueta del jugador
    moveESP();      // Movimiento de la raqueta de la computadora sigue la pelota
    moveBall();     // Movimiento de la pelota
}

void app_main(void)
{
    UART0_Init();
    UART0_SetTheme();
    UART0_ClrScr();

    UART0_RemoveCursor();

    initPongScreen();
    printPongScreen();

    while (!gameOver)
    {
        updatePongState();
        printPongScreen();
        delayMs(10);
    }

    UART0_AddCursor();

    UART0_Gotoxy(1,25); //Colocar al final de la pantalla
    UART0_Puts("GAME OVER");
}