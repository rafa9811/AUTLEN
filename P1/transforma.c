#include "transforma.h"

// AFND *transforma_estructura( auti *a ) {
//   if( !a ) {
//     fprintf( stderr, "transforma_estructura: aut_int es NULL\n");
//     return NULL;
//   }
//   AFND *p_afnd = NULL;
//   AFND *afd = NULL;
//   transicion **transiciones = NULL;
//   nuevoestado **estados = NULL;
//   char **alfabeto;
//   int nestados = 0;
//   int nsimbolos = 0;
//   int ntransiciones = 0;
//
//   // Vemos cuantos estados, simbolos y transiciones tenemos extrayéndolos de la
//   // estructura intermedia.
//   nestados = auti_getNestados( a );
//   nsimbolos = auti_getNsimbolos( a );
//   ntransiciones = auti_getNtransiciones( a );
//
//   // Creamos el AFND.
//   p_afnd = AFNDNuevo( "af11", nestados, nsimbolos );
//   if( !p_afnd ) {
//     fprintf( stderr, "transforma_estructura: error creando AFND\n" );
//     return NULL;
//   }
//
//   // Insertamos el alfabeto letra a letra.
//   alfabeto = auti_getSimbolos( a );
//   for( int i = 0; i < nsimbolos; i++ ) {
//     AFNDInsertaSimbolo( p_afnd, alfabeto[i] );
//   }
//
//   // Insertamos estados.
//   estados = auti_getEstados( a );
//   for( int i = 0; i < nestados; i++ ) {
//     AFNDInsertaEstado( p_afnd, ne_getNombre(estados[i]), ne_getTipo(estados[i]) );
//   }
//
//   // Insertamos transiciones. Asumimos que no tendremos transiciones lambdas.
//   transiciones = auti_getTransiciones( a );
//   for( int i = 0; i < ntransiciones; i++ ) {
//     AFNDInsertaTransicion( p_afnd, t_getEini(transiciones[i]), t_getSimbolo(transiciones[i]), t_getEfin(transiciones[i]) );
//   }
//
//   return p_afnd;
// }


// AFND *AFNDTransforma( FILE *f ) {
//   if( !f ) {
//     fprintf( stderr, "AFNDTransforma: no existe fichero\n" );
//     return NULL;
//   }
//   auti *ai = NULL;
//   AFND *aut = NULL;
//   ai = auti_ini();
//   if( !ai ) {
//     fprintf(stderr, "AFNDTransforma: error creando aut. intermedio\n" );
//     return NULL;
//   }
//
//   //Aquí el codigo del algoritmo.
//
//   aut = transforma_estructura( ai );
//   return aut;
// }


