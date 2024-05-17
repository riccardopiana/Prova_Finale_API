#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define N 20
#define MAX_AUTO 512

typedef enum{BLACK, RED} colore;

typedef struct veicolo{
    int autonomia;
    int uguali;
    struct veicolo* left;
    struct veicolo* right;
    struct veicolo* p;
    colore color;
} veicolo;

typedef struct stazione{
    int distanza;
    struct veicolo* parco_veicoli;
    int macchine;
    struct stazione* left;
    struct stazione* right;
    struct stazione* p;
    colore color;
    int visit;
    struct stazione* precedente;
    int max_autonomia;
} stazione;

stazione* snil = NULL;
veicolo* vnil = NULL;

typedef struct lista {
    stazione** stazione;
    struct lista* next;
} nodo;

typedef struct coda {
    nodo* head;
    nodo* tail;
} coda;

int visitato = 1;

void pianifica_percorso(stazione*, int, int);

/*Funzioni coda*/
void enqueue(coda*, stazione**);
stazione** dequeue(coda*);

/*Funzioni stazioni*/
stazione* cerca_stazione(stazione*, int);
stazione* stazione_precedente(stazione*);
stazione* stazione_successiva(stazione*);
stazione* stazione_minima(stazione*);
stazione* stazione_massima(stazione*);
stazione* rotazione_dx_autostrada(stazione*, stazione*);
stazione* rotazione_sx_autostrada(stazione*, stazione*);
stazione* aggiungi_stazione(stazione*, int);
stazione* aggiungi_stazione_fixup(stazione*, stazione*);
stazione* demolisci_stazione(stazione*, int);
stazione* demolisci_stazione_fixup(stazione*, stazione*);

/*Funzioni veicoli*/
veicolo* cerca_veicolo(veicolo*, int);
veicolo* veicolo_precedente(veicolo*);
veicolo* veicolo_successivo(veicolo*);
veicolo* veicolo_minimo(veicolo*);
veicolo* veicolo_massimo(veicolo*);
veicolo* rotazione_dx_parco_veicoli(veicolo*, veicolo*);
veicolo* rotazione_sx_parco_veicoli(veicolo*, veicolo*);
veicolo* aggiungi_auto(veicolo*, int);
veicolo* aggiungi_auto_fixup(veicolo*, veicolo*);
veicolo* rottama_auto(veicolo*, int);
veicolo* rottama_auto_fixup(veicolo*, veicolo*);
void free_parco_veicoli(veicolo*);

int main() {
    stazione *A1, *s;
    char istruzione[N];
    int distanza, autonomia, scan;
    
    vnil = malloc(sizeof(veicolo));
    vnil->color = BLACK;
    vnil->left = vnil;
    vnil->right = vnil;
    vnil->p = vnil;
    
    snil = malloc(sizeof(stazione));
    snil->color = BLACK;
    snil->left = snil;
    snil->right = snil;
    snil->p = snil;
    snil->parco_veicoli = vnil;
    
    A1 = snil;

    while (scanf("%s", istruzione) == 1){
        if (strcmp("aggiungi-stazione", istruzione) == 0){
            scan = scanf("%d", &distanza);
            A1 = aggiungi_stazione(A1, distanza);
        }else if (strcmp("demolisci-stazione", istruzione) == 0){
            scan = scanf("%d", &distanza);
            A1 = demolisci_stazione(A1, distanza);
        }else if (strcmp("aggiungi-auto", istruzione) == 0){
            scan = scanf("%d %d", &distanza, &autonomia);
            s = cerca_stazione(A1, distanza);
            if (s != snil){
                if (s->macchine < MAX_AUTO){
                    s->parco_veicoli = aggiungi_auto(s->parco_veicoli, autonomia);
                    s->macchine++;
                    printf("aggiunta\n");
                }else{
                    printf("non aggiunta\n");
                }
            }else{
                printf("non aggiunta\n");
            }
        }else if (strcmp("rottama-auto", istruzione) == 0){
            scan = scanf("%d %d", &distanza, &autonomia);
            s = cerca_stazione(A1, distanza);
            if (s != snil){
                s->parco_veicoli = rottama_auto(s->parco_veicoli, autonomia);
                s->macchine--;
            }else{
                printf("non rottamata\n");
            }
        }else if (strcmp("pianifica-percorso", istruzione) == 0){
            scan = scanf("%d %d", &distanza, &autonomia);
            pianifica_percorso(A1, distanza, autonomia);
        }
        if (scan == 0){
        }
        
    }
    free(snil);
    free(vnil);
    return 0;
}

