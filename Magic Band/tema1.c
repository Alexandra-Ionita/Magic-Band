/* Ionita Alexandra-Mihaela 314CB */

#include "tema1.h"

/* Functia principala in care se realizeaza citirea din fisier*/

void File_Read() {
  /* Initializare banda */

  TBanda B = NULL;
  B = Make_Banda('#');

  /* Initializare coada */

  TCoada* c = NULL;
  c = InitQ();
  if (!c) {
    fprintf(stderr, "Queue allocation failed");
    exit(1);
  }

  /* Initializare stive undo si redo */

  TStiva undo = NULL;
  TStiva redo = NULL;
  undo = Make_Stiva();
  redo = Make_Stiva();

  /* Declarare variabila care retine numele comenzii */

  char *command_name = malloc(MAX * sizeof(char));
  if (!command_name) {
    fprintf(stderr, "Malloc failed");
    exit(1);
  }

  /* Declarare variabila care retine numarul comenzilor */

  char *nrCommands = malloc(MAX * sizeof(char));
  if (!nrCommands) {
    fprintf(stderr, "Malloc failed");
    exit(1);
  }

  /* Deschidere fisiere pentru input si output */

  FILE *in = fopen("tema1.in", "r");
  FILE *out = fopen("tema1.out", "w");
  if (!in || !out) {
    fprintf(stderr, "Failed to open the file");
    exit(1);
  }

  /* Citirea din fisier a numarul comenzilor */

  if (!fgets(nrCommands, MAX, in)) {
    fprintf(stderr, "Alloc failed");
    exit(1);
  }

  /* Stergerea enterului de la final, inlocuirea cu un caracter null si
  transformarea char-ului in int */

  nrCommands[strcspn(nrCommands, "\n")] = '\0';
  int nrcommands = atoi(nrCommands);

  /* Structura repetitiva care citeste din fisier, pe rand, cate o linie,
  inlocuieste caracterul enter de la sfarsit cu caracterul null. Daca gaseste
  comanda SHOW sau SHOW_CURRENT, apeleaza direct functia respectiva. Daca 
  gaseste UNDO sau REDO, care se executa direct, se apeleaza niste functii
  care au ca scop: extrag pointerul retinut în vârful stivei, adaugă 
  pointerul la pozitia curentă a degetului în vârful stivei celelalte, după
  care modificăm pozitia degetului astfel încât să indice către pointerul
  extras din stiva apelata. Daca se gaseste o comanda diferita de cele de mai
  sus si de EXECUTE, aceasta este adaugata in coada. Daca se gaseste EXECUTE,
  se apeleaza o functie care, la randul ei, apeleaza functia corespunzatoare
  comenzii executate din coada */

  for (int i = 0; i < nrcommands; i++) {
    if (!fgets(command_name, MAX, in)) {
      fprintf(stderr, "Alloc failed");
      exit(1);
    }

    command_name[strcspn(command_name, "\n")] = '\0';

    if (strcmp(command_name, "EXECUTE")) {
      if (!strcmp(command_name, "SHOW")) {
        SHOW(B, out);
      } else {
        if (!strcmp(command_name, "SHOW_CURRENT")) {
          SHOW_CURRENT(B, out);
        } else {
          if (!strcmp(command_name, "UNDO")) {
            TLista2 curent = pop(&undo);
            push(&redo, B -> deget);
            B -> deget = curent;
          } else {
            if (!strcmp(command_name, "REDO")) {
              TLista2 curent = pop(&redo);
              push(&undo, B -> deget);
              B -> deget = curent;
            } else {
              int ok = IntrQ(c, command_name);
            }
          }
        }
      }
    } else {
      Find_Function(&B, c, out, &undo, &redo);
    }
  }

  /* Eliberare memorie */

  fclose(in);
  fclose(out);

  DistrQ(&c);
  Distr_Banda(&B);
  Distr_Stiva(&undo);
  Distr_Stiva(&redo);

  free(nrCommands);
  free(command_name);
  free(B);
}

  /* Functie care se ocupa cu initializarea stivei */

