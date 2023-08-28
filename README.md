# Simulazione del comportamento di stormi
**Autore:** Dario Eugenio Campisi  
**Data:** Settembre 2023

- [Descrizione generale](#descrizione-generale)
- [Logica di implementazione](#logica-di-implementazione)
- [Altre implementazioni aggiuntive](#implementazioni-aggiuntive)
- [Testing](#testing)
- [Istruzioni per la compilazione](#istruzioni-per-la-compilazione)
- [Interpretazione dei risultati](#interpretazione-dei-risultati)

## Descrizione generale
Il progetto è un tentativo di implementazione del programma **Boids**, sviluppato da Craig Reynolds nel 1986, facente uso della libreria grafica **SFML**. L'obiettivo del programma è quello di simulare il comportamento di un generico **stormo** (e.g. di uccelli), ovvero un sistema complesso costituito da singole entità, i *boid*, che rispettano delle precise regole di movimento, qui chiamate **regole di volo**.

<div align="center">
<img src="utility/boid_shape1.png" width="400">

<b>Figura 1:</b> Forma geometrica usata per la rappresentazione grafica dei boid
</div>

### Regole di volo
Il movimento nel piano di ogni boid è il risultato delle seguenti regole. In modo da favorire la formazione di più stormi indipendenti, esse vengono applicate unicamente ai boid considerati **vicini**, quelli cioè la cui distanza non supera un certo valore.

- **Separazione:** induce i boid a mantenere tra loro una certa distanza, impedendogli così di sovrapporsi
- **Allineamento:** fa sì che i boid seguano la stessa direzione dei vicini
- **Coesione:** spinge i boid a muoversi verso il centro di massa dello stormo

Da queste tre semplici regole emergono dei **comportamenti macroscopici caotici**, del tutto simili a quelli degli stormi reali.

### Comportamento ai bordi
Quando un boid comincia ad avvicinarsi ad uno dei quattro bordi della finestra, questo vira in modo da evitarlo. Per ottenere questo effetto è stato definito un **margine** nei pressi dei bordi, se superato la velocità del boid viene gradualmente invertita secondo un opportuno **fattore di virata**.

### Dinamica di una simulazione
All'avvio del programma viene mostrata una **schermata iniziale** che invita l'utente a scegliere il numero di boid che saranno generati (limitato ad un massimo di 300 per garantire delle buone prestazioni):

<div align="center">
<img src="utility/initial_screen.png" width="800">

<b>Figura 2:</b> Schermata iniziale
</div>

Per dare inizio alla simulazione l'utente può utilizzare il pulsante *start* o alternativamente premere *Enter*.

Durante la simulazione è possibile regolare il valore dei parametri delle regole di volo e del raggio di visione dei boid tramite degli **slider**, osservando in tempo reale come tali modifiche si riflettono sui movimenti macroscopici degli stormi. Per tornare rapidamente ai valori impostati di default, è sufficiente premere il pulsante *reset*.

<div align="center">
<img src="utility/one_flock.png" width="800">

<b>Figura 3:</b> Frame di una simulazione con un solo stormo arancione costituito da 173 boid
</div>


Come è possibile notare dalla **Figura 3**, alle simulazioni è stata aggiunta la presenza costante di un **predatore**, differenziato dai boid per colore e dimensioni. Il suo ruolo è quello di inseguire gli stormi operando sui boid vicini una spinta di **repulsione**.

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

**Nota:** La *frequenza* della stampa a schermo dei parametri dipende dalle prestazioni mantenute dalla simulazione, alcuni dati di riferimento:
- Prestazioni standard (~115 fps fissi): frequenza ≈ 25 secondi
- ~70 fps fissi: frequenza ≈ 40 secondi

## Logica di implementazione
Lo scheletro del programma è costituito da tre classi: **Boid**, **Slider** e **Button**, tutte fortemente basate sulle classi fornite dalla libreria **SFML/Graphics**.

### Boid
È la classe fondamentale del programma, permette la rappresentazione e il movimento dei boid nella finestra. Internamente è costituita da una ```sf::ConvexShape shape_```, definita in modo da ottenere la forma mostrata in [Figura 1](#descrizione-generale), e da un ```sf::Vector2f velocity``` rappresentante la velocità del boid.

**Costruttore**
```cpp
// usato per i boid
Boid::Boid(const sf::Color &color, const sf::Vector2f &position,
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
Boid::Boid(const sf::Vector2f &position, const sf::Vector2f &velocity)
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

- ```bool isCloseAndVisible(const Boid &other, const float d, const float angle_view) const```

    Verifica che i boid siano all'interno del campo visivo del boid su cui è applicato, così da applicare correttamente le regole di volo;

- ```void maxVelocity(const float max_velocity)```

    Esegue un controllo sulla velocità del boid, se questa supera il valore impostato per la velocità massima, viene riportata al valore limite;

- ```void avoidBoundary(const float window_width, const float window_height, const float turn_factor, const float margin)```

    Impone il [comportamento ai bordi](#comportamento-ai-bordi) di cui sopra;

- ```bool isFlockMate(const Boid &other) const```

    Verifica che i boid siano compagni di stormo del boid su cui è applicato, la verifica consiste banalmente nel controllo dei colori dei boid;

### Slider
È la classe utilizzata per la creazione e il funzionamento degli slider a cui si è accennato in [dinamica di una simulazione](#dinamica-di-una-simulazione). Internamente è costituita da: una ```sf::RectangleShape line_``` che rappresenta la barra di scorrimento dello slider, una ```sf::CircleShape dot_``` rappresentante invece il cursore, un ```sf::Text title_``` per mostrare il titolo, una ```float &parameter_``` per legare lo slider ad un certo parametro e un ```const float default_value_``` per il valore di default del parametro.

**Costruttore**

```cpp
Slider::Slider(const std::string &title, const sf::Font &font,
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

- ```bool mouseIsOver(const sf::RenderWindow &window) const```

    Verifica che il mouse sia sopra allo slider, condizione fondamentale affinchè il cursore possa essere mosso;

- ```void stayInRange()```

    Impedisce al cursore di muoversi oltre gli estremi della barra di scorrimento;

- ```void work(const sf::RenderWindow &window, const bool mouse_pressed)```

    Racchiude tutto il funzionamento dello slider, compresa la regolazione del parametro a cui lo slider è legato;

- ```void reset()```

    Riporta il cursore al centro della barra ed il parametro al suo valore di default, viene eseguito in seguito alla pressione del pulsante *reset* mostrato e.g. in [Figura 4](#componente-stocastica);

**Nota:** Il numero degli slider è stato limitato a quattro per esigenze di natura estetica, i parametri potenzialmente modificabili presenti nel programma sono molteplici e la classe Slider è sufficientemente versatile. La scelta fatta è assolutamente arbitraria.

### Button
È una classe piuttosto semplice utilizzata per l'implementazione dei pulsanti *start* e *reset* visti in [dinamica di una simulazione](#dinamica-di-una-simulazione), il cui funzionamento richiede però un forte utilizzo di ```sf::Event```. Internamente è costituita da una ```sf::RectangleShape rect_``` che dà la forma al pulsante e da un ```sf::Text text_``` che ne rappresenta l'etichetta.

**Costruttore**
```cpp
Button::Button(const std::string &title, const sf::Font &font,
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

- ```bool mouseIsOver(const sf::RenderWindow &window) const```
    
    Metodo analogo a ```Slider::mouseIsOver()```;

### Rapida overview di main.cpp
```main.cpp``` è il file principale del programma, costituito da circa 500 righe di codice, si cerca qui di riassumerne la struttura generale.

- Dichiarazione delle **variabili "globali"** del programma, incluse le dimensioni della finestra, i parametri delle regole di volo e gli slider ad essi collegati;
- Gestione della generazione dei **numeri casuali**, utilizzati per le posizioni e le velocità iniziali dei boid;
- Definizione di tutti gli elementi che compongono la **schermata iniziale**;
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
    - Gestione dell'**output delle statistiche**;
    - Gestione della **schermata iniziale**: viene mostrata fino a quando non viene premuto il pulsante *start*;

## Altre implementazioni aggiuntive
### Angolo di vista
Con l'intento di rendere la simulazione più realistica, è stato definito un **angolo di vista** che impedisce ai boid di essere consapevoli di ciò che accade "alle loro spalle". L'implementazione si trova all'interno di [```Boid::isCloseAndVisible()```](#boid). Il valore impostato di default è di 250°.

### Mouse left-click per generare un boid
È possibile aggiungere dei nuovi boid a quelli generati all'inizio della simulazione **cliccando** in un qualsiasi punto della finestra, eccettuando la top bar. La posizione iniziale dei nuovi boid sarà quella del cursore del mouse, mentre la velocità iniziale ed il colore (compatibilmente al numero di stormi della simulazione) saranno casuali.

Tale funzionalità è stata implementata grazie a ```sf::Event::MouseButtonReleased``` e ```sf::Mouse::Left```.

### Gestione del fuori focus
Quando la finestra *Boids Simulation* non è attiva la simulazione va **in pausa**. In particolare il moto dei boid si arresta e la finestra viene leggermente oscurata. 

Per ottenere questo risultato è stato definito un ```bool window_in_focus{1}```, il cui valore dipende dalla coppia di eventi ```sf::Event::GainedFocus``` e ```sf::Event::LostFocus```. L'intero [game loop core](#rapida-overview-di-maincpp) viene eseguito solo se ```window_in_focus == 1```. Inoltre, se ```window_in_focus == 0```, viene disegnata al di sopra di tutti gli altri elementi della finestra una ```sf::RectangleShape darkness``` delle dimensioni della finestra, di colore nero e opacità ridotta.

## Testing
### Strategie di testing
Grazie all'implementazione grafica, una parte considerevole del testing del programma è avvenuta tramite l'osservazione del comportamento di tutti gli elementi della finestra (boid, predatore, slider, pulsanti).  
Si è comunque rivelato utile utilizzare la libreria di testing **doctest** per eseguire dei test più approfonditi su alcune delle funzioni più importanti del programma, in particolare:

- ```Boid::isCloseAndVisible()```
- ```Boid::isFlockMate()```
- ```Slider::reset()```
- ```printStatistics()```

### Comandi per eseguire il testing
```shell
% pwd
/Users/dariocampisi/boids
% cmake -S . -B build
...
% cmake --build build
...
% ./build/boids.t
```

## Istruzioni per la compilazione
### Requisiti
Per compilare ed eseguire il programma sono richiesti **CMake v3.16** e **SFML v2.5**.  
Qualora si disponga di versioni precedenti si consiglia di modificare rispettivamente le righe ```1``` e ```22``` del file ```CMakeLists.txt```.

### Sequenza di comandi
```shell
% pwd
/Users/dariocampisi/boids
% cmake -S . -B build
...
% cmake --build build
...
% ./build/boids
```

## Interpretazione dei risultati
### Resa grafica e prestazioni

### Output delle statistiche