#include "transforma.h"

AFND *transforma_estructura( auti *a ) {
  if( !a ) {
    fprintf( stderr, "transforma_estructura: aut_int es NULL\n");
    return NULL;
  }
  AFND *p_afnd = NULL;
  AFND *afd = NULL;
  transicion **transiciones = NULL;
  nuevoestado **estados = NULL;
  char **alfabeto;
  int nestados = 0;
  int nsimbolos = 0;
  int ntransiciones = 0;

  // Vemos cuantos estados, simbolos y transiciones tenemos extrayéndolos de la
  // estructura intermedia.
  nestados = auti_getNestados( a );
  nsimbolos = auti_getNsimbolos( a );
  ntransiciones = auti_getNtransiciones( a );

  // Creamos el AFND.
  p_afnd = AFNDNuevo( "af11", nestados, nsimbolos );
  if( !p_afnd ) {
    fprintf( stderr, "transforma_estructura: error creando AFND\n" );
    return NULL;
  }

  // Insertamos el alfabeto letra a letra.
  alfabeto = auti_getSimbolos( a );
  for( int i = 0; i < nsimbolos; i++ ) {
    AFNDInsertaSimbolo( p_afnd, alfabeto[i] );
  }

  // Insertamos estados.
  estados = auti_getEstados( a );
  for( int i = 0; i < nestados; i++ ) {
    AFNDInsertaEstado( p_afnd, ne_getNombre(estados[i]), ne_getTipo(estados[i]) );
  }

  // Insertamos transiciones. Asumimos que no tendremos transiciones lambdas.
  transiciones = auti_getTransiciones( a );
  for( int i = 0; i < ntransiciones; i++ ) {
    AFNDInsertaTransicion( p_afnd, t_getEini(transiciones[i]), t_getSimbolo(transiciones[i]), t_getEfin(transiciones[i]) );
  }

  return p_afnd;
}


AFND *AFNDTransforma( FILE *f ) {
  if( !f ) {
    fprintf( stderr, "AFNDTransforma: no existe fichero\n" );
    return NULL;
  }
  auti *ai = NULL;
  AFND *aut = NULL;
  ai = auti_ini();
  if( !ai ) {
    fprintf(stderr, "AFNDTransforma: error creando aut. intermedio\n" );
    return NULL;
  }

  //Aquí el codigo del algoritmo.

  aut = transforma_estructura( ai );
  return aut;
}

void main(int argc, char**argv) {
  char ***estados;
  FILE *file;
  int nestados;
  int nestadosfinales;
  int nalfabeto;
  int *transactuales;
  int ntransactuales;
  nuevoestado * efinales;
  transicion * tfinales;
  int ntfinales;
  int nefinales;
  int *estadosactuales;
  int *estadosproximos;
  int *estadosactualeslambda;
  int flagtransicion = 0;
  int estadoinicial;
  int* estadosfinales;

  //Leemos nuestro autómata de fichero:
  file = fopen("automata.txt", "r");
  nestados = getc(file)-'0';
  //Para avanzar el espacio
  getc(file);
  estadoinicial = getc(file)-'0';
  //Para avanzar el espacio
  getc(file);

  //Reservamos memoria para los estados finales;
  estadosfinales = calloc(nestados, sizeof(int));

  for(int i = 0; i<nestados; i++){
    estadosfinales[i] = getc(file)-'0';
  }
  getc(file);
  //Reservamos memoria para los estados actuales;
  transicion transactuales[50];
  ntransactuales = 0;
  estadosactuales = calloc(nestados, sizeof(int));
  estadosproximos = calloc(nestados, sizeof(int));
  estadosactualeslambda = calloc(nestados, sizeof(int));

  int nestadosactuales = 0;
  int estadoactual = 0;
  //Leemos el cardinal del alfabeto
  nalfabeto = getc(file)-'0';
  printf("%d\n", nalfabeto);
  getc(file);
  //Reservamos memoria para la matriz de estados, inicializando a 0;
  estados = calloc(nestados, sizeof(char**));
  for(int i = 0; i<nestados; i++){
    estados[i] = calloc(nestados, sizeof(char*));
    for(int j = 0; j<nestados; j++){
      estados[i][j] = calloc(nalfabeto, sizeof(char));
    }
  }
  //Como lo hemos hecho es leer del nalfabeto hasta que nos encontramos con un asterisco, que significa que no hay más.
  //Aquí leemos la matriz de estados y transiciones.
  for(int i = 0; i<nestados; i++){
    for(int j = 0; j<nestados; j++){
      for(int k = 0; k<nalfabeto+1; k++){
        char c = getc(file);
        if(c=='*'){
          break;
        }
        estados[i][j][k] = c;
        printf("%d, %d,%d, %c\n", i , j, k, estados[i][j][k]);
      }
    }
  }

  fclose(file);
  transicion *transini;
  transini = t_ini();
  transini = t_set(transini, NULL, estadoinicial, NULL);
  transactuales[ntransactuales]=transini;
  ntransactuales++;
  //Pasamos de las transiciones actuales a estados actuales para encontrar los lambda.
  for(int i=0; i<ntransactuales; i++){
    ne = NumeroEstado(t_getEfin(transactuales[n]));
    estadosactuales[ne]=1;
    memcpy(estadosactualeslambda, estadosactuales, nestados * sizeof(int));

  //Ahora hacemos un dowhile porque en el primero si son iguales.
  //A partir de una transición, queremos ver a cuántas podemos llegar con lambda, por eso lo hacemos de uno en uno.
    do
    {
      memcpy(estadosactuales, estadosactualeslambda, nestados * sizeof(int));
      printf("En estado actual para ver lambdas %d:\ne", n);
      for(int j=0; j<nestados; j++) {
        for(int k=0; k<nalfabeto; k++) {
          if(estados[ne][j][k]=='|') {
            printf("hemos encontrado lambda\n");
              estadosactualeslambda[j]=1;
          }
        }
      }

      printf("he salido \n");
    } while(memcmp(estadosactualeslambda, estadosactuales, nestados * sizeof(int)) != 0);
    //Ahora sabemos que todos los estados actuales tienen que ir en uno solo, es decir, actualizar nuestra transición y su nombre.
    nuevoestado *e;
    e = ne_ini(10);
    for(int i = 0; i<nestados; i++){
      if(estadosactuales[i]==1){
        e = ne_anadirEstado(e, NombreEstado(i));
      }
    }
    ne_setNombre(ne_procesaNombre(e));
    efinales[nefinales]=e;
    nefinales++;
    t_set_efin(transactuales[ntransactuales],e);
    tfinales[ntfinales] = transactuales[ntransactuales];
    ntfinales++;
  }
    //Una vez que lo hemos hecho, podemos seguir con la próxima transición.








}