TStiva Make_Stiva() {
  TStiva stiva = (TStiva)malloc(sizeof(struct celst));

  if (!stiva) {
    fprintf(stderr, "Malloc failed");
    return NULL;
  }

  stiva->info = NULL;
  stiva->urm = NULL;

  return stiva;
}

  /* Functie care se ocupa cu adaugarea unui element in stiva */

void push(TStiva *stiva, TLista2 element) {
  TLista2 copy = element;

  TStiva aux = (TStiva)malloc(sizeof(struct celst));
  if (!aux) {
    fprintf(stderr, "Malloc failed");
    return;
  }
  aux->info = copy;
  aux->urm = NULL;
  if (*stiva == NULL) {
    *stiva = aux;
  } else {
    aux->urm = (*stiva);
    *stiva = aux;
  }
}

  /* Functie care se ocupa cu extragerea unui element din stiva */

TLista2 pop(TStiva *stiva) {
  TStiva aux = NULL;
  if (*stiva == NULL) return NULL;
  TLista2 ptr = (*stiva)->info;

  aux = *stiva;
  *stiva = (*stiva)->urm;

  free(aux);
  return ptr;
}

  /* Functie de distrugere a stivei */

void Distr_Stiva(TStiva *stiva) {
  if (!(*stiva)) {
    return;
  }
  TStiva aux = NULL;
  while (*stiva) {
    aux = *stiva;
    *stiva = (*stiva)->urm;
    free(aux);
  }
  *stiva = NULL;
}

  /* Functie de initializare a cozii */

TCoada* InitQ() {
  TCoada* c = NULL;
  c = (TCoada*)malloc(sizeof(TCoada));
  if (!c) {
    fprintf(stderr, "Malloc failed");
    return NULL;
  }

  c->inc = c->sf = NULL;
  return c;
}

  /* Functie de adaugare a unui element in coada */

int IntrQ(TCoada *c, char *x) {
  TLista aux = NULL;
  aux = (TLista)malloc(sizeof(TCelula));
  if (!aux) {
    fprintf(stderr, "Malloc failed");
    return 0;
  }

  strncpy(aux -> info, x, strlen(x) + 1);
  aux->urm = NULL;

  if (c->inc != NULL) {
    c->sf->urm = aux;
  } else {
    c->inc = aux;
  }

  c->sf = aux;
  return 1;
}

  /* Functie de extragere a unui element din coada */

int ExtrQ(TCoada *c, char **command) {
  TLista p = NULL;
  p = c -> inc;

  if (c -> inc == NULL) {
    return 0;
  } else {
    strncpy(*command, p -> info, strlen(p -> info) + 1);
    c -> inc = c -> inc -> urm;
    free(p);
    return 1;
  }
}

  /* Functie de distrugere a cozii */

void DistrQ(TCoada **c) {
  TLista p = NULL, aux = NULL;
  p = (*c)->inc;

  while (p) {
    aux = p;
    p = p->urm;
    free(aux);
  }

  free(*c);
  *c = NULL;
}

/* Aloca un element de tip TCelula2 si returneaza pointerul aferent */

TLista2 AlocCelula2(char x) {
  TLista2 aux = (TLista2) malloc(sizeof(TCelula2));
  if (!aux) {
    fprintf(stderr, "Malloc failed");
    return NULL;
  }

  aux->info = x;
  aux->pre = aux->urm = NULL;

  return aux;
}

/* Creeaza santinela pentru lista folosita */

TLista2 InitLista2() {
  TLista2 aux = (TLista2) malloc(sizeof(TCelula2));
  if (!aux) {
    fprintf(stderr, "Malloc failed");
    return NULL;
  }

  aux->info = '\0';
  aux->pre = NULL;
  aux->urm = NULL;

  return aux;
}

  /* Functie de creere a benzii magice */

