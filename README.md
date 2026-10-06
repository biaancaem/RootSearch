# Sistem simplificat de indexare al fisierelor
Am impartit implementarea in mai multe fisiere pentru a separa mai clar structurile de date, functiile auxiliare si comenzile principale ale programului.
Fisierul `structuri.h` contine definitiile structurilor folosite in tema: listele de fisiere, listele de cuvinte, listele de referinte, arborele Trie si heap-ul.
Fisierele header `functiiStructuri.h`, `FunctiiHeap.h` si `comenzi.h` contin antetele functiilor implementate in fisierele sursa corespunzatoare.
Fisierul `functiiStructuri.c` contine functiile de baza pentru alocarea, initializarea si modificarea structurilor de date, precum si functiile folosite pentru lucrul cu arborele Trie.
Fisierul `FunctiiHeap.c` contine implementarea heap-ului folosit pentru comanda `TOPK`, unde fisierele sunt ordonate dupa scor si, in caz de egalitate, dupa id.
Fisierul `comenzi.c` contine implementarea comenzilor cerute in tema: `ADD`, `DEL`, `ADDKW`, `DELKW`, `FIND`, `TOPK`, `PRINT` si `PREFIX`.
Fisierul `main.c` se ocupa de citirea comenzilor din fisierul `indexare.in`, apelarea functiilor corespunzatoare si scrierea rezultatelor in `indexare.out`.

## Structurile de date

Pentru implementarea sistemului de indexare am folosit urmatoarele structuri:
1. TListaCuvinte - Este o lista simplu inlantuita atasata fiecarui fisier care retine cuvintele asociate.
1. TListaFisiere - Reprezinta structura principala pentru stocarea fisierelor, implementata sub forma unei liste dublu inlantuite. Fiecare nod retine: id-ul fisierului, un scor de relevanta, un nr de cuvinte asociate si lista propriu-zisa de cuvinte asociate.
1. TListaReferinte - Este o lista simplu inlantuita de pointeri care fac legatura intre nodurile terminale ale arborelui si nodurile corespunzatoare din TListaFisiere.
1. TTrie - Reprezinta un arbore multicai de regasire folosit pentru indexarea cuvintelor-cheie. Fiecare nod contine un vector de 26 de pointeri, o lista de referinte catre fisiere, un flag pentru final de cuvant si numarul de fisiere asociate cuvantului curent.
1. THeap - Un Max-Heap implementat cu un vector dinamic de pointeri catre elementele din TListaFisiere. Se utilizeaza o functie de comparare (comp) pentru a ordona elementele prioritar dupa scor (descrescator) si secundar dupa ID (lexicografic).
---

# Fisierul functiiStructuri.c
Contine functiile ajutatoare pentru implementarea comenzilor propriu-zise.

## `AlocCelulaFisiere`
Aloc dinamic o structura `TCelula` si ii copiez campurile primite ca parametri: `id`, `scor`, `ListaCuvinte` si `nr_cuvinte`. Pointerii urm si pre sunt initializati la NULL. Daca alocarea esueaza, se returneaza NULL.


## `AlocCelulaCuvinte`
Aloc o structura `TCelulaCuvant`, copiez cuvantul primit in `text` si setez ultimul caracter la `'\0'` pentru siguranta. Pointerul `urm` este initializat la `NULL`.


## `InserareListaFisiere`
Daca lista este goala, noul fisier devine capul listei.
Altfel, parcurg lista pana la ultimul element si leg noul nod la final, dupa care actualizez legatura inapoi.

## `EliminaFisierDinListaFisiere`
Functia primeste ca parametru lista de fisiere si referinta la fisierul de eliminat.
Daca fisierul de eliminat este capul listei, actualizez capul listai.
Altfel, refac legatura inainte a predecesorului.
Daca fisierul nu este ultimul, refac legatura inapoi.
La final eliberez memoria celulei cu free. Nu eliberez lista de cuvinte separat aici, aceasta este gestionata in alte functii.

## `IdentificaFisier`
Parcurg lista de fisiere si compar p->id cu indexul primit si returnez referinta.

