
#ifndef TEMA2_SDA_FUNCTIIHEAP_H
#define TEMA2_SDA_FUNCTIIHEAP_H
int comp(void *a, void *b);
THeap* AlocaHeap(int nrMax, TFCmp comp);
void InsertHeap(THeap *h, TListaFisiere val);
TListaFisiere Extragere(THeap *h);
#endif //TEMA2_SDA_FUNCTIIHEAP_H
