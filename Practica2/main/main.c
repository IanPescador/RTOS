// Practica 2: RTOS 

#include "main.h"

void printDelayedTasks(void)
{
	uint8_t i;

	printf("Tareas retrasadas:\n");
	for (i = 0; i < N; i++) {
		if (DelayedTask[i].TskID != 0) {
			printf("Tarea ID: %d, Delay: %d\n", DelayedTask[i].TskID, DelayedTask[i].Delay);
		}
	}
}

int findFreeDlyTsk(void)
{
	uint8_t i = 0;

	for (i = 0; i < N; i++) {
		if (DelayedTask[i].TskID == 0) {
			return i; // Regresar el índice libre
		}
	}
	return -1; // No hay espacio libre
}

void DeleteDlyTsk(void)
{
	uint8_t i = 0;

	// Limpiar el primer elemento del arreglo de tareas retrasadas
	DelayedTask[0].TskID = 0; 
	DelayedTask[0].Delay = 0; 

	for (i = 0; i < N - 1; i++) { // Desplazar las tareas retrasadas
		DelayedTask[i] = DelayedTask[i + 1];
	}

	// Limpiar el último elemento del arreglo de tareas retrasadas
	DelayedTask[N - 1].TskID = 0;
	DelayedTask[N - 1].Delay = 0;
}

void InsertDlyTsk(TD task)
{
	uint8_t i = 0, j = 0;
	int freeIndex = findFreeDlyTsk();
	uint32_t totalDelay = 0;


	if (freeIndex == -1) { // No hay espacio libre
		printf("No hay espacio libre en DelayedTask\n");
		return; 
	}

	if (freeIndex == 0) { // Si el arreglo está vacío, insertar la tarea directamente
		DelayedTask[0] = task;
		return;
	}

	// Si hay espacio libre
	for (i = 0; i < freeIndex; i++) { // Recorrer el arreglo de tareas retrasadas
		if (totalDelay + DelayedTask[i].Delay > task.Delay) {

            // Desplazar los demas tiempos a la derecha
            for (j = freeIndex; j > i; j--) {
                DelayedTask[j] = DelayedTask[j - 1];
            }

            // Insertar la nueva tarea en el lugar correcto
            DelayedTask[i].TskID = task.TskID;
            DelayedTask[i].Delay = task.Delay - totalDelay;

            // Ajustar el delay de la siguiente tarea
            DelayedTask[i + 1].Delay -= DelayedTask[i].Delay;

            return;
        }
        totalDelay += DelayedTask[i].Delay;   // sumar DESPUÉS de comparar
	}

	// La nueva tarea tiene mas delay que el total delay
	DelayedTask[freeIndex].TskID = task.TskID;              // Insertar la tarea al final del arreglo
	DelayedTask[freeIndex].Delay = task.Delay - totalDelay; // Insertar el delay de la tarea al final del arreglo
}

void LRN_DelayTsk(TD task)
{
	InsertDlyTsk(task);
}

void tickISR(void)
{
	if (DelayedTask[0].TskID == 0) { return; } // Si no hay tareas nada que hacer

	DelayedTask[0].Delay--; // Decrementar el delay de la primera tarea

	while (DelayedTask[0].Delay == 0 && DelayedTask[0].TskID != 0) { // Mientras haya tareas con delay 0
		DeleteDlyTsk(); // Eliminar la tarea del arreglo de tareas retrasadas
	}
}

int main(void) // Función principal prueba de codigo
{
	memset(DelayedTask, 0, sizeof(DelayedTask)); // Inicializar DelayedTask con ceros

	TD Tarea1, Tarea2, Tarea3, Tarea4, Tarea5 = {0}; // Inicializar en 0

	// Asignar valores a las tareas
	Tarea1.TskID = 1;
	Tarea1.Delay = 15;
	Tarea2.TskID = 2;
	Tarea2.Delay = 20;
	Tarea3.TskID = 3;
	Tarea3.Delay = 7;
	Tarea4.TskID = 4;
	Tarea4.Delay = 18;
	Tarea5.TskID = 5;
	Tarea5.Delay = 5;

	LRN_DelayTsk(Tarea1);

	printDelayedTasks();

	tickISR();

	printDelayedTasks();

	LRN_DelayTsk(Tarea2);

	printDelayedTasks();

	tickISR();

	printDelayedTasks();

	LRN_DelayTsk(Tarea3);

	printDelayedTasks();

	tickISR();

	printDelayedTasks();

	LRN_DelayTsk(Tarea4);

	printDelayedTasks();

	tickISR();

	printDelayedTasks();

	LRN_DelayTsk(Tarea5);

	printDelayedTasks();

	tickISR();

	printDelayedTasks();

	while (DelayedTask[0].TskID != 0) {
		tickISR();
		printDelayedTasks();
	}

	printf("Todas las tareas han sido ejecutadas.\n");
	return 0;
}