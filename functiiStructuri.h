
#ifndef TEMA2_SDA_FUNCTIISTRUCTURI_H
#define TEMA2_SDA_FUNCTIISTRUCTURI_H
TListaFisiere InitListaFisiere();
TListaCuvinte InitListaCuvinte();
TListaFisiere AlocCelulaFisiere(char id[], int scor, TListaCuvinte lista,int nr_cuvinte);
TListaCuvinte AlocCelulaCuvinte(char cuvant[]);
int InserareListaFisiere(TListaFisiere *listaFisiere, TListaFisiere fisier_nou);
void EliminaFisierDinListaFisiere(TListaFisiere *listaFisiere, TListaFisiere fisier_de_eliminat);
TListaFisiere IdentificaFisier(TListaFisiere *listaFisiere, char index[]);
void DeconecteazaFisierDinNod(TTrie nod_terminator_de_cuvant, TListaFisiere fisier_de_sters);
int EliminaReferinteLaFisier(TTrie radacina, TListaFisiere fisier);
TListaCuvinte CitireL(int nr,FILE *file,int *nr_cuv_unice);
TTrie InitTrie();
void InserareCuvantInArbore(TTrie radacina,char *cuvant,TListaFisiere referinta_fisier);
int StergereCuvantDinArbore(TTrie *t, char *cuvant);
void EliminaReferintaDinNodTerminal(char *cuvant,TListaFisiere fisier,TTrie *t);
TTrie GasesteNodTerminalCuvant(char *cuvant, TTrie radacina);
int cmpLexicograficaFisiere (const void* a, const void* b);
void ColecteazaReferinteFisiere(TTrie nod, TListaReferinte *lista_colectare, int *nr_fisiere);
void DistrugeListaCuvinte(TListaCuvinte *lista);
void DistrugeListaReferinte(TListaReferinte *lista);
void DistrugeTrie(TTrie *t);
void DistrugeListaFisiere(TListaFisiere *lista);
#endif //TEMA2_SDA_FUNCTIISTRUCTURI_H
