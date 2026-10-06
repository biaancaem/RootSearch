#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structuri.h"
#include "functiiStructuri.h"
#include "FunctiiHeap.h"
//adauga un fisier  in sistem
void ADD(FILE *fout, TListaFisiere *lista_fisiere, char idFisier[], int score, int nr_cuvinte, TListaCuvinte lista_cuvinte, TTrie arbore) {
    TListaFisiere fisier = AlocCelulaFisiere(idFisier, score, lista_cuvinte,nr_cuvinte);
    if (fisier == NULL) return;
    //verificam daca exista deja referinta catre acel fisier
    int ok = 0;
    for (TListaFisiere p = *lista_fisiere; p != NULL; p = p->urm) {
        if (strcmp(p->id, idFisier) == 0) {
            ok = 1;
            break;
        }
    }
    if (ok == 1) {
        fprintf(fout, "EXISTS\n");
        DistrugeListaCuvinte(&lista_cuvinte);
        free(fisier);
        return;
    }
    //se creeaza fisierul si se insereaza in lista de fisiere
    InserareListaFisiere(lista_fisiere, fisier);
    TListaCuvinte cuvant = lista_cuvinte;
    //pentru fiecare cuvant al fisierului, se insereaza cuvantul in arborele multicai de regasire
    while (cuvant != NULL) {
        InserareCuvantInArbore(arbore, cuvant->text,fisier);
        cuvant = cuvant->urm;
    }
    fprintf(fout, "OK\n");
}

void DEL(FILE *fout, TListaFisiere *lista_fisiere, char idFisier[], TTrie arbore) {
    //se identifica fisierul in lista de fisiere
    TListaFisiere fisier_de_sters = IdentificaFisier(lista_fisiere,idFisier);
    //fisierul nu e gasit
    if (fisier_de_sters == NULL) {
        fprintf(fout, "NOT FOUND\n" );
        return;
    }
    //fisierul e gasit, deci se elimina referintele lui din toate nodurile
    //terminale corespunzatoare cuvintelor sale
    EliminaReferinteLaFisier(arbore, fisier_de_sters);
    TListaCuvinte lista_cuvinte = fisier_de_sters->ListaCuvinte;
    while (lista_cuvinte != NULL) {
        TTrie nodTerminal = GasesteNodTerminalCuvant(lista_cuvinte->text, arbore);
        // Daca nodul nu mai are nicio referinta, trebuie sters efectiv din arbore
        if (nodTerminal != NULL && nodTerminal->nr_fisiere == 0) {
            // Se apeleaza functia de stergere recursiva
            StergereCuvantDinArbore(&arbore, lista_cuvinte->text);
        }
        lista_cuvinte = lista_cuvinte->urm;
    }
    DistrugeListaCuvinte(&(fisier_de_sters->ListaCuvinte));
    //se elimina fisierul din lista de fisiere
    EliminaFisierDinListaFisiere(lista_fisiere, fisier_de_sters);
    fprintf(fout,"OK\n");
}

void ADDKW(FILE *fout, TListaFisiere *lista_fisiere, char idFisier[], char cuvant[], TTrie arbore) {
    TListaFisiere fisier = IdentificaFisier(lista_fisiere, idFisier);
    if (fisier == NULL) {
        fprintf(fout, "NOT FOUND\n" );
        return;
    }
    TListaCuvinte lista_cuvinte = fisier->ListaCuvinte;
    TListaCuvinte ant = NULL;
    while (lista_cuvinte != NULL) {
        if (strcmp(lista_cuvinte->text,cuvant) == 0) {
            fprintf(fout, "OK\n");
            return;
        }
        ant = lista_cuvinte;
        lista_cuvinte = lista_cuvinte->urm;
    }
    //inseram in lista de cuvinte a fisierului noul cuvant
    TListaCuvinte aux = (TListaCuvinte)malloc(sizeof(TCelulaCuvant));
    if (aux == NULL) return;
    strcpy(aux->text,cuvant);
    aux->urm = NULL;
    if (ant == NULL) {
        fisier->ListaCuvinte = aux;
    } else {
        ant->urm = aux;
    }
    //crestem nr de cuvinte asociate fisierului
    fisier->nr_cuvinte++;
    // inseram noul cuvant in Trie si asociem fisierul cu nodul terminal
    InserareCuvantInArbore(arbore,cuvant, fisier);
    fprintf(fout, "OK\n");
}

