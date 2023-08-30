#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define EMPTY (-1)
#define MAXA 512

typedef struct n{
    int val;
    struct n *next;
}list_t;

typedef list_t *ptr_node;

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
    int maxAut;
    int autonomie[MAXA];
}stazione_t;

stazione_t *root;
stazione_t *NIL;

/*funzioni bst
 *
 *
 *
 * */

stazione_t *createTree(int distanza, int numAuto, const int *parcoAuto);
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
int partition(int *arr, int min, int r);
void quickSort(int *arr, int inf, int sup);

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
void pianificaInverso(stazione_t *stazione, int idPartenza, int idArrivo);
void freeList(ptr_nodo list);
void freeSingleList(ptr_node list);
stazione_t *minStazOf(stazione_t *stazione);

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

int partition (int *arr,int min, int max){
    int x=arr[min];
    int i=min-1;
    int j=max+1;

    while (1) {
        do {
            j--;
        } while (arr[j] < x);
        do {
            i++;
        } while (arr[i] > x);
        if  (i < j)
            swap(&arr[i],&arr[j]);
        else
            return j;
    }
}

void quickSort(int *arr, int inf, int sup){

    if(arr){
        if(inf < sup){
            int pivot = partition(arr, inf, sup);
            printf("pivot = %d", pivot);
            quickSort(arr, inf, pivot-1);
            quickSort(arr, pivot+1, sup);
        }
    }
}

int ricercaBinaria(const int *arr, int dim, int num){
    int min, max, mid;

    min = 0;
    max = dim - 1;
    while(min <= max){
        mid = min + (max - min)/2;
        printf("mid = %d\n", mid);
        printf("arr[mid] = %d\n", arr[mid]);

        if(arr[mid] == num){
            printf("%d uguale a %d\n", arr[mid], num);
            return mid;
        } else if (arr[mid] < num){
            printf("%d minore a %d\n", arr[mid], num);
            max = mid - 1;
        } else {
            printf("%d maggiore a %d\n", arr[mid], num);
            min = mid + 1;
        }
    }
    return EMPTY;
}

void freeSingleList(ptr_node list){
    ptr_node del;

    while(list != NULL){
        del = list;
        list = list->next;
        free(del);
    }
}

void freeList(ptr_nodo list){
    ptr_nodo del;

    while(list != NULL){
        del = list;
        list = list->next;
        free(del);
    }
}

stazione_t *minStazOf(stazione_t *stazione){ //minima stazione che un nodo raggiunge
    stazione_t *curr = NULL;
    curr = treePredecessor(stazione);

    if(stazione){
        while(curr != NULL && stazione->id - stazione->maxAut >= curr->id){
            curr = treePredecessor(curr);
        }
    }
    return curr;
}

stazione_t *createTree(int distanza, int numAuto, const int *parcoAuto){
    stazione_t *stazione = NULL;

    stazione = (stazione_t*)malloc(sizeof(stazione_t));

    if(stazione){

        stazione->father = NIL;
        stazione->id = distanza;
        stazione->left = NIL;
        stazione->right = NIL;
        stazione->maxAut = 0;
        for(int j=0; j<MAXA; j++){
            stazione->autonomie[j] = EMPTY;
        }

        for(int i=0; i<numAuto; i++){
            stazione->autonomie[i] = parcoAuto[i];
            if(parcoAuto[i] > stazione->maxAut){
                stazione->maxAut = parcoAuto[i]; //per salvare l'auto con autonomia max
            }
        }

    } else {
        printf("errore malloc\n");
        return 0;
    }
    return stazione;
}

stazione_t *treeMinimum(stazione_t *x){
    while(x->left != NIL){
        x = x->left;
    }
    return x;
}

stazione_t *treeMaximum(stazione_t *x){
    while(x->right != NIL){
        x = x->right;
    }
    return x;
}

stazione_t *treePredecessor(stazione_t *x){
    if(x->left != NIL){
        return treeMaximum(x->left);
    }
    stazione_t *y = x->father;
    while(y != NIL && x == y->left){
        x = y;
        y = y->father;
    }
    return y;
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

    if(n == NIL || n->id == distanza) return n;
    if(distanza < n->id) {
        return treeSearch(n->left, distanza);
    }
    return treeSearch(n->right, distanza);
}

