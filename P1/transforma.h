/*******************************************************************************
** AUTÓMATAS Y LENGUAJES - PRÁCTICA 1
** transforma.h
**
** Fichero que contiene la declaración de las funciones de transformación:
estructura intermedia - estructura de api.
**
** Autores:
** - Carlos Molinero Alvarado - carlos.molineroa@estudiante.uam.es
** - Rafael Hidalgo Alejo - rafael.hidalgoa@estudiante.uam.es
**
** Última modificación: 17 de noviembre de 2019.
*******************************************************************************/

/*Librerías*/
#include <stdio.h>
#include <stdlib.h>
#include "afnd.h"
#include "intermedia.h"

/**
transforma_estructura

Esta función se encarga de la transformación de un AFND definido
mediante nuestra estructura intermedia en uno equivalente para
su interpretación con la API proporcionada.

Argumentos de entrada:
  - a: autómata con estructura intermedia.
Retorno:
  - AFND: autómata con estructura final.
  - NULL, en caso de error.
**/
AFND *transforma_estructura( auti *a );

/**
AFNDTransforma

Método que, partiendo de un AFND, lo transforma en un AFD equivalente.
El nuevo autómata se guardará en una estructura de tipo AFND, aunque en este
caso será determinista.

Argumentos de entrada:
  - f: fichero donde se encuentra definido el AFND.
Retorno:
  - AFND: AFD con estructura AFND.
  - NULL, en caso de error.
**/
AFND *AFNDTransforma( FILE *f );
