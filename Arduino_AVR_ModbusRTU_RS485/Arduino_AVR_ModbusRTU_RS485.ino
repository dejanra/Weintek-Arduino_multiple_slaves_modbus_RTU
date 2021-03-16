
/****Arduino MODBUS RTU***
 * 
 * dejan.rakijasic@gmail.com
 * 
 * Weintek cMT3103 conA - COM2 (DB9 sub) - RS485 module
 *  pin 1 (Data -) -> B (RS485 module)
 *  pin 2 (Data 2) -> A (RS485 module)
 * 
 * MODBUS library
 * https://github.com/smarmengol/Modbus-Master-Slave-for-Arduino
 */

#include <ModbusRtu.h>
#define ID   1  //2
// assign the Arduino pin that must be connected to RE-DE RS485 transceiver
#define TXEN  3 

//Modbus slave(ID, 0, 0); // this is slave ID and RS-232 or USB-FTDI
Modbus slave(ID,Serial,TXEN); // this is slave @1 and RS-485

boolean led;
int8_t state = 0;
unsigned long tempus;

// data array for modbus network sharing
uint16_t au16data[9];


void setup() {
  io_setup(); // I/O settings

  // start communication
    Serial.begin(9600 ); // baud-rate at 9600
  slave.start();
  tempus = millis() + 100;
}

void loop() {
  // poll messages
  // blink led pin on each valid message
  state = slave.poll( au16data, 9 );

  if (state > 4) {
    tempus = millis() + 50;
  //  digitalWrite(13, HIGH);
  }
  if (millis() > tempus) digitalWrite(13, LOW );

  // link the Arduino pins to the Modbus array
  io_poll();
} 


void io_setup() {
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
 
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
}

/**
 *  Link between the Arduino pins and the Modbus array
 */
void io_poll() {
  // get digital inputs -> au16data[0]
  bitWrite( au16data[0], 0, digitalRead( 4 ));
  bitWrite( au16data[0], 1, digitalRead( 5 ));


  // set digital outputs -> au16data[1]
  digitalWrite( 6, bitRead( au16data[1], 0 ));
  digitalWrite( 7, bitRead( au16data[1], 1 ));

  // set analog outputs
  analogWrite( 9, au16data[2] );
  analogWrite( 10, au16data[3] );

  // read analog inputs
  au16data[4] = analogRead( 0 );
  au16data[5] = analogRead( 1 );

  // diagnose communication
  au16data[6] = slave.getInCnt();
  au16data[7] = slave.getOutCnt();
  au16data[8] = slave.getErrCnt();
}
