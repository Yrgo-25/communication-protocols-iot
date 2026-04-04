# Bilaga A

## Timeout, retries och dubbletthantering

### Timeout
I inbyggda system är det vanligt att följande görs när en nod skickar en frame som kräver ACK:
1. Framen skickas.
2. Noden startar sin interna timeout-räknare.
3. Noden väntar på ACK med matchande SEQ.
4. Om timeout uppnås skickas framen igen (retry).

I vår tick-drivna modell hade vi kunnat implementera detta såsom visas nedan:

```cpp
namespace comm::node
{
void Stub::tick() noexcept
{
    // Wait for ACK, resend frame on timeout.
    if (waitingForAck)
    {       
        if (myMaxTimeouts > myTimeoutCounter) { ++myTimeoutCounter; }
        else 
        { 
            retry();
            myTimeoutCounter = 0U; 
        }
    }
}
} // namespace comm::node
```

---

### Retry
Regler:
* Samma SEQ.
* Samma payload.
* Retry-counter inkrementeras.
* Max antal retries ska vara definierat (t.ex. 3).

Om max retries uppnås:
* Rapportera fel till applikationen.
* Stoppa vidare försök.

---

### Dubbletthantering
Problem:
Om ACK tappas:
* Sändaren skickar framen igen.
* Mottagaren får samma frame igen.

Lösning:
* Mottagaren måste känna igen dubbletter.
* Om `SEQ` redan behandlats/har registrerats:
    * Kör inte applikationslogik igen.
    * Skicka ACK igen.

---
