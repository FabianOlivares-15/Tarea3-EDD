En este *repl* puedes encontrar varios ejemplos que te pueden ayudar con las tareas de estructura.

## Código de (tarea3)
Para ejecutar el ejemplo tarea3 primero debemos compilar (en la carpeta raíz)
````
gcc tdas/*.c tarea3.c -Wno-unused-result -o tarea3
````

Y luego ejecutar:
````
./tarea3
````

## TDAs
En la carpeta `tdas` se encuentran implementados distintos TDAs que puedes utilizar (lista, pila, cola, cola con prioridad y mapas). 

Las implementaciones no son las más eficientes (todas usan como estructura de datos una **lista enlazada**), por lo que puedes reemplazarlas por las que has realizado en los labs.

## TODAS LAS FUNCIONES SIRVEN CORRECTAMENTE
1. busqueda en profundidad(DFS): utilizamos una pila, y buscamos una ruta funcional, no es la ruta mas corta.
2. busqueda en anchura(BFS): Utilizamos una cola, busca en forma de "ondas", encuentra la ruta en una menor cantidad de pasos posibles, pero mas iteraciones.
3. busqueda A*: Utilizamos el heap, debemos en cada iteracion utiizar el estado que minimice la funcion de evaluacion, la distanciaL1 + pasos, igual encuentra la ruta mas corta pero con menos iteraciones. 

## EJEMPLO DE FUNCIONAMIENTO
Iniciamos el programa el sistema nos pide ingresar una dificultad de 0 a 100, el programa imprime en pantalla el mapa recién creado, mostrando el inicio I, la meta M y los muros [X].
Presionamos el 1 y vemos la busqueda en profundidad(DFS), el programa calcula y vuelve a imprimir el mapa, ahora con el camino marcado con caracter +. Nos indica que llegó a la meta en, ejemplo: 40 pasos y con 75 iteraciones.
Presionamos el 2 y vemos la busqueda en anchura(BFS), el programa calcula e imprime el mapa marcado con caracter +, nos indica que llego en ejemplo: 20 pasos y 120 iteraciones.
Presionamos el 3 y vemos la busqueda A*, se imprime mostrando el mismo camino de 20 pasos, pero esta vez solo le tomó 35 iteraciones gracias al calculo de la funcion de evaluacion , la distanciaL1 + pasos.