## `DeconecteazaFisierDinNod`
Functia primeste ca parametri un nod terminal din trie si o referinta la un fisier care trebuie sters.
Extrage lista de fisiere din nodul triei si cauta fisierul_de_sters pastrand si legatura inapoi(ant). Daca fisierul nu e gasit iesim, daca ant a ramas null inseamna ca fisierul este primul element din lista, altfel refacem legatura in lista.
Scadem numarul de fisiere asociate nodului, iar daca nr de fisiere ajunge la 0, nodul nu mai este nod final_cuvant.

## `EliminaReferinteLaFisier`
Functia primeste ca parametru arborele si un fisier.
Se extrage lista de cuvinte asociata fisierului, apoi o parcurgem pana nu mai sunt cuvinte.
Cautam fiecare cuvant in trie, verificam la final daca nodul corespunzator ultimei litere exista si este final de cuvant,daca da apelam functia DeconecteazaFisierDinNod cu nodul din trie p care reprezinta ultimul nod dintr-un cuvant si fisierul.

## `ExistaCuvantInLista`
Functia parcurge lista de cuvinte a unui fisier si verifica daca un anumit cuvant exista deja in lista. Este folosita in `CitireL`, pentru a evita inserarea duplicatelor atunci cand la comanda `ADD` acelasi cuvant apare de mai multe ori.

## `CitireL`
Functia primeste numarul de cuvinte citite din fisier, fisierul de input si un pointer catre numarul de cuvinte unice.
Pentru fiecare cuvant citit, se verifica mai intai daca acesta exista deja in lista folosind `ExistaCuvantInLista`. Daca exista deja, cuvantul este ignorat, deoarece in lista de cuvinte-cheie a unui fisier nu trebuie sa existe duplicate.
Daca nu exista deja, se aloca o celula noua cu `AlocCelulaCuvinte`, se insereaza la finalul listei si se incrementeaza numarul de cuvinte unice prin pointerul primit ca parametru.
Functia returneaza lista de cuvinte fara duplicate.

## `InitTrie`
Aloc dinamic o structura TNod si initializez toti cei 26 de pointeri fii la NULL intr-un for. Setez fisiere_asociate = NULL, nr_fisiere = 0 si final_cuvant = 0.
Functia este apelata atat in `main` pentru crearea radacinii, cat si in InserareCuvantInArbore de fiecare data cand trebuie creat un nod nou pe un drum inexistent in arbore.

## `InserareCuvantInArbore`
Am implementat inserarea recursiv, primind ca parametri radacina curenta, restul cuvantului de procesat si referinta la fisier.
La fiecare apel calculez indexul corespunzator literei curente (index = cuvant[0] - 'a') si verific daca radacina->fii[index] exista. In cazul in care nu exista, creez nodul prin functia InitTrie, apoi apelez recursiv functia trimitand restul sirului (cuvant + 1) si nodul copil nou creat.
Cand am ajuns la finalul cuvantului (cuvant[0] == '\0'), marchez nodul ca fiind terminal prin setarea flag-ului final_cuvant = 1. Inainte de adaugare, parcurg lista fisiere_asociate pentru a verifica daca referinta la fisier exista deja, asigurand astfel unicitatea perechii cuvant-fisier.
Daca referinta nu exista, aloc o noua celula de tip TCelulaReferinte si o inserez in lista de referinte a nodului terminal.
Referinta noua nu este inserata simplu la inceputul listei, ci pe pozitia potrivita astfel incat lista `fisiere_asociate` sa ramana ordonata lexicografic dupa id-ul fisierului.

## `StergereCuvantDinArbore`
Am implementat stergerea unui cuvant din arborele Trie intr-un mod recursiv, pentru a asigura eliminarea nodurilor care devin inutile.
Functia parcurge arborele pana la capatul cuvantului, unde marcheaza nodul ca fiind ne-terminal prin setarea final_cuvant = 0 si elibereaza intreaga lista de referinte asociata.
Dupa intoarcerea din recursivitate, se verifica daca nodul curent mai este util. Un nod este considerat inutil daca nu mai este marcat ca final de cuvant pentru un alt termen si nu mai are niciun descendent in vectorul de fii.
Daca aceste conditii sunt indeplinite, memoria ocupata de nod este eliberata cu free, iar pointerul parintelui corespunzator este setat pe NULL. Procesul se repeta in sus, spre radacina, pana cand se intalneste un nod care fie este terminal pentru un alt cuvant, fie mai are cel putin un copil.

