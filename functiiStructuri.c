
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structuri.h"

TListaFisiere InitListaFisiere() {
    return NULL;
}

TListaCuvinte InitListaCuvinte() {
    return NULL;
}

TListaFisiere AlocCelulaFisiere(char id[], int scor, TListaCuvinte lista,int nr_cuvinte) {
    TListaFisiere aux = (TListaFisiere)malloc(sizeof(TCelula));
    if (aux == NULL) return NULL;
    strcpy(aux->id, id);
    aux->scor = scor;
    aux->ListaCuvinte = lista;
    aux->nr_cuvinte = nr_cuvinte;
    aux->urm = NULL;
    aux->pre = NULL;
    return aux;
}

TListaCuvinte AlocCelulaCuvinte(char cuvant[]) {
    TListaCuvinte aux = (TListaCuvinte)malloc(sizeof(TCelulaCuvant));
    if (aux == NULL) return NULL;
    strcpy(aux->text, cuvant);
    aux->text[sizeof(aux->text) - 1] = '\0';
    aux->urm = NULL;
    return aux;
}

//functie pentru inserarea unui fisier nou in lista dublu inlantuita de fisiere
int InserareListaFisiere(TListaFisiere *listaFisiere, TListaFisiere fisier_nou) {
    if (*listaFisiere == NULL) {
        *listaFisiere = fisier_nou;
        return 1;
    }
    TListaFisiere p = *listaFisiere;
    //parcurgem lista pana la ultimul element
    while (p->urm != NULL) {
        p = p->urm;
    }
    p->urm = fisier_nou; // Legam noul nod la finalul listei
    fisier_nou->pre = p; // refacem legatura inapoi
    return 1;
}

//functie pentru eliminarea fisierului din lista de fisiere
void EliminaFisierDinListaFisiere(TListaFisiere *listaFisiere, TListaFisiere fisier_de_eliminat) {
    if (fisier_de_eliminat->pre == NULL) {
        *listaFisiere = (*listaFisiere)->urm;
    } else {
        fisier_de_eliminat->pre->urm = fisier_de_eliminat->urm;
    }
    if (fisier_de_eliminat->urm != NULL) { //daca fisierul eliminat nu e ultimul in lista
        fisier_de_eliminat->urm->pre = fisier_de_eliminat->pre; // ii actualizam legatura
    }
    free(fisier_de_eliminat);
}

//functie care returneaza un pointer la fisierul cu indexul specificat sau NULL daca fisierul nu se afla in lista
TListaFisiere IdentificaFisier(TListaFisiere *listaFisiere, char index[]) {
    TListaFisiere p = *listaFisiere,aux = NULL;
    while (p != NULL) {
        if ( strcmp(p->id,index) == 0) {
            aux = p;
        }
        p = p->urm;
    }
    return aux;
}

//functie pentru eliminarea legaturii dintre un cuvant din trie care se termina cu litera de la nod_terminator_de_cuvant
// si fisierul_de_sters
void DeconecteazaFisierDinNod(TTrie nod_terminator_de_cuvant, TListaFisiere fisier_de_sters) {
    TListaReferinte p = nod_terminator_de_cuvant->fisiere_asociate;
    TListaReferinte ant= NULL;
    while (p != NULL) {
        //daca fisierul din lista de fisiere asociate cuvantului este egal cu fisierul de sters
        if (p->fisier == fisier_de_sters)
            break;
        ant = p;
        p = p->urm;
    }
    if (p == NULL) return; //daca fiserul nu a fost gasit iesim
    if (ant == NULL)
        nod_terminator_de_cuvant->fisiere_asociate = p->urm;
    else
        ant->urm = p->urm;
    free(p);
    nod_terminator_de_cuvant->nr_fisiere--; //scandem nr de fisiere care au legatura cu acel cuvant
    //daca nodul nu mai are fisiere asociate nodul nu mai trebuie sa fie marcat ca fiind final de cuvant
    if (nod_terminator_de_cuvant->nr_fisiere == 0) {
        nod_terminator_de_cuvant->final_cuvant = 0;
    }
}

// functie care parcurge fiecare cuvant din lista de cuvinte a fisierului si
// elimina legatura acestuia din arborele
void EliminaReferinteLaFisier(TTrie radacina, TListaFisiere fisier) {
    //extragem lista de cuvinte a fisierului
    TListaCuvinte lista_cuvinte = fisier->ListaCuvinte;
    //cat timp lista de cuvinte nu e goala
    while (lista_cuvinte != NULL) {
        // Navigam in Trie litera cu litera pentru cuvantul curent
        TTrie p = radacina;
        char *text = lista_cuvinte->text; //text o sa fie un pointer spre adresa de inceput a primului cuvant
        //parcurgem cuvantul cat timp litera nu e terminatorul de sir
        for (int i = 0; text[i] != '\0'; i++) {
            int index = text[i] - 'a'; //indexul este adresa literei urmatoare
            p = p->fii[index];
        }
        // Daca am gasit nodul unde se termina cuvantul si este nod terminator de cuvant
        if (p != NULL && p->final_cuvant) {
            DeconecteazaFisierDinNod(p, fisier);
        }
        lista_cuvinte = lista_cuvinte->urm; // Trecem la urmatorul cuvant din fisier
    }

 }

