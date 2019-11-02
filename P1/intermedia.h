#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _auti auti;
typedef struct _nuevoestado nuevoestado;
typedef struct _transicion transicion;

auti *auti_ini();
void auti_destroy(auti *);

nuevoestado *ne_ini();
void ne_destroy(nuevoestado *ne);

transicion *trans_ini();
void trans_destroy(transicion *trans);
