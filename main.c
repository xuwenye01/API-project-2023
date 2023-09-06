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
    int maxAut;
    int numAut;
    int autonomies[MAXA];
}station_t;

station_t *root;
station_t *NIL;

/*funzioni bst
 *
 *
 *
 * */

station_t *createTree(int distanza, int numAuto, const int *parcoAuto);
station_t *treeMinimum(station_t *x);
station_t *treeMaximum(station_t *x);
station_t *treePredecessor(station_t *x);
station_t *treeSearch(station_t *n, int distance);
station_t *freeBST(station_t *node);
void freeNodes(station_t *node);

/*funzioni quicksort
 *
 *
 *
 * */

void swap(int *x, int *y);
int partition(int *arr, int min, int max);
void quickSort(int *arr, int inf, int sup);

/*funzioni della specifica
 *
 *
 *
 * */

void aggiungiStazione(station_t *T, int distance, int numAuto, const int *parkAuto);
void demolisciStazione(station_t *T, int stationId);
void aggiungiAuto(station_t *T, int stationId, int autonomy);
void rottamaAuto(station_t *T, int stationId, int autonomy);
void pianificaPercorso(station_t *station, int startPoint, int endPoint);

/*funzioni custom
 *
 *
 *
 * */

int binarySearch(const int *arr, int dim, int num);
void pianificaDiretto(station_t *station, int startPoint, int endPoint);
void pianificaInverso(station_t *station, int startPoint, int endPoint);
void freeList(ptr_nodo list);
station_t *minStazOf(station_t *station);
void transplant(station_t *T, station_t *u, station_t *v);

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

int partition (int *arr, int min, int max){
    int x=arr[min];
    int i=min-1;
    int j=max+1;

    while (1) {
        do {
            j--;
        } while (i < j && arr[j] < x);
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
            quickSort(arr, inf, pivot);
            quickSort(arr, pivot+1, sup);
        }
    }
}

int binarySearch(const int *arr, int dim, int num){
    int min, max, mid;

    min = 0;
    max = dim - 1;
    while(min <= max){
        mid = min + (max - min)/2;
        if(arr[mid] == num){
            return mid;
        } else if (arr[mid] < num){
            max = mid - 1;
        } else {
            min = mid + 1;
        }
    }
    return EMPTY;
}

void freeList(ptr_nodo list){
    ptr_nodo del;

    while(list != NULL){
        del = list;
        list = list->next;
        free(del);
    }
}

station_t *minStazOf(station_t *station){ //minima station che un nodo raggiunge
    station_t *curr = NULL;
    curr = treePredecessor(station);

    if(station){
        while(curr != NULL && station->id - station->maxAut < curr->id){
            curr = treePredecessor(curr);
        }
    }
    return curr;
}

void transplant(station_t *T, station_t *u, station_t *v){
    if(u->father == NIL){
        root = v;
    } else if(u == u->father->left){
        u->father->left = v;
    } else {
        u->father->right = v;
    }
    if(v != NIL){
        v->father = u->father;
    }
}

void freeNodes(station_t *node) {
    if (node != NIL) {
        freeNodes(node->left);
        freeNodes(node->right);
        free(node);
    }
}

station_t *freeBST(station_t *node) {
    if (node != NIL) {
        node->left = freeBST(node->left);
        node->right = freeBST(node->right);
        free(node);
    }
    return NIL;
}

station_t *createTree(int distanza, int numAuto, const int *parcoAuto){
    station_t *station = NULL;

    station = (station_t*)malloc(sizeof(station_t));

    if(station){

        station->father = NIL;
        station->id = distanza;
        station->left = NIL;
        station->right = NIL;
        station->maxAut = 0;
        station->numAut = numAuto;

        for(int j=0; j<MAXA; j++){
            station->autonomies[j] = EMPTY;
        }

        for(int i=0; i<numAuto; i++){
            station->autonomies[i] = parcoAuto[i];
            if(parcoAuto[i] > station->maxAut){
                station->maxAut = parcoAuto[i]; //to save the max autonomy
            }
        }

    } else {
        printf("errore malloc\n");
        return 0;
    }
    return station;
}