void pianifica_percorso(stazione* T, int partenza, int destinazione){
    stazione *p, *d;
    p = cerca_stazione(T, partenza);
    d = cerca_stazione(T, destinazione);
    p->precedente = NULL;
    d->precedente = NULL;

    coda* Q = (coda*)malloc(sizeof(coda));
    Q->head = NULL;
    Q->tail = NULL;
    
    int len;
    stazione** successivi;

    if (partenza == destinazione){
        printf("%d\n", partenza);
        return;
    }
    if (partenza < destinazione){
        len = 1;
        for (stazione* curr = p; curr != d; curr = stazione_successiva(curr)){
            veicolo* m = veicolo_massimo(curr->parco_veicoli);
            curr->max_autonomia = m->autonomia;
            len++;
        }
        successivi = malloc((len+1)*sizeof(stazione*));
        len = 0;
        for (stazione* curr = p; curr != d; curr = stazione_successiva(curr)){
            successivi[len] = curr;
            len++;
        }
        successivi[len] = d;
        len++;
        successivi[len] = snil;
        enqueue(Q, &successivi[0]);
        while (Q->head != NULL){
            stazione** u = dequeue(Q);
            stazione** v = u + 1;
            while (*v != snil && (*v)->distanza <= (*u)->distanza + (*u)->max_autonomia){
                if ((*v)->visit < visitato){
                    (*v)->visit = visitato;
                    (*v)->precedente = *u;
                    enqueue(Q, v);
                }
                if ((*v)->distanza == d->distanza){
                    break;
                }
                v = v + 1;
            }
        }
        if (d->precedente != NULL){
            stazione* x = d;
            int i = 0;
            int percorso[100] = {0};
            while (x != NULL){
                percorso[i] = x->distanza;
                x = x->precedente;
                i++;
            }
            for (int j = i-1; j >= 0; j--){
                printf("%d ", percorso[j]);
            }
            printf("\n");
        }else{
            printf("nessun percorso\n");
        }   
    }else{
        len = 1;
        veicolo* m;
        for (stazione* curr = d; curr != p; curr = stazione_successiva(curr)){
            m = veicolo_massimo(curr->parco_veicoli);
            curr->max_autonomia = m->autonomia;
            len++;
        }
        m = veicolo_massimo(p->parco_veicoli);
        p->max_autonomia = m->autonomia;
        successivi = malloc((len+1)*sizeof(stazione*));
        len = 0;
        for (stazione* curr = d; curr != p; curr = stazione_successiva(curr)){
            successivi[len] = curr;
            len++;
        }
        successivi[len] = p;
        len++;
        successivi[len] = snil;
        enqueue(Q, &successivi[0]);
        stazione** u;
        stazione** v;
        while (Q->head != NULL){
            u = dequeue(Q);
            v = u + 1;
            while (*v != snil){
                if ((*v)->visit < visitato){
                    if (((*v)->distanza - (*v)->max_autonomia) <= (*u)->distanza){
                        (*v)->visit = visitato;
                        (*v)->precedente = *u;
                        enqueue(Q, v);
                    }
                }
                if ((*v)->distanza == d->distanza){
                    break;
                }
                v = v + 1;
            }
        }
        if (p->precedente != NULL){
            stazione* x = p;
            while (x != NULL){
                printf("%d ", x->distanza);
                x = x->precedente;
            }
            printf("\n");
        }else{
            printf("nessun percorso\n");
        }
    }
    free(successivi);
    free(Q);
    visitato++;
    return;
}

/*Funzioni coda*/
void enqueue(coda* Q, stazione** x){
    nodo *n;
    n = (nodo*)malloc(sizeof(nodo));
    n->stazione = x;
    n->next = NULL;
    if (Q->head == NULL){
        Q->head = n;
        Q->tail = Q->head;
    }else{
        Q->tail->next = n;
        Q->tail = Q->tail->next;
    }
    return;
}

stazione** dequeue(coda* Q){
    nodo *x;
    x = Q->head;
    Q->head = Q->head->next;
    stazione** s = x->stazione;
    free(x);
    return s;    
}

/*Funzioni stazioni*/
stazione* cerca_stazione(stazione* x, int k){
    if (x == snil || k == x->distanza){
        return x;
    }
    if (k < x->distanza){
        return cerca_stazione(x->left, k);
    }else{
        return cerca_stazione(x->right, k);
    }
}

stazione* stazione_precedente(stazione* x){
    if (x->left != snil){
        return stazione_massima(x->left);
    }
    stazione* y;
    y = x->p;
    while (y != snil && x == y->left){
        x = y;
        y = y->p;
    }
    return y;
}

