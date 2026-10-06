#include <stdio.h>
#include <string.h>
#include "structuri.h"
#include "functiiStructuri.h"
#include "comenzi.h"
int main(int argc, char *argv[]) {
    const char *input_path = (argc > 1) ? argv[1] : "indexare.in";
    const char *output_path = (argc > 2) ? argv[2] : "indexare.out";

    FILE *fin = fopen(input_path, "r");
    if (!fin) {
        return 1;
    }

    FILE *fout = fopen(output_path, "w");
    if (!fout) {
        fclose(fin);
        return 1;
    }
    int nr_comenzi;
    fscanf(fin,"%d",&nr_comenzi); //nr comenzi citite
    char comanda[30];
    TTrie arbore = InitTrie();
    TListaFisiere ListaFisiere = InitListaFisiere();
    for (int i = 0; i < nr_comenzi; i++) {
        fscanf(fin, "%s", comanda);
        if (strcmp(comanda, "ADD") == 0) {
            char FileName[101];
            int score,nr_cuvinte;
            fscanf(fin,"%s", FileName);
            fscanf(fin,"%d",&score);
            fscanf(fin,"%d",&nr_cuvinte);
            int nr_cuv_unice = 0;
            TListaCuvinte lista_cuvinte = CitireL(nr_cuvinte,fin,&nr_cuv_unice);
            ADD(fout,&ListaFisiere, FileName, score, nr_cuv_unice, lista_cuvinte,arbore);
        } else if (strcmp(comanda, "DEL") == 0) {
            char FileName[101];
            fscanf(fin,"%s", FileName);
            DEL(fout,&ListaFisiere, FileName,arbore);
        } else if (strcmp(comanda, "ADDKW") == 0) {
            char FileName[101];
            char cuvant[101];
            fscanf(fin,"%s",FileName);
            fscanf(fin,"%s",cuvant);
            ADDKW(fout,&ListaFisiere,FileName,cuvant,arbore);
        } else if (strcmp(comanda,"DELKW") == 0) {
            char FileName[101];
            char cuvant[101];
            fscanf(fin,"%s",FileName);
            fscanf(fin,"%s",cuvant);
            DELKW(fout,&ListaFisiere,FileName,cuvant,arbore);
        } else if (strcmp(comanda, "FIND") == 0) {
            char cuvant[101];
            fscanf(fin,"%s",cuvant);
            FIND(fout,cuvant, arbore, &ListaFisiere);
        } else if (strcmp(comanda, "TOPK") == 0) {
            char cuvant[101];
            fscanf(fin,"%s",cuvant);
            int k;
            fscanf(fin,"%d",&k);
            TOPK(fout,cuvant,k,arbore);
        } else if (strcmp(comanda, "PRINT") == 0) {
            char prefix[101];
            int prefix_len = 0;
            // Verificam daca arborele are macar un fiu sau daca radacina insasi e terminala
            int arbore_gol = 1;
            for (int j = 0; j < 26; j++) {
                if (arbore->fii[j] != NULL) {
                    arbore_gol = 0;
                    break;
                }
            }
            // Daca arborele nu are niciun cuvant, afisam EMPTY
            if (arbore_gol && arbore->final_cuvant == 0) {
                fprintf(fout, "EMPTY\n");
            } else {
                PRINT(fout, arbore, prefix, prefix_len);
            }
        } else if (strcmp(comanda, "PREFIX") == 0) {
            char prefix[101];
            fscanf(fin,"%s",prefix);
            PREFIX(fout,arbore,prefix,strlen(prefix));
        }
    }
    DistrugeListaFisiere(&ListaFisiere);
    DistrugeTrie(&arbore);

    fclose(fin);
    fclose(fout);
    return 0;
}