station_t *treeMinimum(station_t *x){
    while(x->left != NIL){
        x = x->left;
    }
    return x;
}

station_t *treeMaximum(station_t *x){
    while(x->right != NIL){
        x = x->right;
    }
    return x;
}

station_t *treePredecessor(station_t *x){
    if(x->left != NIL){
        return treeMaximum(x->left);
    }
    station_t *y = x->father;
    while(y != NIL && x == y->left){
        x = y;
        y = y->father;
    }
    return y;
}

station_t *treeSearch(station_t *n, int distance){

    if(n == NIL || n->id == distance) return n;
    if(distance < n->id) {
        return treeSearch(n->left, distance);
    }
    return treeSearch(n->right, distance);
}

void aggiungiStazione(station_t *T, int distance, int numAuto, const int *parkAuto){
    station_t *prev = NIL;
    station_t *curr = root;

    if(treeSearch(T, distance) == NULL){ //aggiunge se non lo trova
        station_t *station = NULL;
        station = (station_t*)malloc(sizeof(station_t));

        if(station){
            if(T == NIL){
                station->father = NIL;
            }
            station->id = distance;
            station->left = NIL;
            station->right = NIL;
            station->maxAut = 0;
            station->numAut = numAuto;

            for(int j=0; j<MAXA; j++){
                station->autonomies[j] = EMPTY;
            }

            for(int i=0; i<numAuto; i++){
                station->autonomies[i] = parkAuto[i];
                if(parkAuto[i] > station->maxAut){
                    station->maxAut = parkAuto[i]; //per salvare l'auto con autonomia max
                }
            }

            while(curr != NIL) {
                prev = curr;
                if (station->id < curr->id) {
                    curr = curr->left;
                } else {
                    curr = curr->right;
                }
            }

            station->father = prev;

            if(prev == NIL){
                root = station;
            } else if(station->id < prev->id){
                prev->left = station;
            } else {
                prev->right = station;
            }

            if(treeSearch(T, station->id)){
                printf("aggiunta\n");
            } else {
                printf("non aggiunta\n");
            }
        } else {
            printf("errore malloc\n");
        }
    } else {
        printf("non aggiunta\n");
    }
}

void demolisciStazione(station_t *T, int stationId){
    station_t *z, *y;

    z = treeSearch(T, stationId);
    if(z){

        if(z->left == NIL){
            transplant(T, z, z->right);
        } else if(z->right == NIL){
            transplant(T, z, z->left);
        } else {
            y = treeMinimum(z->right);
            if(y->father != z){
                transplant(T, y, y->right);
                y->right = z->right;
                y->right->father = y;
            }
            transplant(T, z, y);
            y->left = z->left;
            y->left->father = y;
        }

        free(z);
        printf("demolita\n");
    } else {
        printf("non demolita\n");
    }
}

