# Simulazione del comportamento di stormi
**Autore:** Dario Eugenio Campisi

- [Descrizione generale](#descrizione-generale)
- [Implementazioni aggiuntive](#implementazioni-aggiuntive)
- [Istruzioni per la compilazione](#istruzioni-per-la-compilazione)

## Descrizione generale
Il progetto è un tentativo di implementazione del programma **Boids**, sviluppato da Craig Reynolds nel 1986, facente uso della libreria grafica **SFML**. L'obiettivo del programma è quello di simulare il comportamento di un generico **stormo** (e.g. di uccelli), ovvero un sistema complesso costituito da singole entità, i *boid*, che rispettano delle precise regole di movimento, qui chiamate **regole di volo**.

<center>
<img src="utility/boid_shape.png" alt="Boid Shape" width="250">

**Figura 1:** Forma geometrica usata per la rappresentazione grafica dei boid
</center>

### Regole di volo
Il movimento nel piano di ogni boid è il risultato delle seguenti regole. In modo da favorire la formazione di più stormi indipendenti, esse vengono applicate unicamente ai boid considerati **vicini**, quelli cioè la cui distanza non supera un certo valore.

- **Separazione:** induce i boid a mantenere tra loro una certa distanza, impedendogli così di sovrapporsi
- **Allineamento:** fa sì che i boid seguano la stessa direzione dei vicini
- **Coesione:** spinge i boid a muoversi verso il centro di massa dello stormo

Da queste tre semplici regole emergono dei **comportamenti macroscopici caotici**, del tutto simili a quelli degli stormi reali.

### Comportamento ai bordi
Quando un boid comincia ad avvicinarsi ad uno dei quattro bordi della finestra, questo vira in modo da evitarlo. Per ottenere questo effetto è stato definito un **margine** nei pressi dei bordi, se superato la velocità del boid viene gradualmente invertita secondo un opportuno **fattore di virata**.

### Dinamica della simulazione
All'avvio del programma viene mostrata una **schermata iniziale** che invita l'utente a scegliere il numero di boid che saranno generati (limitato ad un massimo di 300 per garantire delle buone prestazioni):

<center>
<img src="utility/initial_screen.png">

**Figura 2:** Schermata iniziale
</center>

Per dare inizio alla simulazione l'utente può utilizzare il pulsante *start* o alternativamente premere *enter*.

Durante la simulazione è possibile regolare il valore dei parametri delle regole di volo e del raggio di visione dei boid tramite degli **slider**, osservando in tempo reale come tali modifiche influenzano il movimento dei boid. Per tornare rapidamente ai valori impostati di default, è sufficiente premere il pulsante *reset*.

<center>
<img src="utility/one_flock.png">

**Figura 3:** Frame di una simulazione con un solo stormo arancione costituito da 173 boid
</center>

Come è possibile notare nella **Figura 3**, alle simulazioni è stata aggiunta la presenza costante di un **predatore**, differenziato dai boid per colore e dimensioni. Il suo ruolo è quello di inseguire gli stormi inducendo ai boid vicini una spinta di **repulsione**.

### Componente stocastica
All'inizio di ogni simulazione è equiprobabile la generazione di uno, duo o tre **stormi diversi**, differenziati per colore. I possibili colori degli stormi e dei dettagli grafici della *top bar* sono blu, arancione e verde.

<center>
<img src="utility/three_flocks.png">

**Figura 4:** Frame di una simulazione con 230 boid divisi in tre stormi diversi
</center>



## Logica di implementazione

## Altre implementazioni aggiuntive
### Più stormi
All'avvio di ogni simulazione è equiprobabile la generazione di uno, due o tre stormi distinti, differenziati per colore. L'applicazione delle regole di volo è dunque limitata allo stormo di cui il boid fa parte.

### Predatore
Per aumentare il dinamismo della simulazione, è stata aggiunta la presenza costante di un **predatore**, differenziato dagli altri boid per colore e dimensioni. Il suo ruolo è quello di "inseguire" gli stormi di boid, pertanto 

È stata aggiunta la presenza costante di un predatore, si differenzia dagli altri boid per colore e dimensioni. Esso rispetta la regola della **coesione**, i boid a lui vicini invece avvertono una cosiddetta **repulsione**, regola analoga alla **separazione** ma di maggiore intensità.

### Left-click per far comparire un boid
Oltre ai boid presenti all'inizio della simulazione, generati con posizioni, orientazioni e velocità casuali, è possibile aggiungerne degli altri tramite un mouse left-click, che genererà un boid appartenente ad uno stormo casuale.

### Top bar
Nella top bar viene mostrato il calcolo degli fps in tempo reale. Sono inoltre presenti degli *slider*, grazie ai quali è possibile regolare alcuni parametri delle regole di volo, in modo da osservare i cambiamenti degli stormi e comprenderne appieno il significato.

### Angolo di vista
Per rendere la simulazione più realistica, i boid non tengono conto dei compagni che "non vedono", che sono cioè al di fuori del loro **angolo di vista**.

### Gestione del fuori focus
Quando la finestra va fuori focus la simulazione va in pausa e lo schermo viene leggermente scurito, quando la finestra torna in focus la simulazione riparte.

## Istruzioni per la compilazione