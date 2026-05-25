#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include "tdas/stack.h"
#include "tdas/queue.h"
#include <string.h>
#include <time.h>

#define arriba 1
#define abajo 2
#define derecha 3
#define izquierda 4




// Definición de la estructura para el estado del puzzle
typedef struct State{
    // Matriz NxN que representa el laberinto
    int x;    // Posición x del agente
    int y;    // Posición y del agente
    int steps; // Pasos realizados hasta la posición actual
    struct State* parent; //Secuencia de movimientos para llegar al estado
    // Lista de acciones realizadas
} State;

int distancia_L1(State* state) {
    return abs(state->x - (N-1)) + abs(state->y - (N-1));
}

// Función para imprimir el estado actual
void imprimirEstado(const State *estado, int maze[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (estado->x==i && estado->y==j) printf(" A ");
            else if (i == 0 && j == 0) printf(" I "); 
            else if (i == N-1 && j == N-1) printf(" M ");
            else if (maze[i][j] == 0)
                printf(" . "); // Imprime un espacio en blanco para el espacio vacío
            else
                printf("[X]");
        }
        printf("\n");
    }
}
// se inicializa el estado inicial donde empezamos con sus variables
State* crearEstadoInicial(int maze[N][N], int dificultad){
     // Copiar el laberinto generado al estado
    generate_maze(maze,  dificultad);
    State *estado = (State*)malloc(sizeof(State));
    estado->x = 0;
    estado->y = 0;
    estado->steps = 0;
    estado->parent = NULL;
    //estado.actions = list_create();
    return estado;
}

int is_final_state(State *estado_actual){
    if(estado_actual->x == (N - 1) && estado_actual->y == (N - 1)){
        return 1;
    }
    return 0;
}

State *transition(State *estado_actual, int accion, int maze[N][N]){
    State *nuevo = (State*)malloc(sizeof(State)); // creamos nuevo estado
    if(nuevo == NULL) return NULL;
    nuevo->x = estado_actual->x; // asignamos las variables del struct
    nuevo->y = estado_actual->y;
    nuevo->steps = estado_actual->steps + 1; // le sumamos un paso mas
    nuevo->parent = estado_actual; // dejamos como padre al actual, para saber luego el camino recorrido
    if(accion == arriba) nuevo->x -= 1; // se mueve segun la opcion que se elige
    else if(accion == abajo) nuevo->x += 1;
    else if(accion == derecha) nuevo->y += 1;
    else if(accion == izquierda) nuevo->y -= 1;
    if(nuevo->x < 0 || nuevo->x >= N || nuevo->y < 0 || nuevo->y >= N){ // se valida para que no se salga del maze
        free(nuevo);
        return NULL;
    }
    if(maze[nuevo->x][nuevo->y] != 0){ // se valida que no choque
        free(nuevo);
        return NULL;
    }
    return nuevo;
}

