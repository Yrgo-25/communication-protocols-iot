# Bilaga C

## MQTT
**MQTT (Message Queuing Telemetry Transport)** är ett lättviktsprotokoll för meddelandebaserad kommunikation över nätverk. Det används ofta i IoT-system, där många enheter behöver skicka data till varandra eller till en server.

MQTT är särskilt populärt eftersom det:
* Använder lite bandbredd.
* Är enkelt att implementera.
* Fungerar bra över instabila nätverk.
* Stödjer många samtidiga enheter.

Typiska användningsområden:
* Smarta hem.
* Sensornätverk.
* Industriell IoT.
* Fjärrövervakning.

MQTT används i denna kurs som ett **högre lager ovanpå vårt eget protokoll**:
* Vårt protokoll: Transport på buss (frames, routing, tillförlitlighet).
* MQTT: Kommunikation över nätverk (WiFi, broker, topics).

---

### Public/Subscribe-modellen
Till skillnad från många andra protokoll använder MQTT **publish/subscribe**-modellen:
* Enheter kommunicerar inte direkt med varandra.
* Istället används en **broker** (server).

```text
Sensor ----\
            \
            MQTT Broker ---- Dashboard
            //
Aktuator ---//
```

* Roller i systemet:
    * **Publisher**: Skickar data.
    * **Subscriber**: Tar emot data.
    * **Broker**: Central server som distribuerar meddelanden.
    * **Dashboard**: Visar mottagen data.

---

### Topics
I MQTT organiseras meddelanden i så kallade **topics**:
* En topic fungerar som en kanal eller ett ämne, exempelvis:

```text
home/temperature
home/livingroom/light
factory/machine1/status
```

* En enhet kan publicera (**publish**) data till en topic.
* Andra enheter kan prenumerera (**subscribe**) på samma topic.

---

### Exempel på kommunikation
Anta att en temperatursensor skickar temperaturvärden:

```text
publish
topic: home/temperature
payload: 22.5
```

Dashboarden visar då följande:

```text
subscribe
topic: home/temperature
```

När sensorn publicerar ett värde distribuerar brokern detta till alla prenumeranter.

---

### MQTT-meddelande
Ett MQTT-meddelande innehåller i huvudsak:
* **Topic**
* **Payload**
* **QoS (Quality of Service)**

Payloaden kan bestå av:
* Text
* JSON
* Binär data

Exempel:

```text
Topic: factory/machine1/rpm
Payload: 1450
```

---

### Quality of Service (QoS)
MQTT definierar tre nivåer för leveransgaranti:

| QoS | Betydelse               | Funktion                                   |
|-----|-------------------------|--------------------------------------------|
| 0   | Skickas utan garanti    | Snabbast, men ingen garanti att det når fram |
| 1   | Levereras minst en gång | Kan ge dubbletter                          |
| 2   | Levereras exakt en gång | Säkrast, men mest komplext                 |

---

### Retained messages
En broker kan lagra det **senaste meddelandet** för en topic. Om en ny klient prenumererar på topicen skickas detta meddelande direkt.

Exempel:

```text
Topic: home/temperature
Retained payload: 21.8
```

En ny dashboard får då direkt det senaste värdet.

---

### Last Will and Testament
MQTT har en funktion för att upptäcka om en klient kopplas bort oväntat:
* När en klient ansluter kan den ange ett **Last Will**-meddelande.
* Om klienten försvinner skickar brokern detta meddelande.

Exempel:

```text
Topic: device/status
Payload: offline
```

Detta används ofta för att övervaka system.

---

### Representation i mjukvara
I många bibliotek arbetar man med enkla funktioner.

Exempel i C++:

``` cpp
mqtt.publish("home/temperature", "22.5");
```

Prenumeration:

``` cpp
mqtt.subscribe("home/temperature");
```

När ett meddelande tas emot:

``` cpp
// Callback vid mottaget meddelande
void onMessage(const char* topic, const char* payload)
{
    // hantera data
}
```

---

### MQTT i inbyggda system

MQTT används ofta tillsammans med mikrokontrollers som:
* ESP32
* ESP8266
* Raspberry Pi

Vanliga MQTT-brokers:
* Mosquitto
* HiveMQ
* EMQX

En ESP32 kan exempelvis:
* Läsa en sensor.
* Publicera värdet via WiFi.
* Skicka data till en MQTT-broker.

---

### Sammanfattning

Viktiga egenskaper hos MQTT:
* Publish/subscribe-modell.
* Central broker.
* Topics för datakanaler.
* Låg bandbreddsanvändning.
* Flera nivåer av leveransgaranti.

MQTT används därför ofta i IoT-system och distribuerade sensornätverk.

---