stazione* stazione_successiva(stazione* x){
    if (x->right != snil){
        return stazione_minima(x->right);
    }
    stazione* y;
    y = x->p;
    while (y != snil && x == y->right){
        x = y;
        y = y->p;
    }
    return y;
}

stazione* stazione_minima(stazione* x){
    while (x->left != snil){
        x = x->left;
    }
    return x;
}

stazione* stazione_massima(stazione* x){
    while (x->right != snil){
        x = x->right;
    }
    return x;
}

stazione* aggiungi_stazione(stazione* T, int distanza){
    stazione* z;
    z = (stazione*)malloc(sizeof(stazione));
    z->distanza = distanza;
    z->visit = 0;
    z->precedente = NULL;
    
    stazione *y, *x;
    y = snil;
    x = T;
    
    while (x != snil){
        y = x;
        if (x->distanza == z->distanza){
            printf("non aggiunta\n");
            free(z);
            return T;
        }
        if (z->distanza < x->distanza){
            x = x->left;
        }else{
            x = x->right;
        }
    }
    z->p = y;
    if (y == snil){
        T = z;
    }else if (z->distanza < y->distanza){
        y->left = z;
    }else{
        y->right = z;
    }
    z->left = snil;
    z->right = snil;
    z->color = RED;
    T = aggiungi_stazione_fixup(T, z);

    if (scanf("%d", &z->macchine) == 1){
        z->parco_veicoli = vnil;
        if (z->macchine > 0){
            int a;
            while (scanf("%d", &a) == 1){
                z->parco_veicoli = aggiungi_auto(z->parco_veicoli, a);
            }
        }
        printf("aggiunta\n");
    }
    return T;
}

stazione* aggiungi_stazione_fixup(stazione* T, stazione* z){
    stazione *x, *y;
    if (z == T){
        T->color = BLACK;
    }else{
        x = z->p;
        if (x->color == RED){
            if (x == x->p->left){
                y = x->p->right;
                if (y->color == RED){
                    x->color = BLACK;
                    y->color = BLACK;
                    x->p->color = RED;
                    T = aggiungi_stazione_fixup(T, x->p);
                }else{
                    if (z == x->right){
                        z = x;
                        T = rotazione_sx_autostrada(T, z);
                        x = z->p;
                    }
                    x->color = BLACK;
                    x->p->color = RED;
                    T = rotazione_dx_autostrada(T, x->p);
                }  
            }else{
                y = x->p->left;
                if (y->color == RED){
                    x->color = BLACK;
                    y->color = BLACK;
                    x->p->color = RED;
                    T = aggiungi_stazione_fixup(T, x->p);
                }else{
                    if (z == x->left){
                        z = x;
                        T = rotazione_dx_autostrada(T, z);
                        x = z->p;
                    }
                    x->color = BLACK;
                    x->p->color = RED;
                    T = rotazione_sx_autostrada(T, x->p);
                }
            }   
        }   
    }
    return T;
}

stazione* rotazione_dx_autostrada(stazione* T, stazione* y){
    stazione* x;
    x = y->left;
    y->left = x->right;
    if (x->right != snil){
        x->right->p = y;
    }
    x->p = y->p;
    if (y->p == snil){
        T = x;
    }else if (y == y->p->right){
        y->p->right = x;
    }else{
        y->p->left = x;
    }
    x->right = y;
    y->p = x;
    return T;
}

stazione* rotazione_sx_autostrada(stazione* T, stazione* x){
    stazione* y;
    y = x->right;
    x->right = y->left;
    if (y->left != snil){
        y->left->p = x; 
    }
    y->p = x->p;
    if (x->p == snil){
        T = y;
    }else{
        if (x == x->p->left){
            x->p->left = y;
        }else{
            x->p->right = y;
        }
    } 
    y->left = x;
    x->p = y;
    return T;
}

stazione* demolisci_stazione(stazione* T, int distanza){
    stazione* z;
    z = cerca_stazione(T, distanza);
    
    if (z != snil){
        stazione *y, *x;
        if (z->left == snil || z->right == snil){
            y = z;
        }else{
            y = stazione_successiva(z);
        }
        if (y->left != snil){
            x = y->left;
        }else{
            x = y->right;
        }
        x->p = y->p;
        if (y->p == snil){
            T = x;
        }else if (y == y->p->left){
            y->p->left = x;
        }else{
            y->p->right = x;
        }
        free_parco_veicoli(z->parco_veicoli);
        if (y != z){
            z->distanza = y->distanza;
            z->parco_veicoli = y->parco_veicoli;
            z->macchine = y->macchine;
        }
        if (y->color == BLACK){
            T = demolisci_stazione_fixup(T, x);
        }
        free(y);
        printf("demolita\n");
    }else{
        printf("non demolita\n");
    }
    return T;
}