## `EliminaReferintaDinNodTerminal`
Functie similara cu `EliminaReferinteLaFisier`, dar pentru un singur cuvant. Primeste `cuvant`, `fisier` si `TTrie *t`. Navighez litera cu litera pana la nodul terminal si apelez `DeconecteazaFisierDinNod`.
Este apelata in `DELKW` dupa ce am scos cuvantul din lista fisierului.

## `GasesteNodTerminalCuvant`
Functie recursiva care primeste un cuvant si radacina curenta. 
Daca radacina este `NULL`, returnez `NULL` imediat.
Daca `cuvant[0] == '\0'`, returnez nodul curent. Altfel, calculez index = cuvant[0] - 'a' si returnez apelul recursiv pe `radacina->fii[index]` cu `cuvant + 1`.
Este folosita pentru a localiza rapid un nod fara a rescrie traversarea de fiecare data.

## `cmpLexicograficaFisiere`
Functia `cmpLexicograficaFisiere` este folosita pentru sortarea unui vector de fisiere dupa campul `id`. Este utilizata in comanda `PREFIX`, dupa colectarea tuturor fisierelor asociate cuvintelor care incep cu prefixul cautat.

## `ColecteazaReferinteFisiere`
Am implementat aceasta functie pentru a extrage toate referintele catre fisiere dintr-un anumit subarbore, fiind esentiala pentru operatia de cautare dupa prefix.
Functia parcurge recursiv arborele Trie incepand de la un nod dat (corespunzator ultimului caracter din prefix) si viziteaza toti descendentii acestuia.
La fiecare pas, daca intalnesc un nod marcat ca fiind terminal (final_cuvant == 1), copiez toate referintele sale intr-o lista auxiliara de colectare.
Pentru fiecare referinta gasita, aloc dinamic o noua celula de tip TCelulaReferinte, copiez pointerul catre fisier si o inserez la inceputul listei de colectare, actualizand totodata contorul total de fisiere gasite.
Aceasta metoda imi permite sa adun toate fisierele care contin cuvinte ce incep cu prefixul cautat, urmand ca ulterior rezultatele sa fie filtrate pentru a evita duplicatele si sortate lexicografic.


Pentru a evita pierderile de memorie, am implementat mai multe functii care elibereaza structurile alocate dinamic in timpul executiei programului.

## `DistrugeListaCuvinte`
Functia `DistrugeListaCuvinte` elibereaza memoria ocupata de lista simplu inlantuita de cuvinte asociata unui fisier.
Se parcurge lista nod cu nod, se salveaza adresa urmatorului element, apoi se elibereaza nodul curent cu `free`. La final, pointerul catre lista este setat la `NULL`, pentru a evita folosirea unei adrese deja eliberate.

## `DistrugeListaReferinte`
Functia `DistrugeListaReferinte` elibereaza lista de referinte asociata unui nod terminal din arborele Trie.
Aceasta lista contine celule de tip `TCelulaReferinte`, care pastreaza pointeri catre fisierele din lista principala. Functia elibereaza doar celulele listei de referinte, nu si fisierele propriu-zise, deoarece acestea sunt gestionate separat in lista de fisiere.
La final, pointerul listei este setat la `NULL`.

## `DistrugeTrie`
Functia `DistrugeTrie` elibereaza recursiv memoria ocupata de arborele Trie.
Pentru fiecare nod, functia parcurge mai intai toti cei 26 de fii si apeleaza recursiv distrugerea acestora. Dupa ce subarborii au fost eliberati, se elibereaza lista de referinte a nodului curent prin `DistrugeListaReferinte`, apoi se elibereaza nodul propriu-zis.
La final, pointerul catre nod este setat la `NULL`.

## `DistrugeListaFisiere`
Functia `DistrugeListaFisiere` elibereaza lista principala de fisiere.
Se parcurge lista dublu inlantuita de fisiere, iar pentru fiecare fisier se elibereaza mai intai lista de cuvinte asociata, folosind `DistrugeListaCuvinte`. Dupa aceea, se elibereaza celula fisierului.
La final, pointerul catre lista de fisiere este setat la `NULL`.
Aceste functii sunt apelate la finalul programului, pentru ca toate structurile alocate dinamic sa fie eliberate corect.

---
## fisierul FunctiiHeap.c

