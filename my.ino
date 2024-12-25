#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is conntec to the Arduino digital pin 4
#define ONE_WIRE_BUS 4
#define LED_PIN 13



unsigned long previousMillis = 0;
const long interval = 1000;

const float lowTemp = 15.0;
const float highTemp = 25.0;

OneWire OneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&OneWire);

void setup()  {
  pinMode(LED_PIN, OUTPUT);
  
  digitalWrite(LED_PIN, LOW);

  sensors.begin();

}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    sensors.requestTemperatures();
    float currentTemp = sensors.getTempCByIndex(0);

    if (currentTemp <= lowTemp) {
      digitalWrite(LED_PIN, HIGH);
    } else if (currentTemp >= highTemp) {
      digitalWrite(LED_PIN, LOW);

    }
  }
}

