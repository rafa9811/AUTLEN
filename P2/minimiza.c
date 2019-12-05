#include "minimiza.h"

char ***estados;
int estadoinicial;
int nestados;
int nalfabeto;
int *estadosfinales;
auti *autointer;


/*Función que lee de fichero y lo almacena en las variables globales*/
void leerFichero(FILE* file){
  AFND *aut = NULL;
    int ntrepite, estadoinicial, ne, flagfinal, flaginicial, flagtransitar;
    int ntransactuales = 0, ntransproximas = 0, ntransaux = 0;
    /*Contadores*/
    int i, m, j, k, z, n, p, h, l;
    int *estadosactuales, *estadosactualeslambda;
    char simbolo[2];
    char c;
    char *nombre;
    char **mem;
    transicion **transproximas, **transaux, **trepite, **transactuales;
    transicion *transini, *trans;
    nuevoestado *eaux1, *eaux2, *einicial, *evacio, *e, *aux;


    if( !file ) {
      fprintf( stderr, "AFNDTransforma: no existe fichero\n" );
      return NULL;
    }

    /* Reservamos memoria para todos los punteros de transiciones y estructuras
    que vamos a utilizar. */
    transactuales = calloc( STDTAM, sizeof(transicion*) );
    transproximas = calloc( STDTAM, sizeof(transicion*) );
    transaux = calloc( STDTAM, sizeof(transicion*) );
    trepite = calloc( STDTAM, sizeof(transicion*) );

    for( i = 0; i < STDTAM; i++ ) transaux[i] = t_ini();
    for( i = 0; i < STDTAM; i++ ) trepite[i] = t_ini();
    for( i = 0; i < STDTAM; i++ ) transproximas[i] = t_ini();

    autointer = auti_ini();

    /*Leemos el fichero que recibimos para almacenar toda la información correspondiente*/
    nestados = getc( file ) - '0';
    /*Para avanzar el espacio*/
    getc( file );
    estadoinicial = getc( file ) - '0';
    /*Para avanzar el espacio*/
    getc( file );

    /*Reservamos memoria para los estados finales*/
    estadosfinales = calloc( nestados, sizeof(int) );
    for( i = 0; i < nestados; i++ ) estadosfinales[i] = getc( file ) - '0';
    getc( file );

    /*Reservamos memoria para los estados actuales*/
    estadosactuales = calloc( nestados, sizeof(int) );
    estadosactualeslambda = calloc( nestados, sizeof(int) );

    /*Leemos el cardinal del alfabeto*/
    nalfabeto = getc( file ) - '0';
    auti_iniAlfabeto( autointer, nalfabeto );
    getc( file );
    for( i = 0; i < nalfabeto; i++ ) {
      simbolo[0] = getc( file );
      simbolo[1] = '\0';
      auti_anadirSimbolo( autointer, simbolo );
    }
    getc( file );
    /*Reservamos memoria para la matriz de estados, inicializando a 0*/
    estados = calloc( nestados, sizeof(char**) );
    for( i = 0; i < nestados; i++ ) {
      estados[i] = calloc( nestados, sizeof(char*) );
      for( j = 0; j < nestados; j++ ) {
        estados[i][j] = calloc( nalfabeto, sizeof(char) );
      }
    }
    /*Como lo hemos hecho es leer del nalfabeto hasta que nos encontramos con un asterisco, que significa que no hay más.
    Aquí leemos la matriz de estados y transiciones.*/
    for( i = 0; i < nestados; i++ ) {
      for( j = 0; j < nestados; j++ ) {
        for( k = 0; k < nalfabeto + 1; k++ ) {
          c = getc( file );
          if( c == '*' ) break;
          estados[i][j][k] = c;
        }
      }
    }

    fclose( file );

}




int* eliminarInaccesibles(){

  int n,j;
  int *accesibles;
  int *accesiblesnext;

  if( !estados || nestados <= 0 || estadoinicial > nestados || estadoinicial < 0 ) return NULL;

  accesibles = calloc( nestados, sizeof(int) );
  if( !accesibles ) return NULL;
  accesiblesnext = calloc( nestados, sizeof(int) );
  accesibles[estadoinicial] = 1;
  if( !accesiblesnext ) return NULL;
  accesiblesnext[estadoinicial] = 1;

  /* Ahora hacemos un dowhile porque en la primera iteración sí son iguales.
  En el momento en el que no varíen nuestros estadosactualeslambda hemos de
  parar, ya que ya hemos realizado el cierre transitivo.*/
  do
  {
   memcpy( accesibles, accesiblesnext, nestados * sizeof(int) );
   for( n = 0; n < nestados; n++ ) {
     if( accesibles[n] == 1 ) {
       /* Para cada estado, recorremos nuestra matriz viendo si hay un
       lambda y a qué estado transita. */
       printf(" En estado actual para ver accesibilidad %d:\n", n );
       for( j = 0; j < nestados; j++ ) {

          if( estados[n][j][0] != '-' ) {
           printf( "Hemos encontrado accesible %d\n", j );
           accesiblesnext[j] = 1;
          }
        }
      }
    }
   printf( "He salido\n" );
  } while( memcmp( accesiblesnext, accesibles, nestados * sizeof(int) ) != 0 );

  memcpy( accesibles, accesiblesnext, nestados * sizeof(int) );
  /*En accesibles están todos los accesibles*/
  free(accesiblesnext);
  return accesibles;
}


int** distinguibles(){

}




/* Este es el main que ejecutamos. */
int main( int argc, char**argv ) {
int *accesibles;
int i;
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

leerFichero(file);
accesibles = eliminarInaccesibles();
for(i=0;i<nestados;i++){
  printf("%d\n", accesibles[i]);
}
// aut = AFNDTransforma( file );
// AFNDImprime( stdout, aut );
// AFNDADot( aut );
// AFNDElimina( aut );

return OK;
}
