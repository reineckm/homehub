/**
 * Steuerprogramm für das Senseo Add-On
 * Ablauf:
 *   Wenn High Pegel am IN_PIN gemessen wird,
 *   bewege das Servo am Power-Knopf der Senseo
 *   Maschine nach vorn und zurück, die Maschine
 *   heizt nun auf. Warte 2 Minuten so das sicher
 *   ist, das aufgeheizt wurde und bewege nun das
 *   Servo am "Großer Kaffe Knopf" nach vorn und
 *   zurück. Wiederhole diesen Schritt nach 10
 *   Sekunden da der Knopf recht schwergängig ist.
 *   Am Ende noch einmal das Power-Knopf Servo
 *   betätigen um die Maschine auszuschalten
 *
 * @author reineckm
 * @lastUpdate 09-28-2015
 */

#include <SoftRcPulseOut.h>
#define SERVO_POWERBTN_PIN 2
#define SERVO_COFFEBTN_PIN 0
#define IN_PIN 3
#define LED_PIN 1
#define SERVO_POWERBTN_ON 125
#define SERVO_POWERBTN_OFF 110
#define SERVO_COFFEBTN_ON 100
#define SERVO_COFFEBTN_OFF 70

SoftRcPulseOut ServoPwr;
SoftRcPulseOut ServoCoffe;

// Ports einstellen, Sevos auf Ruheposition
void setup()
{
  pinMode(IN_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  ServoPwr.attach(SERVO_POWERBTN_PIN);
  ServoCoffe.attach(SERVO_COFFEBTN_PIN);
  ServoPwr.write(SERVO_POWERBTN_OFF);
  ServoCoffe.write(SERVO_COFFEBTN_OFF);
  wait(500);
  ServoPwr.detach();
  ServoCoffe.detach();
}

// Prüfe auf 100ms durchgaengig High-Pegel am IN_PIN -> go()
int fade = 0;
void loop()
{
  digitalWrite(LED_PIN, LOW);
  int countHigh = 0;
  bool start = false;
  while (digitalRead(IN_PIN) == HIGH) {
    countHigh++;
    delay(10);
    if (countHigh > 10) {
      digitalWrite(LED_PIN, HIGH);
      go();
      break;
    }
  }
}

// Wartet dur Millisekunden.
void wait(long dur) {
  unsigned long start = millis();
  while ((millis() - start) < dur) {
    SoftRcPulseOut::refresh();
  }
}

// Kaffekochprogramm. Power drücken, warte auf auheizen, Kaffe drücken, 5 sek warten
// Maschine aus, 1 Sek warten, Maschine an, 10 Sekunden warten, Kaffe drücken, 90 Sek warten
// Maschine aus.
void go() {
  pushPwr();
  wait(90000);
  pushCoffe();
  wait(3000);
  pushPwr();
  wait(1000);
  pushPwr();
  wait(20000);
  pushCoffe();
  wait(20000);
  pushCoffe();
  wait(90000);
  pushPwr();
}

// Unterprogramm - Powerknopf drücken
void pushPwr() {
  ServoPwr.attach(SERVO_POWERBTN_PIN);
  ServoPwr.write(SERVO_POWERBTN_ON);
  wait(300);
  ServoPwr.write(SERVO_POWERBTN_OFF);
  wait(500);
  ServoPwr.detach();
}

// Unterprogramm - Kaffeknopf drücken
void pushCoffe() {
  ServoPwr.attach(SERVO_COFFEBTN_PIN);
  ServoPwr.write(SERVO_COFFEBTN_ON);
  wait(300);
  ServoPwr.write(SERVO_COFFEBTN_OFF);
  wait(500);
  ServoPwr.detach();
}