int ExistaCuvantInLista(TListaCuvinte lista, char cuvant[]) {
    while (lista != NULL) {
        if (strcmp(lista->text, cuvant) == 0) {
            return 1;
        }
        lista = lista->urm;
    }
    return 0;
}
//functie citire cuvinte din fisier
TListaCuvinte CitireL(int nr,FILE *file,int *nr_cuv_unice) {
    TListaCuvinte L = NULL,aux,ultim = NULL;
    char cuvant[101];
    for (int i = 0; i < nr; i++) {
        fscanf(file, "%s", cuvant);
        if (ExistaCuvantInLista(L, cuvant)) {
            continue;
        }
        aux = AlocCelulaCuvinte(cuvant);
        if (aux == NULL) return L;
        if (ultim == NULL) L = aux;
        else
            ultim->urm = aux;
        ultim = aux;
        (*nr_cuv_unice)++;
    }
    return L;
}

//functie pentru initializarea arborelui de regasire
TTrie InitTrie() {
    TTrie t = (TTrie)malloc(sizeof(TNod));
    if (t == NULL) return NULL;
    for (int i = 0; i < 26; i++) {
        t->fii[i] = NULL;
    }
    t->fisiere_asociate = NULL;
    t->nr_fisiere = 0;
    t->final_cuvant = 0;
    return t;
}
//functie inserare in trie
//cuvant reprezinta restul cuvantului initial "cuvant" care urmeaza sa fie procesat
void InserareCuvantInArbore(TTrie radacina,char *cuvant,TListaFisiere referinta_fisier) {
    //daca am ajuns la litera finala a cuvantului
    if (cuvant[0] == '\0') {
        // Verificam daca nu cumva am adaugat deja acest fisier pentru acest cuvant
        TListaReferinte curr = radacina->fisiere_asociate;
        TListaReferinte ant = NULL;
        // Verificam duplicatele si cautam pozitia alfabetica in acelasi timp
        while (curr != NULL) {
            if (curr->fisier == referinta_fisier) return; // Fisierul e deja in lista
            // Daca ID-ul curent e mai mare alfabetic, am gasit locul unde sa inseram
            //Funcția compara ID-ul fisierului curent cu cel pe care vrei sa il inserezi.
            //Daca cel din lista este mai mare din punct de vedere alfabetic  bucla se oprește
            //locul de inserat: noul nod trebuie inserat între ant și curr.
            if (strcmp(curr->fisier->id, referinta_fisier->id) > 0) {
                break;
            }
            ant = curr;
            curr = curr->urm;
        }
        //cream o celula de referinta la fisier
        TListaReferinte noua_ref = (TListaReferinte)malloc(sizeof(TCelulaReferinte));
        if (noua_ref == NULL) return;
        noua_ref->fisier = referinta_fisier;
        noua_ref->urm = curr; // Noul nod pointeaza catre nodul mai mare (curr)
        if (ant == NULL) {
            // Inserare la inceputul listei (noul element este cel mai mic alfabetic)
            radacina->fisiere_asociate = noua_ref;
        } else {
            // Inserare la mijloc sau la final (legam dupa nodul anterior)
            ant->urm = noua_ref;
        }
        radacina->final_cuvant = 1; //marcam nodul ca fiind final de cuvant
        radacina->nr_fisiere++;
        return;
    }
    //calculam indexul corespunzator literei curente
    int index = cuvant[0] - 'a';
    TTrie next = radacina->fii[index];
    //daca nu exista deja litera respectiva, cream un nod nou
    if (next == NULL) {
        radacina->fii[index] = InitTrie();
        next = radacina->fii[index];
    }
    // Trecem la urmatorul nod, trimitand restul cuvantului
    InserareCuvantInArbore(next,cuvant + 1 ,referinta_fisier);
}

