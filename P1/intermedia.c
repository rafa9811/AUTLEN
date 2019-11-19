/*******************************************************************************
** AUTÓMATAS Y LENGUAJES - PRÁCTICA 1
** intermdia.c
**
** Fichero que contiene la implementación de las funciones de la estructura
** intermedia.
**
** Autores:
** - Carlos Molinero Alvarado - carlos.molineroa@estudiante.uam.es
** - Rafael Hidalgo Alejo - rafael.hidalgoa@estudiante.uam.es
**
** Última modificación: 17 de noviembre de 2019.
*******************************************************************************/

#include "intermedia.h"

struct _transicion {
    nuevoestado *eini; /* Estado inicial de la transicion.*/
    nuevoestado *efin; /* Estado final de la transicion.*/
    char *simbolo; /* Simbolo mediante el cual se transita.*/
};

struct _nuevoestado {
    char *nombre; /* Nombre del nuevo estado*/
    char **estados; /* Estados del AFND original.*/
    int nestados;
    int tipo;
};

struct _auti {
    nuevoestado **estados;
    transicion **transiciones;
    char **simbolos;
    int nestados;
    int ntransiciones;
    int nsimbolos;
};

/* Funcion auxiliar de ordenación.*/
void bubble_sort( int *lista ) {
    int a = 0, b = 0, c = 0;
    int j = 0;
    int n = 0;
    if( !lista ) return;

    /* Obtenemos la longitud.*/
    while( lista[j] != -1 ) {
        n++;
        j++;
    }
    /* Ordenamos.*/
    for( a = 0; a < n; a++ ) {
        for( b = 0; b < (n - a - 1); b++) {
            if( lista[b] > lista[b + 1] ) {
                c = lista[b];
                lista[b] = lista[b + 1];
                lista[b + 1] = c;
            }
        }
    }
}

/* Funciones auxiliares de tratamiento de estados.*/
int NumeroEstado( char *nombre ) {
  int numero;
  if( !nombre ) return -1;
  numero = atoi( (nombre + 1) );
  return numero;
}

char *NombreEstado( int numero ) {
  char *nombre;
  char aux[16];
  if( numero < 0 ) return NULL;
  nombre = malloc(16 * sizeof(char));

  nombre[0] = 'q';
  nombre[1] = '\0';
  sprintf(aux, "%d", numero);
  strcat(nombre, aux);
  return nombre;
}

/*Funciones transición.*/
transicion* t_ini() {
    transicion *t;
    t = malloc( sizeof(transicion) );
    t->eini = ne_ini(10);
    t->efin = ne_ini(10);
    if( !t || !t->eini || !t->efin ) {
        fprintf( stderr, "Error iniciando transicion" );
        return NULL;
    }
    t->simbolo = malloc( 16 * sizeof(char) );
    if( !t->simbolo ){
      fprintf( stderr, "Error iniciando simbolo en transicion\n" );
    }
    return t;
}

void t_free( transicion *t ) {
  if( !t ) {
    return;
  }
  if( t->eini ) ne_free( t->eini );
  if( t->efin ) ne_free( t->efin );
  free( t->simbolo );
  free( t );
  return;
}

nuevoestado* t_getEini( const transicion *t ) {
    nuevoestado *estado;
    if( !t ) return NULL;
    estado = ne_ini(10);
    if( !estado ) return NULL;
    copy_nuevoestado(estado, t->eini);
    return estado;
}

nuevoestado* t_getEfin( const transicion *t ) {
  nuevoestado *estado;
  if( !t ) return NULL;
  estado = ne_ini(10);
  if( !estado ) return NULL;
  copy_nuevoestado(estado, t->efin);
  return estado;
}

char* t_getSimbolo( const transicion *t ) {
    char *s;
    s = (char*) malloc( 16 * sizeof(char) );
    if( !s ) return NULL;
    strcpy( s, t->simbolo );
    return s;
}

void copy_transicion( transicion *t1, transicion *t2 ) {
    copy_nuevoestado( t1->eini, t2->eini );
    copy_nuevoestado( t1->efin, t2->efin );
    strcpy( t1->simbolo, t2->simbolo );
    return;
}