## `comp(void *a, void *b)`
Functia comp este folosita pentru a stabili ordinea intr-un heap.
In interiorul functiei sunt comparate doua elemente de tip ListaFisiere.
Heap-ul este organizat ca un Max-Heap dupa campul scor, astfel incat fisierul cu scorul cel mai mare sa fie cel mai prioritar si sa ajunga in radacina.
Daca primul fisier are scor mai mare decat al doilea, functia returneaza o valoare negativa.
Daca primul fisier are scor mai mic, functia returneaza o valoare pozitiva.
Daca scorurile sunt egale, fisierele sunt comparate lexicografic dupa campul id, folosind strcmp.

## `THeap* AlocaHeap(int nrMax, TFCmp comp)`
Functia `AlocaHeap` aloca memorie pentru o structura de tip heap.
Mai intai se aloca memoria pentru structura `THeap`, apoi se aloca vectorul in care vor fi retinuti pointerii catre fisierele din heap.
Daca una dintre alocari esueaza, functia elibereaza memoria alocata anterior si returneaza NULL.
Dupa alocare, se initializeaza numarul maxim de elemente, numarul curent de elemente, care este initial 0, si functia de comparare.
La final, functia returneaza heap-ul creat.

## `void InsertHeap(THeap *h, TListaFisiere val)`
Daca heap-ul este plin, dimensiunea vectorului de pointeri catre fisiere este dublata folosind `realloc`.
Daca realocarea reuseste, vectorul si capacitatea maxima a heap-ului sunt actualizate.
Elementul nou este adaugat pe ultima pozitie din vector, iar numarul de elemente este incrementat.
Apoi se reface proprietatea de heap prin urcarea elementului in arbore: cat timp elementul curent este mai prioritar decat parintele sau, cele doua elemente sunt interschimbate.
Astfel, fisierul cu scor mai mare, sau cu `id` mai mic lexicografic in caz de egalitate, va fi pozitionat mai aproape de radacina.

## `TListaFisiere Extragere(THeap *h)`
Functia `Extragere` elimina si returneaza elementul aflat in radacina heap-ului, adica fisierul cel mai prioritar.
Daca heap-ul este invalid sau nu contine elemente, functia returneaza `NULL`.
Elementul din radacina este salvat, apoi ultimul element din heap este mutat pe pozitia radacinii. Numarul de elemente este decrementat.
Dupa aceasta modificare, se reface proprietatea de heap prin coborarea elementului din radacina.
La fiecare pas, elementul curent este comparat cu fiii sai, iar daca unul dintre fii este mai prioritar, se face interschimbarea cu acesta.
Procesul continua pana cand elementul ajunge pe o pozitie corecta.
Functia returneaza elementul extras initial din radacina.

---

## Fisierul comenzi.c

Fisierul `comenzi.c` contine implementarea comenzilor principale ale sistemului de indexare. Aceste functii folosesc structurile definite in `structuri.h` si functiile auxiliare din `functiiStructuri.c` si `FunctiiHeap.c`.

Comenzile lucreaza asupra listei principale de fisiere si asupra arborelui Trie, care retine legaturile dintre cuvinte si fisierele in care acestea apar.

## `ADD`
Functia `ADD` este folosita pentru a adauga un fisier nou in sistemul de indexare.
Lista de cuvinte primita de `ADD` este deja filtrata in `CitireL`, astfel incat nu contine duplicate. Numarul de cuvinte transmis catre `ADD` este numarul de cuvinte unice, nu neaparat numarul brut citit din input.
Aceasta primeste ca parametri fisierul de output, lista de fisiere, id-ul fisierului, scorul, numarul de cuvinte, lista de cuvinte si radacina arborelui Trie.
Mai intai se verifica daca fisierul exista deja in lista. Daca fisierul este deja prezent, acesta nu mai este adaugat.
Daca fisierul nu exista, se aloca o noua celula de tip TListaFisiere folosind functia `AlocCelulaFisiere`. Aceasta celula contine id-ul fisierului, scorul, lista de cuvinte si numarul de cuvinte asociate.
Dupa alocare, fisierul este inserat in lista principala de fisiere cu ajutorul functiei `InserareListaFisiere`.
La final, fiecare cuvant din lista de cuvinte a fisierului este inserat in arborele Trie prin functia `InserareCuvantInArbore`, iar in nodul terminal al fiecarui cuvant se adauga o referinta catre fisierul respectiv.
Astfel, comanda `ADD` actualizeaza atat lista de fisiere, cat si structura de indexare pe cuvinte.

