#include <avr/sleep.h>
#include <avr/interrupt.h>

const int inputPin = 1; // PB1 - ورودی تحریک
const int relayPin = 3; // PB3 - خروجی رله

volatile bool wakeUpFlag = false;
volatile bool timerRunning = false;

unsigned long lastTriggerTime = 0;
unsigned long relayStartTime = 0;
int triggerCount = 0;

void setup() {
  pinMode(inputPin, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  // فعالسازی وقفه تغییر پایه
  GIMSK |= (1 << PCIE);
  PCMSK |= (1 << PCINT1);

  sei(); // فعال کردن وقفه کلی
}

void loop() {
  if (!wakeUpFlag && !timerRunning) {
    goToSleep();
  } else if (wakeUpFlag) {
    wakeUpFlag = false;

    unsigned long currentTime = millis();

    if (currentTime - lastTriggerTime <= 2000) {
      triggerCount++;
    } else {
      triggerCount = 1;
    }

    lastTriggerTime = currentTime;

    if (triggerCount >= 3) {
      digitalWrite(relayPin, HIGH);
      relayStartTime = millis();
      timerRunning = true;
      triggerCount = 0;
      lastTriggerTime = 0;
    }
  }

  if (timerRunning) {
    if (millis() - relayStartTime >= 1000) { // بعد از ۱ ثانیه
      digitalWrite(relayPin, LOW);
      timerRunning = false;
    }
  }
}

// رفتن به خواب عمیق
void goToSleep() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_cpu();
  sleep_disable();
}

// وقفه تغییر پایه ورودی
ISR(PCINT0_vect) {
  wakeUpFlag = true;
}