List *get_adjacent_nodes(State* estado_actual, int maze[N][N]){
    List *adj = list_create(); // crea lista adyacentes
    for(int accion = 1; accion <= 4; accion ++){ // recorremos en los movimientos
        State *nuevo = transition(estado_actual, accion, maze);  // generamos el estado
        if(nuevo != NULL){ //si fue valido lo agregamos a la lista
            list_pushBack(adj, nuevo);
        }
    }
    return adj;
}
void imprimirRutaFinal(State *estado_final, int maze[N][N]){
    int copiaMaze[N][N]; //creamos una copia de la original para no modificar la original
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            copiaMaze[i][j] = maze[i][j];
    State *actual = estado_final; //nos ubicamos en el ultimo y vamos retrocediendo y marcamos camino
    while(actual != NULL){
        copiaMaze[actual->x][actual->y] = 2;
        actual = actual->parent;
    }
    for(int i = 0; i < N; i++){ //marcamos nuestro camino con un caracter por ejemplo +
        for(int j = 0; j < N; j++){
            if(i == 0 && j == 0) printf(" I ");
            else if(i == N-1 && j == N-1) printf(" M ");
            else if(copiaMaze[i][j] == 2) printf(" + ");
            else if(copiaMaze[i][j] == 1) printf("[X]");
            else printf(" . ");
        }
        printf("\n");
    }
}
void dfs(State *estado_actual, int maze[N][N]){
    Stack *pila = stack_create(NULL); //creamos la pila 
    int visitado[N][N] = {0}; // usamos una matriz con ceros para marcar los visitados
    int cont = 0; //creamos un contador para las iteraciones
    stack_push(pila, estado_actual); //agregamos a la pila el incio 
    while(stack_top(pila) != NULL){ 
        State *actual = (State*)stack_pop(pila); //sacamos de la pila
        cont++; //aumenta el contador
        if(is_final_state(actual)){ // si llegamos al final, imprimimos la la ruta encontrado, mostramos pasos e iteraciones
            printf("\n Busqueda en profundidad\n");
            imprimirRutaFinal(actual, maze);
            printf("Pasos desde el principio: %d\n", actual->steps);
            printf("Cantidad iteraciones: %d\n", cont);
            free(pila);
            return;
        }
        if(visitado[actual->x][actual->y] == 1){ //revisamos si ya pasamos por ahi entonces no lo tomamos y avanzamos
            free(actual);
            continue;
        }
        visitado[actual->x][actual->y] = 1; // lo marcamos como visitado
        List *adyacentes = get_adjacent_nodes(actual, maze); //obtenemos los adyacentes
        State *adyacenteValido = (State*)list_popFront(adyacentes); 
        while(adyacenteValido != NULL){ //recorremos adyacentes
            if(visitado[adyacenteValido->x][adyacenteValido->y] == 0){ // si no esta visitado lo metemos a la pila
                stack_push(pila, adyacenteValido);
            }
            else free(adyacenteValido);
            adyacenteValido = (State*)list_popFront(adyacentes);
        }
        free(adyacentes);
    }
    printf("\nSolucion de busqueda no encontrada\n");
    stack_clean(pila);
    free(pila);
}
void bfs(State *estado_actual, int maze[N][N]){
    Queue *cola = queue_create(NULL); //creamos la cola
    int visitado[N][N] = {0}; // usamos una matriz con ceros para marcar los visitados
    int cont = 0; //creamos contador de iteraciones
    queue_insert(cola, estado_actual); //agregamos a la cola el estado actual
    while(queue_front(cola) != NULL){
        State *actual = (State*)queue_remove(cola); //sacamos de la cola
        cont++; //aumentamos el contador
        if(is_final_state(actual)){ // si llegamos al final, imprimimos la la ruta encontrado, mostramos pasos e iteraciones
            printf("\n Busqueda en anchura\n");
            imprimirRutaFinal(actual, maze);
            printf("Pasos desde el principio: %d\n", actual->steps);
            printf("Cantidad iteraciones: %d\n", cont);
            free(cola);
            return;
        }
        if(visitado[actual->x][actual->y] == 1){ //revisamos si ya pasamos por ahi entonces no lo tomamos y avanzamos
            free(actual);
            continue;
        }
        visitado[actual->x][actual->y] = 1; //lo marcamos como visitado
        List *adyacentes = get_adjacent_nodes(actual,maze); //obtenemos los adyacentes
        State *adyacenteValido = (State*)list_popFront(adyacentes);
        while(adyacenteValido != NULL){ //recorremos los adyacentes
            if(visitado[adyacenteValido->x][adyacenteValido->y] == 0){ //si no esta visitado lo metemos a la cola
                queue_insert(cola, adyacenteValido);
            }
            else free(adyacenteValido);
            adyacenteValido = (State*)list_popFront(adyacentes);
        }
        free(adyacentes);
    }
    printf("\nSolucion de busqueda no encontrada\n");
    queue_clean(cola);
    free(cola);
    
}

