// Practica 3: RTOS 
#include "main.h"

/**
 * @brief Inicializacion del UART con valores por defecto
 * nota: es necesario realizar la instalacion del driver por fuera
 */
void UART0_Init(void){
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, 2048, 0, 0, NULL, 0));       // Instalar controlador de UART0 con buffer de recepción de 2KB));
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_0, &uart_config));
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
    const char caClearScr[] = "\e[2J";
    uart_write_bytes(UART_NUM_0, caClearScr, sizeof(caClearScr));
}
  
//Function Kbhit
bool UART0_Kbhit(void){
    uint8_t length;
    uart_get_buffered_data_len(UART_NUM_0, (size_t*)&length);
    return (length > 0);
}
  
//Function Putchar
void UART0_Putchar(char c){
    uart_write_bytes(UART_NUM_0, &c, sizeof(c));
}
  
//Function Puts
void UART0_Puts(char *str){
    while (*str)
    {
        UART0_Putchar(*str);
        str++;
    }
}
  
//Function Getchar
char UART0_Getchar(void){
    char c;
    // Wait for a received byte
    while(!UART0_Kbhit())
    {
        delayMs(10);
    }
    // read byte, no wait
    uart_read_bytes(UART_NUM_0, &c, sizeof(c), 0);

    return c;
}
  
//Function Gets
void UART0_Gets(char *str){
    char c;
    uint8_t i=0;
    //Lee los caracteres con UART0_Getchar hasta que recibe un cambio de linea '\r'
    while ((c = UART0_Getchar()) != '\r')
    {
        if (c == '\b' && i > 0)
        {
            UART0_Putchar(c);   // Enviar el carácter de retroceso para mover el cursor a la izquierda
            UART0_Putchar(' '); // Enviar un espacio en blanco para borrar el carácter anterior
            UART0_Putchar(c);   // Enviar el carácter de retroceso para mover el cursor a la izquierda
            i--;

        //Limite de caracteres
        }else if(c != '\b')
        {
            //Añadimos caracteres a apuntador str y incrementamos para el siguiente
            UART0_Putchar(c);
            str[i++] = c;
        }
    }
    //Si ya no se detectan mas se agrega un caracter nulo para que termine la cadena
    str[i] = '\0';
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

// Function GetsAUTO
void UART0_GetsAUTO(char *str) {
    char c;
    uint8_t i = 0;

    while (UART0_Kbhit()) {
        uart_read_bytes(UART_NUM_0, &c, sizeof(c), portMAX_DELAY); // Espera hasta que llegue un byte
        str[i++] = c;
    }
    // Agrega un carácter nulo para terminar la cadena
    str[i] = '\0';
}

// Delay Function
void delayMs(uint16_t ms){
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

// FUNCIONES PARA EL JUEGO PONG
void initPongScreen(){
    for (int i = 0; i < PONG_SCREEN_ROWS; i++) {
        for (int j = 0; j < PONG_SCREEN_COLUMS; j++) {
            pongScreen[i][j] = ' '; // Inicializa con espacios en blanco
        }
    }
    // Coloca las raquetas y la pelota en sus posiciones iniciales
    for (int i = 0; i < 6; i++) {
        pongScreen[POS_ROW_RAQUETA_ESP][i] = Raqueta[i]; // Raqueta del jugador
        pongScreen[POS_ROW_RAQUETA_PC][i] = Raqueta[i];  // Raqueta de la PC
    }
    pongScreen[PONG_SCREEN_ROWS / 2][PONG_SCREEN_COLUMS / 2] = Pelota; // Pelota en el centro
}

void printPongScreen(){
    UART0_ClrScr();
    for (int i = 0; i < PONG_SCREEN_ROWS; i++) {
        UART0_Puts(pongScreen[i]);
        UART0_Putchar('\n');
    }
}

void app_main(void)
{
    // Inicializar UART0
    UART0_Init();

    // Inicializar la pantalla del juego Pong
    initPongScreen();

    while (1)
    {
        // Imprimir la pantalla del juego Pong
        // printPongScreen();
        UART0_Puts("Pong Game Screen\n");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Esperar 1 segundo
    }
}