void DELKW(FILE *fout,TListaFisiere *lista_fisiere,char idFisier[],char cuvant[], TTrie arbore) {
    TListaFisiere fisier = IdentificaFisier(lista_fisiere,idFisier);
    if (fisier == NULL) {
        fprintf(fout, "NOT FOUND\n" );
        return;
    }

    TListaCuvinte lista_cuvinte = fisier->ListaCuvinte;
    TListaCuvinte ant = NULL;
    while (lista_cuvinte != NULL) {
        if (strcmp(lista_cuvinte->text,cuvant) == 0) {
            break;
        }
        ant = lista_cuvinte;
        lista_cuvinte = lista_cuvinte->urm;
    }
    //nu am gasit cuvantul in lista de cuvinte
    if (lista_cuvinte == NULL) {
        fprintf(fout,"OK\n");
        return;
    }
    //eliminam cuvantul din lista de cuvinte a fisierului
    if (ant == NULL) {
        fisier->ListaCuvinte = lista_cuvinte->urm;
    } else {
        ant->urm = lista_cuvinte->urm;
    }
    free(lista_cuvinte);
    fisier->nr_cuvinte--;
    //eliminam referinta din nodul terminal
    EliminaReferintaDinNodTerminal(cuvant,fisier,&arbore);
    // Cautam nodul sa vedem cate fisiere mai are acum
    TTrie nodTerminal = GasesteNodTerminalCuvant(cuvant, arbore);
    //cuvantul se sterge doar daca nu mai are referinte
    if (nodTerminal != NULL && nodTerminal->nr_fisiere == 0)
        StergereCuvantDinArbore(&arbore, cuvant);
    fprintf(fout,"OK\n");
}

void FIND(FILE *fout, char cuvant[], TTrie arbore, TListaFisiere *lista_fisiere) {
    TTrie nod = GasesteNodTerminalCuvant(cuvant,arbore);
    //Daca nodul nu exista sau nu este marcat ca final de cuvant,
    //inseamna ca nu exista fisiere asociate acelui cuvant
    if (nod == NULL || nod->final_cuvant == 0) {
        fprintf(fout, "EMPTY\n");
        return;
    }
    //daca nodul e valid
    TListaReferinte fisiere_asociate = nod->fisiere_asociate;
    fprintf(fout,"%d", nod->nr_fisiere);
    while (fisiere_asociate != NULL) {
        fprintf(fout, " %s", fisiere_asociate->fisier->id);
        fisiere_asociate = fisiere_asociate->urm;
    }
    fprintf(fout, "\n");
      //verificare daca mai sunt fisiere care nu mai au cuvinte asociate
    TListaFisiere p = *lista_fisiere;
    TListaFisiere ultim;
    while (p != NULL) {
        ultim = p->urm;
        if (p->ListaCuvinte == NULL) {
            EliminaFisierDinListaFisiere(lista_fisiere,p);
        }
        p = ultim;
    }
}