TBanda Make_Banda(char x) {
  TBanda B = (TBanda)malloc(sizeof(struct banda));
  if (!B) {
    fprintf(stderr, "Malloc failed");
    return NULL;
  }

  TLista2 aux = NULL, ultim = NULL;
  B->list = NULL;
  B->deget = NULL;

  TLista2 santinela = InitLista2();
  B->list = santinela;
  if (!B->list) {
    fprintf(stderr, "Malloc failed");
    return NULL;
  }

  ultim = B -> list;

  aux = AlocCelula2(x);
  if (!aux) {
    fprintf(stderr, "Malloc failed");
    return NULL;
  }

  ultim->urm = aux;
  aux->pre = ultim;
  ultim = aux;
  B -> deget = ultim;

  return B;
}

/* Afisare continut lista */

void SHOW(TBanda B, FILE *out) {
  TLista2 p = B -> list -> urm, curent = B -> deget;

  while (p != NULL) {
    if (p == curent) {
      fprintf(out, "|");
      fprintf(out, "%c", p->info);
      fprintf(out, "|");
    } else {
      fprintf(out, "%c", p->info);
    }
     p = p->urm;
  }
  fprintf(out, "\n");
}

  /* Functie de adaugare a unui element in banda magica, care schimba si
  pozitia degetului */

void AdaugareElement(TBanda *B, char x) {
  TLista2 p = NULL, aux = NULL;
  p = (*B) -> list;

  while (p -> urm) {
    p = p -> urm;
  }
  aux = AlocCelula2(x);
  if (!aux) {
    fprintf(stderr, "Malloc failed");
    exit(1);
  }
  aux -> pre = p;
  p -> urm = aux;
  (*B) -> deget = aux;
}

  /* Functie de distrugere a benzii */

void Distr_Banda(TBanda *B) {
  if ((*B) == NULL) {
    return;
  }

  TLista2 p = NULL;
  if ((*B)->list) {
    p = (*B)->list->urm;
  }

  TLista2 aux = NULL;
  while (p) {
    aux = p;
    p = p->urm;
    free(aux);
  }

  free((*B)->list);
  (*B)->list = NULL;

  (*B)->deget = NULL;

  free(*B);
  *B = NULL;
}

  /* Insereaza un element in stanga elementului pe care se afla degetul */

int INSERT_LEFT(TBanda *B, char x) {
  TLista2 p = NULL, aux = NULL;
  p = (*B) -> deget;

  if (!((*B) -> deget -> pre -> pre)) {
    return 0;
  }
  aux = AlocCelula2(x);
  if (!aux) {
    fprintf(stderr, "Malloc failed");
    exit(1);
  }

  p -> pre -> urm = aux;
  aux -> pre = p -> pre;
  p -> pre = aux;
  aux -> urm = p;
  (*B)->deget = aux;

  return 1;
}

  /* Insereaza un element in dreapta elementului pe care se afla degetul */

void INSERT_RIGHT(TBanda *B, char x) {
  TLista2 p = NULL, aux = NULL;
    p = (*B) -> deget;

  aux = AlocCelula2(x);
  if (!aux) {
    fprintf(stderr, "Malloc failed");
    exit(1);
  }

  aux -> urm = p -> urm;
  aux -> pre = p;
  p -> urm = aux;
  if (aux -> urm) {
    aux -> urm -> pre = aux;
  }
  (*B) -> deget = aux;
}

  /* Muta pozitia degetului la stanga pozitiei curente, daca degetul nu se
  afla pe prima pozitie */

void MOVE_LEFT(TBanda *B, TStiva *stack) {
  if (((*B) -> deget -> pre -> pre)) {
    push(&(*stack), (*B)->deget);
    (*B) -> deget = (*B) -> deget -> pre;
  }
}

  /* Muta pozitia degetului la dreapta pozitiei curente, daca degetul nu se
  afla pe ultima pozitie. In caz contrar, se adauga elementul # in dreapta
  si se muta pozitia degetului pe acel # */

