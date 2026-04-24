# Bilaga A 

## I2C
**I2C (Inter‑Integrated Circuit)** är ett seriellt bussprotokoll som ofta används i inbyggda system för att koppla ihop flera integrerade kretsar på samma kretskort.

Protokollet utvecklades av Philips (numera NXP) och används idag i många typer av system, till exempel:
* Sensorer
* Minneskretsar
* ADC/DAC
* Realtidsklockor
* Displaykontrollers

I2C är särskilt populärt eftersom det kräver väldigt få signalledningar.

---

### I2C-bussens arkitektur
Ett I2C-system använder endast två signalledningar, som alla enheter är anslutna till.
* **SDA (Serial Data)**: För överföring av data.
* **SCL (Serial Clock Line)**: För generering av klockpulser.

Alla enheter är anslutna till samma två ledningar.

---

### Master och Slave
I2C använder en master/slave‑modell:
* **Master**:
    * Startar kommunikationen.
    * Genererar klocksignalen.
* **Slave**:
    * Svarar på förfrågningar.
    * Har en unik adress.

Exempel:

```text
      Master (mikrokontroller)
                  |
    ------------------------------
    |             |              |
Sensor         EEPROM        Display
Slave          Slave          Slave
```

---

### I2C-adresser
Varje slav på bussen identifieras normalt med en 7-bitars adress, som exempel:

```text
Enhet              Adress
------------------ --------
Temperatursensor   0x48
EEPROM             0x50
Display            0x3C
```

Den minst signifikanta biten i en byte används som R/W-bit, där
* `R/W = 0` betyder skrivning.
* `R/W = 1` betyder läsning.

Som exempel:
* En enhet kan ha adressen `0x48`.
* När adressen skickas på bussen skiftas den ett steg åt vänster (`0x48 << 1 = 0x90`).
* Vid skrivning skickas adressbyten `0x90`.
* Vid läsning skickas adressbyten `0x91`.

**Notering**: Många datablad anger I2C-adresser som 7-bitars adresser.
Den byte som skickas på bussen:
* Innehåller även R/W-biten.
* Består av sju adressbitar och en R/W-bit.

---

### ACK/NACK-bitar
När mastern vill kommunicera med en enhet skickar den först adressen.  
Endast den enhet som har rätt adress svarar (med ACK).

ACK skickas alltid av den enhet som tog emot den senaste byten. Som exempel:
* När mastern skickar data → Slaven skickar ACK.
* När slaven skickar data → Mastern skickar ACK.

Om ingen enhet svarar med ACK betyder det att ingen slav med den adressen finns på bussen.

Den sista byten i en läsoperation avslutas med en NACK från mastern för att indikera att ingen mer data ska skickas.

---

### Grundläggande kommunikationssekvens
En typisk I2C-kommunikation ser ut så här:

```text
START
ADDRESS + R/W
ACK
DATA
ACK
DATA
ACK
STOP
```

Förklaring:

| Signal  | Betydelse                  |
| ------- | -------------------------- |
| START   | Start på kommunikation     |
| ADDRESS | Slavens adress             |
| R/W     | Läs eller skriv            |
| ACK     | Bekräftelse från mottagare |
| DATA    | Data som skickas           |
| STOP    | Avslutar kommunikationen   |

Följande tidsschema visar hur SCL och SDA beter sig när en byte skickas på I2C-bussen:

```text
SCL:  _|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|
SDA:     A6  A5  A4  A3  A2  A1  A0  R/W  ACK
```

**Notering**: Databitarna ändras när SCL är låg och läses av när SCL är hög.  
Detta gör att alla enheter på bussen läser samma bitvärde vid varje klockpuls.

---

### START och STOP
Kommunikationen börjar med en START‑signal:

```text
SDA: 1 -> 0 medan SCL = 1
```

Mastern sänker alltså SDA till låg medan SCL är hög för att starta kommunikationen.

Kommunikationen avslutas med en STOP‑signal:

```text
SDA: 0 -> 1 medan SCL = 1
```

Mastern höjer alltså SDA till hög medan SCL är hög för att stoppa kommunikationen.

Dessa signaler gör att alla enheter på bussen vet när en överföring börjar och slutar.

Varje byte som skickas på I2C följs av en ACK/NACK-bit. Den mottagande enheten drar då SDA låg under den nionde klockpulsen för att indikera att byten togs emot korrekt.

---

### Exempel: Skrivning till en sensor
Anta att en I2C-master vill skriva ett registervärde till en sensor:
* Sensorn har I2C-adress `0x48`.
* Värdet `0x37` ska skrivas till register `0x01`.
* Eftersom skrivning ska genomföras sätts R/W-biten till `0`.

I2C-kommunikationen ser då ut såsom visas nedan:

```text
START
0x90 (address = 0x48 + R/W = 0)
ACK
0x01 (register)
ACK
0x37 (data)
ACK
STOP
```

Sekvensen betyder att mastern:
1.  Startar kommunikationen (SDA sätts till låg medan SCL är hög).
2.  Väljer skrivning till sensorn på adress `0x48`.
3.  Väntar på att sensorn verifierar att föregående byte tagits emot (med ACK).
4.  Anger vilket register som ska skrivas till (`0x01`).
5.  Väntar på att sensorn verifierar att föregående byte tagits emot (med ACK).
6.  Skickar data `0x37` till register `0x01`.
7.  Väntar på att sensorn verifierar att föregående byte tagits emot (med ACK).
8.  Stoppar kommunikationen (SDA sätts till hög medan SCL är hög).

