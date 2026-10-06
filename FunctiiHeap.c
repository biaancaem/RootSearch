
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structuri.h"

int comp(void *a, void *b) {
    TListaFisiere Fisier1 = (TListaFisiere)a;
    TListaFisiere Fisier2 = (TListaFisiere)b;
    // Pentru Max-Heap, vrem ca scorul mai mare sa produca un rezultat negativ
    // pentru a urca in fata
    if (Fisier1->scor > Fisier2->scor)
        return -1; // Fisier1 este mai prioritar
    else if (Fisier1->scor < Fisier2->scor)
        return 1; // Fisier2 este mai prioritar
    else {
        // Scoruri egale: sortare lexicografica dupa id-ul fisierului
        return strcmp(Fisier1->id, Fisier2->id);
    }
}

/* functie de alocare a unui heap primeste ca parametru numarul
maxim de elemente si functia de comparare */
THeap* AlocaHeap(int nrMax, TFCmp comp) {
    // alocam memorie pentru structura heapului
    THeap* h = (THeap*) malloc(sizeof(THeap));
    // verificam daca alocarea a avut succes
    if (!h) {
        return NULL;
    }

    // alocam memorie pentru vectorul de pointeri catre fisiere
    h->v = (TListaFisiere*) malloc(nrMax * sizeof(TListaFisiere));
    // verificam daca alocarea a avut succes
    if (!h->v) {
        free(h);
        return NULL;
    }
    /* initializam numarul maxim de elemente, numarul de
    elemente curente si functia de comparare*/
    h->nrMax = nrMax;
    h->nrElem = 0;
    h->comp = comp;
    return h;
}

void InsertHeap(THeap *h, TListaFisiere val) {
    // verificam daca heapul si vectorul de pointeri sunt valide
    if (h == NULL || h->v == NULL) {
        return;
    }
    /* daca heap-ul este plin, realocam vectorul de pointeri
    pentru a putea insera elementul nou */
    if (h->nrElem >= h->nrMax) {
        int dim_noua = h->nrMax * 2;  // crestem dimensiunea de 2 ori
        // alocam memorie pentru un vector mai mare de pointeri catre fisiere
        TListaFisiere *vector_nou = (TListaFisiere*)realloc(h->v, dim_noua * sizeof(TListaFisiere));
        // verificam daca alocarea a avut succes
        if (vector_nou == NULL) return;
        // actualizam pointerul la vectorul heap-ului
        h->v = vector_nou;
        // actualizam numarul maxim de elemente
        h->nrMax = dim_noua;
    }
    // inseram elementul in heap pe ultima pozitie
    int curent = h->nrElem;
    h->v[curent] = val;
    h->nrElem++; // crestem numarul de elemente din heap
    int parinte = (curent - 1) / 2;
    /* refacem proprietatea de heap prin urcarea elementului:
    cat timp elementul curent este mai prioritar decat parintele sau,
    cele doua elemente sunt interschimbate. Procesul continua pana
    cand elementul ajunge pe o pozitie corecta sau devine radacina */
    while (curent > 0 && h->comp( h->v[curent], h->v[parinte]) < 0) {
        //facem swap
        TListaFisiere aux = h->v[parinte];
        h->v[parinte] = h->v[curent];
        h->v[curent] = aux;
        curent = parinte;
        parinte = (curent - 1) / 2;
    }
}

TListaFisiere Extragere(THeap *h) {
    // Verificam daca heap-ul este valid si are elemente
    if ( h == NULL || h->v == NULL || h->nrElem == 0)
        return NULL;
    // Valoarea extrasa este intotdeauna radacina (cel mai relevant fisier)
    TListaFisiere rez =  h->v[0];
    // Aducem ultimul element in locul radacinii
    h->nrElem--;
    h->v[0] = h->v[h->nrElem];
    int curent = 0;
    while (1) {
        int index = curent;
        int left = 2 * curent + 1;
        int right = 2 * curent + 2;
        // h->comp trebuie sa returneze < 0
        // pentru ca elementul sa fie considerat mai prioritar (scor mai mare)
        if (left < h->nrElem && h->comp(h->v[left], h->v[index]) < 0) {
            index = left;
        }
        if (right < h->nrElem && h->comp(h->v[right], h->v[index]) < 0) {
            index = right;
        }
        if (index == curent) {
            break;
        }
        // Swap curent si cel mai bun fiu
        TListaFisiere aux = h->v[curent];
        h->v[curent] = h->v[index];
        h->v[index] = aux;

        curent = index;
    }
    return rez;
}