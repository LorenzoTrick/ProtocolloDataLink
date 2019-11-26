# Protocolli DataLink
Dispensa che contiene i vari moduli per creare un protocollo DataLink.__
Ricordarsi che:__
> A me interessa che capiate come si crea il protocollo, non che funzioni veramente.

## TODO
Sezione dedicata ai moduli ancora da implementare.__
La cosa migliore sarebbe creare un file per ogni sezione di questa TODO list, in modo da avere più metodi della stessa tipologia raggruppati assieme.__
Il nome del file è stato proposto tra parentesi dopo il nome della sezione.

**Le implementazioni dei metodi _dovranno_ essere tutte nel .h, per limitare il numero dei file su cui studiare.**

### Regolazione del flusso (flux.h):
- [ ]Heaven
- [ ]Stop&Wait (Simplex Stop&Wait)
- [ ]Stop&Wait per canali rumorosi (Simplex per canali rumorosi)
- [ ]Sliding Window (A finestra scorrevole)

Si ricorda che in questa sezione è necessario gestire l'invio e la ricezione degli ACK e l'implementazione della tecnica "piggybacking".__
I metodi di regolazione che implementano l'ACK sono lo Stop&Wait e la Sliding Window.__
Lo Stop&Wait per canali rumorosi implementa anche il "time-out".__
Per ora NON si implementa il piggybacking.

### Framing (framing.h):
- [ ]Character stuffing / Byte stuffing
- [ ]Bit stuffing

Si ricorda che il flag per il bit stuffing e i codici ASCII per il character stuffing possono essere aggiunti nel file "tools.h" per tenere tutto più organizzato.

### Rilevamento e correzione errori (checksum.h):
- [ ]Parity bit
- [ ]CRC (?)
- [ ]Parity bit matrix (?)

## Precisazioni
ALOHA e CSMA/CD (1-persistent, nonpersistent, p-persistent) fanno parte del sottolivello MAC e sono usati per l'accesso multiplo al canale. Servono, quindi, per capire chi può inviare dati sul mezzo fisico e per la rilevazione delle collisione.__
In quanto ciò non fa parte dell'LLC, non dovrebbe chiederlo.
