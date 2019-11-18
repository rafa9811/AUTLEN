/*******************************************************************************
** AUTÓMATAS Y LENGUAJES - PRÁCTICA 1
** intermdia.h
**
** Fichero que contiene la declaración de las funciones de la estructura inter-
** media, así como las macros identificativas de los tipos de estado.
**
** Autores:
** - Carlos Molinero Alvarado - carlos.molineroa@estudiante.uam.es
** - Rafael Hidalgo Alejo - rafael.hidalgoa@estudiante.uam.es
**
** Última modificación: 17 de noviembre de 2019.
*******************************************************************************/

/* Librerías.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Macros.*/
#define TAM 64

#define INICIAL	0
#define FINAL	1
#define INICIAL_Y_FINAL	2
#define NORMAL 3
#define UNDEFINED 10

/**
struct auti

Tipo Abstracto de Dato auti (automata intemedio). Está compuesto a su vez por
dos tipos abstractos de datos: nuevoestado y transicion.
Un elemento del tipo auti* estará formado por tanto por:
   1. Una lista (array) de estados.
   2. Una lista (array) de transiciones.
   3. El alfabeto que reconoce.
   4.y 5. El número de estados y transiciones que posee.
   6. El número de símbolos que posee el alfabeto.
**/
typedef struct _auti auti;

/**
struct nuevoestado

Tipo Abstracto de Dato nuevoestado. Posee un nombre, estados antiguos que lo
forman, una variable indicativa del tipo de estado y el número de estados por
los que está formado.
**/
typedef struct _nuevoestado nuevoestado;

/**
struct nuevoestado

Tipo Abstracto de Dato transicion. Se compone de un estado (nuevoestado) inicial,
otro final y el símbolo con el que se transita.
**/
typedef struct _transicion transicion;

/*******************************************************************************
** 1.FUNCIONES TRANSICIÓN.
*******************************************************************************/

/**
t_ini

Esta función se encarga de inicializar y reservar memoria para una transicion.

Argumentos de entrada:
  - Ninguno.
Retorno:
  - transicion: la transición inicializada.
  - NULL en caso de error.
**/
transicion* t_ini();

/**
t_free

Libera la memoria utilizada por una transición.

Argumentos de entrada:
  - t: la transicion a liberar.
Retorno:
  - Ninguno.
**/
void t_free( transicion *t );

/**
t_getEini

Retorna el estado inicial de la transicion. OJO, devuelve una copia.

Argumentos de entrada:
  - t: transicion de la que queremos obtener el estado inicial.
Retorno:
  - nuevoestado: el estado inicial.
  - NULL, en caso de error.
**/
nuevoestado* t_getEini( const transicion *t );

/**
t_getEfin

Retorna el estado final de la transicion. OJO, devuelve una copia.

Argumentos de entrada:
  - t: transicion de la que queremos obtener el estado final.
Retorno:
  - nuevoestado: el estado final.
  - NULL, en caso de error.
**/
nuevoestado* t_getEfin( const transicion *t );

/**
t_getSimbolo

Retorna el atributo simbolo mediante el cual se transita en una transicion.
Ojo, retorna una copia de la cadena.

Argumentos de entrada:
   - t: transicion de la que queremos obtener el simbolo.
Retorno:
   - char*: el simbolo.
   - NULL, en caso de error.
**/
char* t_getSimbolo( const transicion *t );

/**
copy_transicion

Funcion auxiliar que realiza una copia de la transicion t2 en la transicion t1.

Argumentos de entrada:
   - t1: transicion sobre la que se copia.
   - t2: transicion de la cual se realiza la copia.
Retorno:
   - Ninguno.
**/
void copy_transicion( transicion *t1, transicion *t2 );

/**
t_set

"Setter" de una transicion. Establece sus parámetros.

Argumentos de entrada:
   - t: transicion a settear.
   - inicial: nuevoestado inicial a settear.
   - final: nuevoestado final a settear.
   - simbolo: simbolo a settear.
Retorno:
   - transicion: la transicion actualizada.
**/
transicion* t_set( transicion *t, nuevoestado *inicial, nuevoestado *final, char *simbolo );

/**
t_set

"Setter" de una transicion. Únicamente establece su estado final.

Argumentos de entrada:
   - t: transicion a settear.
   - final: nuevoestado final a settear.
Retorno:
   - transicion: la transicion actualizada.
**/
transicion* t_set_efin( transicion *t, nuevoestado *final );

/**
print_transicion

Funcion auxiliar que imprime una transicion.

Argumentos de entrada:
   - t: transicion que debe ser impresa.
Retorno:
   - Ninguno.
**/
void print_transicion( transicion *t );


/*******************************************************************************
** 2. FUNCIONES NUEVOESTADO.
*******************************************************************************/

/**
t_ini

Esta función se encarga de inicializar y reservar memoria para una transicion.

Argumentos de entrada:
  - Ninguno.
Retorno:
  - transicion: la transición inicializada.
  - NULL en caso de error.
**/
nuevoestado *ne_ini( int tipo );

