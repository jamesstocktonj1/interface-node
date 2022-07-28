#include "pins.h"



void init_pins() {

    pinMode(FL1, OUTPUT);
    pinMode(FL2, OUTPUT);
    digitalWrite(FL1, HIGH);
    digitalWrite(FL2, HIGH);

    pinMode(TXLED1, OUTPUT);
    pinMode(RXLED1, OUTPUT);
    pinMode(TXLED2, OUTPUT);
    pinMode(RXLED2, OUTPUT);

    pinMode(USR0, OUTPUT);
    pinMode(USR1, OUTPUT);
    pinMode(USR2, OUTPUT);
    pinMode(USR3, OUTPUT);

    pinMode(ACT, OUTPUT);
    pinMode(ERR, OUTPUT);
    digitalWrite(ACT, HIGH);
    digitalWrite(ERR, LOW);
}


void setup() {

  init_pins();
}

void loop() {

  digitalWrite(USR0, HIGH);
  delay(600);
  digitalWrite(USR0, LOW);
  delay(400);
}

void setup1() {}

void loop1() {}
