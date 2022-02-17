#include <stdio.h>
#include <stdlib.h>

#define ISTRUZIONE_AGGIUNGI 'A'
#define ISTRUZIONE_TOPK 'T'

//definizione tipo vertice del grafo
typedef struct{
	int indice;
	int distanza;
} TipoVertice;

//definizione tipo per la struttura dati coda
typedef struct{
	TipoVertice *vettore;
	int *posizione;			//mantiene traccia delle posizioni degli elementi nel array 'vettore' (rende la complessità temporale di 'decrementa_priorita' logaritmica)
	int dimensione;
} TipoCoda;

int numero_nodi;

//funzione necessaria per ordinare tramite qsort della lib standard
#ifndef EVAL
int comp (const void * elem1, const void * elem2) {
    int f = *((int*)elem1);
    int s = *((int*)elem2);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}
#endif

int figlio_sx(int n){
	return 2*(n+1)-1;
}

int figlio_dx(int n){
	return 2*(n+1);
}

int padre(int n){
	if(n == 0)
		return 0;
	return (n-1)/2;
}

//compara in maniera corretta due distanze: -1 sta per infinito
int piu_grande(int n, int p){
	if(n == -1 && p == -1)
		return 0;
	if(n == -1)
		return 1;
	if(p == -1)
		return 0;
	if(n > p)
		return 1;
	return 0;
}

//legge un numero intero carattere per carattere, effettuandone il parsing
//si ferma al primo carattere letto diverso da una cifra
void leggi_numero(int* n){
	char carattere;
	*n = 0;
	carattere = getchar_unlocked();
	do{
		*n = (*n << 1) + (*n << 3) + carattere - '0';
		carattere = getchar_unlocked();
	} while(carattere >= '0' && carattere <= '9');
	return;
}

//inserisce nel posto giusto un nuovo elemento in coda (heap min)
void accoda_min(TipoCoda *coda, TipoVertice vertice){
	int i;
	TipoVertice tmp;
	//inserisco prima come ultima foglia
	i = coda->dimensione;
	coda->vettore[i] = vertice;
	coda->posizione[vertice.indice] = i;
	//verifico che il padre sia più piccolo sennò scambio e controllo con il nuovo padre
	while(i > 0 && piu_grande(coda->vettore[padre(i)].distanza, coda->vettore[i].distanza)){
		tmp = coda->vettore[padre(i)];
		coda->vettore[padre(i)] = coda->vettore[i];
		coda->vettore[i] = tmp;
		coda->posizione[coda->vettore[padre(i)].indice] = padre(i);
		coda->posizione[coda->vettore[i].indice] = i;	
		i = padre(i);
	}
	coda->dimensione++;
	return;
}

//inserisce nel posto giusto un nuovo elemento in coda (heap max)
void accoda_max(TipoCoda *coda, TipoVertice vertice){
	int i;
	TipoVertice tmp;
	//inserisco prima come ultima foglia
	i = coda->dimensione;
	coda->vettore[i] = vertice;
	//verifico che il padre sia più piccolo sennò scambio e controllo la relazione con il nuovo padre
	while(i > 0 && coda->vettore[i].distanza > coda->vettore[padre(i)].distanza){
		tmp = coda->vettore[padre(i)];
		coda->vettore[padre(i)] = coda->vettore[i];
		coda->vettore[i] = tmp;
		i = padre(i);
	}
	coda->dimensione++;
}

void min_heapfy(TipoCoda *coda, int n){
	int sx, dx, pos_min;
	TipoVertice tmp;
	sx = figlio_sx(n);
	dx = figlio_dx(n);
	pos_min = n;
	if(sx < coda->dimensione && piu_grande(coda->vettore[n].distanza, coda->vettore[sx].distanza))
		pos_min = sx;
	if(dx < coda->dimensione && piu_grande(coda->vettore[pos_min].distanza, coda->vettore[dx].distanza))
		pos_min = dx;
	if(pos_min != n){
		tmp = coda->vettore[pos_min];
		coda->vettore[pos_min] = coda->vettore[n];
		coda->vettore[n] = tmp;
		coda->posizione[coda->vettore[pos_min].indice] = pos_min;
		coda->posizione[coda->vettore[n].indice] = n;
		min_heapfy(coda, pos_min);
	}
}