int StergereCuvantDinArbore(TTrie *t, char *cuvant) {
    TTrie nod = *t;
    if (nod == NULL) return 0; // Cuvantul nu exista
    // Am ajuns la finalul cuvantului
    if (cuvant[0] == '\0') {
        if (nod->final_cuvant) {
            // eliberam lista de referinte catre fisiere
            TListaReferinte curr = nod->fisiere_asociate;
            while (curr != NULL) {
                TListaReferinte aux = curr;
                curr = curr->urm;
                free(aux);
            }
            nod->fisiere_asociate = NULL;
            nod->nr_fisiere = 0;
            nod->final_cuvant = 0;
        }
        // Verificam daca nodul poate fi sters adica nu are copii
        for (int i = 0; i < 26; i++)
            if (nod->fii[i] != NULL) return 0; //nodul nu poate fi sters pt ca mai are copii
        return 1; //nodul poate fi sters
    }
    //Navigam catre "nextNode" corespunzator primei litere
    //calculam indexul literei curente
    int idx = cuvant[0] - 'a';
    //verificam daca exista drum pana acolo
    if (nod->fii[idx] != NULL) {
        //apelam recursiv pe restul cuvantului cuvant+1
        // Daca apelul recursiv returneaza true, nodul poate fi sters
        if (StergereCuvantDinArbore(&(nod->fii[idx]), cuvant + 1) == 1) {
            free(nod->fii[idx]);
            nod->fii[idx] = NULL;
            // Dupa stergere, verificam daca si nodul curent a devenit inutil
            //sa nu fie sf de cuvant pt altcineva
            if (nod->final_cuvant == 0) {
                //sa nu aibe alti copii
                for (int i = 0; i < 26; i++)
                    if (nod->fii[i] != NULL) return 0;
                return 1;
            }
        }
    }
    return 0; // return false
}
void EliminaReferintaDinNodTerminal(char *cuvant,TListaFisiere fisier,TTrie *t) {
    TTrie p = *t;
    for (int i = 0; cuvant[i] != '\0'; i++) {
        int index = cuvant[i] - 'a'; //indexul este adresa literei urmatoare
        p = p->fii[index];
    }
    // Daca am gasit nodul unde se termina cuvantul si este nod terminator de cuvant
    if (p != NULL && p->final_cuvant) {
        DeconecteazaFisierDinNod(p, fisier);
    }
}

TTrie GasesteNodTerminalCuvant(char *cuvant, TTrie radacina) {
    // daca nodul curent este NULL
    // inseamna ca drumul cautat nu exista in arbore
    if (radacina == NULL) {
        return NULL;
    }
    if (cuvant[0] == '\0') {
        return radacina;
    }
    //calculam indexul corespunzator literei curente
    int index = cuvant[0] - 'a';
    TTrie next = radacina->fii[index];
    // Adaugam return pentru a trimite rezultatul inapoi prin stiva de apeluri
    return GasesteNodTerminalCuvant(cuvant + 1,next);
}


int cmpLexicograficaFisiere (const void* a, const void* b) {
    TListaFisiere ref1 = *(TListaFisiere*)a;
    TListaFisiere ref2 = *(TListaFisiere*)b;
    return strcmp(ref1->id, ref2->id);
}
//colecteaza referintele la fisiere plecand dintr un anumit nod
void ColecteazaReferinteFisiere(TTrie nod, TListaReferinte *lista_colectare, int *nr_fisiere) {
    if (nod == NULL) return;
    // Daca nodul curent este terminal, ii copiem referintele
    if (nod->final_cuvant == 1) {
        TListaReferinte p = nod->fisiere_asociate;
        while (p != NULL) {
            // Alocam o celula noua pentru lista noastra de colectare
            TListaReferinte nou = (TListaReferinte)malloc(sizeof(TCelulaReferinte));
            if (!nou) return; // Verificare alocare

            nou->fisier = p->fisier; // Copiem pointerul catre fisier
            nou->urm = *lista_colectare; // Inseram la inceputul listei mari
            *lista_colectare = nou;
            p = p->urm;
        }
        (*nr_fisiere) = (*nr_fisiere) + nod->nr_fisiere; //nr de referinte creste cu cate referinte la fisiere aveam
    }

    // Vizitam toti copiii
    for (int i = 0; i < 26; i++) {
        //daca nodul are ca succesor una din litere, apelam functia pe acel nod
        if (nod->fii[i] != NULL) {
            ColecteazaReferinteFisiere(nod->fii[i], lista_colectare, nr_fisiere);
        }
    }
}

void DistrugeListaCuvinte(TListaCuvinte *lista) {
    TListaCuvinte p = *lista;
    while (p != NULL) {
        TListaCuvinte aux = p;
        p = p->urm;
        free(aux);
    }
    *lista = NULL;
}

void DistrugeListaReferinte(TListaReferinte *lista) {
    TListaReferinte p = *lista;
    while (p != NULL) {
        TListaReferinte aux = p;
        p = p->urm;
        free(aux);
    }
    *lista = NULL;
}

void DistrugeTrie(TTrie *t) {
    if (t == NULL || *t == NULL) {
        return;
    }
    for (int i = 0; i < 26; i++) {
        DistrugeTrie(&((*t)->fii[i]));
    }
    DistrugeListaReferinte(&((*t)->fisiere_asociate));
    free(*t);
    *t = NULL;
}

void DistrugeListaFisiere(TListaFisiere *lista) {
    TListaFisiere p = *lista;
    while (p != NULL) {
        TListaFisiere aux = p;
        p = p->urm;
        DistrugeListaCuvinte(&(aux->ListaCuvinte));
        free(aux);
    }
    *lista = NULL;
}