/**
ne_free

Libera la memoria utilizada por un nuevoestado.

Argumentos de entrada:
  - ne: el nuevoestado a liberar.
Retorno:
  - Ninguno.
**/
void ne_free( nuevoestado *ne );

/**
ne_getNombre

Retorna el atributo nombre del nuevoestado. Ojo, retorna una copia de la cadena.

Argumentos de entrada:
   - ne: nuevoestado del que queremos obtener el simbolo.
Retorno:
   - char*: el nombre.
   - NULL, en caso de error.
**/
char *ne_getNombre( const nuevoestado *ne );

/**
ne_getEstados

Retorna el array de estados del que está compuesto el nuevoestado. Cada uno de
los elementos del array se trata de una cadena de caracteres del tipo qx.
Ojo, retorna una copia.

Argumentos de entrada:
   - t: transicion de la que queremos obtener el simbolo.
Retorno:
   - char*: el simbolo.
   - NULL, en caso de error.
**/
char **ne_getEstados( const nuevoestado *ne );

/**
ne_getTipo

Retorna el atributo tipo, que representa el tipo de estado del que se trata.

Argumentos de entrada:
   - ne: nuevoestado del que se quiere obtener el tipo.
Retorno:
   - int: entero que representa un tipo (Veanse las macros definidas).
**/
int ne_getTipo( const nuevoestado *ne );

/**
ne_getNestados

Retorna el atributo nestados, que representa la cantidad de estados de la que se
compone un elemento del tipo nuevoestado.

Argumentos de entrada:
   - ne: nuevoestado del que se quiere obtener el número de estados.
Retorno:
   - int: entero que representa el número de estados.
**/
int ne_getNestados( const nuevoestado *ne );

/**
ne_setNombre

"Setter" del atributo nombre de la estructura nuevoestado.

Argumentos de entrada:
   - ne: nuevoestado al que se le quiere establecer un nombre.
   - nombre: nombre que se le quiere poner al nuevoestado.
Retorno:
   - nuevoestado: nuevoestado modificado.
   - NULL, en caso de error.
**/
nuevoestado *ne_setNombre( nuevoestado *ne, char *nombre );

/**
ne_anadirEstado

Método que se encarga de añadir un estado "antiguo" a uno del tipo nuevoestado.
Aunque no se controla, la nomenclatura a seguir debe ser qx, donde x es un
entero.

Argumentos de entrada:
   - ne: nuevoestado al que se le quiere añadir un estado.
   - estado: string que contiene el nombre del estado que debe ser añadido al
   nuevoestado.
Retorno:
   - nuevoestado: nuevoestado modificado.
   - NULL, en caso de error.
**/
nuevoestado *ne_anadirEstado( nuevoestado *ne, char *estado );

/**
ne_setTipo

"Setter" del atributo tipo de la estructura nuevoestado.

Argumentos de entrada:
   - ne: nuevoestado al que se le quiere establecer un tipo.
   - tipo: entero que representa un tipo de nuevoestado.
Retorno:
   - nuevoestado: nuevoestado modificado.
   - NULL, en caso de error.
**/
nuevoestado *ne_setTipo( nuevoestado *ne, int tipo );

/**
ne_procesaNombre

Rutina que concatena todos los nombres de los estados de los que se compone un
nuevoestado con orden ascendente, de forma que varios estados dan siempre lugar
al mismo nombre.
Es decir, por ejemplo: si un nuevoestado está compuesto por q1 y q2, su nombre
será q1q2; y este se tratará del mismo nuevoestado que esté compuesto por q2 y
q1.

Argumentos de entrada:
   - ne: nuevoestado del que se quiere procesar el nombre.
Retorno:
   - char*: nombre procesado.
   - NULL, en caso de error.
**/
char *ne_procesaNombre( nuevoestado *ne );

/**
ne_cmp

Compara dos elementos del tipo nuevoestado. Serán iguales si y solo si sus
nombres son iguales. Ojo, no hablamos del argumento nombre, que puede ser NULL,
si no del nombre resultante al concatenar los estados de los cuales está
compuesto.

Argumentos de entrada:
   - ne1: nuevoestado a comparar.
   - ne2: nuevoestado a comparar.
Retorno:
   - 0, si son iguales.
   - 1, si son distintos.
   - (-1), en caso de error.
**/
int ne_cmp( nuevoestado *ne1, nuevoestado *ne2 );

/**
copy_nuevoestado

Funcion auxiliar que realiza una copia del nuevoestado n2 en el nuevoestado n1.

Argumentos de entrada:
   - n1: nuevoestado sobre el que se copia.
   - n2: nuevoestado de el cual se realiza la copia.
Retorno:
   - Ninguno.
**/
void copy_nuevoestado( nuevoestado *n1, nuevoestado *n2 );

/**
print_nuevoestado

Funcion auxiliar que imprime una estructura del tipo nuevoestado.

Argumentos de entrada:
   - ne: nuevoestado que debe ser impreso.
Retorno:
   - Ninguno.
**/
void print_nuevoestado( nuevoestado *ne );

