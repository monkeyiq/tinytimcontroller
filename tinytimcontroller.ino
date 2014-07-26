#include <Streaming.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

RF24 radio(9,10);
// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

enum { RMSG_JOYXY = 1 } RADIOMSGTYPES;
struct radiomsg {
  int type;
  union {
    struct {
      int h;
      int v;
    } joyxy;
    int dummy;
  };
};

byte pin_b1 = 2;
byte pin_b2 = 3;
byte pin_b3 = 4;
byte pin_b4 = 5;
byte pin_b5 = 6;

byte pin_joyv = 0;
byte pin_joyh = 1;

void setup()
{
  Serial.begin(57600);
    printf_begin();

  Serial << "starting up..." << endl;
  pinMode( pin_b1, INPUT );
  pinMode( pin_b2, INPUT );
  pinMode( pin_b3, INPUT );
  pinMode( pin_b4, INPUT );
  pinMode( pin_b5, INPUT );
  pinMode( pin_joyv, INPUT );
  pinMode( pin_joyh, INPUT );

  Serial << "starting radio..." << endl;
  radio.begin();
  radio.setRetries(15,15);
  // write mode.
      radio.openWritingPipe(pipes[0]);
      radio.openReadingPipe(1,pipes[1]);  
      
      
      
  radio.startListening();
  radio.printDetails();
  Serial << "starting loop..." << endl;

}

void loop() 
{
  int joyv =   analogRead( pin_joyv );
  int joyh =   analogRead( pin_joyh );
  
  radio.stopListening();
  struct radiomsg m;
  m.type = RMSG_JOYXY;
  m.joyxy.h = joyh;
  m.joyxy.v = joyv;
  bool ok = radio.write( &m, sizeof(radiomsg) );
  radio.startListening();
//  Serial << "sz:" << sizeof(radiomsg) << " send ok:" << ok << " joyh:" << joyh << " joyv:" << joyv << endl;
  
   
  delay(20);
}

