

 ELiminarInaccesibles(p_afnd){

   int i,j;
   int nestados;
   int *estadosfinales;
   int estadoinicial;
   /* Reservamos la memoria para la matriz. Para no duplicar el tamaño,
   reservamos de la siguiente forma*/

   int *accesibles;
   int *accesiblesnext;
   accesibles[estadoinicial] = 1;
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

            if( estados[n][j][0] != '*' ) {
             printf( "Hemos encontrado lambda %d\n", j );
             accesiblesnext[j] = 1;
            }
          }
        }
      }


     printf( "He salido\n" );
   } while( memcmp( accesiblesnext, accesibles, nestados * sizeof(int) ) != 0 );
   }











 }
