#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define EMPTY (-1)
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

/*funzioni bst
 *
 *
 *
 * */

stazione_t *createTree();
stazione_t *treeMinimum(stazione_t *x);
stazione_t *treeMaximum(stazione_t *x);
stazione_t *treePredecessor(stazione_t *x);
stazione_t *treeSuccessor(stazione_t *x);
stazione_t *treeSearch(stazione_t *n, int distanza);

/*funzioni quicksort
 *
 *
 *
 * */

void swap(int *x, int *y);
int partition(int *arr, int min, int max);
void quickSort(int *arr, int min, int max);

/*funzioni della specifica
 *
 *
 *
 * */

void aggiungiStazione(stazione_t *T, int distanza, int numAuto, const int *parcoAuto);
void demolisciStazione(stazione_t *T, int idStazione);
void aggiungiAuto(stazione_t *T, int idStazione, int autonomia);
void rottamaAuto(stazione_t *T, int idStazione, int autonomia);
void pianificaPercorso(stazione_t *stazione, int idPartenza, int idArrivo);

/*funzioni custom
 *
 *
 *
 * */

int ricercaBinaria(const int *arr, int dim, int num);
void pianificaDiretto(stazione_t *stazione, int idPartenza, int idArrivo);
/*void pianificaInverso(stazione_t *stazione, int idPartenza, int idArrivo);*/

/*codice
 *
 *
 *
 * */

void swap(int *x, int *y){
    int tmp;

    tmp = *x;
    *x = *y;
    *y = tmp;
}

int partition(int *arr, int min, int max){
    int x = arr[min];
    int i = max - 1;

    for(int j=min; j<max; j++){
        if(arr[j]<=x){
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i+1], &arr[max]);
    return i+1;
}

void quickSort(int *arr, int min, int max){
    if(min < max){
        int pivot = partition(arr, min, max);
        quickSort(arr, min, pivot-1);
        quickSort(arr, pivot+1, max);
    }
}