void aggiungiStazione(stazione_t *T, int distanza, int numAuto, const int *parcoAuto){
    stazione_t *prev = NIL;
    stazione_t *curr = root;

    if(treeSearch(T, distanza) == NULL){ //aggiunge se non lo trova
        stazione_t *stazione = NULL;
        stazione = (stazione_t*)malloc(sizeof(stazione_t));

        if(stazione){
            //debug***************************************
            //printf("malloc stazione ok\n");
            //*******************************************
            //inizializzazioni
            if(T == NIL){
                stazione->father = NIL;
            }
            stazione->id = distanza;
            stazione->left = NIL;
            stazione->right = NIL;
            stazione->maxAut = 0;
            for(int j=0; j<MAXA; j++){
                stazione->autonomie[j] = EMPTY;
            }
            //debug******************************
            //printf("distanza: %d\n", stazione->id);
            //********************************

            /*for(int k=0; parcoAuto[k] != EMPTY; k++){
                printf("%d ", parcoAuto[k]);
            }
            printf("\n");*/

            for(int i=0; i<numAuto; i++){
                stazione->autonomie[i] = parcoAuto[i];
                if(parcoAuto[i] > stazione->maxAut){
                    stazione->maxAut = parcoAuto[i]; //per salvare l'auto con autonomia max
                }
            }
            //debug******************************
            //printf("maxAut: %d\n", stazione->maxAut);
            //***********************************
            while(curr != NIL) {
                prev = curr;
                if (stazione->id < curr->id) {
                    curr = curr->left;
                } else {
                    curr = curr->right;
                }
            }

            stazione->father = prev;

            if(prev == NIL){
                root = stazione;
            } else if(stazione->id < prev->id){
                prev->left = stazione;
            } else {
                prev->right = stazione;
            }

            //******************************
            /*if(treeSearch(T, stazione->id)){
                printf("%d è presente nel bst\n", stazione->id);
            } else {
                printf("%d non è presente nel bst\n", stazione->id);
            }*/
            //******************************

            if(treeSearch(T, stazione->id)){
                printf("aggiunta %d\n", stazione->id);
            } else {
                printf("non aggiunta %d\n",stazione->id);
            }
        } else {
            printf("errore malloc\n");
        }
    } else {
        printf("non aggiunta\n");
    }
}

void demolisciStazione(stazione_t *T, int idStazione){
    stazione_t *figli, *del, *z;

    z = treeSearch(T, idStazione);
    if(z){
        if(z->left == NIL || z->right == NIL){
            del = z;
        } else del = treeSuccessor(z);
        if(del->left != NIL){
            figli = del->left;
        } else figli = del->right;
        if(figli != NIL){
            figli->father = del->father;
        }
        if(del->father == NIL){
            root = figli;
        } else if(del == del->father->left){
            del->father->left = figli;
        } else {
            del->father->right = figli;
        }
        if(del != z){
            z->id = del->id;
        }
        free(del);
        printf("demolita\n");
    } else {
        printf("non demolita\n");
    }
}

void aggiungiAuto(stazione_t *T, int idStazione, int autonomia){
    int i, flag = 0;
    stazione_t *stazione;

    stazione = treeSearch(T, idStazione);
    if(stazione){
        for(i=0; i < MAXA; i++){
            if(stazione->autonomie[i] == EMPTY){
                stazione->autonomie[i] = autonomia;
                flag = 1;
                if(autonomia > stazione->maxAut){ //aggiorno l'autonomia max
                    stazione->maxAut = autonomia;
                }
                printf("aggiunta\n");
                break;
            }
        }
        if(!flag){
            printf("non aggiunta\n");
        }
    } else {
        printf("non aggiunta\n");
    }
}

void rottamaAuto(stazione_t *T, int idStazione, int autonomia){
    int i;
    stazione_t *stazione;

    stazione = treeSearch(T, idStazione);
    if(stazione){

        for(int j = 0; j<MAXA; j++){
            printf("%d ", stazione->autonomie[j]);
        }
        printf("\n");

        quickSort(stazione->autonomie, 0, MAXA-1);

        for(int j = 0; j<MAXA; j++){
            printf("%d ", stazione->autonomie[j]);
        }
        printf("\n");


        i = ricercaBinaria(stazione->autonomie, MAXA, autonomia);

        if(i >= 0 && i < MAXA){
            stazione->autonomie[i] = EMPTY;
            if(stazione->autonomie[0] == EMPTY){
                stazione->maxAut = 0;
            } else {
                stazione->maxAut = stazione->autonomie[0];
            }
            printf("rottamata\n");
        } else {
            printf("non rottamata\n");
        }
    } else {
        printf("non rottamata\n");
    }
}
/*
void pianificaDiretto(stazione_t *stazione, int idPartenza, int idArrivo){
    int minStaz, ok;
    stazione_t *partenza, *arrivo, *curr, *prec, *save;
    ptr_nodo list, head;
    //il minStaz in caso da cambiare con array statico se dovesse avere problemi

    list = (ptr_nodo) malloc(sizeof(ptr_nodo));
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
            printf("\n");
        } else {
            printf("nessun percorso");
        }
    } else {
        printf("nessun percorso");
    }
    list = NULL;
}*/

