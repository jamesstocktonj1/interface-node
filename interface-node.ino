#include "pins.h"
#include "comms.h"



void init_pins(void);
void set_pins(uint8_t state);
void set_flow1(uint8_t state);
void set_flow2(uint8_t state);

void init_serial(void);



void setup() {

  init_pins();
  init_serial();
}

void loop() {
  
  Serial1.println("Hello World!");
  Serial2.println("Hello World!");
  digitalWrite(USR0, HIGH);
  delay(600);
  digitalWrite(USR0, LOW);
  delay(400);
}

void setup1() {}

void loop1() {}



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

void set_pins(uint8_t state) {
  
}


void init_serial() {

  pinMode(FL1, OUTPUT);
  pinMode(FL2, OUTPUT);
  digitalWrite(FL1, HIGH);
  digitalWrite(FL2, HIGH);

  Serial1.setTX(TX1);
  Serial1.setRX(RX1);
  set_flow1(TRANSMIT);

  Serial2.setTX(TX2);
  Serial2.setRX(RX2);
  set_flow2(RECEIVE);

  Serial1.begin(9600);
  Serial2.begin(9600);
}


void set_flow1(uint8_t state) {

  digitalWrite(FL1, state);

  digitalWrite(TXLED1, state);
  digitalWrite(RXLED1, !state);
}

void set_flow2(uint8_t state) {

  digitalWrite(FL2, state);

  digitalWrite(TXLED2, state);
  digitalWrite(RXLED2, !state);
}
