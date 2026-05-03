# Bilaga A

## Felmodell för databussar
I verkliga inbyggda system är kommunikationen inte perfekt. Följande fel kan uppstå:
* En byte tappas (data försvinner).
* En byte ändras (bitflip).
* Bytes fördröjs.
* Frames kolliderar (på vissa bussar med flera sändare).

Dessa fel påverkar hur information tolkas hos mottagaren.

---

## Effekter av fel
Fel i byte-strömmen kan leda till:
* Felaktig tolkning av data.
* Att en frame inte kan tolkas alls.
* Att flera frames blandas ihop.
* Att system reagerar vid fel tidpunkt (fördröjning).

Det är därför viktigt att systemet kan hantera fel på ett kontrollerat sätt.

---

## Varför determinism?
Vid analys av fel vill man kunna upprepa samma scenario flera gånger.

Slumpmässiga fel leder till:
* Svår felsökning.
* Olika resultat mellan körningar.

Deterministiska fel innebär att:
* Samma fel uppstår varje gång.
* Beteendet blir förutsägbart.
* Systemet kan testas systematiskt.

Exempel på deterministiska fel:
* En specifik byte tappas varje gång.
* En viss bit ändras konsekvent.
* All data fördröjs lika mycket.

---

## Felmodell
En felmodell beskriver:
* Vilka typer av fel som kan uppstå.
* När de uppstår.
* Hur ofta de uppstår.

En enkel felmodell kan innehålla:
* Tappade bytes.
* Korrupt data.
* Fördröjning.

Syftet med felmodellen är att förstå hur systemet reagerar när kommunikationen inte fungerar som förväntat.

---

## Robusthet i system
Ett robust system ska:
* Ignorera felaktig data.
* Kunna återhämta sig efter fel.
* Fortsätta fungera när enstaka fel uppstår.

Detta är en förutsättning för att senare kunna införa tillförlitlighet, till exempel:
* Kvittens (ACK/NACK).
* Omsändning.
* Tidsgränser (timeouts).

---