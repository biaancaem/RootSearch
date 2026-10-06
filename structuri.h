
#ifndef TEMA2_SDA_STRUCTURI_H
#define TEMA2_SDA_STRUCTURI_H

typedef struct cuvant {
    char text[101]; // Cuvantul
    struct cuvant *urm;
} TCelulaCuvant, *TListaCuvinte;

typedef struct celula {
    char id[101]; //id-ul unic al fisierului
    int scor; //scorul fisierului
    TListaCuvinte ListaCuvinte; //lista de cuvinte
    int nr_cuvinte; // nr de cuvinte asociate fisierului
    struct celula *urm, *pre;
}TCelula, *TListaFisiere;

typedef struct celRef {
    TListaFisiere fisier;     // Pointer catre nodul din lista mare de fisiere
    struct celRef *urm;
} TCelulaReferinte, *TListaReferinte;

typedef struct nodTrie {
    struct nodTrie *fii[26];    // Vector de pointeri pentru literele a-z
    TListaReferinte fisiere_asociate;    // Capul listi care contine adresele catre fisierele din lista principala de fisiere
    int nr_fisiere; //reprezinta nr fisiere care sunt asociate cuvantului care se termina in nodul respectiv
    int final_cuvant; //flag pentru a vedea daca a drumul parcurs de la radacina pana la acest nod formeaza un cuvant
} TNod, *TTrie;

/* structura unui heap, care contine numarul maxim de
elemente, numarul de elemente curente, un vector de pointeri
la lista de fisiere si o functie de comparare(pentru minHeap sau maxHeap) */
typedef int (*TFCmp)(void*, void*);
typedef struct Heap {
    int nrMax, nrElem; //nrMax = capacitate maxima vector v
    //nrElem = nr curent de elemente in heap
    TListaFisiere *v; //Vector de pointeri catre fisierele din lista
    TFCmp comp; //adresa functiei de comparare
} THeap;

#endif //TEMA2_SDA_STRUCTURI_H
