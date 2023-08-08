#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define EMPTY -1
#define MAXA 512

typedef struct nodo{
    int val;
    struct nodo *prev;
    struct nodo *next;
}lista_t;

typedef lista_t *ptr_nodo;

typedef struct node{
    int id;
    struct node *left;
    struct node *right;
    struct node *father;
    struct node *root;
    int maxAut;
    int autonomie[];
}stazione_t;

stazione_t *NIL;
stazione_t *root = NULL;

/*funzioni bst
 *
 *
 *
 * */

stazione_t *createTree();
stazione_t *treeMinimum(stazione_t *x);
stazione_t *treeSuccessor(stazione_t *x);
stazione_t *treeSearch(stazione_t *n, int distanza);

/*funzioni della specifica
 *
 *
 *
 * */

void aggiungiStazione(stazione_t *T, stazione_t *stazione);
void demolisciStazione(stazione_t *T, int idStazione);
void aggiungiAuto(int idStazione, int autonomia);
void rottamaAuto(int idStazione, int autonomia);
void pianificaPercorso(int idPartenza, int idArrivo);

/*funzioni custom
 *
 *
 *
 * */

int ricercaBinaria(int *arr, int dim, int num);
stazione_t *createStation(int distanza, int numAuto, int parcoAuto[]);
void pianificaDiretto(int idPartenza, int idArrivo);
void pianificaInverso(int idPartenza, int idArrivo);

/*codice
 *
 *
 *
 * */

int ricercaBinaria(int *arr, int dim, int num){
    int min, max, mid;

    min = 0;
    max = dim - 1;
    while(min<=max){
        mid = min + (max - min)/2;
        if(arr[mid] == num) return mid;
        else if (arr[mid] < num) min = mid + 1;
        else max = mid -1;
    }
    return EMPTY;
}

stazione_t *createStation(int distanza, int numAuto, int parcoAuto[]) {

    stazione_t *stazione = (stazione_t *)malloc(sizeof(stazione_t));
    stazione->id = distanza;
    stazione->maxAut = 0;
    for(int i = 0; i<numAuto; i++){
        stazione->autonomie[i] = parcoAuto[i];
        if(parcoAuto[i] > stazione->maxAut){
            stazione->maxAut = parcoAuto[i]; //per salvare l'auto con autonomia max
        }
    }
    return stazione;
}

stazione_t *createTree(){
    stazione_t *new;

    new = (stazione_t*) malloc(sizeof(stazione_t));
    new->father = NULL;
    new->right = NULL;
    new->left = NULL;

    return new;
}

stazione_t *treeMinimum(stazione_t *x){
    while(x->left != NIL){
        x = x->left;
    }
    return x;
}

stazione_t *treeSuccessor(stazione_t *x){
    if(x->right != NIL){
        return treeMinimum(x->right);
    }
    stazione_t *y = x->father;
    while(y != NIL && x == y->right){
        x = y;
        y = y->father;
    }
    return y;
}

stazione_t *treeSearch(stazione_t *n, int distanza){

    if(n == NIL || distanza == n->id) return n;
    if(n->id < distanza){
        return treeSearch(n->left, distanza);
    } else {
        return treeSearch(n->right, distanza);
    }
}

void aggiungiStazione(stazione_t *T, stazione_t *z){
    stazione_t *x = root;
    stazione_t *y = NIL;

    if(treeSearch(T, z->id) == NIL){
        z = createStation();
        while(x!=NIL){
            y = x;
            if(z->id < x->id){
                x = x->left;
            } else {
                x = x->right;
            }
            z->father = y;
            if(y == NIL){
                T->root = z;
            } else if(z->id < y->id){
                y->left = z;
            } else {
                y->right = z;
            }
        }
        printf("aggiunta");
    } else {
        printf("non aggiunta");
    }
}

void demolisciStazione(stazione_t *T, int idStazione){
    stazione_t *x, *y, *z;

    z = treeSearch(T, idStazione);
    if(z){
        if(z->left == NIL || z->right == NIL){
            y = z;
        } else y = treeSuccessor(z);
        if(y->left != NIL){
            x = y->left;
        } else x = y->right;
        if(x != NIL){
            x->father = y->father;
        }
        if(y->father == NIL){
            T->root = x;
        } else if(y == y->father->left){
            y->father->left = x;
        } else {
            y->father->right = x;
        }
        if(y != z){
            z->id = y->id;
        }
        free(y);
        printf("demolita");
    } else {
        printf("non demolita");
    }
}

void aggiungiAuto(int idStazione, int autonomia){
    int i = -1;
    stazione_t *T, *stazione;

    stazione = treeSearch(T, idStazione);
    i = ricercaBinaria(stazione->autonomie, MAXA, EMPTY);
    if(i >= 0 || i < MAXA){
        stazione->autonomie[i] = autonomia;
        printf("aggiunta\n");

    } else {
        printf("non aggiunta\n");
    }
}

void rottamaAuto(int idStazione, int autonomia){
    int j = -1;
    stazione_t *T, *stazione;

    stazione = treeSearch(T, idStazione);
    j = ricercaBinaria(stazione->autonomie, MAXA, autonomia);
    if(j >= 0 && j < MAXA){
        stazione->autonomie[j] = EMPTY;
        printf("rottamata\n");
    } else {
        printf("non rottamata\n");
    }
}

void pianificaDiretto(int idPartenza, int idArrivo){
    int distanzaCurr;
    stazione_t *stazione;
    ptr_nodo list, head;

    list = NULL;
    head = list;
    head->val = idPartenza;
    head = head->next;
    distanzaCurr = 0;

    do{ //stampa le stazioni attraversate a partire dalla fine
        printf("%d", head->val);
        head = head->prev;
    }while(head != NULL);
}

void pianificaInverso(int idPartenza, int idArrivo){

}

void pianificaPercorso(int idPartenza, int idArrivo){
    if(idPartenza < idArrivo){
        pianificaDiretto(idPartenza, idArrivo);
    } else {
        pianificaInverso(idArrivo, idPartenza);
    }
}

/* main
 *
 *
 * 
 * */

int main(){
    int autonomia, distanza;
    int numAuto[MAXA];
    stazione_t *new;
    stazione_t *stazione;

    new = createTree();

    printf("inserisci la distanza della stazione da aggiungere: ");
    scanf("%d", &distanza);
    stazione = createStation(distanza, 512, numAuto);
    aggiungiStazione(new, stazione);
    printf("inserisci autonomia della macchina da aggiungere: ");
    scanf("%d", &autonomia);
    aggiungiAuto(stazione->id, autonomia);
    demolisciStazione(new, stazione->id);

    return 0;
}