//PARTE DALLA FINE
void pianificaDiretto(stazione_t *stazione, int idPartenza, int idArrivo){
    int minStaz, ok, ko;
    stazione_t *partenza, *arrivo, *curr, *prec, *save;
    ptr_nodo head;
    ptr_nodo list = NULL;

    list = (lista_t*) malloc(sizeof(lista_t));
    if(list){
        list->val = 0;
        list->next = NULL;
        list->prev = NULL;

        head = list;
        head->val = idArrivo;
        head->next = (lista_t*) malloc(sizeof(lista_t));
        if (head->next != NULL) {
            head->next->prev = head;
            head = head->next;
            head->next = NULL;
        }

        partenza = treeSearch(stazione, idPartenza);
        arrivo = treeSearch(stazione, idArrivo);

        if(partenza != NULL && arrivo != NULL){
            ko = 0;
            ok = 0;
            prec = arrivo;
            save = arrivo;
            curr = prec;
            minStaz = partenza->id;

            while(curr != NULL && !ok && !ko){
                curr = treePredecessor(prec);

                while(curr != NULL){

                    if(curr->id == partenza->id){
                        if(curr->id + curr->maxAut < prec->id){
                            if(save == prec){
                                ko = 1;
                                break;
                            } else {
                                head->val = minStaz;
                                head->next = (lista_t*) malloc(sizeof(lista_t));
                                if (head->next != NULL) {
                                    head->next->prev = head;
                                    head = head->next;
                                    head->next = NULL;
                                } else {
                                    printf("errore malloc");
                                    break;
                                }
                                prec = save;
                                curr = prec;
                            }
                        } else { //curr (che è partenza) raggiunge prec
                            head->val = idPartenza;
                            ok = 1;
                            break;
                        }
                    } else { //stazione intermedia
                        if(curr->id + curr->maxAut >= prec->id) { //raggiunge la stazione dopo
                            if (curr->id < save->id && curr->maxAut != 0) {
                                minStaz = curr->id;
                                save = curr;
                            }
                        }
                    }
                    curr = treePredecessor(curr);
                }
            }

            if(ok){
                do{ //stampa le stazioni attraversate a partire dalla fine
                    printf("%d ", head->val);
                    head = head->prev;
                }while(head->prev != NULL);
                printf("%d", head->val);
                printf("\n");
            } else {
                printf("nessun percorso\n");
            }
        } else {
            printf("nessun percorso\n");
        }
        freeList(list);
        list = NULL;
    } else {
        printf(("errore malloc"));
    }
}

