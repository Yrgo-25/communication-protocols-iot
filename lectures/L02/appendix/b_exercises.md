# Bilaga B

## Övningsuppgifter – Byte-parsing

Följande övningsuppgifter bygger vidare på övningsuppgifterna från 
[L01](../../L01/appendix/b_exercises.md).

### **1.** Implementera en frame-parser
Lägg till given fil [comm/frame/parser.h](./code/cpp/parser.h) i ert projekt.  
Definiera metoddefinitionerna i en ny fil `comm/frame/parser.cpp`.

Parsern ska:
* **1.** Vänta på SOF byte 1.
* **2.** Läsa SOF byte 2. Om en felaktig byte tas emot ska parsern starta om från början.
* **3.** Läsa LEN.
* **4.** Läsa TYPE.
* **5.** Läsa DST.
* **6.** Läsa SRC.
* **7.** Läsa SEQ.
* **8.** Läsa payload (DATA) om tillgängligt.
* **0.** Läsa CHK byte 1.
* **10.** Läsa CHK byte 2.
* **11.** Markera att en komplett frame har mottagits via `isFrameReady()`.
* **12.** Ge tillgång till frame-data via `extractFrame()`, som även validerar framen via 
`Frame::deserialize()`.  
Om framen är ogiltig ska parsern återgå till att söka efter ny SOF.

Motsvarande parser skriven i C finns här:
* [comm/frame/parser.h](./code/c/include/comm/frame/parser.h)
* [comm/frame/parser.c](./code/c/source/comm/frame/parser.c)

---

### **2.** Sammankoppling
Använd bifogat testprogram i [main.cpp](./code/cpp/main.cpp), som gör följande:
* **1.** Skapar en PING-frame. 
* **2.** Serialiserar framen. 
* **3.** Matar in bytes en och en via en frame parser. 
* **4.** När framen har extraherats:
    * Anropar `handleFrame()` för att svara på mottagen frame.
    * Serialiserar mottagen frame (som bör vara en PONG-frame).
    * Skriver ut resultatet på hexadecimal form.
---

### **3.** Robust SOF-hantering (om tid finns)
Verifiera följande scenario:
* En byte `0xA5` (SOF1) tas emot.
* Nästa byte är inte `0xF7` (SOF2), utan exempelvis `0xA5` igen.
* Parsern ska kunna hantera detta utan att missa en potentiell frame-start:
    * Testa att starta om parsningen med rätt SOF efter att föregående parsning misslyckades.
    * Verifiera att parsningen lyckades.

---