## `DEL`
Functia `DEL` elimina un fisier din sistemul de indexare.
Aceasta primeste ca parametri fisierul de output, lista de fisiere, id-ul fisierului care trebuie sters si radacina arborelui Trie.
Mai intai se cauta fisierul in lista principala de fisiere folosind functia `IdentificaFisier`.
Daca fisierul nu exista, functia afiseaza mesajul corespunzator in fisierul de output si nu modifica structurile de date.
Daca fisierul este gasit, trebuie eliminate toate referintele catre acesta din arborele Trie. Pentru acest lucru se foloseste functia `EliminaReferinteLaFisier`, care parcurge lista de cuvinte asociata fisierului si elimina referinta fisierului din fiecare nod terminal corespunzator.
Dupa eliminarea referintelor, se verifica pentru fiecare cuvant al fisierului daca nodul terminal mai are fisiere asociate. Daca nu mai are nicio referinta, cuvantul este sters din Trie prin `StergereCuvantDinArbore`.
Dupa aceste operatii, lista de cuvinte a fisierului este eliberata, iar fisierul este eliminat din lista principala prin `EliminaFisierDinListaFisiere`.

## `ADDKW`
Functia `ADDKW` adauga un cuvant nou unui fisier deja existent.
Mai intai se cauta fisierul in lista principala folosind functia `IdentificaFisier`.
Daca fisierul nu exista, se afiseaza `NOT FOUND`.
Daca fisierul exista, se parcurge lista de cuvinte a acestuia pentru a verifica
daca noul cuvant este deja asociat fisierului. Daca acesta exista deja, se
afiseaza `OK` si nu se mai modifica structurile.
Daca nu exista, se aloca o noua celula pentru cuvant, se insereaza la finalul
listei de cuvinte a fisierului si se insereaza cuvantul in Trie prin
`InserareCuvantInArbore`, adaugand o referinta catre fisier in nodul terminal.
Dupa inserarea cuvantului in lista, campul `nr_cuvinte` al fisierului este incrementat.

## `DELKW`
Functia `DELKW` sterge un cuvant asociat unui fisier.
Aceasta primeste ca parametru fisierul de output, lista de fisiere, id-ul fisierului, cuvantul care trebuie sters si radacina arborelui Trie.
Mai intai se cauta fisierul in lista principala folosind functia `IdentificaFisier`.
Daca fisierul nu exista, functia afiseaza mesajul corespunzator si nu modifica structurile de date.
Daca fisierul este gasit, se cauta cuvantul in lista de cuvinte a fisierului. Daca fisierul exista, dar cuvantul nu este gasit in lista sa de cuvinte, se afiseaza `OK` si nu se modifica structurile.
Daca este gasit, cuvantul este eliminat din lista de cuvinte a fisierului, iar numarul de cuvinte este decrementat.
Dupa eliminarea din lista fisierului, trebuie eliminata si legatura dintre cuvant si fisier din arborele Trie. Pentru acest lucru se foloseste functia `EliminaReferintaDinNodTerminal`, care ajunge in nodul terminal al cuvantului si elimina referinta catre fisier.
Dupa eliminarea referintei, se cauta din nou nodul terminal al cuvantului. Daca acesta nu mai are fisiere asociate, cuvantul este eliminat din Trie prin `StergereCuvantDinArbore`.
Astfel, fisierul nu va mai aparea in rezultatele cautarilor pentru acel cuvant.

## `FIND`
Functia `FIND` cauta fisierele asociate unui anumit cuvant.
Aceasta primeste ca parametri fisierul de output, cuvantul cautat, radacina arborelui Trie si lista principala de fisiere.
Mai intai se cauta nodul terminal al cuvantului in arbore, folosind functia `GasesteNodTerminalCuvant`.
Daca nodul nu exista sau nu este marcat ca final de cuvant, inseamna ca nu exista fisiere asociate acelui cuvant.
Daca nodul este valid, functia parcurge lista de referinte din campul `fisiere_asociate`. Aceasta lista contine pointeri catre fisierele care au asociat cuvantul cautat.
Lista `fisiere_asociate` este deja mentinuta in ordine lexicografica dupa id-ul fisierelor in momentul inserarii referintelor in Trie. Din acest motiv, functia `FIND` parcurge direct aceasta lista si afiseaza fisierele in ordinea in care apar in ea.
Comanda `FIND` afiseaza toate fisierele care contin cuvantul cautat.
La final, functia verifica lista principala de fisiere si elimina eventualele fisiere care nu mai au niciun cuvant asociat.