transicion* t_set( transicion *t, nuevoestado *inicial, nuevoestado *final, char *simbolo ) {
    if( !t ) return NULL;
    if( inicial ) copy_nuevoestado( t->eini, inicial );
    if( final ) copy_nuevoestado( t->efin, final );
    if( simbolo ) strcpy( t->simbolo, simbolo );
    return t;
}

transicion* t_set_efin( transicion *t, nuevoestado *final ) {
    if( !t || !final ) return NULL;
    copy_nuevoestado( t->efin, final );
    return t;
}

void print_transicion( transicion *t ) {
    printf("Estado inicial: %s, estado final: %s, con simbolo %s\n", ne_getNombre(t->eini), ne_getNombre(t->efin), t->simbolo);
    return;
}

/* Funciones nuevo estado.*/
nuevoestado *ne_ini( int tipo ) {
    nuevoestado *ne;
    int i;
    ne = calloc( 1, sizeof(nuevoestado) );
    ne->nombre = calloc( TAM, sizeof(char) );
    ne->estados = calloc( 1024,  sizeof(char*) );
    if( !(ne->estados) ) {
        fprintf(stderr, "Error iniciando estado\n");
        return NULL;
    }
    for( i = 0; i < 1024; i++ ) {
        ne->estados[i] = calloc( TAM , sizeof(char) );
        if( !ne->estados[i] ) return NULL;
    }
    ne->tipo = tipo;
    ne->nestados = 0;

    return ne;
}

void ne_free( nuevoestado *ne ) {
  int i;
  if( !ne ) {
    return;
  }
  free( ne->nombre );
  for( i = 0; i < 1024; i++ ) free( ne->estados[i] );
  free( ne->estados );
  free( ne );
  return;
}

void ne_arr_free( nuevoestado **ne, int len ){
  for (int i = 0; i < len; i++) {
    ne_free(ne[i]);
  }
  free(ne);
}

char* ne_getNombre( const nuevoestado *ne ) {
    char *nombre;
    if( !ne ) return NULL;
    nombre = malloc( TAM * sizeof(char) );
    if( !nombre ) return NULL;
    strcpy( nombre, ne->nombre );
    return nombre;
}

char** ne_getEstados( const nuevoestado *ne ) {
    char **estados;
    int i;
    if( !ne ) return NULL;
    estados = malloc( 1024 * sizeof(char*) );
    if( !(estados) ) return NULL;
    for( i = 0; i < 1024; i++ ) {
        estados[i] = malloc( TAM * sizeof(char) );
        if( !estados[i] ) return NULL;
    }
    for( i = 0; i < ne->nestados; i++ ) strcpy( estados[i], ne->estados[i] );
    return estados;
}

nuevoestado* ne_setNombre( nuevoestado *ne, char *nombre ) {
    if( !ne || !nombre ) return NULL;
    strcpy( ne->nombre, nombre );
    return ne;
}

nuevoestado *ne_setTipo( nuevoestado *ne, int tipo ) {
    if( !ne ) {
      fprintf( stderr, "El estado a modificar es NULL\n" );
      return NULL;
    }
    if( tipo != INICIAL && tipo != FINAL && tipo != INICIAL_Y_FINAL && tipo != NORMAL && tipo != UNDEFINED ) {
      fprintf( stderr, "Tipo de estado no identificado\n" );
      return NULL;
    }
    ne->tipo = tipo;
    return ne;
}

nuevoestado* ne_anadirEstado( nuevoestado *ne, char *estado ) {
    if( !estado || !ne ) return NULL;
    strcpy( ne->estados[ ne->nestados ], estado );
    ne->nestados++;
    strcpy( ne->estados[ ne->nestados ], "" );
    return ne;
}

int ne_getNestados( const nuevoestado *ne ) {
    return ne->nestados;
}

int ne_getTipo( const nuevoestado *ne ) {
    return ne->tipo;
}

int ne_cmp( nuevoestado *ne1, nuevoestado *ne2 ) {
  char* nombre1, *nombre2;
  if( !ne1 || !ne2 ) {
    fprintf( stderr, "Alguno de los estados a comparar es NULL\n" );
    return -1;
  }
  /*Dos estados serán iguales si poseen los mismos estados internos.*/
  nombre1 = ne_procesaNombre(ne1);
  nombre2 = ne_procesaNombre(ne2);
  if( strcmp(nombre1, nombre2) == 0 ){
    if(nombre1) free(nombre1);
    if(nombre2) free(nombre2);
    return 0;
  }
  else{
    if(nombre1) free(nombre1);
    if(nombre2) free(nombre2);
    return 1;
  }
}