void MOVE_RIGHT(TBanda *B, TStiva *stack) {
  push(&(*stack), (*B)->deget);
  if (!((*B) -> deget -> urm)) {
    AdaugareElement(&(*B), '#');
  } else {
    (*B) -> deget = (*B) -> deget -> urm;
  }
}

  /* Cauta caracterul x prin toate elementele aflate in stanga pozitiei
  degetului. Daca il gaseste, pozitia degetului se muta pe acea celula gasita,
  in caz contrar, nu se schimba nimic */

int MOVE_LEFT_CHAR(TBanda *B, char x) {
  TLista2 p = NULL;
  int found = 0;

  for (p = (*B) -> deget; p != NULL; p = p -> pre) {
    if (p -> info == x) {
      (*B) -> deget = p;
      return 1;
    }
  }
}

 /* Cauta caracterul x prin toate elementele aflate in dreapta pozitiei
  degetului. Daca il gaseste, pozitia degetului se muta pe acea celula gasita,
  in caz contrar, se adauga elementul # pe ultima pozitia, iar degetul se muta
  pe acel # */

void MOVE_RIGHT_CHAR(TBanda *B, char x) {
  TLista2 p = NULL;
  int found = 0;

  for (p = (*B) -> deget; p != NULL; p = p -> urm) {
    if (p -> info == x) {
      (*B) -> deget = p;
      found = 1;
      break;
    }
  }

  if (found == 0) {
    AdaugareElement(&(*B), '#');
  }
}

  /* Inlocuieste caracterul de pe pozitia degetului cu caracterul x */

void WRITE(TBanda *B, char x) {
  (*B) -> deget -> info = x;
}

  /* Afiseaza caracterul de pe pozitia curenta */

void SHOW_CURRENT(TBanda B, FILE *out) {
  fprintf(out, "%c\n", B -> deget -> info);
}

  /* Functie prin care se extrage un element din coada si se apeleaza functia
  corespunzatoare comenzii. */

void Find_Function(TBanda *B, TCoada *c, FILE *out, TStiva *undo, TStiva *redo) {
  /* Variabila care retine comanda */

  char *command = malloc(MAX * sizeof(char));
  if (!command) {
    fprintf(stderr, "Alloc failed");
    exit(1);
  }

  /* Variabila prin care se verifica daca comanda este insotita de vreun
  parametru */

  int withParam = 1;

  ExtrQ(&(*c), &command);

  if (!strcmp(command, "MOVE_LEFT")) {
    MOVE_LEFT(&(*B), &(*undo));
    withParam = 0;
  }
  if (!strcmp(command, "MOVE_RIGHT")) {
    MOVE_RIGHT(&(*B), &(*undo));
    withParam = 0;
  }

  /* Daca comanda are parametru, stivele se elibereaza. */

  if (withParam == 1) {
    Distr_Stiva(&(*undo));
    Distr_Stiva(&(*redo));

    /* Variabila ce retine parametrul */

    char param = command[strlen(command) - 1];

    /* Variabila prin care se retine comanda data */

    char *copyCommand = malloc(MAX * sizeof(char));
    if (!copyCommand) {
      fprintf(stderr, "Malloc failed");
      exit(1);
    }

    strncpy(copyCommand, command, strlen(command) - 2);
    copyCommand[strlen(command) - 2] = '\0';

    if (!strcmp(copyCommand, "MOVE_LEFT_CHAR")) {
      if (!MOVE_LEFT_CHAR(&(*B), param)) {
        fprintf(out, "ERROR\n");
      }
    }
    if (!strcmp(copyCommand, "MOVE_RIGHT_CHAR")) {
      MOVE_RIGHT_CHAR(&(*B), param);
    }
    if (!strcmp(copyCommand, "WRITE")) {
      WRITE(&(*B), param);
    }
    if (!strcmp(copyCommand, "INSERT_LEFT")) {
      if (!INSERT_LEFT(&(*B), param)) {
        fprintf(out, "ERROR\n");
      }
    }
    if (!strcmp(copyCommand, "INSERT_RIGHT")) {
      INSERT_RIGHT(&(*B), param);
    }
    free(copyCommand);
  }

  free(command);
}
