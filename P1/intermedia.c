#include "intermedia.h"
#define TAM 64

struct _transicion {
    nuevoestado *eini;
    nuevoestado *efin;
    char *simbolo;
};

struct _nuevoestado {
    char *nombre; // Nombre del nuevo estado
    char **estados; // Estados del AFND original.
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

// Funcion auxiliar de ordenación.
void bubble_sort( int *lista ) {
    if( !lista ) return;
    int a, b, c;
    int j = 0;
    int n = 0;
    // Obtenemos la longitud.
    while( lista[j] != -1 ) {
        n++;
        j++;
    }
    // Ordenamos.
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

// Funciones auxiliares de tratamiento de estados.
int NumeroEstado( char *nombre ) {
  if( !nombre ) return -1;
  int numero;
  numero = atoi( (nombre + 1) );
  return numero;
}

char *NombreEstado( int numero ) {
  if( numero < 0 ) return NULL;
  char *nombre = NULL;
  nombre = malloc(16 * sizeof(char));
  char aux[16];
  nombre[0] = 'q';
  nombre[1] = '\0';
  sprintf(aux, "%d", numero);
  strcat(nombre, aux);
  return nombre;
}

// Funciones transición.
transicion* t_ini() {
    transicion *t = NULL;
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
    fprintf( stderr, "La transición a liberar es NULL\n" );
    return;
  }
  if( t_getEini(t) ) ne_free( t_getEini(t) );
  if( t_getEfin(t) ) ne_free( t_getEfin(t) );
  free( t_getSimbolo(t) );
  free( t );
  return;
}

nuevoestado* t_getEini( const transicion *t ) {
    if( !t ) return NULL;
    nuevoestado *estado = NULL;
    estado = ne_ini(10);
    if( !estado ) return NULL;
    copy_nuevoestado(estado, t->eini);
    return estado;
}

nuevoestado* t_getEfin( const transicion *t ) {
  if( !t ) return NULL;
  nuevoestado *estado = NULL;
  estado = ne_ini(10);
  if( !estado ) return NULL;
  copy_nuevoestado(estado, t->efin);
  return estado;
}

char* t_getSimbolo( const transicion *t ) {
    char *s = NULL;
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

// Funciones nuevo estado.
nuevoestado *ne_ini( int tipo ) {
    nuevoestado *ne = NULL;
    ne = malloc( sizeof(nuevoestado) );
    ne->nombre = malloc( TAM * sizeof(char) );
    ne->estados = malloc( 1024 * sizeof(char*) );
    if( !(ne->estados) ) {
        fprintf(stderr, "Error iniciando estado\n");
        return NULL;
    }
    for( int i = 0; i < 1024; i++ ) {
        ne->estados[i] = malloc( TAM * sizeof(char) );
        if( !ne->estados[i] ) return NULL;
    }
    ne->tipo = tipo;

    return ne;
}

void ne_free( nuevoestado *ne ) {
  if( !ne ) {
    fprintf( stderr, "El nuevoestado a liberar es NULL\n" );
    return;
  }
  free( ne->nombre );
  for( int i = 0; i < 1024; i++ ) free( ne->estados[i] );
  free( ne->estados );
  free( ne );
  return;
}

char* ne_getNombre( const nuevoestado *ne ) {
    if( !ne ) return NULL;
    char *nombre = NULL;
    nombre = malloc( TAM * sizeof(char) );
    if( !nombre ) return NULL;
    strcpy( nombre, ne->nombre );
    return nombre;
}

char** ne_getEstados( const nuevoestado *ne ) {
    if( !ne ) return NULL;
    char **estados = NULL;
    estados = malloc( 1024 * sizeof(char*) );
    if( !(estados) ) return NULL;
    for( int i = 0; i < 1024; i++ ) {
        estados[i] = malloc( TAM * sizeof(char) );
        if( !estados[i] ) return NULL;
    }
    for( int i = 0; i < ne->nestados; i++ ) strcpy( estados[i], ne->estados[i] );
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
    ne->estados[ ne->nestados ] = estado;
    ne->nestados++;
    return ne;
}

int ne_getNestados( const nuevoestado *ne ) {
    return ne->nestados;
}

int ne_getTipo( const nuevoestado *ne ) {
    return ne->tipo;
}

int ne_cmp( nuevoestado *ne1, nuevoestado *ne2 ) {
  if( !ne1 || !ne2 ) {
    fprintf( stderr, "Alguno de los estados a comparar es NULL\n" );
    return -1;
  }
  // Dos estados serán iguales si poseen los mismos estados internos.
  if( strcmp(ne_procesaNombre(ne1), ne_procesaNombre(ne2)) == 0 ) return 0;
  else return 1;
}

char *ne_procesaNombre( nuevoestado *ne ) {
    int *final = NULL;
    char *ids = NULL;
    char *nombre = NULL;
    char *token = NULL;
    char **estados;
    char temp[20];
    int nestados;
    int j = 0;
    int aux = 0;

    final = malloc( 64 * sizeof(int) );
    ids = malloc( 128 * sizeof(char) );
    nombre = malloc( 64 * sizeof(char) );
    if( !final || !ids ) return NULL;
    estados = ne_getEstados( ne );
    nestados = ne_getNestados( ne );

    for( int i = 0; i < nestados; i++ ) {
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

    return nombre;

}

void copy_nuevoestado( nuevoestado *n1, nuevoestado *n2 ) {
    strcpy( n1->nombre, n2->nombre );
    for( int i = 0; i < n2->nestados; i++ ) strcpy( n1->estados[i], n2->estados[i] );
    n1->nestados = ne_getNestados(n2);
    n1->tipo = ne_getTipo(n2);
    return;
}

void print_nuevoestado( nuevoestado *ne ) {
    printf( "Nombre: %s\n", ne->nombre );
    printf( "Se compone de los estados: " );
    for( int i = 0; i < ne->nestados; i++ ) printf( "%s ", ne->estados[i] );
    if( ne_getTipo(ne) == 0 ) printf( "\n del tipo INICIAL" );
    if( ne_getTipo(ne) == 1 ) printf( "\n del tipo FINAL" );
    if( ne_getTipo(ne) == 2 ) printf( "\n del tipo INICIAL_Y_FINAL" );
    if( ne_getTipo(ne) == 3 ) printf( "\n del tipo NORMAL" );
    if( ne_getTipo(ne) == 10 ) printf( "\n del tipo UNDEFINED" );
    printf( "\n" );
    return;
}

// Funciones del autómata intermedio.
auti *auti_ini() {
    auti *aut = NULL;
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
  if( !a ) {
    fprintf( stderr, "El autómata es NULL\n" );
    return;
  }
  for( int i = 0; i<nalfabeto; i++) {
    a->simbolos[i] = malloc(TAM * sizeof(char));
  }
  return;
}

int auti_anadirEstado( auti *aut, nuevoestado *ne ) {
    if( !aut || !ne ) return -2;
    for( int w = 0; w < auti_getNestados(aut); w++ ) {
      if( ne_cmp(ne, auti_getEstados(aut)[w]) == 0 ) {
        fprintf( stderr, "El estado que intenta añadir ya está en el autómata\n" );
        return -1;
      }
    }
    aut->estados[ aut->nestados ] = ne;
    aut->nestados++;
    return 0;
}

void auti_anadirTransicion( auti *aut, transicion *t ) {
    if ( !aut || !t ) return;
    aut->transiciones[ aut->ntransiciones ] = t;
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
    if( !aut ) return NULL;
    transicion **ts = NULL;
    ts = malloc( aut->ntransiciones * sizeof(transicion) );
    if( !ts ) return NULL;

    for( int i = 0; i < aut->ntransiciones; i++ ) {
        ts[i] = t_ini();
        copy_transicion( ts[i], aut->transiciones[i] );
    }
    return ts;
}

nuevoestado** auti_getEstados( const auti *aut ) {
    if( !aut ) return NULL;
    nuevoestado **nes = NULL;
    nes = malloc( aut->nestados * sizeof(nuevoestado) );
    if( !nes ) return NULL;

    for( int i = 0; i < aut->nestados; i++ ) {
        nes[i] = ne_ini( ne_getTipo(aut->estados[i]) );
        copy_nuevoestado( nes[i], aut->estados[i] );
    }
    return nes;
}

char** auti_getSimbolos( const auti *aut ) {
  char **simbolos = NULL;
  simbolos = malloc( TAM * sizeof(char*) );
  if( !simbolos ) return NULL;

  for( int i = 0; i < aut->nsimbolos; i++ ) {
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

void print_auti( auti *a ) {
    printf("Imprimiendo autómata.\nEstados...\n");
    for( int i = 0; i < a->nestados; i++) print_nuevoestado( a->estados[i] );
    printf("Transiciones...\n");
    for( int i = 0; i < a->ntransiciones; i++) print_transicion( a->transiciones[i] );
    return;
}



/*
int main(int argc, char **argv) {
    transicion *t;
    transicion *t2;
    t = t_ini();
    t2 = t_ini();
    t_set(t, "q0", "q1", "a");
    printf("TRANSICION...\n");
    print_transicion(t);
    printf("%s, %s, %s\n", t_getEini(t), t_getEfin(t), t_getSimbolo(t));
    copy_transicion(t2, t);
    print_transicion(t2);

    nuevoestado *ne;
    nuevoestado *ne2;
    ne = ne_ini(0);
    ne2 = ne_ini(1);
    ne_setNombre(ne, "q0q1q2");
    ne_anadirEstado(ne, "q0");
    ne_anadirEstado(ne, "q20");
    ne_anadirEstado(ne, "q50");
    ne_anadirEstado(ne, "q1");
    ne_anadirEstado(ne, "q1");
    ne_anadirEstado(ne, "q2");
    ne_setNombre(ne, ne_procesaNombre(ne));
    printf("\nNUEVO ESTADO...\n");
    print_nuevoestado(ne);
    printf("Nombre: %s, se compone de los estados: ", ne_getNombre(ne));
    char **estados;
    estados = ne_getEstados(ne);
    for( int i = 0; i<ne->nestados; i++ ) printf("%s ", estados[i]);
    printf("\nNúmero de estados: %d\n",  ne_getNestados(ne));
    copy_nuevoestado(ne2, ne);
    print_nuevoestado(ne2);


    printf("\nAUTOMATA INTERMEDIO...\n");

    auti *a;
    a = auti_ini();
    auti_anadirEstado(a, ne);
    auti_anadirEstado(a, ne2);
    printf("Número de estados: %d\n", a->nestados);
    auti_anadirTransicion(a, t);
    auti_anadirTransicion(a, t2);
    printf("Número de transiciones: %d\n", a->ntransiciones);
    transicion **ts;
    ts = auti_getTransiciones(a);
    nuevoestado **nes;
    nes = auti_getEstados(a);
    for( int i = 0; i < a->ntransiciones; i++ ) print_transicion(ts[i]);
    for( int i = 0; i < a->nestados; i++ ) print_nuevoestado(nes[i]);
    print_auti(a);
    return 0;
}
*/
