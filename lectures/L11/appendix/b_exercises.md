
# Bilaga B

## Övningsuppgifter

### **1.** Utöka transport-stubben
Lägg till följande i `comm::transport::Stub`:
* `bool myWaitingForAck`
* `std::uint16_t myExpectedSeq`
* `std::uin16_t myTimeoutCounter`
* `std::uint16_t myMaxTimeouts`.
* `std::uin8_t myRetryCounter`
* `std::uint8_t myMaxRetries`

* Se till att `myMaxTimeouts` initieras direkt via kontruktorn.

När en frame skickas:
* Sätt `myWaitingForAck` till `true`.
* Sätt `myExpectedSeq`.
* Initiera `myTimeoutCounter`.

---

### **2.** I `onRxFrame()`
Om frame är:
* `Type::Ack`
* Och `SEQ == myExpectedSeq`

Gör då följande:
* Stoppa timeout-räknaren.
* Sätt `myWaitingForAck` till `false`.
* Nollställ `myMaxRetries`.

---

### **3.** I `tick()`
Om:
* `myWaitingForAck == true`
* `myTimeoutCounter == 0`

Gör då följande:
* Om `myRetryCounter < myMaxRetries`
    * Skicka om senaste frame.
    * Inkrementera retry-counter.
    * Återställ timeout-counter.
* Annars: Rapportera permanent fel.

---

### **4.** Implementera dubblettdetektion
I noden:
* Spara senaste behandlade SEQ per avsändare.
* Om samma SEQ tas emot igen:
    * Kör inte applikationslogik.
    * Skicka ACK igen.

Verifiera att applikationskoden enbart körs en gång.

---

### **5.** Drop-test
* Droppa första framen.
* Timeout → retry.
* ACK på retry.

Verifiera att applikationskoden enbart körs en gång.

---

### **6.** Drop ACK-test
* Låt mottagaren ta emot frame.
* Droppa ACK.
* Timeout → retry.
* Mottagaren ska:
    * Inte köra applikationslogik igen.
    * Skicka ACK igen.

---
* Fördröj ACK med 2 tick.
* Justera timeout så att ingen onödig retry sker.

---

## Reflektion
Diskutera:
* Hur väljer man timeout-värde?
* Vad händer om timeout är för kort?
* Vad händer om timeout är för lång?

---
