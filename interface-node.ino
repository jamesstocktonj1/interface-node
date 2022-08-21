#include "pins.h"
#include "comms.h"



void init_pins(void);
void set_pins(void);
void reset_leds(void);
void set_flow1(uint8_t state);
void set_flow2(uint8_t state);

void init_serial(void);
void timeout_reply(void);

void init_timer(void);
void reset_timer(void);

void parse_usb_serial(char c);
void parse_serial(char c);


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
uint16_t timer1;



void setup() {

  init_pins();
  init_serial();
  init_timer();

  comm_state = COM_WAIT;
}

void loop() {

  while(Serial.available()) {
    parse_usb_serial(Serial.read());
  }

  while(Serial1.available()) {
    parse_serial(Serial1.read());
  }

  while(Serial2.available()) {
    parse_serial(Serial2.read());
  }

  if(timer1 == 0) {

    set_flow1(TRANSMIT);
    set_flow2(TRANSMIT);

    if(comm_state == COM_END) {
      comm_state = COM_TIMEOUT;
      timeout_reply();
    }
    else if(comm_state != COM_WAIT) {
      comm_state = COM_WAIT;
      reset_leds();
    }
  }
}

void setup1() {}

void loop1() {

  if (timer1) {
    timer1--;
  }
  
  set_pins();

  delay(1);
}



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
  digitalWrite(ACT, LOW);
  digitalWrite(ERR, LOW);
}

void set_pins() {
  uint8_t state = 0;

  switch (comm_state) {
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

  if (state) {

    digitalWrite(USR0, state & (1 << 0));
    digitalWrite(USR1, state & (1 << 1));
    digitalWrite(USR2, state & (1 << 2));
    digitalWrite(USR3, state & (1 << 3));

    digitalWrite(ERR, state & 0x3);
  }
  digitalWrite(ACT, timer1);
}

void reset_leds() {

  digitalWrite(USR0, LOW);
  digitalWrite(USR1, LOW);
  digitalWrite(USR2, LOW);
  digitalWrite(USR3, LOW);

  digitalWrite(ACT, LOW);
  digitalWrite(ERR, LOW);
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
  set_flow2(TRANSMIT);

  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial.begin();
}

void timeout_reply() {

  Serial.write("RTE\n");
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


void init_timer() {

  timer1 = 0;
}

void reset_timer() {

  timer1 = COMMS_TIMEOUT;
}


void parse_usb_serial(char c) {
  reset_timer();
  
  Serial.write(c);
  Serial1.write(c);
  Serial2.write(c);

  if(c == SEND_START_CHAR) {
    comm_state = COM_START;
  }
  else if (c == END_CHAR) {
    comm_state = COM_END;
    
    Serial1.flush();
    Serial2.flush();
    
    delay(TX_DELAY);
    set_flow1(RECEIVE);
    set_flow2(RECEIVE);
  }
  else {
    comm_state = COM_MID;
  }
}

void parse_serial(char c) {
  reset_timer();

  Serial.write(c);

  if(c == REPLY_START_CHAR) {
    comm_state = COM_REPLY;
  }
  else if(c == REPLY_ACK) {
    comm_state = COM_ACK;
  }
  else if(c == REPLY_NACK) {
    comm_state = COM_NAK;
  }
  else if(c == END_CHAR) {
    comm_state = COM_WAIT;
    
    set_flow1(TRANSMIT);
    set_flow2(TRANSMIT);

    Serial.flush();
    Serial.write("\n");
  }
}
