#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INICIAL	0
#define FINAL	1
#define INICIAL_Y_FINAL	2
#define NORMAL 3


typedef struct _auti auti;
typedef struct _nuevoestado nuevoestado;
typedef struct _transicion transicion;

// FUNCIONES TRANSICIÓN
transicion* t_ini();
char* t_getEini( const transicion *t );
char* t_getEfin( const transicion *t );
char* t_getSimbolo( const transicion *t );
void copy_transicion( transicion *t1, transicion *t2 );
transicion* t_set( transicion *t, char *inicial, char *final, char *simbolo );
void print_transicion( transicion *t );

// FUNCIONES NUEVOESTADO
nuevoestado *ne_ini( int tipo );
char* ne_getNombre( const nuevoestado *ne );
char** ne_getEstados( const nuevoestado *ne );
int ne_getTipo( const nuevoestado *ne );
int ne_getNestados( const nuevoestado *ne );
nuevoestado* ne_setNombre( nuevoestado *ne, char *nombre );
void ne_anadirEstado( nuevoestado *ne, char *estado );
char *ne_procesaNombre( nuevoestado *ne );
void copy_nuevoestado( nuevoestado *n1, nuevoestado *n2 );
void print_nuevoestado( nuevoestado *ne );

// FUNCIONES AUTOMATA INTERMEDIO
auti *auti_ini();
void auti_anadirEstado( auti *aut, nuevoestado *ne );
void auti_anadirTransicion( auti *aut, transicion *t );
void auti_anadirSimbolo( auti *aut, char simbolo );
nuevoestado** auti_getEstados( const auti *aut );
transicion** auti_getTransiciones( const auti *aut );
char **auti_getSimbolos( const auti *aut );
int auti_getNestados( auti *aut );
int auti_getNtransiciones( auti *aut );
int auti_getNsimbolos( auti *aut );
void print_auti( auti *a );
