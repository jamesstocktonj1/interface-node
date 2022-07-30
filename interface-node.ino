#include "pins.h"
#include "comms.h"



void init_pins(void);
void set_pins();
void set_flow1(uint8_t state);
void set_flow2(uint8_t state);

void init_serial(void);
uint8_t forward_data(void);


enum {
  COM_WAIT,
  COM_START,
  COM_MID,
  COM_REPLY,
  COM_END,
  COM_ACK,
  COM_NAK,
  COM_TIMEOUT
} comm_state;

//uint8_t comms_state;
uint8_t input_buffer;



void setup() {

  init_pins();
  init_serial();

  comm_state = COM_WAIT;
}

void loop() {

  switch(comm_state) {

    case COM_WAIT:
      if(Serial.available()) {
        input_buffer = forward_data();

        if(input_buffer == SEND_START_CHAR) {
          comm_state = COM_START;
        }
      }
      break;

    case COM_START:
      if(Serial.available()) {
        input_buffer = forward_data();

        comm_state = COM_MID;
      }
      break;

    case COM_MID:
      if(Serial.available()) {
        input_buffer = forward_data();
        
        if(input_buffer == END_CHAR) {
          comm_state = COM_END;
        }
      }
      break;

    case COM_END:
      if(Serial1.available()) {

        input_buffer = Serial1.read();
        Serial.write(input_buffer);

        if(input_buffer == REPLY_START_CHAR) {
          comm_state = COM_REPLY;
        }
      }

      if(Serial2.available()) {

        input_buffer = Serial2.read();
        Serial.write(input_buffer);

        if(input_buffer == REPLY_START_CHAR) {
          comm_state = COM_REPLY;
        }
      }
      break;

    case COM_REPLY:
      if(Serial1.available()) {

        input_buffer = Serial1.read();
        Serial.write(input_buffer);

        if(input_buffer == REPLY_ACK) {
          comm_state = COM_ACK;
        }
        else if(input_buffer == REPLY_NACK) {
          comm_state = COM_NAK;
        }
      }

      if(Serial2.available()) {

        input_buffer = Serial2.read();
        Serial.write(input_buffer);

        if(input_buffer == REPLY_ACK) {
          comm_state = COM_ACK;
        }
        else if(input_buffer == REPLY_NACK) {
          comm_state = COM_NAK;
        }
      }
      break;
    
    case COM_ACK:
    case COM_NAK:
      if(Serial1.available()) {

        input_buffer = Serial1.read();
        Serial.write(input_buffer);

        if(input_buffer == END_CHAR) {
          comm_state = COM_WAIT;
          set_flow1(TRANSMIT);
          set_flow2(TRANSMIT);
        }
      }

      if(Serial2.available()) {

        input_buffer = Serial2.read();
        Serial.write(input_buffer);

        if(input_buffer == END_CHAR) {
          comm_state = COM_WAIT;
          set_flow1(TRANSMIT);
          set_flow2(TRANSMIT);
        }
      }
      break;

    case COM_TIMEOUT:
      set_flow1(TRANSMIT);
      set_flow2(TRANSMIT);
      break;
  }

  set_pins();
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

void set_pins() {
  uint8_t state = 0;

  switch(comm_state) {
    case COM_WAIT:
      break;
    case COM_START:
      state = (1 << 2);
      break;
    case COM_MID:
    case COM_END:
      state = (1 << 2) | (1 << 3);
      break;

    case COM_REPLY:
      state = (1 << 3);
      break;
    case COM_ACK:
      state = (1 << 3);
      break;
    case COM_NAK:
      state = (1 << 3) | (1 << 1);
      break;
    case COM_TIMEOUT:
      state = (1 << 0);
      break;
  }
  
  if(state) {
    
    digitalWrite(USR0, state & (1 << 0));
    digitalWrite(USR1, state & (1 << 1));
    digitalWrite(USR2, state & (1 << 2));
    digitalWrite(USR3, state & (1 << 3));

    digitalWrite(ERR, state & 0x3);
  }
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

uint8_t forward_data() {

  uint8_t temp = Serial.read();

  Serial1.write(temp);
  Serial2.write(temp);

  return temp;
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