void TOPK(FILE *fout, char cuvant[],int k, TTrie arbore) {
    TTrie nod = GasesteNodTerminalCuvant(cuvant,arbore);
    if (nod == NULL || nod->nr_fisiere == 0 || nod->final_cuvant == 0) {
        fprintf(fout,"EMPTY\n");
        return;
    }
    TListaReferinte fisiere_asociate = nod->fisiere_asociate;
    THeap *heap = AlocaHeap(nod->nr_fisiere,comp);
    if (heap == NULL) {
        return;
    }
    while (fisiere_asociate != NULL) {
        //inseram in heap pointerul catre fisier
        InsertHeap(heap, fisiere_asociate->fisier);
        fisiere_asociate = fisiere_asociate->urm;
    }
    //daca in heap nu sunt destule elemente
    if (k > heap->nrElem)
        k = heap->nrElem;
    fprintf(fout, "%d", k);
    while (k){
        TListaFisiere fisier_extras = Extragere(heap);
        fprintf(fout, " %s", fisier_extras->id);
        k--;
    }
    fprintf(fout, "\n");
    // Eliberam memoria heap-ului
    free(heap->v);
    free(heap);
}

void PRINT(FILE *fout, TTrie arbore, char prefix[], int prefix_len) {
    if (arbore == NULL) return;
    //verificam daca nodul marcheza un final de cuvant
    if (arbore->final_cuvant == 1) {
        // inchidem sirul pentru a putea fi printat
        prefix[prefix_len] = '\0';
        // Afisam cuvantul si numarul de fisiere
        fprintf(fout, "%s %d", prefix, arbore->nr_fisiere);
        //Afisam id-ul fisierelor
        TListaReferinte fisiere_asociate = arbore->fisiere_asociate;
        while (fisiere_asociate != NULL) {
            fprintf(fout, " %s", fisiere_asociate->fisier->id);
            fisiere_asociate = fisiere_asociate->urm;
        }
        fprintf(fout, "\n");
    }
    // Traversam copiii in ordine alfabetica
    for (int i = 0; i < 26; i++) {
        if (arbore->fii[i] != NULL) {
            // Reconstruim cuvantul, adaugam caracterul curent
            prefix[prefix_len] = i + 'a';

            // Apel recursiv pentru nivelul urmator
            PRINT(fout,arbore->fii[i], prefix, prefix_len + 1);
        }
    }
}

void PREFIX(FILE *fout,TTrie arbore, char prefix[], int prefix_len) {
    //gasim nodul unde se termina prefixul
    TTrie nod = GasesteNodTerminalCuvant(prefix,arbore);
    if (nod == NULL) {
        fprintf(fout, "EMPTY\n");
        return;
    }
    TListaReferinte cuvinte_colectate = NULL;
    int nr_fisiere = 0;
    ColecteazaReferinteFisiere(nod, &cuvinte_colectate, &nr_fisiere);
    if (nr_fisiere == 0) {
        DistrugeListaReferinte(&cuvinte_colectate);
        fprintf(fout, "EMPTY\n");
        return;
    }
    TListaFisiere *vectorRez = (TListaFisiere*)malloc(nr_fisiere * sizeof(TListaFisiere));
    if (!vectorRez) {
        DistrugeListaReferinte(&cuvinte_colectate);
        return;
    }
    for (int i = 0; i < nr_fisiere; i++) {
        vectorRez[i] = cuvinte_colectate->fisier;
        TListaReferinte aux = cuvinte_colectate;
        cuvinte_colectate = cuvinte_colectate->urm;
        free(aux); // Curatam celula listei imediat ce am mutat-o
    }
    qsort(vectorRez, nr_fisiere, sizeof(TListaFisiere), cmpLexicograficaFisiere);
    //Afisam rezultatele fara duplicate
    int nr = 1;// pentru ca am stabilit deja ca avem fisiere
    for (int i = 1; i < nr_fisiere; i++) {
        if (strcmp(vectorRez[i]->id, vectorRez[i - 1]->id) != 0) {
            nr++;
        }
    }
    fprintf(fout, "%d", nr); //nr fisiere distince
    fprintf(fout, " %s", vectorRez[0]->id);
    for (int i = 1; i < nr_fisiere; i++) {
        // Verificam daca ID-ul curent este diferit de cel anterior
        if (strcmp(vectorRez[i]->id, vectorRez[i - 1]->id) != 0) {
            fprintf(fout, " %s", vectorRez[i]->id);
        }
    }
    fprintf(fout, "\n");
    free(vectorRez);
}