/**
NombreEstado

Dado un número x, esta función devuelve la cadena "qx". Sirve para trabajar
auxiliarmente con nuestra estructura.

Argumentos de entrada:
   - numero: el número del estado.
Retorno:
   - char*: cadena de la forma "qx".
**/
char *NombreEstado( int numero );

/**
NumeroEstado

Dada la cadena "qx", esta función devuelve el entero x. Sirve para trabajar
auxiliarmente con nuestra estructura.

Argumentos de entrada:
   - char*: cadena de la forma "qx".
Retorno:
   - numero: el número del estado.
**/
int NumeroEstado( char *nombre );


/*******************************************************************************
** FUNCIONES AUTOMATA INTERMEDIO.
*******************************************************************************/

/**
auti_ini

Esta función se encarga de inicializar y reservar memoria para un auti.

Argumentos de entrada:
  - Ninguno.
Retorno:
  - auti: el auti inicializado.
  - NULL en caso de error.
**/
auti *auti_ini();

/**
auti_anadirEstado

Método que se encarga de añadir un nuevoestado al autómata intermedio.
Incrementa la variable nestados (número de estados) en uno.

Argumentos de entrada:
   - aut: auti al que se le quiere añadir un estado.
   - ne: nuevoestado a añadir.
**/
int auti_anadirEstado( auti *aut, nuevoestado *ne );

/**
auti_iniAlfabeto

Reserva memoria para cada uno de los elementos del alfabeto. Recordemos que el
array es un array de arrays.

Argumentos de entrada:
   - a: auti que se trata.
   - número de elementos que posee el alfabeto que es reconocido por el automata.
Retorno:
   - Ninguno.
**/
void auti_iniAlfabeto( auti *a ,int nalfabeto );

/**
auti_anadirTransicion

Método que se encarga de añadir una transicion al autómata intermedio.
Incrementa la variable ntransiciones (número de transiciones) en una unidad.

Argumentos de entrada:
   - aut: auti al que se le quiere añadir una transicion.
   - t: transicion a añadir.
Retorno:
   - Ninguno.
**/
void auti_anadirTransicion( auti *aut, transicion *t );

/**
auti_anadirSimbolo

Método que se encarga de añadir un simbolo al alfabeto del autómata intermedio.
Incrementa la variable nsimbolos (número de simbolos) en una unidad.

Argumentos de entrada:
   - aut: auti al que se le quiere añadir un simbolo.
   - simbolo: simbolo a añadir.
Retorno:
   - Ninguno.
**/
void auti_anadirSimbolo( auti *aut, char *simbolo );

/**
auti_getEstados

Función que nos devuelve el array de estructuras nuevoestado del que está
compuesto un auti.

Argumentos de entrada:
   - aut: auti del que se quiere obtener los nuevoestado.
Retorno:
   - nuevoestado**: lista de nuevoestado.
   - NULL, en caso de error.
**/
nuevoestado **auti_getEstados( const auti *aut );

/**
auti_getTransiciones

Función que nos devuelve el array de estructuras transicion del que está
compuesto un auti.

Argumentos de entrada:
   - aut: auti del que se quiere obtener las transiciones.
Retorno:
   - transicion**: lista de transicion.
   - NULL, en caso de error.
**/
transicion **auti_getTransiciones( const auti *aut );

/**
auti_getSimbolos

Método que devuelve una copia del alfabeto del automata intermedio.

Argumentos de entrada:
   - aut: automata intermedio del que se quiere obtener el alfabeto.
Retorno:
   - char**: array de arrays (alfabeto).
   - NULL, en caso de error.
**/
char **auti_getSimbolos( const auti *aut );

/**
auti_getNestados

Retorna el atributo nestados, que representa la cantidad de nuevos estados de la
que se compone un elemento del tipo auti.

Argumentos de entrada:
   - aut: auti del que se quiere obtener el número de estados.
Retorno:
   - int: entero que representa el número de estados.
**/
int auti_getNestados( auti *aut );

/**
auti_getNtransiciones

Retorna el atributo ntransiciones, que representa la cantidad de transiciones
de las que se compone un auti.

Argumentos de entrada:
   - aut: auti del que se quiere obtener el número de transiciones.
Retorno:
   - int: entero que representa el número de transiones del auti.
**/
int auti_getNtransiciones( auti *aut );

/**
auti_getNsimbolos

Retorna el atributo nsimbolos, que representa la cantidad de simbolos que
contiene el alfabeto del auti.

Argumentos de entrada:
   - aut: auti del que se quiere obtener el número de simbolos.
Retorno:
   - int: entero que representa el número de simbolos.
**/
int auti_getNsimbolos( auti *aut );

/**
auti_free

Función que libera la memoria utlizada por un autómata intermedio.

Argumentos de entrada:
  - aut: autómata a liberar.
Retorno:
  - Ninguno.
**/
void auti_free( auti *aut );

/**
print_auti

Funcion auxiliar que imprime una estructura del tipo auti.

Argumentos de entrada:
   - a: auti que debe ser impreso.
Retorno:
   - Ninguno.
**/
void print_auti( auti *a );