void best_first(State *estado_actual, int maze[N][N]){
    Heap *heap = heap_create(); //creamos el heap
    int visitado[N][N] = {0}; // usamos una matriz con ceros para marcar los visitados
    int cont = 0; //creamos contador de iteraciones
    int pasos = estado_actual->steps;
    int prioridad = -(pasos + distancia_L1(estado_actual)); //calculamos la prioridad sumando pasos + distanciaL1, se deja en negativo 
    heap_push(heap, estado_actual, prioridad); //se agrega al heap
    while(heap_top(heap) != NULL){
        State *actual = (State*)heap_top(heap); //sacamos el mejor por prioridad
        heap_pop(heap);
        cont++; //aumentamos el contador
        if(is_final_state(actual)){ // si llegamos al final, imprimimos la la ruta encontrado, mostramos pasos e iteraciones
            printf("\nBusqueda A* encontrada\n");
            imprimirRutaFinal(actual, maze);
            printf("Pasos desde el principio: %d\n", actual->steps);
            printf("Cantidad iteraciones: %d\n", cont);
            free(heap);
            return;
        }
        if(visitado[actual->x][actual->y] == 1){ //revisamos si ya pasamos por ahi entonces no lo tomamos y avanzamos
            free(actual);
            continue;
        }
        visitado[actual->x][actual->y] = 1; //lo marcamos como visitado
        List *adyacentes = get_adjacent_nodes(actual, maze); //obtenemos los adyacentes
        State *adyacenteValido = (State*)list_popFront(adyacentes);
        while(adyacenteValido != NULL){ //recorremos los adyacentes
            if(visitado[adyacenteValido->x][adyacenteValido->y] == 0){ //si no esta visitado, calculamos su prioridad, se agrega al heap
                int steps = adyacenteValido->steps;
                int prio = -(steps + distancia_L1(adyacenteValido));
                heap_push(heap, adyacenteValido, prio);
            }
            else{
                free(adyacenteValido);
            }
            adyacenteValido = (State*)list_popFront(adyacentes);
        }
        free(adyacentes);
    }
    printf("\nSolucion de busqueda no encontrada\n");
    free(heap);
}

int main() {
    // Inicializar la semilla de aleatoriedad
    srand(time(NULL));

    int maze[N][N];
    int dificultad;

    // Solicitar la dificultad al usuario con validación
    do {
        printf("Ingrese la dificultad del laberinto (porcentaje de obstáculos, 0 a 100): ");
        scanf("%d", &dificultad);
        if (dificultad < 0 || dificultad > 100) {
            printf("Error: Por favor ingrese un valor válido entre 0 y 100.\n");
        }
    } while (dificultad < 0 || dificultad > 100);

    // Estado inicial del puzzle
    // Laberinto generado con la dificultad ingresada por el usuario
    State* estado_inicial = crearEstadoInicial(maze, dificultad);

    // Imprime el estado inicial
    printf("\nEstado inicial del puzzle:\n");
    imprimirEstado(estado_inicial, maze);

    printf("Distancia L1: %d\n", distancia_L1(estado_inicial));

    // Ejemplo de heap (cola con prioridad)
    /*printf("\n***** EJEMPLO USO DE HEAP ******\nCreamos un Heap e insertamos 3 elementos con distinta prioridad\n");
    Heap* heap = heap_create();
    char* data = strdup("Cinco");
    printf("Insertamos el elemento %s con prioridad -5\n", data);
    heap_push(heap, data, -5 /*prioridad*);
    data = strdup("Seis");
    printf("Insertamos el elemento %s con prioridad -6\n", data);
    heap_push(heap, data, -6 /*prioridad*);
    data = strdup("Siete");
    printf("Insertamos el elemento %s con prioridad -7\n", data);
    heap_push(heap, data, -7 /*prioridad*);

    printf("\nLos elementos salen del Heap ordenados de mayor a menor prioridad\n");
    while (heap_top(heap) != NULL){
        printf("Top: %s\n", (char*) heap_top(heap));      
        heap_pop(heap);
    }
    printf("No hay más elementos en el Heap\n");*/

    char opcion;
    do {
        printf("\n***** EJEMPLO MENU ******\n");
        puts("========================================");
        puts("     Escoge método de búsqueda");
        puts("========================================");

        puts("1) Búsqueda en Profundidad");
        puts("2) Buscar en Anchura");
        puts("3) Buscar Mejor Primero");
        puts("4) Salir");

        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);

        switch (opcion) {
        case '1':
            dfs(estado_inicial, maze);
          break;
        case '2':
            bfs(estado_inicial, maze);
          break;
        case '3':
            best_first(estado_inicial, maze);
          break;
        }

        // Evitamos pausar y limpiar pantalla si el usuario eligió salir
        if (opcion != '4') {
            presioneTeclaParaContinuar();
            limpiarPantalla();
        }

  } while (opcion != '4');

  return 0;
}