stazione* demolisci_stazione_fixup(stazione* T, stazione* x){
    stazione* w;
    if (x->color == RED || x->p == snil){
        x->color = BLACK;
    }else if (x == x->p->left){
        w = x->p->right;
        if (w->color == RED){
            w->color = BLACK;
            x->p->color = RED;
            T = rotazione_sx_autostrada(T, x->p);
            w = x->p->right;
        }
        if (w->left->color == BLACK && w->right->color == BLACK){
            w->color = RED;
            T = demolisci_stazione_fixup(T, x->p);
        }else{
            if (w->right->color == BLACK){
                w->left->color = BLACK;
                w->color = RED;
                T = rotazione_dx_autostrada(T, w);
                w = x->p->right;
            }
            w->color = x->p->color;
            x->p->color = BLACK;
            w->right->color = BLACK;
            T = rotazione_sx_autostrada(T, x->p);
        }
    }else{
        w = x->p->left;
        if (w->color == RED){
            w->color = BLACK;
            x->p->color = RED;
            T = rotazione_dx_autostrada(T, x->p);
            w = x->p->left;
        }
        if (w->right->color == BLACK && w->left->color == BLACK){
            w->color = RED;
            T = demolisci_stazione_fixup(T, x->p);
        }else{
            if (w->left->color == BLACK){
                w->right->color = BLACK;
                w->color = RED;
                T = rotazione_sx_autostrada(T, w);
                w = x->p->left;
            }
            w->color = x->p->color;
            x->p->color = BLACK;
            w->left->color = BLACK;
            T = rotazione_dx_autostrada(T, x->p);
        }
    }
    return T;
}

/*Funzioni veicoli*/
veicolo* cerca_veicolo(veicolo* x, int k){
    if (x == vnil || k == x->autonomia){
        return x;
    }
    if (k < x->autonomia){
        return cerca_veicolo(x->left, k);
    }else{
        return cerca_veicolo(x->right, k);
    }
}

veicolo* veicolo_precedente(veicolo* x){
    if (x->left != vnil){
        return veicolo_massimo(x->left);
    }
    veicolo* y;
    y = x->p;
    while (y != NULL && x == y->left){
        x = y;
        y = y->p;
    }
    return y;
}

veicolo* veicolo_successivo(veicolo* x){
    if (x->right != vnil){
        return veicolo_minimo(x->right);
    }
    veicolo* y;
    y = x->p;
    while (y != NULL && x == y->right){
        x = y;
        y = y->p;
    }
    return y;
}

veicolo* veicolo_minimo(veicolo* x){
    while (x->left != vnil){
        x = x->left;
    }
    return x;
}

veicolo* veicolo_massimo(veicolo* x){
    while (x->right != vnil){
        x = x->right;
    }
    return x;
}

veicolo* aggiungi_auto(veicolo* T, int autonomia){
    veicolo* z;
    z = (veicolo*)malloc(sizeof(veicolo));
    z->autonomia = autonomia;
    
    veicolo *y, *x;
    y = vnil;
    x = T;
    
    while (x != vnil){
        y = x;
        if (z->autonomia == x->autonomia){
            x->uguali++;
            free(z);
            return T;
        }
        if (z->autonomia < x->autonomia){
            x = x->left;
        }else{
            x = x->right;
        }
    }
    z->p = y;
    z->uguali = 1;
    if (y == vnil){
        T = z;
    }else if (z->autonomia < y->autonomia){
        y->left = z;
    }else{
        y->right = z;
    }
    z->left = vnil;
    z->right = vnil;
    z->color = RED;
    T = aggiungi_auto_fixup(T, z);
    return T;
}