void max_heapfy(TipoCoda *coda, int n){
	int sx, dx, pos_max;
	TipoVertice tmp;
	sx = figlio_sx(n);
	dx = figlio_dx(n);
	pos_max = n;
	if(sx < coda->dimensione && coda->vettore[sx].distanza > coda->vettore[n].distanza)
		pos_max = sx;
	if(dx < coda->dimensione && coda->vettore[dx].distanza > coda->vettore[pos_max].distanza)
		pos_max = dx;
	if(pos_max != n){
		tmp = coda->vettore[pos_max];
		coda->vettore[pos_max] = coda->vettore[n];
		coda->vettore[n] = tmp;
		max_heapfy(coda, pos_max);
	}
}

//estrae e restituisce il minimo della coda
TipoVertice estrai_min(TipoCoda *coda){
	TipoVertice min;
	min = coda->vettore[0];
	coda->vettore[0] = coda->vettore[coda->dimensione -1];
	coda->posizione[coda->vettore[0].indice] = 0;
	coda->dimensione--;
	min_heapfy(coda, 0);
	return min;
}

//decrementa il valore 'distanza' nel vertice passato come parametro all'interno della coda
void decrementa_priorita(TipoCoda *coda, TipoVertice vertice){
	int i;
	TipoVertice tmp;
	i = coda->posizione[vertice.indice];
	if(coda->vettore[i].indice == vertice.indice){
		coda->vettore[i].distanza = vertice.distanza;
		while(i > 0 && piu_grande(coda->vettore[padre(i)].distanza, coda->vettore[i].distanza)){
			tmp = coda->vettore[padre(i)];
			coda->vettore[padre(i)] = coda->vettore[i];
			coda->vettore[i] = tmp;
			coda->posizione[coda->vettore[padre(i)].indice] = padre(i);
			coda->posizione[coda->vettore[i].indice] = i;
			i = padre(i);
		}
		return;
	}
}

//restituisce il massimo dalla coda
int max_value(TipoCoda coda){
	return coda.vettore[0].distanza;
}

//rimuove il massimo dalla coda
void rimuovi_max(TipoCoda *coda){
	coda->vettore[0] = coda->vettore[coda->dimensione -1];
	coda->dimensione--;
	max_heapfy(coda, 0);
	return;
}

//restituisce la somma dei cammini minimi a partire dal nodo 0
int Dijkstra(int mat_adiacenza[numero_nodi][numero_nodi]){
	int *vertici;
	int i, nuova_distanza, somma_cammini;
	TipoCoda codaVertici;
	TipoVertice tmp, nodo_estratto;

	//dichiaro un array 'vertici' che conterrà la distanza minima per arrivare al nodo utilizzato come indice dell'array
	vertici = malloc(numero_nodi*sizeof(int));
	//dichiaro la coda contenente i vertici
	codaVertici.dimensione = 0;
	codaVertici.vettore = malloc(numero_nodi*sizeof(TipoVertice));
	codaVertici.posizione = malloc(numero_nodi*sizeof(int));

	//inizializzo la coda e l'array 'vertici' inserendo tutti i nodi con distanza infinita tranne il nodo zero che avrà distanza nulla
	vertici[0] = 0;
	tmp.indice = 0;
	tmp.distanza = 0;
	accoda_min(&codaVertici, tmp);
	for(i = 1; i < numero_nodi; i++){
		vertici[i] = -1;
		tmp.indice = i;
		tmp.distanza = -1;
		accoda_min(&codaVertici, tmp);
	}

	while(codaVertici.dimensione > 0){
		nodo_estratto = estrai_min(&codaVertici);
		//procedo soltanto se il nodo estratto è raggiungibile (con distanza finita)
		if(nodo_estratto.distanza != -1){
			//ciclo per iterare con i vertici adiacenti al nodo estratto tranne se stesso
			for(i = 0; i < numero_nodi; i++){
				if(mat_adiacenza[nodo_estratto.indice][i] > 0 && nodo_estratto.indice != i){
					nuova_distanza = nodo_estratto.distanza + mat_adiacenza[nodo_estratto.indice][i];
					//se trovo un cammino più breve allora aggiorno la distanza sia in 'vertici' che nella coda
					if(piu_grande(vertici[i], nuova_distanza)){
						vertici[i] = nuova_distanza;
						tmp.indice = i;
						tmp.distanza = nuova_distanza;
						decrementa_priorita(&codaVertici, tmp);
					}
				}
			}
		}
	}

	somma_cammini = 0;
	for(i = 0; i < numero_nodi; i++)
		if(vertici[i] != -1)
			somma_cammini += vertici[i];

	free(vertici);
	free(codaVertici.vettore);
	free(codaVertici.posizione);
	
	return somma_cammini;
}

