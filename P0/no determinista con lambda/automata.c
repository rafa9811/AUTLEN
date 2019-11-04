#include <stdio.h>
#include <stdlib.h>
#include <string.h>





void main(int argc, char**argv) {

  if (argc < 2) {
    fprintf(stderr, "Introduzca la palabra a reconocer\n");
    return;
  }
  char ***estados;
  FILE *file;
  char *cadena;
  int nestados;
  int nestadosfinales;
  int tamano_cadena;
  int nalfabeto;

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
  cadena = argv[1];
  tamano_cadena = strlen(cadena);
  estadosactuales[estadoinicial]=1;
  printf("Partimos de estado %d\n", estadoinicial);
  for(int i=0; i<tamano_cadena; i++){
    printf("Tratando el caracter %c:\n", cadena[i]);
    flagtransicion = 0;
//Para cada caracter, tenemos que pasar a estadosactualeslambda
//Copiamos antes de empezar todos los estados actuales a esta''lambda.
    memcpy(estadosactualeslambda, estadosactuales, nestados * sizeof(int));
//Ahora hacemos un dowhile porque en el primero si son iguales.

  do 
  {
    memcpy(estadosactuales, estadosactualeslambda, nestados * sizeof(int));
    for(int n=0; n<nestados; n++) {
      if(estadosactuales[n]==1) {
         printf("En estado actual para ver lambdas %d:\n", n);
         for(int j=0; j<nestados; j++) {
            for(int k=0; k<nalfabeto; k++) {
               if(estados[n][j][k]=='|') {
                  estadosactualeslambda[j]=1;
               }
            }
         }
      }
    }
  } while(memcmp(estadosactualeslambda, estadosactuales, nestados * sizeof(int)) != 0);

//Aquí ya pasamos a calcular los proximos.
    for(int n=0; n<nestados; n++){
      if(estadosactuales[n]==1){
         printf("En estado actual %d:\n", n);
         for(int j=0; j<nestados; j++){
            for(int k=0; k<nalfabeto; k++){
               if(estados[n][j][k]==cadena[i]){
                  estadosproximos[j]=1;
                  flagtransicion = 1;
               }
            }
         }
      }
   }

   if(flagtransicion == 0){
      printf("No hemos terminado de reconocer la palabra, y ya no tenemos transiciones\n");
      return;
   }

   memcpy(estadosactuales, estadosproximos, nestados * sizeof(int));

   for(int z=0; z<nestados; z++){
      estadosproximos[z]=0;
   }
}
for(int i=0; i<nestados;i++){
      if(estadosactuales[i]==1 && estadosfinales[i]==1){
         printf("Hemos reconocido la palabra en %d\n", i);
         return;
 
   }
}
printf("No hemos reconocido la palabra");
return;



//       for(int j=0; j<nestados; j++){
//         for(int k=0; k<nalfabeto+1;k++){
//           if(estados[estadosactuales[estadoactual]][j][k]==cadena[i]){
//             nestadosactuales++;
//             estadosactuales[nestadosactuales] = j;
//             //Indicamos que hemos encontrado transicion
//             flagtransicion = 1;
//             printf("Añadimos estado actual %d\n", j);
//             break;
//           }
//         }
//       }
//     }
//   }
//
//
//
//   //Una vez leída toda la cadena, si en uno de los estados actuales es un estado final, hemos reconocido la palabra.
//   for(int i=0; i<nestadosfinales; i++){
//     for(int j=0; j<nestadosactuales; j++){
//       if(estadosactuales[j] == estadosfinales[i]){
//         printf("Hemos reconocido la palabra\n");
//         return;
//       }
//     }
//
// }
//   printf("Hemos reconocido la palabra\n");
//   //   if(flagtransicion == 0){
//   //     printf("No reconocemos la palabra\n");
//   //     return;
//   //   }
//   // }
//   // for(int i=0; i<nestadosfinales; i++){
//   //   if(estadoactual == estadosfinales[i]){
//   //     printf("Hemos reconocido la palabra\n");
//   //     return;
//   //   }
//   // }
//   // //Si no hemos encontrado ningún estado final, nos salimos.
//   // printf("No hemos reconocido la palabra\n");




}