void main(int argc, char**argv) {
  char ***estados;
  FILE *file;
  int nestados;
  int nestadosfinales;
  int nalfabeto;
  transicion **transactuales;
  int ntransactuales = 0;
  transicion **transproximas;
  int ntransproximas = 0;
  transicion **transproximasantiguas;
  int ntransproximasantiguas = 0;
  transicion **transaux;
  int ntransaux = 0;
  int *estadosactuales;
  int *estadosproximos;
  int *estadosactualeslambda;
  int flagtransicion = 0;
  int estadoinicial;
  int* estadosfinales;
  int ne;
  transicion *trans;
  nuevoestado *eaux1, *eaux2;
  char simbolo[2];

  transactuales = malloc( 50 * sizeof(transicion*) );

  transproximas = malloc( 50 * sizeof(transicion*) );

  transproximasantiguas = malloc( 50 * sizeof(transicion*) );

  for(int i=0; i<50; i++){
    transproximasantiguas[i]=t_ini();
  }

  transaux = malloc( 50 * sizeof(transicion*) );
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




  estadosactuales = calloc(nestados, sizeof(int));
  estadosproximos = calloc(nestados, sizeof(int));
  estadosactualeslambda = calloc(nestados, sizeof(int));

  int nestadosactuales = 0;
  int estadoactual = 0;
  //Leemos el cardinal del alfabeto
  auti *autointer;
  autointer = auti_ini();
  nalfabeto = getc(file)-'0';
  auti_iniAlfabeto(autointer, nalfabeto);
  getc(file);
  for(int i = 0; i<nalfabeto; i++){
    simbolo[0] = getc(file);
    printf("%c", simbolo[0]);
    simbolo[1] = '\0';
    auti_anadirSimbolo(autointer, simbolo);
  }
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
  //Creamos nuestra estructura intermedia.

  transicion *transini;
  transini = t_ini();
  nuevoestado *einicial;
  einicial = ne_ini(INICIAL);
  ne_anadirEstado(einicial, NombreEstado(estadoinicial));
  ne_setNombre(einicial, NombreEstado(estadoinicial));
  transini = t_set_efin(transini, einicial);
  print_nuevoestado(t_getEfin(transini));
  transactuales[ntransactuales]=transini;
  ntransactuales++;
  trans = t_ini();
  eaux1 = ne_ini(UNDEFINED);
  eaux2 = ne_ini(UNDEFINED);
  //Pasamos de las transiciones actuales a estados actuales para encontrar los lambda.
while(ntransactuales != 0){


  for(int i=0; i<ntransactuales; i++){
    ntransproximas = 0;
    printf("Tratamos para lambda la siguiente transición:\n");
    print_transicion(transactuales[i]);
    printf("\n\n\n");
    //Ponemos a cero todos los estados actuales para esta transición.
    for(int m=0; m<nestados; m++){
      estadosactuales[m]=0;
    }
    for(int n = 0; n< ne_getNestados(t_getEfin(transactuales[i])); n++){
      ne =  NumeroEstado(ne_getEstados(t_getEfin(transactuales[i]))[n]);
      printf("Añadimos a estados actuales antes de ver lambda: %d\n", ne);
      estadosactuales[ne]=1;
    }


    memcpy(estadosactualeslambda, estadosactuales, nestados * sizeof(int));

  //Ahora hacemos un dowhile porque en el primero si son iguales.
  //A partir de una transición, queremos ver a cuántas podemos llegar con lambda, por eso lo hacemos de uno en uno.
    do
    {
      memcpy(estadosactuales, estadosactualeslambda, nestados * sizeof(int));
      for(int n=0; n<nestados; n++) {
        if(estadosactuales[n]==1) {
          printf("En estado actual para ver lambdas %d:\n", n);
          for(int j=0; j<nestados; j++) {
            for(int k=0; k<nalfabeto; k++) {
              if(estados[n][j][k]=='|') {
                printf("Hemos encontrado lambda %d\n", j);
                  estadosactualeslambda[j]=1;
              }
            }
          }
        }
      }

    printf("He salido\n");
    } while(memcmp(estadosactualeslambda, estadosactuales, nestados * sizeof(int)) != 0);
    //Ahora sabemos que todos los estados actuales tienen que ir en uno solo, es decir, actualizar nuestra transición y su nombre.
    nuevoestado *e;
    e = ne_ini(UNDEFINED);
    for(int m = 0; m<nestados; m++){
      if(estadosactuales[m]==1){
        e = ne_anadirEstado(e, NombreEstado(m));
      }
    }
    ne_setNombre(e, ne_procesaNombre(e));
    //Añadimos a nuestra estructura intermedia el estado y la transicion.
    auti_anadirEstado(autointer, e);
    t_set_efin(transactuales[i],e);
    printf("Añadimos al autómata la transicion:\n");
    print_transicion(transactuales[i]);
    auti_anadirTransicion(autointer, transactuales[i]);
    //Una vez que lo hemos hecho, podemos seguir con la próxima transición.
  }

  //Ahora que ya tenemos nuestras transiciones almacenadas, tenemos que coger estas actuales que son definitivas
  //y ver hacia dónde nos llevan.
  ntransproximas = 0;
  for(int i=0; i<ntransactuales; i++){
    int ntrepite = 0;
    for(int m=0; m<nestados; m++){
      estadosactuales[m]=0;
    }
    //Añadimos los estados de los que se compone este nuevo estado para ver en la matriz
    //hacia dónde transiciona.
    for(int n = 0; n< ne_getNestados(t_getEfin(transactuales[i])); n++){
      ne =  NumeroEstado(ne_getEstados(t_getEfin(transactuales[i]))[n]);
      printf("\n\n\n");
      printf("Añadimos a estados actuales tras ver lambda: %d\n", ne);
      estadosactuales[ne]=1;
    }

    for(int j=0; j<nestados; j++){

      if(estadosactuales[j]==1){
         printf("En estado actual %d:\n", j);

         for(int k=0; k<nestados; k++){

            for(int z=0; z<nalfabeto; z++){
                fflush(stdout);
               if(estados[j][k][z]!='|' && estados[j][k][z]!='-' && estados[j][k][z]!=0 ){
                  //Nos creamos ahora una nueva transicion
                  ne_setNombre(eaux1, NombreEstado(k));
                  ne_anadirEstado(eaux1, NombreEstado(k));
                  //print_nuevoestado(eaux1);
                  //print_nuevoestado(eaux2);
                  printf("%c, %d, %d, %d\n", estados[j][k][z], j, k, z);

                  simbolo[0]=estados[j][k][z];
                  simbolo[1]='\0';
                  t_set(trans, t_getEfin(transactuales[i]), eaux1, simbolo);

                  transaux[ntransaux]=t_ini();
                  copy_transicion(transaux[ntransaux], trans);

                  print_transicion(transaux[ntransaux]);
                  ntransaux++;


                  //printf("Hemos añadido a estados proximos el %d\n", j);

               }
            }
         }
      }
   }
   transicion **trepite;
   ntrepite = 0;
   trepite = malloc(50*sizeof(transicion*));
   for(int h = 0; h<nalfabeto; h++){
     eaux2 = ne_ini(UNDEFINED);

     for(int l=0; l<ntransaux; l++){
       if(strcmp(t_getSimbolo(transaux[l]), auti_getSimbolos(autointer)[h]) == 0){
         trepite[ntrepite]=t_ini();
         copy_transicion(trepite[ntrepite], transaux[l]);

         ntrepite++;
       }
     }
     if(ntrepite!=0){

        simbolo[0] = auti_getSimbolos(autointer)[h][0];
        printf("Este es el símbolo %c, y ntrepite %d\n", simbolo[0], ntrepite);
        simbolo[1] = '\0';
        t_set(trans, t_getEini(trepite[0]), NULL, simbolo);
        for(int p = 0; p<ntrepite; p++){
          eaux2 = ne_anadirEstado(eaux2, ne_getNombre(t_getEfin(trepite[p])));
        }
        ne_setNombre(eaux2, ne_procesaNombre(eaux2));
        t_set_efin(trans, eaux2);
        print_transicion(trans);
        transproximas[ntransproximas] = t_ini();
        copy_transicion(transproximas[ntransproximas], trans);
        ntransproximas++;
        ntrepite = 0;

      }
    }
    ntransaux = 0;
  }
  printf("%d,%d JESUSUSUSUSUSUSUSUSUSUSUSUSUSUSU", ntransproximasantiguas, ntransproximas);
  fflush(stdout);


    if(memcmp(transproximasantiguas, transproximas, ntransproximasantiguas * sizeof(transicion*))==0){
      printf("Hemos terminado");
      return;
    }
  
  memcpy(transproximasantiguas, transproximas, ntransproximas * sizeof(transicion*));
  ntransproximasantiguas = ntransproximas;

  memcpy(transactuales, transproximas, ntransproximas * sizeof(transicion*));
  ntransactuales = ntransproximas;

}

  //print_auti(autointer);
  return;







}