//decide se inserire o meno (e quindi aggiornare) il grafo in basa alla sua somma dei cammini minimi nella classifica K
void aggiorna_codaK(TipoCoda *codaK,int k_value ,int indice, int somma_cammini){
	TipoVertice tmp;
	//se la coda può essere ancora riempita allora il grafo va aggiunto a prescindere dalla sua 'somma_commini'
	if(codaK->dimensione < k_value){
		tmp.indice = indice;
		tmp.distanza = somma_cammini;
		accoda_max(codaK, tmp);
	} else if(somma_cammini < max_value(*codaK)){	//se la coda è piena allora bisogna vedere se la sua 'somma_commini' è minore di quella più grande nella coda
		rimuovi_max(codaK);							//e nel coso di esito positivo procedere rimuovendo il massimo ed inserendo il nuovo grafo nella coda-classifica
		tmp.indice = indice;
		tmp.distanza = somma_cammini;
		accoda_max(codaK, tmp);
	}
	return;
}

//legge ciascun grafo e ne restituisce la somma dei cammini minimi
int leggi_somma_grafo(){
	int mat_adiacenza[numero_nodi][numero_nodi];
	int i,j;

	for(i = 0; i < numero_nodi; i++)
		for(j = 0; j < numero_nodi; j++)
			leggi_numero(&(mat_adiacenza[i][j]));

	return Dijkstra(mat_adiacenza);
}

int main(){
	int indice, i, somma_cammini;
	int k_value;
	char carattere;
	TipoCoda codaK;

	leggi_numero(&numero_nodi);
	leggi_numero(&k_value);

	#ifndef EVAL
	//serve come supporto per ordinare gli elementi
	int* vet = malloc(k_value*sizeof(int));
	#endif

	//inizializzo la coda k contente la classifica
	codaK.dimensione = 0;
	codaK.vettore = malloc(k_value*sizeof(TipoVertice));
	codaK.posizione = NULL;

	indice = 0;
	carattere = getchar_unlocked();
	while(carattere != EOF){
		if(carattere == ISTRUZIONE_AGGIUNGI){
			//itero e leggo fino a quando finisce il nome dell'istruzione
			while(carattere != '\n')
				carattere = getchar_unlocked();
			somma_cammini = leggi_somma_grafo();
			aggiorna_codaK(&codaK, k_value, indice, somma_cammini);
			indice++;
		} else if(carattere == ISTRUZIONE_TOPK) {
			//itero e leggo fino a quando finisce il nome dell'istruzione
			while(carattere != '\n' && carattere != EOF)
				carattere = getchar_unlocked();
			
			#ifndef EVAL
			//copio i valori della coda nell'array ausiliario
			//l'array viene poi ordinato e stampato
			for(i = 0; i < codaK.dimensione; i++)
				vet[i] = codaK.vettore[i].indice;
			qsort(vet, codaK.dimensione, sizeof(int), comp);
			for(i = 0; i < codaK.dimensione; i++){
				if(i == 0)
					printf("%d", vet[i]);
				else
					printf(" %d", vet[i]);
			}
			printf("\n");
			#endif

			#ifdef EVAL
			for(i = 0; i < codaK.dimensione; i++){
				if(i == 0)
					printf("%d", codaK.vettore[i].indice);
				else
					printf(" %d", codaK.vettore[i].indice);
			}
			printf("\n");
			#endif
		}
		carattere = getchar_unlocked();
	}

	free(codaK.vettore);
	return 0;
}