---

### Exempel: Läsning från en sensor
För att läsa data används en något längre sekvens, då:
* Registret som ska läsas från först måste anges.
* Växling från skrivning till läsning måste ske (via repeated start).

Anta att en I2C-master nu vill läsa ett registervärde från samma sensor:
* Sensorn har I2C-adress `0x48`.
* Värdet i register `0x01` ska läsas.
* Eftersom läsning ska genomföras sätts R/W-biten till `1`.

I2C-kommunikationen ser då ut såsom visas nedan:

```text
START
0x90 (address = 0x48 + R/W = 0)
ACK
0x01 (register)
ACK
RESTART
0x91 (address = 0x48 + R/W = 1)
ACK
DATA
NACK
STOP
```

Sekvensen betyder att mastern:
1.  Startar kommunikationen (SDA sätts till låg medan SCL är hög).
2.  Väljer skrivning till sensorn på adress `0x48`.
3.  Väntar på att sensorn verifierar att föregående byte tagits emot (med ACK).
4.  Anger vilket register som ska läsas (`0x01`).
5.  Väntar på att sensorn verifierar att föregående byte tagits emot (med ACK).
6.  Startar om kommunikationen (växlar mellan skrivning och läsning).
7.  Väljer läsning från sensorn på adress `0x48`.
8.  Väntar på att sensorn verifierar att föregående byte tagits emot (med ACK).
9.  Läser data från sensorn.
10. Skickar NACK för att indikera att kommunikationen är färdig.
11. Stoppar kommunikationen (SDA sätts till hög medan SCL är hög).


---

### Pull‑up‑resistorer
I2C använder öppen kollektor / open‑drain‑signaler.  
Detta betyder att bussen behöver pull‑up‑resistorer.

```text
VCC
    |
[Pull‑up]
    |
SDA ----------- devices
    |
[Pull‑up]
    |
SCL ----------- devices
```

När ingen enhet aktivt drar linjen låg kommer pull-up-resistorerna att dra SDA och SCL till hög nivå.

---

### Representation i mjukvara
I programkod kan en I2C-transaktion representeras med en enkel struktur `i2c::Message`:

``` cpp
namespace i2c
{
struct Message
{
    static constexpr std::size_t DataLen{16U};
    std::uint8_t address{};
    std::uint8_t length{};
    std::uint8_t data[DataLen]{};
};
} // namespace i2c
```

| Fält    | Betydelse                |
| ------- | ------------------------ |
| address | Slavens adress på bussen |
| length  | Antal bytes som skickas  |
| data    | Payload                  |

**Notering**: Denna struktur representerar ett meddelande i mjukvara. På den fysiska I2C-bussen skickas samma data som en sekvens av bytes mellan START och STOP.

---

#### Exempel - I2C-meddelande

Nedan skapas ett I2C-meddelande för att skriva två bytes (``0x0180`) till en sensor på address `0x48`:

``` cpp
i2c::Message msg{};
msg.address  = 0x48U;
msg.length   = 2U;
msg.data[0U] = 0x01;
msg.data[1U] = 0x80;
```

Detta kan till exempel representera att en mikrokontroller skriver två bytes till en enhet med adress `0x48`.

---

#### Exempel - I2C-sensor
En enhet, här implementerad via en strukt `i2c::Device`, kan kontrollera om ett meddelande är adresserat till den:

``` cpp
namespace i2c
{
struct Device
{
    std::uint8_t address{};

    void onReceive(const Message& msg) noexcept
    {
        if (address == msg.address)
        {
            // Handle incoming data.
        }
    }
};
} // namespace i2c
```

--- 

#### Exempel: Temperaturgivare

Vi såg tidigare hur olika adresser kan sättas för olika enheter:

```text
Enhet              Adress
------------------ --------
Temperatursensor   0x48
EEPROM             0x50
Display            0x3C
```

När ett meddelande tas emot kan följande göras för en temperatursensor:
* Vi kollar om mottagaradressen är `0x48`.
* Om så är fallet läses registeradressen från payloaden.
* Om registeradressen är `0x00` returneras temperaturen.

``` cpp
if(0x48U == msg.address)
{
    std::uint8_t regAddress{msg.data[0U]};

    if(0x00U == regAddress)
    {
        // Return the temperature.
    }
}
```

---

### Vanliga hastigheter
I2C stöder flera standardhastigheter.


 | Mode            | Hastighet  |
| --------------- | ---------- |
| Standard Mode   | 100 kbit/s |
| Fast Mode       | 400 kbit/s |
| Fast Mode Plus  | 1 Mbit/s   |
| High Speed Mode | 3.4 Mbit/s |

I många inbyggda system används 100 kbit/s eller 400 kbit/s.

---

### Sammanfattning
Viktiga egenskaper hos I2C:
* Endast två ledningar (SDA och SCL).
* Master/slave‑arkitektur.
* Varje enhet har en unik adress.
* Start/stop‑baserad kommunikation.
* Enkel att använda för sensorer och kringkretsar.

Därför är I2C ett av de vanligaste protokollen för kommunikation mellan kretsar på samma kort.

---