void aggiungiAuto(station_t *T, int stationId, int autonomy){
    int i, flag = 0;
    station_t *station;

    station = treeSearch(T, stationId);
    if(station){
        for(i=0; i < MAXA; i++){
            if(station->autonomies[i] == EMPTY){
                station->autonomies[i] = autonomy;
                station->numAut++;
                flag = 1;
                if(autonomy > station->maxAut){ //aggiorno l'autonomy max
                    station->maxAut = autonomy;
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

void rottamaAuto(station_t *T, int stationId, int autonomy){
    int i;
    station_t *station;

    station = treeSearch(T, stationId);
    if(station){

        quickSort(station->autonomies, 0, MAXA - 1);

        if(autonomy == station->maxAut){
            station->autonomies[0] = EMPTY;
            station->maxAut = station->autonomies[1];
            station->numAut--;
            printf("rottamata\n");
        } else {
            i = binarySearch(station->autonomies, MAXA, autonomy);
            if (i >= 0 && i < MAXA) {
                station->autonomies[i] = EMPTY;
                station->numAut--;
                if (station->numAut == 0) {
                    station->maxAut = 0;
                }
                printf("rottamata\n");
            } else {
                printf("non rottamata\n");
            }
        }
    } else {
        printf("non rottamata\n");
    }
}

//PARTE DALLA FINE
void pianificaDiretto(station_t *station, int startPoint, int endPoint){
    int minStaz, ok, ko;
    station_t *start, *end, *curr, *prec, *save;
    ptr_nodo head;
    ptr_nodo list = NULL;

    list = (lista_t*) malloc(sizeof(lista_t));
    if(list){
        list->val = 0;
        list->next = NULL;
        list->prev = NULL;

        head = list;
        head->val = endPoint;
        head->next = (lista_t*) malloc(sizeof(lista_t));
        if (head->next != NULL) {
            head->next->prev = head;
            head = head->next;
            head->next = NULL;
        }

        start = treeSearch(station, startPoint);
        end = treeSearch(station, endPoint);

        if(start != NULL && end != NULL){
            ko = 0;
            ok = 0;
            prec = end;
            save = end;
            curr = prec;
            minStaz = start->id;

            while(curr != NULL && !ok && !ko){
                curr = treePredecessor(prec);

                if(start->maxAut <= 0){
                    ko = 1;
                    break;
                }

                while(curr != NULL){

                    if(curr->id == start->id && curr->maxAut > 0){
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
                        } else { //curr (which equals start) reaches prec
                            head->val = startPoint;
                            ok = 1;
                            break;
                        }
                    } else {
                        if(curr->id + curr->maxAut >= prec->id) {
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
                do{ //prints out the stations starting from the end
                    printf("%d ", head->val);
                    head = head->prev;
                }while(head->prev != NULL);
                printf("%d", head->val);
                printf("\n");
            } else {
                printf("nessun percorso\n");
            }
        } else {
            printf("nessun percorso perché non trovo start o end\n");
        }
        freeList(list);
        list = NULL;
    } else {
        printf(("errore malloc"));
    }
}

//PARTE DALL'INIZIO
void pianificaInverso(station_t *station, int startPoint, int endPoint){
    int ok, ko, ambiguo;
    station_t *start, *end, *curr, *prec, *pros, *save;
    ptr_nodo head;
    ptr_nodo list = NULL;

    list = (lista_t*) malloc(sizeof(lista_t));

    if(list){
        list->val = 0;
        list->next = NULL;

        head = list;
        head->val = startPoint;
        head->next = (lista_t*) malloc(sizeof(lista_t));
        if (head->next != NULL) {
            head->next->prev = head;
            head = head->next;
            head->next = NULL;
        }

        start = treeSearch(station, startPoint);
        end = treeSearch(station, endPoint);

        if(start != NULL && end != NULL && head != NULL){
            ok = 0;
            ko = 0;
            ambiguo = 0;
            prec = start;
            save = start;
            curr = prec;

            while(curr != NULL && !ok && !ko){
                curr = treePredecessor(prec);

                if(start->maxAut <= 0){
                    ko = 1;
                    break;
                }

                while(curr != NULL && !ok && !ko){

                    while(prec->id - prec->maxAut <= curr->id){

                        if((curr->id - curr->maxAut) <= (save->id - save->maxAut) && curr->maxAut > 0){
                            save = curr;
                        }

                        curr = treePredecessor(curr);

                        if(curr->id == end->id) {
                            if (save->id - save->maxAut <= end->id && prec->id - prec->maxAut > curr->id) {

                                head->val = save->id;
                                head->next = (lista_t *) malloc(sizeof(lista_t));
                                if (head->next != NULL) {
                                    head->next->prev = head;
                                    head = head->next;
                                    head->next = NULL;
                                }
                                break;
                            } else {
                                break;
                            }
                        }
                    }

                    if(curr == treePredecessor(prec)){
                        ko = 1;
                    }

                    //metto la prima condizione per evitare il problema dell'open 10
                    if(curr != end && save == prec){
                        ko = 1;
                    }

                    if(curr != end){

                        if(save != minStazOf(prec)){
                            ambiguo = 1;
                        }

                        head->val = save->id;
                        head->next = (lista_t*) malloc(sizeof(lista_t));
                        if (head->next != NULL) {
                            head->next->prev = head;
                            head = head->next;
                            head->next = NULL;
                        }
                        prec = save;
                        curr = treePredecessor(prec);
                    }

                    if(curr->id == end->id){
                        if(save->id - save->maxAut <= end->id){
                            head->val = curr->id;
                            ok = 1;
                        } else {
                            ko = 1;
                        }
                    }
                }
            }

            if(ok && !ko){
                if(ambiguo){

                    while(head && head->prev && head->prev->prev){
                        prec = treeSearch(station, head->val);
                        save = treeSearch(station, head->prev->val);
                        pros = treeSearch(station, head->prev->prev->val);

                        curr = treePredecessor(save);

                        while(curr->id > (pros->id - pros->maxAut)){
                            if((curr->id - curr->maxAut) <= prec->id){
                                save = curr;
                            }
                            curr = treePredecessor(curr);
                        }

                        head->prev->val = save->id;
                        head = head->prev;
                    }
                }

                head = list;
                while (head->next != NULL) { //stampa le stazioni attraversate a partire dall'inizio'
                    printf("%d ", head->val);
                    head = head->next;
                }
                printf("%d\n", head->val);
            } else {
                printf("nessun percorso\n");
            }
        } else {
            printf("nessun percorso\n");
        }
        freeList(list);
        list = NULL;
    } else {
        printf(("errore malloc\n"));
    }
}

void pianificaPercorso(station_t *station, int startPoint, int endPoint){
    if(startPoint == endPoint){
        printf("%d", startPoint);
    } else if(startPoint < endPoint) {
        pianificaDiretto(station, startPoint, endPoint);
    } else {
        pianificaInverso(station, startPoint, endPoint);
    }
}

int main(){
    FILE* fp;
    int i, autonomy, distance, numAuto, startDistance, endDistance;
    int parkAuto[MAXA];
    char command[20];

    fp = stdin;
    root = NIL;

    while(fscanf(fp, "%s", command) != EOF){
        for(i=0; i<MAXA; i++){
            parkAuto[i] = EMPTY;
        }
        if (strcmp(command, "aggiungi-stazione") == 0) {
            if (fscanf(fp, "%d %d", &distance, &numAuto) == 2) {
                for (i = 0; i < numAuto; i++) {
                    if(fscanf(fp, "%d", &parkAuto[i]) != 1){
                        printf("errore lettura input aggiungi-stazione");
                    }
                }
                if(root == NULL){
                    root = createTree(distance, numAuto, parkAuto);
                    if(root){
                        printf("aggiunta\n");
                    }
                } else {
                    aggiungiStazione(root, distance, numAuto, parkAuto);
                }
            } else {
                printf("errore lettura input aggiungi-stazione");
                return 0;
            }
        }
        if(strcmp(command, "demolisci-stazione") == 0){
            if(fscanf(fp, "%d", &distance) != EOF){
                demolisciStazione(root, distance);
            }
        }
        if(strcmp(command, "aggiungi-auto") == 0){
            if(fscanf(fp, "%d %d", &distance, &autonomy) == 2){
                aggiungiAuto(root, distance, autonomy);
            }
        }
        if(strcmp(command, "rottama-auto") == 0){
            if(fscanf(fp, "%d %d", &distance, &autonomy) == 2){
                rottamaAuto(root, distance, autonomy);
            }
        }
        if(strcmp(command, "pianifica-percorso") == 0){
            if(fscanf(fp, "%d %d", &startDistance, &endDistance) == 2){
                pianificaPercorso(root, startDistance, endDistance);
            }
        }
    }

    freeBST(root);
    root = NIL;

    return 0;
}