char *ne_procesaNombre( nuevoestado *ne ) {
    int *final;
    char *ids;
    char *nombre;
    char *token;
    char **estados;
    char temp[20];
    int nestados;
    int j = 0;
    int i;
    int aux = 0;

    final = calloc( 64, sizeof(int) );
    ids = calloc( 128, sizeof(char) );
    nombre = calloc( 64, sizeof(char) );
    if( !final || !ids ) return NULL;
    estados = ne_getEstados( ne );
    nestados = ne_getNestados( ne );

    for( i = 0; i < nestados; i++ ) {
        j = 0;
        if( estados[i][j] == 'q' ) j++;
        while( estados[i][j] != 'q' && estados[i][j] != '\0' ) {
            ids[aux] = estados[i][j];
            aux++;
            j++;
        }
        if( estados[i][j] == '\0' ) {
            ids[aux] = '#';
            aux++;
        }
    }
    ids[aux] = '\0';
    aux = 0;

    token = strtok(ids, "#");

    while( token != NULL ) {
        final[aux] = atoi(token);
        token = strtok(NULL, "#");
        aux++;
    }
    aux++;
    final[aux] = -1;

    bubble_sort( final );
    aux = 1;

    while( final[aux] != -1 ) {
        strcat( nombre, "q" );
        sprintf( temp, "%d", final[aux] );
        strcat( nombre, temp );
        aux++;
    }
    free( ids );
    free( final );
    for(i = 0; i < 1024; i++) free( estados[i] );
    free( estados );
    return nombre;

}

void copy_nuevoestado( nuevoestado *n1, nuevoestado *n2 ) {
  int i;
    strcpy( n1->nombre, n2->nombre );
    for( i = 0; i < n2->nestados; i++ ) strcpy( n1->estados[i], n2->estados[i] );
    n1->nestados = ne_getNestados(n2);
    n1->tipo = ne_getTipo(n2);
    return;
}

void print_nuevoestado( nuevoestado *ne ) {
  int i;
    printf( "Nombre: %s\n", ne->nombre );
    printf( "Se compone de los estados: " );
    for( i = 0; i < ne->nestados; i++ ) printf( "%s ", ne->estados[i] );
    if( ne_getTipo(ne) == 0 ) printf( "\n del tipo INICIAL" );
    if( ne_getTipo(ne) == 1 ) printf( "\n del tipo FINAL" );
    if( ne_getTipo(ne) == 2 ) printf( "\n del tipo INICIAL_Y_FINAL" );
    if( ne_getTipo(ne) == 3 ) printf( "\n del tipo NORMAL" );
    if( ne_getTipo(ne) == 10 ) printf( "\n del tipo UNDEFINED" );
    printf( "\n" );
    return;
}

/* Funciones del autómata intermedio.*/
auti *auti_ini() {
    auti *aut;
    aut = malloc( sizeof(auti) );
    if( !aut ) {
        fprintf( stderr, "Error creando automata intermedio 1\n" );
        return NULL;
    }

    aut->nestados = 0;
    aut->ntransiciones = 0;
    aut->nsimbolos = 0;

    aut->estados = malloc( 1024 * sizeof(nuevoestado*) );
    if( !aut->estados ) {
        fprintf( stderr, "Error creando automata intermedio 2\n" );
        free(aut);
        return NULL;
    }

    aut->transiciones = malloc( 1024 * sizeof(transicion*) );
    if( !aut->transiciones ) {
        fprintf( stderr, "Error creando automata intermedio 3\n" );
        free( aut->estados );
        free( aut );
        return NULL;
    }

    aut->simbolos = malloc( TAM * sizeof(char*) );
    if( !aut->simbolos ) {
      fprintf( stderr, "Error creando automata intermedio 4\n" );
      free( aut-> estados );
      free( aut-> transiciones );
      free( aut );
      return NULL;
    }

    return aut;
}

void auti_iniAlfabeto(auti *a ,int nalfabeto) {
  int i;
  if( !a ) {
    fprintf( stderr, "El autómata es NULL\n" );
    return;
  }
  for( i = 0; i<nalfabeto; i++) {
    a->simbolos[i] = malloc(TAM * sizeof(char));
  }
  return;
}

