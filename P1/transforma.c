/*******************************************************************************
** AUTÓMATAS Y LENGUAJES - PRÁCTICA 1
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

  AFND *aut = NULL;
  int nestados, nalfabeto, ntrepite, estadoinicial, ne, flagfinal, flaginicial, flagtransitar;
  int ntransactuales = 0, ntransproximas = 0, ntransaux = 0;
  /*Contadores*/
  int i, m, j, k, z, n, p, h, l;
  int *estadosactuales, *estadosactualeslambda, *estadosfinales;
  char ***estados;
  char simbolo[2];
  char c;
  char *nombre;
  char **mem;
  transicion **transproximas, **transaux, **trepite, **transactuales;
  transicion *transini, *trans;
  nuevoestado *eaux1, *eaux2, *einicial, *evacio, *e, *aux;
  auti *autointer;

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

  /*Creamos nuestra estructura intermedia.*/
  transini = t_ini();
  einicial = ne_ini( INICIAL );
  evacio = ne_ini( UNDEFINED );
  ne_setNombre( evacio, "principio" );
  nombre = NombreEstado( estadoinicial );
  ne_anadirEstado( einicial, nombre );
  ne_setNombre( einicial, nombre );
  free( nombre );
  transini = t_set( transini, evacio, einicial, "-" );
  aux =  t_getEfin( transini );
  print_nuevoestado( aux );
  ne_free( aux );
  transactuales[ntransactuales] = transini;
  ntransactuales++;
  trans = t_ini();
  eaux1 = ne_ini( UNDEFINED );

  /*Pasamos de las transiciones actuales a estados actuales para encontrar los
  lambda. Inicialmente tenemos solo la transición inicial, y luego en los
  siguientes bucles iremos entrando con nuevas transiciones.*/
  while( ntransactuales != 0 ) {
    /*Flag con la que controlamos que hemos descubierto un estado nuevo.*/
    flagtransitar = 0;
    /*Recorremos cada transición actual cogiendo su estado destino y viendo si
    de este se puede transitar con lambda y si es así, creamos un nuevo estado.*/
    for( i = 0; i < ntransactuales; i++ ) {
      ntransproximas = 0;
      printf( "Tratamos para lambda la siguiente transición:\n" );
      print_transicion( transactuales[i] );
      printf( "\n" );
      /* Ponemos a cero todos los estados actuales para esta transición, ya que
      en estados actuales tendremos todos los estados destino a los que se puede
      transitar con lambda y que por tanto en nuestro AFD deben estar en un
      mismo estado. */
      for( m = 0; m < nestados; m++ ) estadosactuales[m] = 0;
      /* Añadimos los estados que ya forman parte de nuestro estado final de la
      transición. */
      aux = t_getEfin( transactuales[i] );
      mem = ne_getEstados( aux );
      for( m = 0; m < ne_getNestados(aux); m++ ) {
        ne =  NumeroEstado( mem[m] );
        printf("Añadimos a estados actuales antes de ver lambda: %d\n", ne);
        estadosactuales[ne] = 1;
      }

      for( m = 0; m < 1024; m++ ) free( mem[m] );
      free( mem );
      ne_free( aux );

      /* A continuación expandimos con lambda como hacíamos en la práctica
      anterior. */
      memcpy( estadosactualeslambda, estadosactuales, nestados * sizeof(int) );

      /* Ahora hacemos un dowhile porque en la primera iteración sí son iguales.
      En el momento en el que no varíen nuestros estadosactualeslambda hemos de
      parar, ya que ya hemos realizado el cierre transitivo.*/
      do
      {
        memcpy( estadosactuales, estadosactualeslambda, nestados * sizeof(int) );
        for( n = 0; n < nestados; n++ ) {
          if( estadosactuales[n] == 1 ) {
            /* Para cada estado, recorremos nuestra matriz viendo si hay un
            lambda y a qué estado transita. */
            printf(" En estado actual para ver lambdas %d:\n", n );
            for( j = 0; j < nestados; j++ ) {
              for(k = 0; k < nalfabeto; k++ ) {
                if( estados[n][j][k] == '|' ) {
                  printf( "Hemos encontrado lambda %d\n", j );
                  estadosactualeslambda[j] = 1;
                }
              }
            }
          }
        }

        printf( "He salido\n" );
      } while( memcmp( estadosactualeslambda, estadosactuales, nestados * sizeof(int) ) != 0 );
      /* Ahora sabemos que todos los estados actuales tienen que ir en uno solo,
      es decir, actualizar nuestra transición añadiendo un nuevo estado destino
      con la unión de todos. */

      /* Inicializamos nuestro estado. */
      e = ne_ini( NORMAL );
      /* Añadimos cada uno de ellos y su nombre, también añadiendo su tipo
      correspondiente. */
      flaginicial = 0;
      flagfinal = 0;
      for( m = 0; m < nestados; m++ ) {
        if( estadosactuales[m] == 1 ) {
          nombre = NombreEstado( m );
          ne_anadirEstado( e, nombre );
          free( nombre );
          if( estadosfinales[m] == 1 ) flagfinal = 1;
          if( estadoinicial == m ) flaginicial = 1;
        }
      }
      if( flaginicial == 1 && flagfinal ==1 ) ne_setTipo( e, INICIAL_Y_FINAL );
      else if( flagfinal == 1 ) ne_setTipo( e, FINAL );
      else if( flaginicial == 1 ) ne_setTipo( e,INICIAL );
      /* Finalmente creamos el nombre con la unión de todos. */
      nombre = ne_procesaNombre( e );
      ne_setNombre( e, nombre );
      free( nombre );

      /* Una vez transitado con lambda, es el momento cuando podemos asegurar que
      tanto los estados que tenemos como nuestras transciones actuales no van
      a ser ya modificadas, y por tanto podemos almacenarlas en nuestro
      autómata que es nuestra estructura intermedia.*/

      /* Comprobamos que el estado que hemos querido añadir no estaba ya en
      nuestra lista, es decir, que es nuevo, y que por tanto aún no hemos
      acabado el algoritmo. */
      if( auti_anadirEstado(autointer, e) == 0 ) flagtransitar = 1;

      /* Modificamos nuestra transición actual añadiendo el nuevo estado destino,
      y ya la añadimos al autómata. */
      t_set_efin( transactuales[i], e );

      /* Liberamos el estado, ya que en la siguiente iteración lo necesitamos
      vacío. */
      ne_free( e );

      fprintf( stdout, "Añadimos al autómata la transicion:\n" );
      print_transicion( transactuales[i] );
      auti_anadirTransicion( autointer, transactuales[i] );
      /* Una vez que lo hemos hecho, podemos seguir con la próxima transición
      viendo si transita con lambda y la almacenamos..*/
    }

    /* Miramos si realmente hemos añadido estados: */
    if( flagtransitar != 1 ) {
      /* Si ya hemos terminado, creamos a partir de nuestra estructura intermedia
        el automata determinista con las funciones de la api. */
      printf( "\nLa conversión ha terminado. Imprimiendo...\n\n" );
      print_auti( autointer );
      aut = transforma_estructura( autointer );
      /*Antes de salirnos, liberamos la memoria utilizada:*/
      for( i = 0; i < STDTAM; i++ ) {
        if( transactuales[i] ) t_free( transactuales[i] );
      }
      free( transactuales );

      for( i = 0; i < STDTAM; i++ ) {
        t_free( transproximas[i] );
      }
      free( transproximas );

      for( i = 0; i < STDTAM; i++ ) {
        t_free( trepite[i] );
      }
      free( trepite );

      for( i = 0; i < STDTAM; i++ ) {
        t_free( transaux[i] );
      }
      free( transaux );

      ne_free( eaux1 );
      ne_free( einicial );
      ne_free( evacio );

      t_free( trans );
      auti_free( autointer );

      free( estadosfinales );
      free( estadosactuales );
      free( estadosactualeslambda );

      /* Eliminamos la tabla. */
      for( i = 0; i < nestados; i++ ) {
        for( j = 0; j < nestados; j++ ) free( estados[i][j] );
        free( estados[i] );
      }
      free( estados );

      return aut;
    }

    /* Si no hemos terminado, hemos de ver qué transiciones nuevas tenemos. Por
    tanto, cogemos los nuevos estados que acabamos de almacenar (que son los
    efin de las transiciones que hemos almacenado) y,en cada uno, para cada
    estado que lo compone, vemos hacia dónde pueden transitar y con qué
    símbolos. */

    /* Ponemos a cero las transiciones próximas que vamos a generar. */
    ntransproximas = 0;
    for( i = 0; i < ntransactuales; i++ ) {

      ntrepite = 0;
      for( m = 0; m < nestados; m++ ) estadosactuales[m] = 0;

      /* Añadimos los estados de los que se compone este nuevo estado para ver
      en la matriz hacia dónde transiciona cada uno de ellos. .*/
      aux = t_getEfin( transactuales[i] );
      mem = ne_getEstados( aux );
      for( m = 0; m < ne_getNestados( aux ); m++ ) {
        ne =  NumeroEstado( mem[m] );
        printf( "\n\n\n" );
        printf( "Añadimos a estados actuales tras ver lambda: %d\n", ne );
        estadosactuales[ne] = 1;
      }
      for( m = 0; m < 1024; m++ ) free( mem[m] );
      free( mem );
      ne_free( aux );

      /* Ahora miramos en nuestra matriz hacia dónde se transita, y vemos creando
      transiciones temporales para cada una que vamos encontrando, añadiendo
      símbolo y estado final. */
      for( j = 0; j < nestados; j++ ) {

        if( estadosactuales[j] == 1 ) {
          printf("En estado actual %d:\n", j);

          for( k = 0; k < nestados; k++ ) {

            for( z = 0; z < nalfabeto; z++ ) {

              if( estados[j][k][z] != '|' && estados[j][k][z] != '-' && estados[j][k][z] != 0 ) {
                nombre = NombreEstado( k );
                /*Nos creamos ahora una nueva transicion auxiliar.*/
                ne_setNombre( eaux1, nombre );
                ne_anadirEstado( eaux1, nombre );
                free( nombre );
                printf( "%c, %d, %d, %d\n", estados[j][k][z], j, k, z );

                simbolo[0] = estados[j][k][z];
                simbolo[1] = '\0';
                aux = t_getEfin( transactuales[i] );
                t_set( trans, aux, eaux1, simbolo );
                ne_free( aux );

                copy_transicion( transaux[ntransaux], trans );

                print_transicion( transaux[ntransaux] );
                ntransaux++;
              }
            }
          }
        }
      }

      ntrepite = 0;

      /* Ahora hemos de ver si,a partir de un mismo estado origen(eini), hemos
      generado dos transiciones auxiliares con el mismo símbolo, que en este
      caso hemos de unirlas yendo a un mismo estado destino. */
      mem = auti_getSimbolos( autointer );
      for( h = 0; h < nalfabeto; h++ ) {
        eaux2 = ne_ini( UNDEFINED );
        /* Para cada símbolo del alfabeto recorremos todas las transauxiliares
        que hemos generado comprobando si tienen ese símbolo y en ese caso la
        añadimos a una lista auxiliar de transiciones que se repiten. */
        for( l = 0; l < ntransaux; l++ ) {
          nombre = t_getSimbolo( transaux[l] );
          if( strcmp( nombre, mem[h] ) == 0 ) {
            copy_transicion( trepite[ntrepite], transaux[l] );
            ntrepite++;
          }
          free( nombre );
        }

        /* Si ntrepite es distinto de cero, es decir, tenemos transiciones con
        ese símbolo, creamos un nuevo estado y ya una transición próxima
        definitiva. */
        if( ntrepite != 0 ) {
          simbolo[0] = mem[h][0];
          printf( "Este es el símbolo %c, y ntrepite %d\n", simbolo[0], ntrepite );
          simbolo[1] = '\0';

          aux = t_getEini( trepite[0] );
          t_set( trans, aux, NULL, simbolo );
          ne_free( aux );

          for( p = 0; p < ntrepite; p++ ) {
            aux = t_getEfin( trepite[p] );
            nombre = ne_getNombre( aux );
            eaux2 = ne_anadirEstado( eaux2,  nombre );
            ne_free( aux );
            free( nombre );
          }

          nombre = ne_procesaNombre( eaux2 );
          ne_setNombre( eaux2, nombre );
          free( nombre );
          t_set_efin( trans, eaux2 );

          print_transicion( trans );
          /*Aquí tendremos todas nuestras transiciones próximas definitivas con los símbolos unidos*/
          copy_transicion( transproximas[ntransproximas], trans );
          ntransproximas++;
          ntrepite = 0;
        }
        ne_free( eaux2 );
      }
      for( l = 0; l < auti_getNsimbolos(autointer); l++ ) free( mem[l] );
      free( mem );
      ntransaux = 0;
    }

    /* Ahora, una vez que ya hemos transitado con los símbolos, hemos de seguir
    con la siguiente iteración del bucle inicial. Por tanto, copiamos las
    próximas a las actuales, y ya volvemos a ver si se puede en cada una de
    ellas transitar con lambdas y las almacenamos en nuestra estructura
    intermedia. */

    for( p = 0; p < ntransproximas; p++ ) {
      if( transactuales[p] ) t_free( transactuales[p] );
      transactuales[p] = t_ini();
      copy_transicion( transactuales[p], transproximas[p] );
    }
    ntransactuales = ntransproximas;
  }
  return NULL;
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