veicolo* aggiungi_auto_fixup(veicolo* T, veicolo* z){
    veicolo *x, *y;
    if (z == T){
        T->color = BLACK;
    }else{
        x = z->p;
        if (x->color == RED){
            if (x == x->p->left){
                y = x->p->right;
                if (y->color == RED){
                    x->color = BLACK;
                    y->color = BLACK;
                    x->p->color = RED;
                    T = aggiungi_auto_fixup(T, x->p);
                }else{
                    if (z == x->right){
                        z = x;
                        T = rotazione_sx_parco_veicoli(T, z);
                        x = z->p;
                    }
                    x->color = BLACK;
                    x->p->color = RED;
                    T = rotazione_dx_parco_veicoli(T, x->p);
                }  
            }else{
                y = x->p->left;
                if (y->color == RED){
                    x->color = BLACK;
                    y->color = BLACK;
                    x->p->color = RED;
                    T = aggiungi_auto_fixup(T, x->p);
                }else{
                    if (z == x->left){
                        z = x;
                        T = rotazione_dx_parco_veicoli(T, z);
                        x = z->p;
                    }
                    x->color = BLACK;
                    x->p->color = RED;
                    T = rotazione_sx_parco_veicoli(T, x->p);
                }
            }   
        }   
    }
    return T;
}

veicolo* rotazione_dx_parco_veicoli(veicolo* T, veicolo* y){
    veicolo* x;
    x = y->left;
    y->left = x->right;
    if (x->right != vnil){
        x->right->p = y;
    }
    x->p = y->p;
    if (y->p == vnil){
        T = x;
    }else if (y == y->p->right){
        y->p->right = x;
    }else{
        y->p->left = x;
    }
    x->right = y;
    y->p = x;
    return T;
}

veicolo* rotazione_sx_parco_veicoli(veicolo* T, veicolo* x){
    veicolo* y;
    y = x->right;
    x->right = y->left;
    if (y->left != vnil){
        y->left->p = x; 
    }
    y->p = x->p;
    if (x->p == vnil){
        T = y;
    }else{
        if (x == x->p->left){
            x->p->left = y;
        }else{
            x->p->right = y;
        }
    } 
    y->left = x;
    x->p = y;
    return T;
}

veicolo* rottama_auto(veicolo* T, int autonomia){
    veicolo* z;
    z = cerca_veicolo(T, autonomia);
    if (z != vnil){
        if (z->uguali > 1){
            z->uguali--;
        }else{
            veicolo *x, *y;
            if (z->left == vnil || z->right == vnil){
                y = z;
            }else{
                y = veicolo_successivo(z);
            }
            if (y->left != vnil){
                x = y->left;
            }else{
                x = y->right;
            }
            x->p = y->p;
            if (y->p == vnil){
                T = x;
            }else if (y == y->p->left){
                y->p->left = x;
            }else{
                y->p->right = x;
            }
            if (y != z){
                z->autonomia = y->autonomia;
                z->uguali = y->uguali;
            }
            if (y->color == BLACK){
                T = rottama_auto_fixup(T, x);
            }
            free(y);
        }
        printf("rottamata\n");
    }else
    {
        printf("non rottamata\n");
    }
    return T;
}

veicolo* rottama_auto_fixup(veicolo* T, veicolo* x){
    veicolo* w;
    if (x->color == RED || x->p == vnil){
        x->color = BLACK;
    }else if (x == x->p->left){
        w = x->p->right;
        if (w->color == RED){
            w->color = BLACK;
            x->p->color = RED;
            T = rotazione_sx_parco_veicoli(T, x->p);
            w = x->p->right;
        }
        if (w->left->color == BLACK && w->right->color == BLACK){
            w->color = RED;
            T = rottama_auto_fixup(T, x->p);
        }else{
            if (w->right->color == BLACK){
                w->left->color = BLACK;
                w->color = RED;
                T = rotazione_dx_parco_veicoli(T, w);
                w = x->p->right;
            }
            w->color = x->p->color;
            x->p->color = BLACK;
            w->right->color = BLACK;
            T = rotazione_sx_parco_veicoli(T, x->p);
        }
    }else{
        w = x->p->left;
        if (w->color == RED){
            w->color = BLACK;
            x->p->color = RED;
            T = rotazione_dx_parco_veicoli(T, x->p);
            w = x->p->left;
        }
        if (w->right->color == BLACK && w->left->color == BLACK){
            w->color = RED;
            T = rottama_auto_fixup(T, x->p);
        }else{
            if (w->left->color == BLACK){
                w->right->color = BLACK;
                w->color = RED;
                T = rotazione_sx_parco_veicoli(T, w);
                w = x->p->left;
            }
            w->color = x->p->color;
            x->p->color = BLACK;
            w->left->color = BLACK;
            T = rotazione_dx_parco_veicoli(T, x->p);
        }
    }
    return T;
}

void free_parco_veicoli(veicolo* x){
    if (x == vnil){
        return;
    }
    free_parco_veicoli(x->left);
    free_parco_veicoli(x->right);
    free(x);
}
