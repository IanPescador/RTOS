#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define N 10

// Descriptor de tarea
typedef struct {
	uint8_t TskID;
	uint32_t Delay;
}TD;

TD DelayedTask[N]; // Arreglo de tareas retrasadas

// Protocolos de funciones
int findFreeDlyTsk(void); 		   // Devuelve el indice libre en el arreglo de tareas retrasadas
void DeleteDlyTsk(void);			   // Elimina la tarea del arreglo de tareas retrasadas
void InsertDlyTsk(TD task);         // Inserta la tarea en el arreglo de tareas retrasadas
void LRN_DelayTsk(TD task);  // Coloca la tarea en el arreglo de tareas retrasadas
void tickISR(void);                    // Simula la interrupción de tick

#endif /* MAIN_H */
