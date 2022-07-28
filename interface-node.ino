#include "pins.h"
#include "comms.h"



void init_pins(void);
void set_pins(uint8_t state);
void set_flow1(uint8_t state);
void set_flow2(uint8_t state);

void init_serial(void);


uint8_t comms_state;
uint8_t input_buffer;



void setup() {

  init_pins();
  init_serial();

  comms_state = 0;
}

void loop() {

  if(Serial.available()) {

    input_buffer = Serial.read();

    #ifdef ECHO
    Serial.write(input_buffer);
    #endif

    set_flow1(TRANSMIT);
    set_flow2(TRANSMIT);

    delay(TX_DELAY);

    Serial1.write(input_buffer);
    Serial2.write(input_buffer);

    delay(TX_DELAY);
    
    set_flow1(RECEIVE);
    set_flow2(RECEIVE);
  }

  if(Serial1.available()) {

    input_buffer = Serial1.read();
    Serial.write(input_buffer);
  }

  if(Serial2.available()) {

    input_buffer = Serial2.read();
    Serial.write(input_buffer);
  }
}

void setup1() {}

void loop1() {}



void init_pins() {

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
  
  digitalWrite(USR0, state & (1 << 0));
  digitalWrite(USR1, state & (1 << 1));
  digitalWrite(USR2, state & (1 << 2));
  digitalWrite(USR3, state & (1 << 3));

  digitalWrite(ERR, state & 0x3);
}


void init_serial() {

  pinMode(FL1, OUTPUT);
  pinMode(FL2, OUTPUT);
  digitalWrite(FL1, HIGH);
  digitalWrite(FL2, HIGH);

  Serial1.setTX(TX1);
  Serial1.setRX(RX1);
  set_flow1(RECEIVE);

  Serial2.setTX(TX2);
  Serial2.setRX(RX2);
  set_flow2(RECEIVE);

  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial.begin();
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
