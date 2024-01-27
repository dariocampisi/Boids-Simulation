# Simulazione del comportamento di stormi
**Autore:** Dario Eugenio Campisi  
**Data:** Gennaio 2024  

- [Descrizione generale](#descrizione-generale)
- [Logica di implementazione](#logica-di-implementazione)
- [Strategie di testing](#strategie-di-testing)
- [Istruzioni per la compilazione](#istruzioni-per-la-compilazione)
- [Interpretazione dei risultati](#interpretazione-dei-risultati)

## Descrizione generale
Il progetto è un tentativo di implementazione del programma **Boids**, sviluppato da Craig Reynolds nel 1986, facente uso della libreria grafica **SFML**. L'obiettivo del programma è quello di simulare il comportamento di un generico **stormo**, ovvero un sistema complesso costituito da singole entità, i *boid*, che rispettano delle precise regole di movimento, qui chiamate **regole di volo**.

<div align="center">
<img src="utility/boid_shape1.png" width="400">

<b>Figura 1:</b> Forma geometrica usata per la rappresentazione grafica dei boid
</div>

### Regole di volo
Il movimento nel piano di ogni boid è il risultato delle seguenti **regole**. In modo da favorire la formazione di più stormi indipendenti, esse vengono applicate unicamente ai boid considerati **vicini**, quelli cioè la cui distanza non supera un certo valore.

- **Separazione:** induce i boid a mantenere tra loro una distanza minima, impedendogli così di sovrapporsi
- **Allineamento:** fa sì che i boid seguano la stessa direzione dei vicini
- **Coesione:** spinge i boid a muoversi verso il centro di massa dello stormo

Da queste tre semplici regole emergono dei **comportamenti macroscopici caotici**, del tutto simili a quelli degli stormi reali. Per quanto concerne il **comportamento ai bordi** della finestra grafica, quando un boid comincia ad avvicinarsi ad uno dei quattro bordi della finestra, questo vira in modo da evitarlo. Per ottenere questo effetto è stato definito un **margine** nei pressi dei bordi, se superato la velocità del boid viene gradualmente invertita secondo un opportuno **fattore di virata**.

### Dinamica di una simulazione
All'avvio del programma viene mostrata una **schermata iniziale** che invita l'utente a scegliere il numero di boid che saranno generati, questo è limitato ad un massimo di 300 per non appesantire troppo la finestra grafica ed evitare comportamenti indesiderati:

<div align="center">
<img src="utility/initial_screen.png" width="800">

<b>Figura 2:</b> Schermata iniziale
</div>

Per dare inizio alla simulazione l'utente può utilizzare il pulsante *start* o alternativamente premere *Enter*.

Durante la simulazione è possibile regolare il valore dei parametri delle regole di volo e del raggio di visione dei boid tramite degli **slider**, osservando in tempo reale come tali modifiche si riflettono sui movimenti macroscopici degli stormi. Per tornare rapidamente ai valori impostati di default, è sufficiente premere il pulsante *reset*:

<div align="center">
<img src="utility/one_flock.png" width="800">

<b>Figura 3:</b> Frame di una simulazione con un solo stormo arancione costituito da 173 boid
</div>


Come è possibile notare dalla **Figura 3**, alle simulazioni è stata aggiunta la presenza costante di un **predatore**, differenziato dai boid per colore e dimensioni. Il suo ruolo è quello di inseguire gli stormi (su di esso è dunque applicata la regola della **coesione**), operando sui boid vicini una spinta di **repulsione**, analoga, in termini di implementazione, alla **separazione** ma di maggiore intensità.

### Componente stocastica
In ogni simulazione è equiprobabile la generazione di uno, due o tre **stormi diversi**, differenziati per colore. I possibili colori degli stormi e dei dettagli grafici della *top bar* sono <span style="color:rgb(0, 102, 204)">blu</span>, <span style="color:rgb(0, 153, 0)">verde</span> e <span style="color:rgb(255, 128, 0)">arancione</span>.

<div align="center">
<img src="utility/three_flocks.png" width="800">

<b>Figura 4:</b> Frame di una simulazione con 230 boid divisi in tre stormi diversi
</div>

### Output delle statistiche
Periodicamente, il programma stampa a schermo la **distanza media** dei boid (tenendo conto, se presenti, dei diversi stormi) e la **velocità media** di tutti i boid della simulazione, con relative **deviazioni standard**. Esempio:

```shell
Mean distance: (252.012 +/- 116.827) px 
Mean speed: (2.00593 +/- 0.184291) px/frameTime
```

L'implementazione di ciò si trova all'intero della funzione ```st::printStatistics()```, contenuta nel file ```statistics.hpp``` insieme ad alcune funzioni ausiliarie per il calcolo della media e della deviazione standard.

**Nota:** La *frequenza* della stampa a schermo dei parametri dipende dalle prestazioni mantenute dalla simulazione, alcuni dati di riferimento:
- Prestazioni standard (~115 fps fissi): ~25 secondi
- Prestazioni medio-basse (~70 fps fissi): ~40 secondi

## Logica di implementazione
Lo scheletro del programma è costituito da tre classi: ```bd::Boid```, ```sd::Slider``` e ```bt::Button```, tutte fortemente basate sulle classi fornite dalla libreria **SFML/Graphics** e contenute in ```boid.hpp```, ```slider.hpp``` e ```button.hpp``` rispettivamente.

### bd::Boid
È la classe fondamentale del programma, permette la rappresentazione e il movimento dei boid nella finestra. Internamente è costituita da una ```sf::ConvexShape```, definita in modo da ottenere la forma mostrata in [Figura 1](#descrizione-generale), e da un ```sf::Vector2f``` rappresentante la velocità del boid.

**Costruttore**
```cpp
// usato per i boid
bd::Boid::Boid(const sf::Color &color, const sf::Vector2f &position,
           const sf::Vector2f &velocity)
    : velocity_{velocity} {
  shape_.setPointCount(4);
  shape_.setPoint(0, sf::Vector2f(6.f, 0.f));
  shape_.setPoint(1, sf::Vector2f(-5.f, 4.f));
  shape_.setPoint(2, sf::Vector2f(-1.f, 0.f));
  shape_.setPoint(3, sf::Vector2f(-5.f, -4.f));
  shape_.setOrigin(0.f, 0.f);

  shape_.setFillColor(color);

  shape_.setPosition(position);
}
```

```cpp
// usato per il predatore
bd::Boid::Boid(const sf::Vector2f &position, const sf::Vector2f &velocity)
    : velocity_{velocity} {
  shape_.setPointCount(4);
  shape_.setPoint(0, sf::Vector2f(8.f, 0.f));
  shape_.setPoint(1, sf::Vector2f(-7.f, 6.f));
  shape_.setPoint(2, sf::Vector2f(-3.f, 0.f));
  shape_.setPoint(3, sf::Vector2f(-7.f, -6.f));
  shape_.setOrigin(0.f, 0.f);

  shape_.setFillColor(sf::Color(204, 0, 0));

  shape_.setPosition(position);
}
```

**Metodi principali**

- ```bool isCloseAndVisible()```

    Verifica che un boid si trovi all'interno del campo visivo del boid su cui è applicato;

- ```void maxVelocity()```

    Esegue un controllo sulla velocità del boid, se questa supera il valore impostato per la **velocità massima**, viene riportata al valore limite;

- ```void avoidBoundary()```

    Impone il [comportamento ai bordi](#comportamento-ai-bordi) di cui sopra;

- ```bool isFlockMate()```

    Verifica che un boid sia un compagno di stormo del boid su cui è applicato, la verifica consiste banalmente nel controllo dei colori dei due;

### sd::Slider
È la classe utilizzata per la creazione e il funzionamento degli slider a cui si è accennato in [dinamica di una simulazione](#dinamica-di-una-simulazione). Internamente è costituita da: una ```sf::RectangleShape``` che rappresenta la barra di scorrimento dello slider, una ```sf::CircleShape``` rappresentante invece il cursore dello slider, un ```sf::Text``` per mostrarne il titolo, una ```float&``` per legare lo slider ad un certo parametro e un ```const float``` per il valore di default del parametro.

**Costruttore**

```cpp
sd::Slider::Slider(const std::string &title, const sf::Font &font,
               const sf::Vector2f &line_size, const float dot_radius,
               const sf::Vector2f &position, float &parameter,
               const float default_value)
    : line_{line_size},
      dot_{dot_radius},
      parameter_{parameter},
      default_value_{default_value} {
  line_.setOrigin(line_size.x / 2, line_size.y / 2);
  dot_.setOrigin(dot_radius, dot_radius);

  title_.setFont(font);
  title_.setString(title);
  title_.setCharacterSize(15);

  line_.setPosition(position.x, position.y);
  dot_.setPosition(position.x, position.y);
  title_.setPosition(position.x - line_.getSize().x / 2, position.y - 28.f);
}
```

**Metodi principali**

- ```bool mouseIsOver()```

    Verifica che il mouse sia sopra allo slider, condizione necessaria affinchè il cursore possa essere mosso;

- ```void stayInRange()```

    Impedisce al cursore di muoversi oltre gli estremi della barra di scorrimento;

- ```void work()```

    Racchiude tutto il funzionamento dello slider, compresa la regolazione del parametro a cui lo slider è legato;

- ```void reset()```

    Riporta il cursore al centro della barra ed il parametro al suo valore di default, viene eseguito in seguito alla pressione del pulsante *reset* mostrato in e.g. [Figura 4](#componente-stocastica);

**Nota:** Il numero degli slider è stato limitato a quattro per esigenze di natura estetica, i parametri potenzialmente modificabili presenti nel programma sono molteplici e la classe ```sd::Slider``` è sufficientemente versatile. La particolare scelta fatta è assolutamente arbitraria.

### bt::Button
È una classe piuttosto semplice utilizzata per l'implementazione dei pulsanti *start* e *reset* visti in [dinamica di una simulazione](#dinamica-di-una-simulazione), il cui funzionamento richiede però un forte utilizzo di ```sf::Event```, classe di SFML che gestisce gli eventi della finestra grafica. Internamente è costituita da una ```sf::RectangleShape``` che dà la forma al pulsante e da un ```sf::Text``` che ne rappresenta l'etichetta.

**Costruttore**
```cpp
bt::Button::Button(const std::string &title, const sf::Font &font,
               const sf::Vector2f &rect_size, unsigned int text_size,
               const sf::Vector2f &position)
    : rect_{rect_size} {
  rect_.setOrigin(rect_.getSize().x / 2, rect_.getSize().y / 2);
  rect_.setPosition(position.x, position.y);

  text_.setFont(font);
  text_.setString(title);
  text_.setCharacterSize(text_size);
  text_.setStyle(sf::Text::Bold);
  text_.setOrigin(text_.getGlobalBounds().getSize() / 2.f +
                  text_.getLocalBounds().getPosition());
  text_.setPosition(position.x, position.y);
}
```

**Metodi principali**

- ```bool mouseIsOver()```
    
    Metodo analogo a ```sd::Slider::mouseIsOver()```;

### Rapida overview di main.cpp
```main.cpp``` è il file principale del programma, si cerca qui di riassumerne la struttura generale. Nell'ordine:

- Dichiarazione delle **variabili "globali"** del programma, incluse le dimensioni della finestra, i parametri delle regole di volo e gli slider ad essi collegati;
- Gestione della generazione dei **numeri casuali**, utilizzati per le posizioni e le velocità iniziali dei boid;
- Definizione di tutti gli elementi che compongono la **[schermata iniziale](#dinamica-di-una-simulazione)**;
- Gestione dei **[colori casuali](#componente-stocastica)** dei boid e costruzione del **predatore**;
- Gestione della **finestra** e dichiarazione degli elementi della **top bar**;
- **Game loop**
    - Gestione degli **```sf::Event```**;
    - **Game loop core**
        - Costruzione dei boid (avviene solo una volta);
        - Calcolo degli fps;
        - Funzionamento degli slider;
        - Gestione del movimento dei boid e del predatore;
    - **Rendering**;
    - Gestione dell'**[output delle statistiche](#output-delle-statistiche)**;
    - Gestione della **schermata iniziale**;

### Altre implementazioni aggiuntive

- **Angolo di vista:** con l'intento di rendere la simulazione più realistica, è stato definito un **angolo di vista** che impedisce ai boid di essere consapevoli di ciò che accade "alle loro spalle". L'implementazione si trova all'interno di [```bd::Boid::isCloseAndVisible()```](#boid). Il valore impostato di default è di 250°.  

- **Mouse left-click per generare un boid:** è possibile aggiungere dei nuovi boid a quelli generati all'inizio della simulazione **cliccando** in un qualsiasi punto della finestra, eccettuando la top bar. La posizione iniziale dei nuovi boid sarà quella del cursore del mouse, mentre la velocità iniziale ed il colore (compatibilmente al numero di stormi della simulazione) saranno casuali.  
Tale funzionalità è stata implementata grazie a ```sf::Event::MouseButtonReleased``` e ```sf::Mouse::Left```. Per evitare di perturbare l'equilibrio della simulazione e per mantenere accettabili le prestazioni, si consiglia di utilizzarla *cum grano salis*.  

- **Gestione del fuori focus:** quando la finestra *Boids Simulation* non è attiva la simulazione va **in pausa**. In particolare il moto dei boid si arresta e la finestra viene leggermente oscurata.  
Per ottenere questo risultato è stato definito un ```bool window_in_focus{1}```, il cui valore dipende dalla coppia di eventi ```sf::Event::GainedFocus``` e ```sf::Event::LostFocus```. L'intero [game loop core](#rapida-overview-di-maincpp) viene eseguito solo se ```window_in_focus == 1```. Inoltre, se ```window_in_focus == 0```, viene disegnata al di sopra di tutti gli altri elementi della finestra una ```sf::RectangleShape darkness``` delle dimensioni della finestra, di colore nero e opacità ridotta.

## Strategie di testing
In virtù dell'implementazione grafica, è stato possibile svolgere una parte considerevole del testing del programma tramite l'osservazione del comportamento dei vari elementi della finestra: boid, predatore, slider e pulsanti. 
Si è comunque rivelato utile utilizzare la libreria di testing **doctest** per eseguire dei test più approfonditi su alcune delle funzioni più importanti del codice, in particolare:

- ```bd::Boid::isCloseAndVisible()```
- ```bd::Boid::isFlockMate()```
- ```sd::Slider::reset()```
- ```st::printStatistics()```

Tali test si trovano all'interno di ```boids.test.cpp```. Comandi per eseguirli:

```shell
% pwd
/Users/dariocampisi/boids-campisi
% cmake -S . -B build
...
% cmake --build build
...
% ./build/boids.t
```

## Istruzioni per la compilazione
Si consiglia di compilare ed eseguire il programma tramite **CMake**, utilizzando il file ```CMakeLists.txt```. **Importante:** il programma è stato sviluppato su MacOS utilizzando la versione **2.6** di SFML (versione installata in automatico da Homebrew). Ubuntu 22.04, piattaforma di riferimento del corso, fa invece uso della versione **2.5**, per compilare su questa piattaforma (o più in generale, per compilare disponendo di una qualunque versione di SFML precedente alla 2.6) è dunque necessario modificare la **riga 22** del file ```CMakeLists.txt```.  

Comandi per compilare ed eseguire:

```shell
% pwd
/Users/dariocampisi/boids-campisi
% cmake -S . -B build
...
% cmake --build build
...
% ./build/boids
```

## Interpretazione dei risultati
Il programma funziona discretamente bene, il movimento dei boid è generalmente abbastanza simile a quello di uno **stormo reale**.

Gli slider assolvono correttamente alla loro funzione sebbene concedano, volutamente, poca libertà nel modificare i parametri a cui sono legati (questi ultimi possono subire un aumento/diminuzione del 50% al massimo). Questo perché dalla fase di [testing](#strategie-di-testing) del programma è emerso come una modifica eccessiva dei suddetti porti inevitabilmente a comportamenti errati da parte dei boid. Invero, quest'ultimo problema persiste anche nella versione finale del codice, sebbene in misura e frequenza molto ridotte. È infatti possibile che particolari combinazioni dei parametri delle regole di volo, unite ad un numero di boid molto elevato, generino comportamenti indesiderati, specialmente ai margini della finestra. Qualora questo accada, si consiglia di utilizzare l'apposito pulsante *reset*.

Rispettando la linea guida di non eccedere con il numero dei boid (specialmente tramite il [mouse left-click](#mouse-left-click-per-generare-un-boid)), le prestazioni del programma risultano sempre buone e stabili.

Per quanto concerne le statistiche stampate a schermo, esse sembrano essere coerenti con le aspettative, in particolare la **distanza media** sembra rispondere correttamente alle variazioni dei parametri delle regole di volo, mentre la **velocità media** è sempre inferiore alla velocità massima e ha una bassa **deviazione standard**.