int ricercaBinaria(const int *arr, int dim, int num){
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

stazione_t *createTree(){
    stazione_t *tree;

    tree = (stazione_t*) malloc(sizeof(stazione_t));
    tree->father = NULL;
    tree->right = NULL;
    tree->left = NULL;

    return tree;
}

stazione_t *treeMinimum(stazione_t *x){
    while(x->left != NULL){
        x = x->left;
    }
    return x;
}

stazione_t *treeMaximum(stazione_t *x){
    while(x->right != NULL){
        x = x->right;
    }
    return x;
}

stazione_t *treePredecessor(stazione_t *x){
    if(x->left != NULL){
        return treeMaximum(x->left);
    }
    stazione_t *y = x->father;
    while(y != NULL && x == y->left){
        x = y;
        y = y->father;
    }
    return y;
}

stazione_t *treeSuccessor(stazione_t *x){
    if(x->right != NULL){
        return treeMinimum(x->right);
    }
    stazione_t *y = x->father;
    while(y != NULL && x == y->right){
        x = y;
        y = y->father;
    }
    return y;
}

stazione_t *treeSearch(stazione_t *n, int distanza){

    if(n == NULL || distanza == n->id) return n;
    if(n->id < distanza) {
        return treeSearch(n->left, distanza);
    }
    return treeSearch(n->right, distanza);
}

void aggiungiStazione(stazione_t *T, int distanza, int numAuto, const int *parcoAuto){
    stazione_t *prev = NULL;
    stazione_t *curr = T->root;

    if(treeSearch(T, distanza) == NULL){ //aggiunge se non lo trova
        stazione_t *stazione = (stazione_t*)malloc(sizeof(stazione_t));
        stazione->id = distanza;
        stazione->maxAut = 0;
        for(int i = 0; i<numAuto; i++){
            stazione->autonomie[i] = parcoAuto[i];
            if(parcoAuto[i] > stazione->maxAut){
                stazione->maxAut = parcoAuto[i]; //per salvare l'auto con autonomia max
            }
        }

        while(curr != NULL){
            prev = curr;
            if(stazione->id < curr->id){
                curr = curr->left;
            } else {
                curr = curr->right;
            }
            stazione->father = prev;
            if(prev == NULL){
                T->root = stazione;
            } else if(stazione->id < prev->id){
                prev->left = stazione;
            } else {
                prev->right = stazione;
            }
        }
        printf("aggiunta");
    } else {
        printf("non aggiunta");
    }
}

void demolisciStazione(stazione_t *T, int idStazione){
    stazione_t *figli, *del, *z;

    z = treeSearch(T, idStazione);
    if(z){
        if(z->left == NULL || z->right == NULL){
            del = z;
        } else del = treeSuccessor(z);
        if(del->left != NULL){
            figli = del->left;
        } else figli = del->right;
        if(figli != NULL){
            figli->father = del->father;
        }
        if(del->father == NULL){
            T->root = figli;
        } else if(del == del->father->left){
            del->father->left = figli;
        } else {
            del->father->right = figli;
        }
        if(del != z){
            z->id = del->id;
        }
        free(del);
        printf("demolita");
    } else {
        printf("non demolita");
    }
}

void aggiungiAuto(stazione_t *T, int idStazione, int autonomia){
    int i;
    stazione_t *stazione;

    stazione = treeSearch(T, idStazione);
    i = ricercaBinaria(stazione->autonomie, MAXA, EMPTY);
    if(i >= 0 && i < MAXA){
        stazione->autonomie[i] = autonomia;
        if(autonomia > stazione->maxAut){ //aggiorno l'autonomia max
            stazione->maxAut = autonomia;
        }
        printf("aggiunta\n");
    } else {
        printf("non aggiunta\n");
    }
}

void rottamaAuto(stazione_t *T, int idStazione, int autonomia){
    int j;
    stazione_t *stazione;

    stazione = treeSearch(T, idStazione);
    j = ricercaBinaria(stazione->autonomie, MAXA, autonomia);
    if(j >= 0 && j < MAXA){
        stazione->autonomie[j] = EMPTY;
        quickSort(stazione->autonomie, 0, MAXA-1);
        stazione->maxAut = stazione->autonomie[0];
        printf("rottamata\n");
    } else {
        printf("non rottamata\n");
    }
}

void pianificaDiretto(stazione_t *stazione, int idPartenza, int idArrivo){ //partenza < arrivo
    int minStaz, ok;
    stazione_t *partenza, *arrivo, *curr, *prec, *save;
    ptr_nodo list, head;
    //il minStaz in caso da cambiare con array statico se dovesse avere problemi

    list = NULL;
    head = list;
    head->val = idArrivo;
    head = head->next;

    partenza = treeSearch(stazione, idPartenza);
    arrivo = treeSearch(stazione, idArrivo);

    if(partenza != NULL && arrivo != NULL){
        ok = 0;
        prec = arrivo;
        save = arrivo;
        curr = NULL;
        minStaz = arrivo->id;

        while(save != NULL && !ok){
            curr = treePredecessor(prec);

            while(curr != NULL){
                if(curr == partenza){
                    head->val = minStaz;
                    head = head->next;
                    head->val = partenza->id;
                    ok = 1;
                    break;
                }
                if(curr->id + curr->maxAut >= prec->id){ //raggiunge la stazione dopo
                    if(curr->id < minStaz && curr->maxAut!=0){
                        minStaz = curr->id;
                        save = curr;
                    }
                    curr = treePredecessor(curr);
                } else {
                    head->val = minStaz; //salva minStaz sulla lista da stampare
                    head = head->next;
                    prec = save;
                    break;
                }
            }
        }
        if(ok){
            do{ //stampa le stazioni attraversate a partire dalla fine
                printf("%d ", head->val);
                head = head->prev;
            }while(head != NULL);
        } else {
            printf("nessun percorso");
        }
        printf("\n");
    } else {
        printf("nessun percorso");
    }
}
/*
void pianificaInverso(stazione_t *stazione, int idPartenza, int idArrivo){ //partenza > arrivo
    int ko, maxStazAut;
    stazione_t *partenza, *arrivo, *prec, *curr, *save;
    ptr_nodo list, head;

    list = NULL;
    head = list;
    head->val = idPartenza;
    head = head->next;

    partenza = treeSearch(stazione, idPartenza);
    arrivo = treeSearch(stazione, idArrivo);

    prec = partenza;
    save = partenza;
    maxStazAut = partenza->id - partenza->maxAut;

    while(save != arrivo && !ko){
        ko = 1;
        curr = treeSuccessor(prec);
        while(curr != arrivo){
            if(prec->id - prec->maxAut >= curr->id){ //raggiunge la stazione dopo
                if(curr->id - curr->maxAut < maxStazAut && curr->id < save->id){
                    save = curr;
                    maxStazAut = curr->id - curr->maxAut;
                }
                ko = 0;
            }
            curr = treeSuccessor(curr);
        }
        head->val = save->id; //salva maxStazAut sulla lista da stampare
        head = head->next;
        prec = save;
    }
    head->val = idPartenza;

    if(!ko){
        do{ //stampa le stazioni attraversate a partire dall'inizio'
            printf("%d ", list->val);
            list = list->next;
        }while(list != NULL);
    } else {
        printf("nessun percorso");
    }
    printf("\n");
}
*/
void pianificaPercorso(stazione_t *stazione, int idPartenza, int idArrivo){
    if(idPartenza == idArrivo){
        printf("%d", idPartenza);
    } else if(idPartenza < idArrivo){
        pianificaDiretto(stazione, idPartenza, idArrivo);
    } /*else {
        pianificaInverso(stazione, idArrivo, idPartenza);
    }*/
}

/* main
 *
 *
 *
 * */

int main(){
    FILE* fp;
    int i, autonomia, distanza, numAuto, distanzaPartenza, distanzaArrivo;
    int parcoAuto[MAXA];
    char comando[20];

    stazione_t *new = createTree();
    fp = stdin;
    while(fscanf(fp, "%s", comando) != EOF){
        if (strcmp(comando, "aggiungi-stazione") == 0) {
            if (fscanf(fp, "%d %d", &distanza, &numAuto) == 2) {
                for (i = 0; i < numAuto && fscanf(fp, "%d", &parcoAuto[i]) == 1; i++) {

                }
                aggiungiStazione(new, distanza, numAuto, parcoAuto);
            }
        }
        if(strcmp(comando, "demolisci-stazione") == 0){
            if(fscanf(fp, "%d", &distanza) != EOF){
                demolisciStazione(new, distanza);
            }
        }
        if(strcmp(comando, "aggiungi-auto") == 0){
            if(fscanf(fp, "%d %d", &distanza, &autonomia) != EOF){
                aggiungiAuto(new, distanza, autonomia);
            }
        }
        if(strcmp(comando, "rottama-auto") == 0){
            if(fscanf(fp, "%d %d", &distanza, &autonomia) != EOF){
                rottamaAuto(new, distanza, autonomia);
            }
        }
        if(strcmp(comando, "pianifica-percorso") == 0){
            if(fscanf(fp, "%d %d", &distanzaPartenza, &distanzaArrivo) != EOF){
                pianificaPercorso(new, distanzaPartenza, distanzaArrivo);
            }
        }
    }
    printf("lamolamo\n");
    return 0;
}
