#include "minimiza.h"

char ***estados;
int estadoinicial;
int nestados;
int nalfabeto;
int *estadosfinales;
auti *autointer;


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

/*Función que lee de fichero y lo almacena en las variables globales*/
void leerFichero(FILE* file){
  AFND *aut = NULL;
    int ntrepite, ne, flagfinal, flaginicial, flagtransitar;
    int ntransactuales = 0, ntransproximas = 0, ntransaux = 0;
    /*Contadores*/
    int i, m, j, k, z, n, p, h, l;
    int *estadosactuales, *estadosactualeslambda;
    char simbolo[2];
    char c;
    char *nombre;
    char **mem;
    char nestadoschar[5];
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
    c = getc( file );
    i = 0;
    while(c!=' '){
      nestadoschar[i] = c;
      c = getc( file );
      printf("%c, %d\n", nestadoschar[i],i);
      i++;
    }

    nestadoschar[i]='\0';

    nestados = atoi(nestadoschar);

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


int** verdistinguibles(){
	typedef struct _listal listal;
	typedef struct _listal{
		int pares[50][2];
		int n;
	};

	/*Hemos de crear ahora la matriz de indistinguibles, y también en la porque
	almacenaremos la lista de recursividad en cada caso.*/
	listal **listarecursiva;
	int **distinguibles;
	int i,j,z,p,n,h;
	char** alfb;
	int transprimero, transsegundo;
	int **transpareslista;
	int ntranspareslista;
	int num;
	int flagmarcados;
	/*Definimos una estructura en la que almacenar, dado un par, el conjunto de pares que dependen de él recursivamente.*/


	transpareslista = calloc(50, sizeof(int*));
	for(i=0; i<50;i++){
		transpareslista[i]=calloc(2, sizeof(int));
	}

	/*Ahora reservamos memoria*/
	distinguibles = calloc(nestados, sizeof(int*));
	for(i=0; i<nestados; i++){
		distinguibles[i] = calloc(nestados, sizeof(int));
	}

	listarecursiva = calloc(nestados, sizeof(listal*));
	for(i=0; i<nestados; i++){
		listarecursiva[i] = calloc(nestados, sizeof(listal));
	}

	/*Lo primero que hemos de hacer es diferenciar los estados finales de los que no*/
	for(i=0; i<nestados; i++){
    if(estadosfinales[i]==1){

      for(j=i+1; j < nestados; j++){
        if(estadosfinales[j]!=1){
          distinguibles[j][i]=1;
          printf("%d-%d:%d\n", i, j, distinguibles[j][i]);
        }
      }
  		for(j=0; j<i; j++){
  			if(estadosfinales[j]!=1){
  				distinguibles[i][j]=1;
  				printf("%d-%d:%d\n", i, j, distinguibles[i][j]);
  			}


  		}
    }

	}
	//Obtenemos todos los símbolos del alfabeto.
	alfb = auti_getSimbolos( autointer );
	transprimero = -1;
	transsegundo = -1;
	/*Ahora lo que tenemos que hacer es para cada par, vemos si para algún a van a parar a un par marcado.*/
	for(i=0; i<nestados; i++){
		for(j=0; j<i; j++){

			/*Antes de nada comprobamos que no lo hemos marcado antes por ser final*/
			if(distinguibles[i][j]!=1){
				ntranspareslista = 0;
				flagmarcados = 0;
				/*Vemos que, para cada letra del alfabeto, si transicionan con ella:*/
				for(h=0; h<nalfabeto; h++){
          transprimero = -1;
          transsegundo = -1;
					/*Vemos si el primero estado transiciona, y a dónde:*/
					for(z=0; z<nestados; z++){
						/*Vamos probando con todos los destinos*/
						for(n=0; n<nalfabeto; n++){
							/*Vemos si transiciona a este símbolo.*/
							if(alfb[h][0]==estados[i][z][n]){
								/*Almacenamos a dónde transiciona.*/
								transprimero = z;
								break;
							}
						}
					}
					/*Vemos si el segundo estado transiciona, y a dónde:*/
					for(z=0; z<nestados; z++){
						/*Vamos probando con todos los destinos*/
						for(n=0; n<nalfabeto; n++){
							/*Vemos si transiciona a este símbolo.*/
							if(alfb[h][0]==estados[j][z][n]){
								/*Almacenamos a dónde transiciona.*/
								transsegundo = z;
								break;
							}
						}
					}
					/*Ahora, si transprimero y transsegundo son distintos de -1,
					significa que hemos logrado transicionar con este símbolo en ambos casos*/
					if(transprimero != -1 && transsegundo != -1){
						/*Entonces, hemos de comprobar si están marcados para marcar a este par. */
						if(distinguibles[transprimero][transsegundo]==1 || distinguibles[transsegundo][transprimero]==1){
							/*Lo marcamos dos veces por si acaso, ya que la memoria tiene doble. */
							distinguibles[i][j]=1;
							// distinguibles[j][i]=1;
							/*Ahora hemos de marcar todos aquellos que dependían de nosotros en nuestra lista recursiva.*/
							if(listarecursiva[i][j].n != 0){
								for(n=0; n<listarecursiva[i][j].n;n++){
									distinguibles[listarecursiva[i][j].pares[n][0]][listarecursiva[i][j].pares[n][1]] = 1;
                  // distinguibles[listarecursiva[i][j].pares[n][1]][listarecursiva[i][j].pares[n][0]] = 1;
								}
							}
							if(listarecursiva[j][i].n != 0){
								for(n=0; n<listarecursiva[j][i].n;n++){
									distinguibles[listarecursiva[j][i].pares[n][0]][listarecursiva[j][i].pares[n][1]] = 1;
									// distinguibles[listarecursiva[j][i].pares[n][1]][listarecursiva[j][i].pares[n][0]] = 1;
								}
							}
							/*Nos salimos del for, marcando antes que estamos ya marcados*/
							flagmarcados = 1;
							/*TODO: pensar si de un break salimos del for del alfabeto*/
							/*TODO: eliminar aquellos que pudiesen estar en nuestra lista.*/

						}
						else{
							/*Si no están marcados, los añadimos a la lista de futuribles.*/
							transpareslista[ntranspareslista][0]=transprimero;
							transpareslista[ntranspareslista][1]=transsegundo;
							ntranspareslista++;
						}

					}
          if(flagmarcados == 1){
            break;
          }
				}
				/*Ahora hemos de ver si hemos marcado, porque si no, para cada estado que han ido a transicionar pero no estaban marcados, los añadimos*/
				/*Es decir, lo que hacemos es coger los pares que hemos ido añadiendo, y los almacenamos en la lista de recursividad.*/
        if(flagmarcados == 0){
          for(p=0; p<ntranspareslista; p++){
            num = listarecursiva[transpareslista[ntranspareslista][0]][transpareslista[ntranspareslista][1]].n;
            listarecursiva[transpareslista[p][0]][transpareslista[p][1]].pares[num][0] = i;
            listarecursiva[transpareslista[p][0]][transpareslista[p][1]].pares[num][0] = j;
            listarecursiva[transpareslista[p][0]][transpareslista[p][1]].n ++;
          }
        }

			}
		}
	}
	return distinguibles;
}

AFND* transformafin (int **distinguibles, int*accesibles){
	AFND *aut = NULL;
	  int ntrepite, ne, flagfinal, flaginicial, flagtransitar;
	  int ntransactuales = 0, ntransproximas = 0, ntransaux = 0;
	  /*Contadores*/
	  int i, m, j, k, z, n, p, h, l;
	  int *estadosactuales;
	  char simbolo[2];
	  char c;
	  char *nombre;
	  char **mem;
	  transicion **transproximas, **transaux, **trepite, **transactuales;
	  transicion *transini, *trans;
	  nuevoestado *eaux1, *eaux2, *einicial, *evacio, *e, *aux;



	  /* Reservamos memoria para todos los punteros de transiciones y estructuras
	  que vamos a utilizar. */
	  transactuales = calloc( STDTAM, sizeof(transicion*) );
	  transproximas = calloc( STDTAM, sizeof(transicion*) );
	  transaux = calloc( STDTAM, sizeof(transicion*) );
	  trepite = calloc( STDTAM, sizeof(transicion*) );

	  for( i = 0; i < STDTAM; i++ ) transaux[i] = t_ini();
	  for( i = 0; i < STDTAM; i++ ) trepite[i] = t_ini();
	  for( i = 0; i < STDTAM; i++ ) transproximas[i] = t_ini();

	  /*Reservamos memoria para los estados actuales*/
      estadosactuales = calloc( nestados, sizeof(int) );



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
      printf( "Tratamos para accesibles e distinguibles la siguiente transición:\n" );
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
		if(accesibles[ne] == 1){
			printf("Añadimos a estados actuales después de ver accesibilidad: %d\n", ne);
			estadosactuales[ne] = 1;
		}
      }

      for( m = 0; m < 1024; m++ ) free( mem[m] );
      free( mem );
      ne_free( aux );

      /* A continuación expandimos con lambda como hacíamos en la práctica
      anterior. */


      /* Ahora hacemos un dowhile porque en la primera iteración sí son iguales.
      En el momento en el que no varíen nuestros estadosactualeslambda hemos de
      parar, ya que ya hemos realizado el cierre transitivo.*/

      for( n = 0; n < nestados; n++ ) {

          if( estadosactuales[n] == 1 ) {
            /* Para cada estado, recorremos nuestra matriz viendo si hay un
            lambda y a qué estado transita. */
            printf(" En estado actual para ver no distin %d:\n", n );

        		for( j = n; j < nestados -1; j++ ) {

        			if( distinguibles[j][n] == 0 && accesibles[j]==1) {
        			  printf( "Hemos encontrado no distin %d a partir de %d\n", j , n);
        			  estadosactuales[j] = 1;
        			}

        		}


      			for( j = 0; j < n; j++ ) {

      				if( distinguibles[n][j] == 0 && accesibles[j] == 1) {
      				  printf( "Hemos encontrado no distin %d a partir de %d\n", j,n );
      				  estadosactuales[j] = 1;
      				}

      			}

          }
      }



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
	int *accesibles;
	int i, j;
	int **distinguibles;
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

  printf("nestados: %d\n",nestados);
  printf("estado inicial: %d\n",estadoinicial);
  printf("estados finales:");
  for(i = 0; i<nestados; i++){
    printf("%d", estadosfinales[i]);
  }
  printf("\n");
  printf("estados finales: %d", estadosfinales[i]);
  fflush(stdout);
	accesibles = eliminarInaccesibles();
	for(i=0;i<nestados;i++){
	  printf("%d\n", accesibles[i]);
	}
	distinguibles = verdistinguibles();



	for(i = 0; i<nestados; i++){
		for(j=0; j<i; j++){
			printf("%d ",distinguibles[i][j]);
      fflush(stdout);
		}
		printf("\n");
	}
  fflush(stdout);
	/*Ahora, una vez que tengamos todos los distinguibles, hemos de unir aquellos que no lo son en uno solo*/
	/*Antes hemos de comprobar que son accesibles*/







	aut = transformafin( distinguibles, accesibles );
	AFNDImprime( stdout, aut );
	AFNDADot( aut );
	AFNDElimina( aut );

	return OK;
}
