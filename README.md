# Luftkvalitets-overvakning-i-inomhusmiljo



## Översikt

Detta projekts mål är att skapa en inomhusluftkvalitetsövervakare med hjälp av ESP32 och olika sensorer för att mäta PM2.5, CO2, TVOC, temperatur och luftfuktighet. För att sedan överföra den insamlade datan till ThingSpeak för visualisering och analys.
För närvarande stöds endast GP2Y1014AU-sensorn. ENS160 och AHT21 stöds inte.
## Komponenter

- **ESP32:** Huvudkontrollenheten och Wi-Fi-anslutning.
- **GP2Y1014AU-sensor:** Mäter PM2.5-partiklar i luften.
- **ENS160-sensor:** Mäter CO2-nivåer och TVOC.
- **AHT21-sensor:** Mäter temperatur och luftfuktighet.
- **ThingSpeak:** Molnbaserad plattform för lagring och visualisering av sensordata.
  
## ESP32-GP2Y1014AU Anslutning

- **Blå, V-LED:** Detta är VCC-pinnen för LED-lampan. Anslut denna pinne till 5V-pinnen på Arduino med en 150Ω Resistans.

- **Grön, LED-GND:** Detta är jordpinnen för LED-lampan. Anslut denna pinne till jordpinnen på Arduino.

- **Grå, LED:** Denna pinne kan användas för att slå av och på LED-lampan. Anslut den till valfri digital pinne på Arduino.

- **Gul, S-GND:** Detta är jordpinnen för dammsensor-modulen och den bör anslutas till jordpinnen på Arduino.

- **Svart, Vout:** Detta är utgången från dammsensor-modulen, du kan ansluta den till valfri analog pinne på Arduino.

- **Röd, VCC:** Strömpinnen för dammsensor-modulen ansluts till 5V- eller 3.3V-pinnen på Arduino.
![Arduino-Dust-Sensor-Connection](https://github.com/Timmieh99/Luftkvalitets-overvakning-i-inomhusmiljo/assets/60445245/b9235730-40ec-4e57-8ff3-dc0d865ce6ee)

## Användning

För att använda luftkvalitetsövervakaren, följ dessa steg:

1. Koppla ihop GP2Y1014AU-sensorn enligt bilden ovan.

2. Ladda ner filen `Luftkvalitets_overvakning.ino`.

3. Öppna `Luftkvalitets_overvakning.ino` i din Arduino IDE.

4. I koden, ändra följande variabler enligt din egen information:

   ```cpp
   const char *ssid = "DittWiFiSSID";
   const char *password = "DittWiFiLosenord";
   const char *thingspeakApiKey = "DinThingSpeakAPIKey";
   const unsigned long channelNumber = DittThingSpeakKanalnummer;


Efter framgångsrik installation kommer GP2Y1014AU-sensorn börja mäta luftkvaliteten och överföra data till ThingSpeak var 15de sekund så länge värdet är större än 0. Besök sedan din ThingSpeak-dashboard för att övervaka och analysera insamlad data

![thingspeak data](https://github.com/Timmieh99/Luftkvalitets-overvakning-i-inomhusmiljo/assets/60445245/9d3b1ab7-6b6b-419f-aac2-94ccdf642823)
