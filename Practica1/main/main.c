/* 
 *
 * Practica 1: RTOS 
 *
*/

#include "main.h"

void vDelayMs(int ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

void Init_recursos(void)
{
    // Configurar GPIOs para LEDs
    gpio_reset_pin(LED1_GPIO);
    gpio_set_direction(LED1_GPIO, GPIO_MODE_OUTPUT);
    gpio_reset_pin(LED2_GPIO);
    gpio_set_direction(LED2_GPIO, GPIO_MODE_OUTPUT);

    // Inicializar UART0
    const uart_config_t uart_config = {
        .baud_rate = 115200,                    // Velocidad de transmisión
        .data_bits = UART_DATA_8_BITS,          // Tamaño de palabra: 8 bits
        .parity = UART_PARITY_DISABLE,          // Sin paridad
        .stop_bits = UART_STOP_BITS_1,          // Bit de parada: 1
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE   // Sin control de flujo
    };
    
    uart_param_config(UART_NUM_0, &uart_config);                // Configurar parámetros de UART0
    uart_driver_install(UART_NUM_0, 2048, 0, 0, NULL, 0);   // Instalar controlador de UART0 con buffer de recepción de 2KB
}

void Led1_On(void)
{
    gpio_set_level(LED1_GPIO, 1);
}

void Led1_Off(void)
{
    gpio_set_level(LED1_GPIO, 0);
}

void Led2_On(void)
{
    gpio_set_level(LED2_GPIO, 1);
}

void Led2_Off(void)
{
    gpio_set_level(LED2_GPIO, 0);
}

void UART0_putchar(char data)
{
    uart_write_bytes(UART_NUM_0, &data, 1);
}

void UART_clrscr(void)
{
    uart_write_bytes(UART_NUM_0, "\033[2J\033[H", 7);
}

// Tarea 1: Blink LED1
void Tarea1 (void *pvParameters){
    int x;
    while(1){
        x = ( fast ? 50: 250 );
        Led1_On();
        vDelayMs( x );
        Led1_Off();
        vDelayMs( x );
    }
}

// Tarea 2: UART Output
void Tarea2 (void *pvParameters){
    static char msg[20]={"Task-2 Running\r\n"};
    uint8_t i=0;
    uint8_t k=0;
    char x;
    
    while(1){
        x= msg[i++];
        if( x!=0 ){
            UART0_putchar(x);
            vDelayMs(500);
        }else{
            i=0;
            vDelayMs(500);
            k++;
            if ( k==10 ){
                k=0;
                UART_clrscr();
            }
        }
    }
}

// Tarea 3: Blink LED2
void Tarea3 (void *pvParameters){
    uint8_t cnt = 0;
    
    while(1){
        Led2_On();
        vDelayMs(100);
        Led2_Off();
        vDelayMs(100);
        cnt++;
        if( cnt > 20 ){ // cada 20 contadores cambiar la variable fast
            cnt=0;
            fast = !fast;
            vDelayMs(1000);
        }
    }
}

void app_main(void)
{
    Init_recursos();

    xTaskCreate(
        Tarea1,       // Función
        "Tarea1",     // Nombre
        2048,         // Stack en bytes en ESP-IDF
        NULL,         // Parámetro
        1,            // Prioridad
        NULL          // Handle opcional
    );

    xTaskCreate(
        Tarea2,       // Función
        "Tarea2",     // Nombre
        2048,         // Stack en bytes en ESP-IDF
        NULL,         // Parámetro
        1,            // Prioridad
        NULL          // Handle opcional
    );

    xTaskCreate(
        Tarea3,       // Función
        "Tarea3",     // Nombre
        2048,         // Stack en bytes en ESP-IDF
        NULL,         // Parámetro
        1,            // Prioridad
        NULL          // Handle opcional
    );
}