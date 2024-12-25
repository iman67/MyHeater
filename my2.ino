#include <OneWire.h>
#include <DallasTemperature.h>

// تعریف پایه‌ها
#define ONE_WIRE_BUS 2  // پایه‌ای که سنسور DS18B20 به آن متصل است
#define LED_TEMP 13     // پایه‌ای که LED دما به آن متصل است
#define BUTTON_PIN 7    // پایه‌ای که کلید به آن متصل است
#define LED_BUTTON 12   // پایه‌ای که LED کلید به آن متصل است

// تنظیمات تایمر
unsigned long previousMillis = 0;
const long interval = 1000; // بازه زمانی به میلی‌ثانیه (1 ثانیه)

// دمای تنظیم شده
const float lowTemp = 15.0;  // دمای پایین برای روشن کردن LED
const float highTemp = 25.0; // دمای بالا برای خاموش کردن LED

// تنظیمات برای سنسور
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  // تنظیم پایه‌ها
  pinMode(LED_TEMP, OUTPUT);
  pinMode(LED_BUTTON, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // کلید با مقاومت داخلی پول‌آپ استفاده می‌شود

  // خاموش کردن LED‌ها در ابتدا
  digitalWrite(LED_TEMP, LOW);
  digitalWrite(LED_BUTTON, LOW);

  // شروع ارتباط با سنسور
  sensors.begin();
}

void loop() {
  // بررسی وضعیت کلید
  if (digitalRead(BUTTON_PIN) == LOW) { // کلید فشرده شده
    digitalWrite(LED_BUTTON, HIGH);     // روشن کردن LED کلید
  } else {
    digitalWrite(LED_BUTTON, LOW);      // خاموش کردن LED کلید
  }

  // بررسی تایمر
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // خواندن دما از سنسور
    sensors.requestTemperatures();
    float currentTemp = sensors.getTempCByIndex(0); // دما از اولین سنسور

    // کنترل LED بر اساس دما
    if (currentTemp <= lowTemp) {
      digitalWrite(LED_TEMP, HIGH); // روشن کردن LED دما
    } else if (currentTemp >= highTemp) {
      digitalWrite(LED_TEMP, LOW);  // خاموش کردن LED دما
    }
  }
}