int auti_anadirEstado( auti *aut, nuevoestado *ne ) {
  int w;
    if( !aut || !ne ) return -2;
    /* Debemos comprobar que el estado a añadir no esté en el auti */
    nuevoestado **aux = auti_getEstados(aut);
    for( w = 0; w < auti_getNestados(aut); w++ ) {

      if( ne_cmp(ne, aux[w]) == 0 ) {
        ne_arr_free(aux, auti_getNestados(aut));
        fprintf( stderr, "El estado que intenta añadir ya está en el autómata\n" );
        return -1;
      }

    }
    ne_arr_free(aux, auti_getNestados(aut));
    aut->estados[ aut->nestados ] = ne_ini( UNDEFINED );
    copy_nuevoestado( aut->estados[ aut->nestados ], ne );
    aut->nestados++;
    return 0;
}

void auti_anadirTransicion( auti *aut, transicion *t ) {
    if ( !aut || !t ) return;
    aut->transiciones[ aut->ntransiciones ] = t_ini();
    copy_transicion( aut->transiciones[ aut->ntransiciones ], t );
    aut->ntransiciones++;
    return;
}

void auti_anadirSimbolo( auti *aut, char* simbolo ) {
    if ( !aut ) return;
    strcpy( aut->simbolos[ aut->nsimbolos ], simbolo );
    aut->nsimbolos++;
    return;
}

transicion** auti_getTransiciones( const auti *aut ) {
    int i;
    transicion **ts;
    if( !aut ) return NULL;

    ts = malloc( aut->ntransiciones * sizeof(transicion) );
    if( !ts ) return NULL;

    for( i = 0; i < aut->ntransiciones; i++ ) {
        ts[i] = t_ini();
        copy_transicion( ts[i], aut->transiciones[i] );
    }
    return ts;
}

nuevoestado** auti_getEstados( const auti *aut ) {
    int i;
    nuevoestado **nes;
    if( !aut ) return NULL;

    nes = malloc( aut->nestados * sizeof(nuevoestado) );
    if( !nes ) return NULL;

    for( i = 0; i < aut->nestados; i++ ) {
        nes[i] = ne_ini( ne_getTipo(aut->estados[i]) );
        copy_nuevoestado( nes[i], aut->estados[i] );
    }
    return nes;
}

char** auti_getSimbolos( const auti *aut ) {
  int i;
  char **simbolos;
  simbolos = malloc( TAM * sizeof(char*) );
  if( !simbolos ) return NULL;

  for( i = 0; i < aut->nsimbolos; i++ ) {
      simbolos[i] = malloc( TAM * sizeof(char) );
      if( !simbolos[i] ) {
        free(simbolos);
        return NULL;
      }
      strcpy( simbolos[i], aut->simbolos[i] );
  }
  return simbolos;
}

int auti_getNestados( auti *aut ) {
  if( !aut ) return -1;
  return aut->nestados;
}

int auti_getNtransiciones( auti *aut ) {
  if( !aut ) return -1;
  return aut->ntransiciones;
}

int auti_getNsimbolos( auti *aut ) {
  if( !aut ) return -1;
  return aut->nsimbolos;
}

void auti_free( auti *aut ) {
  int i;
  if( !aut ) {
    fprintf( stderr, "El automata intermedio a liberar es NULL\n" );
    return;
  }
  for( i = 0; i < auti_getNsimbolos(aut); i++ ) {
    free( aut->simbolos[i] );
  }
  free( aut->simbolos );

  for( i = 0; i < auti_getNestados(aut); i++ ) {
    ne_free( aut->estados[i] );
  }
  free( aut->estados );

  for( i = 0; i < auti_getNtransiciones(aut); i++ ) {
    t_free( aut->transiciones[i] );
  }
  free( aut->transiciones );

  free(aut);
  return;
}

void print_auti( auti *a ) {
    int i;
    printf("Imprimiendo autómata.\nEstados...\n");
    for( i = 0; i < a->nestados; i++) print_nuevoestado( a->estados[i] );
    printf("Transiciones...\n");
    for( i = 0; i < a->ntransiciones; i++) print_transicion( a->transiciones[i] );
    return;
}
