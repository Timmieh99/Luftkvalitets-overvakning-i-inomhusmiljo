#include <WiFi.h>
#include <ThingSpeak.h>

const char *ssid = "";// WiFi-nätverkets namn (SSID)
const char *password = "";// WiFi-lösenord
const char *thingSpeakApiKey = "";// ThingSpeak API-nyckel
unsigned long channelNumber = ;// ThingSpeak-kanalnummer

#define dustPin 36                    // Pin-nummer för dammsensor
#define ledPin 14                     // Pin-nummer för LED

float ADbits = 4095.0f;               // Maximalt värde för ADC (12 bitar)
float uPvolts = 5.0f;                 // Spänningsreferens för ADC (5V)
float adcRawValue = 0.0f;             // Råa ADC-värden från dammsensorn
float dustDensity = 0.0f;             // Beräknad dammtäthet (ug/m3)
const float K = 0.5f;                 // Konstant för omvandling från spänning till dammtäthet
unsigned long readingInterval = 15000; // Avläsningsintervall (15 sekunder)
unsigned long startTime = 0;          // Tidpunkten när avläsningen startar
int readingsCount = 0;                // Antal avläsningar under intervallen
// Summa av sensorvärden för medelvärdesberäkning
float sumDustDensity = 0.0;
float sumAdcRawValue = 0.0;
float sumCO2 = 0.0;
float sumTemp = 0.0;
float sumHumidity = 0.0;   
float sumTVOC = 0.0; 

WiFiClient client; // Skapar en instans av WiFiClient för att hantera anslutningen till WiFi

void connectToWiFi() {
  Serial.println("Connecting to WiFi..."); // Skriver ut meddelande om pågående anslutning till WiFi
  WiFi.begin(ssid, password); // Initierar anslutningen till WiFi med angivna SSID och lösenord
  int attempts = 0;

  // Väntar på att anslutningen till WiFi ska upprättas, eller tills maximalt antal försök har uppnåtts
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(1000); 
    Serial.print("."); 
    attempts++;
  }
  Serial.println();

  // Kontrollerar om anslutningen lyckades och skriver ut ifall det lyckades eller inte
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi");
  } else {
    Serial.println("Failed to connect to WiFi. Check your credentials or network.");
  }

  ThingSpeak.begin(client); // Inleder anslutningen till ThingSpeak med hjälp av WiFiClient-instansen
}


void setup() {
  Serial.begin(9600); // Startar den seriella kommunikationen med baudrate 9600
  while (!Serial)
    delay(100); // Väntar på att den seriella kommunikationen ska upprättas helt

  pinMode(ledPin, OUTPUT); // Sätter ledPin som en utgång (OUTPUT)

  connectToWiFi(); // Anropar funktionen för att ansluta till WiFi-nätverket
}

void loop() {
  if ((millis() - startTime) >= readingInterval) {
    startTime = millis(); // Återställer starttiden för avläsningsintervallet
    readingsCount = 0; // Återställer antalet avläsningar för den aktuella intervallen
    // Återställer summan av all sensor data
    sumDustDensity = 0.0;
    sumAdcRawValue = 0.0;
    sumCO2 = 0.0;
    sumTemp = 0.0;
    sumHumidity = 0.0;
    sumTVOC = 0.0;

    // Samla in data från sensorn under 15 sekunder
    while ((millis() - startTime) < readingInterval) {
      // Läser in det råa ADC-värdet från dammsensorn
      digitalWrite(ledPin, HIGH);
      delayMicroseconds(280);
      adcRawValue = analogRead(dustPin); 
      delay(1);
      digitalWrite(ledPin, LOW);

      adcRawValue = (adcRawValue * uPvolts) / ADbits; // Omvandlar ADC-värdet till spänning
      dustDensity = (adcRawValue / K) * 100.0; // Beräknar dammtätheten baserat på spänningen
      // Genererar slumpmässiga värden som exempel
      float CO2Value = random(400, 500); // Exempelvärden för CO2 i ppm
      float tempValue = random(22, 25); // Exempelvärden för temperatur i Celsius
      float humidityValue = random(30, 45); // Exempelvärden för luftfuktighet i %
      float TVOCValue = random(100, 200); // Exempelvärden för TVOC i ppb

      if (dustDensity > 0.0f) { // Utesluter nollvärden
        // Lägger till sensordata till summan för medelvärdesberäkning
        sumDustDensity += dustDensity;
        sumAdcRawValue += adcRawValue;
        sumCO2 += CO2Value;
        sumTemp += tempValue;
        sumHumidity += humidityValue;
        sumTVOC += TVOCValue;
        readingsCount++; // Ökar antalet avläsningar
      }

      // Skriver ut värdena i Serial Monitor
      Serial.print("Voltage = ");
      Serial.print(adcRawValue);
      Serial.print(",\tDust Density = ");
      Serial.print(dustDensity);
      Serial.print(" ug/m3");
      Serial.print(",\tCO2 = ");
      Serial.print(CO2Value);
      Serial.print(",\tTemperature = ");
      Serial.print(tempValue);
      Serial.print(",\tHumidity = ");
      Serial.print(humidityValue);
      Serial.print(",\tTVOC = ");
      Serial.println(TVOCValue);

      delay(1000); // En sekund mellan varje avläsning
    }

    // Beräkna medelvärden om det finns några giltiga värden
    if (readingsCount > 0) {
      float averageDustDensity = sumDustDensity / readingsCount;
      float averageAdcRawValue = sumAdcRawValue / readingsCount;
      float averageCO2 = sumCO2 / readingsCount;
      float averageTemp = sumTemp / readingsCount;
      float averageHumidity = sumHumidity / readingsCount;
      float averageTVOC = sumTVOC / readingsCount;

      // Lägger till varje separat sensor data i ett enskilt fält
      ThingSpeak.setField(1, averageDustDensity);
      ThingSpeak.setField(2, averageCO2);
      ThingSpeak.setField(3, averageTemp);
      ThingSpeak.setField(4, averageHumidity);
      ThingSpeak.setField(5, averageTVOC);
      // Skickar alla fält till thingspeak kanalen
      int writeSuccess = ThingSpeak.writeFields(channelNumber, thingSpeakApiKey);
      if (writeSuccess == 200) {
        Serial.println("Data skickat till ThingSpeak!");
      } else {
        Serial.println("Fel vid skickande av data till ThingSpeak...");
      }
    }
  }
}
