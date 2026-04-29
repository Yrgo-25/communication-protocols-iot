# L02 - Anteckningar
Lösningsförslag till övningsuppgifterna i [bilaga B](../appendix/b_exercises.md).

---

## Översikt
Implementationen består av implementation av en frame-parser via klassen `comm::frame::Parser`.

Följande funktionalitet har implementerats:
* `processByte()` parsar varje mottagen byte en efter en.
* `isFrameReady()` flaggar när en komplett frame har parsats.
* `extractFrame()` extraherar frame-data och lagrar i en instans av strukten `comm::frame::Frame`.

### Filer
* [comm/frame/def.h](./include/comm/frame/def.h) Innehåller protokolldefinitioner (offsetar storlekar på fält samt frame-typer).
* [comm/frame/frame.h](./include/comm/frame/frame.h): Innehåller deklaration av strukten `comm::frame::Frame`.
* [comm/frame/frame.cpp](./source/comm/frame/frame.cpp): Innehåller implementationsdetaljer för strukten `comm::frame::Frame`.
* [comm/frame/parser.h](./include/comm/frame/parser.h): Innehåller deklaration av klassen `comm::frame::Parser`.
* [comm/frame/parser.cpp](./source/comm/frame/parser.cpp): Innehåller implementationsdetaljer för klassen `comm::frame::Parser`.
* [main.cpp](./source/main.cpp): Innehåller testprogrammet som validerar frame-parsern.

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