## `TOPK`
Functia `TOPK` afiseaza cele mai relevante `k` fisiere asociate unui cuvant.
Aceasta primeste ca parametri fisierul de output, cuvantul cautat, valoarea `k` si radacina arborelui Trie.
Mai intai se cauta nodul terminal al cuvantului in arbore. Daca nodul nu exista sau nu este final de cuvant, inseamna ca nu exista fisiere asociate cuvantului respectiv.
Daca exista fisiere asociate, acestea sunt introduse intr-un heap. Heap-ul este organizat ca Max-Heap dupa scorul fisierelor, iar in caz de egalitate fisierele sunt ordonate lexicografic dupa id.
Pentru construirea heap-ului se folosesc functiile `AlocaHeap` si `InsertHeap`.
Dupa ce toate fisierele asociate cuvantului au fost inserate in heap, se extrag pe rand cel mult `k` elemente folosind functia `Extragere`.
Astfel, comanda `TOPK` afiseaza fisierele cele mai relevante pentru cuvantul cautat, in ordinea prioritatii date de scor si de id.

## `PRINT`
Functia `PRINT` afiseaza toate cuvintele existente in arborele Trie.
Aceasta primeste ca parametri fisierul de output, nodul curent din Trie, un vector `prefix` si lungimea curenta a prefixului.
Functia parcurge recursiv arborele Trie. Pe masura ce coboara in arbore, adauga litera corespunzatoare in vectorul `prefix`.
Daca nodul curent este marcat ca final de cuvant, atunci prefixul format pana in acel punct reprezinta un cuvant complet si este afisat.
Dupa afisare, parcurgerea continua prin toti copiii nodului curent, in ordinea literelor de la a la z.
In `main`, inainte de apelul functiei `PRINT`, se verifica daca arborele este gol. Daca nu exista niciun cuvant in arbore, se afiseaza `EMPTY`.
Pentru fiecare cuvant terminal gasit, functia afiseaza cuvantul, numarul de fisiere asociate si id-urile fisierelor respective. Id-urile sunt 
afisate direct din lista `fisiere_asociate`, deoarece aceasta lista este mentinuta in ordine lexicografica dupa id-ul fisierelor la inserarea in Trie.

## `PREFIX`

Functia `PREFIX` afiseaza fisierele asociate cuvintelor care incep cu un prefix dat.
Mai intai se cauta in Trie nodul corespunzator ultimei litere din prefix, folosind functia `GasesteNodTerminalCuvant`.
Daca acest nod nu exista, inseamna ca nu exista niciun cuvant indexat care sa inceapa cu prefixul primit, iar functia afiseaza `EMPTY`.
Daca nodul exista, se apeleaza `ColecteazaReferinteFisiere` pentru a colecta toate referintele catre fisiere din subarborele care porneste din acel nod.
Referintele colectate sunt mutate intr-un vector de fisiere, apoi vectorul este sortat lexicografic dupa id-ul fisierelor cu ajutorul functiei
`cmpLexicograficaFisiere`.
Deoarece acelasi fisier poate fi asociat cu mai multe cuvinte care au acelasi prefix, dupa sortare se elimina duplicatele la afisare.
Functia afiseaza numarul de fisiere distincte, urmat de id-urile acestora. Daca nu exista niciun fisier asociat, se afiseaza `EMPTY`.

## Punctaj obtinut

La rularea locala a checker-ului am obtinut:
- teste principale: 95/95
- bonus PREFIX: 20/20
- readme 5/5
- total: 120/120
- La rularea locala a checker-ului am obtinut punctaj maxim pe testele principale si pe bonus.

Valgrind:
- in use at exit: 0 bytes in 0 blocks
- ERROR SUMMARY: 0 errors from 0 contexts

Surse consultate pentru intelegerea structurii de trie(arbore multicai de regasire):
https://www.geeksforgeeks.org/dsa/trie-insert-and-search/
https://ocw.cs.pub.ro/courses/sd-ca/2019/laboratoare/lab-11
https://www.digitalocean.com/community/tutorials/trie-data-structure-in-c-plus-plus