void pianificaInverso(stazione_t *stazione, int idPartenza, int idArrivo){
    int ok, ko;
    stazione_t *partenza, *arrivo, *curr, *prec, *save;
    ptr_node head;
    ptr_node list = NULL;

    list = (list_t*) malloc(sizeof(list_t));
    if(list){
        list->val = 0;
        list->next = NULL;

        head = list;
        head->val = idPartenza;
        head->next = (list_t*) malloc(sizeof(list_t));
        if (head->next != NULL) {
            head = head->next;
            head->next = NULL;
        }

        partenza = treeSearch(stazione, idPartenza);
        arrivo = treeSearch(stazione, idArrivo);

        if(partenza != NULL && arrivo != NULL && head != NULL){
            ok = 0;
            ko = 0;
            prec = partenza;
            save = partenza;
            curr = prec;
            printf("id CURR: %d\nid PREC: %d\n", curr->id, prec->id);

            while(curr != NULL && !ok && !ko){
                curr = treePredecessor(prec);

                while(curr != NULL && !ok && !ko){
                    printf("id CURR: %d\n", curr->id);

                    while(prec->id - prec->maxAut <= curr->id){
                        printf("id prec - maxAut PREC = %d\n", prec->id-prec->maxAut);

                        if((curr->id - curr->maxAut) <= (save->id - save->maxAut)) {
                            save = curr;
                        } else {
                            if(minStazOf(save)->id - minStazOf(save)->maxAut < minStazOf(prec)->id - minStazOf(prec)->maxAut){
                                printf("ramo then caso incriminato\n");
                                save = minStazOf(save);
                                curr = minStazOf(curr);

                                printf("id SAVE: %d\n", save->id);
                                printf("id CURR: %d\n", curr->id);

                                if(minStazOf(curr) != NULL && minStazOf(save) != NULL &&
                                    minStazOf(save)->id - minStazOf(save)->maxAut <=
                                        (minStazOf(curr)->id - minStazOf(curr)->maxAut)){
                                    curr = treePredecessor(curr);
                                } else {
                                    save = curr;
                                    curr = treePredecessor(save);
                                }
                                break;
                            } else {
                                save = treePredecessor(save);
                                curr = save;
                            }
                        }
                        curr = treePredecessor(curr);
                        printf("id CURR: %d\n", curr->id);

                        if(curr->id == arrivo->id){
                            break;
                        }
                    }

                    if(curr == treePredecessor(prec)){
                        ko = 1;
                    }

                    head->val = save->id;
                    printf("VALORE IN HEAD: %d\n", head->val);
                    head->next = (list_t*) malloc(sizeof(list_t));
                    if (head->next != NULL) {
                        head = head->next;
                        head->next = NULL;
                    }

                    prec = save;
                    printf("id prec: %d\n", prec->id);
                    curr = treePredecessor(save);

                    if(curr->id == arrivo->id){

                        head->val = curr->id;
                        head->next = (list_t*) malloc(sizeof(list_t));
                        if (head->next != NULL) {
                            head = head->next;
                            head->next = NULL;
                        }

                        ok = 1;
                        printf("OK, ARRIVATO ALLA FINE\n");
                    }
                }
            }

            if(ok){
                printf("ook = 1\n");

                head = list;
                while (head->next != NULL) { //stampa le stazioni attraversate a partire dall'inizio'
                    printf("%d ", head->val);
                    head = head->next;
                }
                /*
                do{
                    printf("%d ", list->val);
                    list = list->next;
                }while(list != NULL);
                printf("%d", list->val);
                printf("\n");
                */
            } else {
                printf("nessun percorso\n");
            }
        } else {
            printf("nessun percorso\n");
        }
        freeSingleList(list);
        list = NULL;
    } else {
        printf(("errore malloc"));
    }
}

void pianificaPercorso(stazione_t *stazione, int idPartenza, int idArrivo){
    if(idPartenza == idArrivo){
        printf("%d", idPartenza);
    } else if(idPartenza < idArrivo) {
        pianificaDiretto(stazione, idPartenza, idArrivo);
    } else {
        pianificaInverso(stazione, idPartenza, idArrivo);
    }
}

int main(){
    FILE* fp;
    int i, autonomia, distanza, numAuto, distanzaPartenza, distanzaArrivo;
    int parcoAuto[MAXA];
    char comando[20];

    fp = stdin;
    root = NIL;
    //debug******************************
    /*if(treeSearch(root, 45)){
        printf("main: trovato bastardo\n");
    } else {
        printf("main: non trovato bastardo\n");
        return 0;
    }*/
    //**************************************

    printf("\n");

    while(fscanf(fp, "%s", comando) != EOF){
        for(i=0; i<MAXA; i++){
            parcoAuto[i] = EMPTY;
        }
        if (strcmp(comando, "aggiungi-stazione") == 0) {
            if (fscanf(fp, "%d %d", &distanza, &numAuto) == 2) {
                for (i = 0; i < numAuto; i++) {
                    if(fscanf(fp, "%d", &parcoAuto[i]) != 1){
                        printf("errore lettura input aggiungi-stazione");
                    }
                }
                if(root == NULL){
                    root = createTree(distanza, numAuto, parcoAuto);
                    if(root){
                        printf("aggiunta\n");
                    }
                } else {
                    aggiungiStazione(root, distanza, numAuto, parcoAuto);
                }
            } else {
                printf("errore lettura input aggiungi-stazione");
                return 0;
            }
        }
        if(strcmp(comando, "demolisci-stazione") == 0){
            if(fscanf(fp, "%d", &distanza) != EOF){
                demolisciStazione(root, distanza);
            }
        }
        if(strcmp(comando, "aggiungi-auto") == 0){
            if(fscanf(fp, "%d %d", &distanza, &autonomia) == 2){
                aggiungiAuto(root, distanza, autonomia);
            }
        }
        if(strcmp(comando, "rottama-auto") == 0){
            if(fscanf(fp, "%d %d", &distanza, &autonomia) == 2){
                rottamaAuto(root, distanza, autonomia);
            }
        }
        if(strcmp(comando, "pianifica-percorso") == 0){
            if(fscanf(fp, "%d %d", &distanzaPartenza, &distanzaArrivo) == 2){
                pianificaPercorso(root, distanzaPartenza, distanzaArrivo);
            }
        }
    }
    free(root);
    root = NULL;

    return 0;
}