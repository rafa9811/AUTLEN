/*******************************************************************************
** AUTÓMATAS Y LENGUAJES - PRÁCTICA 2
** transforma.c
**
** Fichero que contiene la implementación de las funciones de transformación:
** estructura intermedia - estructura de api.
** Contiene el algoritmo principal mediante el cual se transforma el autómata.
**
** Autores:
** - Carlos Molinero Alvarado - carlos.molineroa@estudiante.uam.es
** - Rafael Hidalgo Alejo - rafael.hidalgoa@estudiante.uam.es
**
** Última modificación: 17 de noviembre de 2019.
*******************************************************************************/

#include "transforma.h"

AFND *transforma_estructura( auti *a ) {

  AFND *p_afnd = NULL;
  transicion **transiciones = NULL;
  nuevoestado **estados = NULL;
  nuevoestado *aux1, *aux2;
  char *nombre1, *nombre2, *simbolo;
  char **alfabeto;
  int nestados = 0, nsimbolos = 0, ntransiciones = 0, i = 0;

  if( !a ) {
    fprintf( stderr, "transforma_estructura: aut_int es NULL\n");
    return NULL;
  }
  /* Vemos cuantos estados, simbolos y transiciones tenemos extrayéndolos de la
  // estructura intermedia.*/
  nestados = auti_getNestados( a);
  nsimbolos = auti_getNsimbolos( a );
  ntransiciones = auti_getNtransiciones( a );

  /* Creamos el AFND.*/
  p_afnd = AFNDNuevo( "af11", nestados, nsimbolos );
  if( !p_afnd ) {
    fprintf( stderr, "transforma_estructura: error creando AFND\n" );
    return NULL;
  }

  /*Insertamos el alfabeto letra a letra*/
  alfabeto = auti_getSimbolos( a );
  for( i = 0; i < nsimbolos; i++ ) {
    AFNDInsertaSimbolo( p_afnd, alfabeto[i] );
    free( alfabeto[i] );
  }
  /* Ya no necesitamos el alfabeto */
  free( alfabeto );

  /* Insertamos estados*/
  estados = auti_getEstados( a );
  for( i = 0; i < nestados; i++ ) {
    nombre1 = ne_getNombre( estados[i] );
    AFNDInsertaEstado( p_afnd, nombre1, ne_getTipo(estados[i]) );
    free( nombre1 );
    ne_free( estados[i] );
  }
  free( estados );

  /* Insertamos transiciones. Asumimos que no tendremos transiciones lambdas*/
  transiciones = auti_getTransiciones( a );
  for( i = 0; i < ntransiciones; i++ ) {
    aux1 = t_getEini( transiciones[i] );
    nombre1 = ne_getNombre( aux1 );
    aux2 = t_getEfin( transiciones[i] );
    nombre2 = ne_getNombre( aux2 );

    if( strcmp(nombre1, "principio") != 0 ) {
      simbolo = t_getSimbolo(transiciones[i]);
      AFNDInsertaTransicion( p_afnd, nombre1, simbolo, nombre2 );
      free( simbolo );
    }
    t_free( transiciones[i] );
    ne_free( aux1 );
    free( nombre1 );
    ne_free( aux2 );
    free( nombre2 );
  }
  free( transiciones );
  return p_afnd;
}


AFND *AFNDTransforma( FILE *file ) {

}

  /* Este es el main que ejecutamos. */
int main( int argc, char**argv ) {

  AFND *aut = NULL;
  FILE *file = NULL;
  if( argc != 2 ) {
    fprintf( stderr, "MODO DE EJECUCION: ./transforma path_automata.txt\n" );
    return ERROR;
  }

  /* Leemos nuestro autómata de fichero: */
  file = fopen( argv[1], "r" );
  if( !file ) {
    fprintf( stderr, "La ruta no existe. ERROR\n" );
    return ERROR;
  }

  aut = AFNDTransforma( file );
  AFNDImprime( stdout, aut );
  AFNDADot( aut );
  AFNDElimina( aut );

  return OK;
}
