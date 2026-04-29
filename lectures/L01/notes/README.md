# L01 - Anteckningar
Lösningsförslag till övningsuppgifterna i [bilaga B](../appendix/b_exercises.md).

---

## Översikt
Implementationen består av en frame-representation via strukten `comm::frame::Frame`, i enlighet med given framespecifikation.

Följande funktionalitet har implementerats:
* En frame kan serialiseras till en byte-buffer via `serialize()`.
* En frame kan deserialiseras från en byte-buffer via `deserialize()`.
* Inkommande data valideras innan den accepteras.

### Filer
* [comm/frame/def.h](./include/comm/frame/def.h) Innehåller protokolldefinitioner (offsetar storlekar på fält samt frame-typer).
* [comm/frame/frame.h](./include/comm/frame/frame.h): Innehåller deklaration av strukten `comm::frame::Frame`.
* [comm/frame/frame.cpp](./source/comm/frame/frame.cpp): Innehåller implementationsdetaljer för strukten `comm::frame::Frame`.
* [main.cpp](./source/main.cpp): Innehåller testprogrammet som validerar frame-implementationen.

---

## Frame-struktur
Framen följer specifikationen i bilagan:

```text
[ SOF ][ LEN ][ TYPE ][ DST ][ SRC ][ SEQ ][ DATA ][ CHK ]
```


### Fält
* **SOF** – Start Of Frame (`0xA5F7`)
* **LEN** – Payload-längd
* **TYPE** – Meddelandetyp
* **DST** – Destinationsadress
* **SRC** – Källadress
* **SEQ** – Sekvensnummer (big endian)
* **DATA** – Payload
* **CHK** – Checksumma (summan av alla föregående bytes)

---

## Serialisering
Metoden `serialize()`:
* Skriver samtliga fält till en given buffer i korrekt ordning.
* Hanterar:
  * Big endian för flerbytesfält (SOF, SEQ).
  * Byte-för-byte checksumma.
* Returnerar:
  * Total framelängd vid lyckad serialisering.
  * `0` vid fel.

---

## Deserialisering
Metoden `deserialize()`:
* Tolkar inkommande byte-data till en `Frame`.
* Validerar:
  * SOF korrekt (`0xA5F7`).
  * Payload-längd inom giltig range (`0-MaxPayloadLen`).
  * Tillräcklig bufferstorlek (måste rymma hela framen).
  * Giltig frametyp (kontroll av det numeriska värdet).
  * Korrekt checksumma (matchning med beräknat värde).

Vid lyckad validering sparas datan i frame-instansen och `true` returneras.  
Vid fel returneras `false`.  

---

## Designval
* `Frame` representerar protokolldata.
* Serialisering och deserialisering hålls lokalt i strukten.
* Validering sker i samband med deserialisering.

---

## Test
Lösningen kan verifieras via testprogrammet:

```bash
make
```

Testet:
* Skapar en Ping.
* Verifierar mottagning.
* Skickar tillbaka en Pong.

---

