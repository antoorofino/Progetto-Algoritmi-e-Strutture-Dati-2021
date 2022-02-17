# Progetto Algoritmi e Strutture Dati 2021

---

Il progetto è stato realizzato come **prova finale** del corso di **Algoritmi e Strutture Dati** del Politecnico di Milano, anno accademico 2020/2021.

### Specifiche

La descrizione e le specifiche della prova sono illustrate nel file `Presentazione_Prova_Finale_2021.pdf`. Nella cartella `open_tests` è possibile inoltre trovare i test pubblici utilizzati per valutare la corretta del programma.

### Obiettivo

L'obiettivo del progetto è realizzare un'implementazione che sia allo tempo efficace, ma sopratutto efficiente in spazio ed in tempo.

---

### Implementazione

Il codice sorgente è situato all'interno della cartella `src`

#### Strutture dati utilizzate

- [x] **Min heap**
- [x] **Max heap**

#### Funzionamento generale

Viene implementato l'algoritmo di **Dijkstra** gestendo la coda con priorità come un **min-heap**. Per quanto riguarda la
distanza infinita si è scelto di utilizzare il valore `-1` e poi è stata creata la funzione `piu_grande` per fare la giusta valutazione tra due distanze (compreso il caso infinito).

La classifica dei K grafi viene gestita con un **max-heap** così da tenere sempre conto del grafo (all'interno della classifica) che possiede la somma massima delle
distanze; in questo modo per sapere se aggiungere o meno un grafo nella classifica basta confrontare la sua somma dei cammini con quella più
grande nella classifica.

*Per ulteriori chiarimenti è stata creata una copia del sorgente arricchita con i commenti che spiegano il significato di quasi tutte le istruzioni.*

#### Generazione eseguibile

Per compilare il sorgente si è utilizzato il compilatore **gcc** passandogli i seguenti parametri:
```
gcc -Wall -Werror -O2 -g3 sorgente.c -o eseguibile.out
```

#### Ordinamento

Nelle specifiche non è richiesto che l'output sia ordinato, di conseguenza ci possono essere molteplici output diversi ma corretti per un dato input.

Gli output forniti per i test pubblici sono comunque ordinati crescenti, quindi per effettuare il confronto con quelli si è scelto di ordinare l'output. L'ordinamento avviene solo se non viene definita la macro **EVAL**: ai fini della valutazione l'ordinamento è disabilitato per evitare che ne appesantisca il funzionamento.

Compilazione *con* ordinamento dell'output:
```
gcc -Wall -Werror -O2 -g3 sorgente.c -o eseguibile.out
```

Compilazione *senza* ordinamento dell'output
```
gcc -Wall -Werror -O2 -g3 -D EVAL sorgente.c -o eseguibile.out
```









