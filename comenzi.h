
#ifndef TEMA2_SDA_COMENZI_H
#define TEMA2_SDA_COMENZI_H
void ADD(FILE *fout, TListaFisiere *lista_fisiere, char idFisier[], int score, int nr_cuvinte, TListaCuvinte lista_cuvinte, TTrie arbore);
void DEL(FILE *fout, TListaFisiere *lista_fisiere, char idFisier[], TTrie arbore);
void ADDKW(FILE *fout,TListaFisiere *lista_fisiere,char idFisier[],char cuvant[], TTrie arbore);
void DELKW(FILE *fout,TListaFisiere *lista_fisiere,char idFisier[],char cuvant[], TTrie arbore);
void FIND(FILE *fout, char cuvant[], TTrie arbore, TListaFisiere *lista_fisiere);
void TOPK(FILE *fout, char cuvant[],int k, TTrie arbore);
void PRINT(FILE *fout, TTrie arbore, char prefix[], int prefix_len);
void PREFIX(FILE *fout,TTrie arbore, char prefix[], int prefix_len);
#endif //TEMA2_SDA_COMENZI_H

