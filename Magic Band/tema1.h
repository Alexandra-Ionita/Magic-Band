/* Ionita Alexandra-Mihaela 314CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50

  /* Structura listei dublu inlantuite */

typedef struct celula2 {
  char info;
  struct celula2 *pre, *urm;
} TCelula2, *TLista2;

  /* Structura banda magica */

typedef struct banda {
  TLista2 list;
  TLista2 deget;
} *TBanda;

  /* Structura lista simplu inlantuita */

typedef struct celula {
  char info[MAX];
  struct celula* urm;
} TCelula, *TLista;

  /* Structura coada */

typedef struct coada {
  TLista inc, sf;
} TCoada;

  /* Structura stiva */

typedef struct celst {
  TLista2 info;
  struct celst *urm;
} *TStiva;

TStiva Make_Stiva();
void push(TStiva *stiva, TLista2 celula);
TLista2 pop(TStiva *stiva);
void Distr_Stiva(TStiva *stiva);
void File_Read();
TLista2 AlocCelula2(char x);
TLista2 InitLista2();
TBanda Make_Banda(char x);
void SHOW(TBanda B, FILE *out);
int INSERT_LEFT(TBanda *B, char x);
void MOVE_LEFT(TBanda *B, TStiva *stack);
void MOVE_RIGHT(TBanda *B, TStiva *stack);
void AdaugareElement(TBanda *B, char x);
int MOVE_LEFT_CHAR(TBanda *B, char x);
void MOVE_RIGHT_CHAR(TBanda *B, char x);
void WRITE(TBanda *B, char x);
void INSERT_RIGHT(TBanda *B, char x);
void SHOW_CURRENT(TBanda B, FILE *out);
void Find_Function(TBanda *B, TCoada *c, FILE *out, TStiva *stack, TStiva *redo);
void Distr_Banda(TBanda *B);
TCoada* InitQ();
int ExtrQ(TCoada *c, char **command);
int IntrQ(TCoada *c, char *x);
void DistrQ(TCoada **c);
