#include <Streaming.h>
#include <SPI.h>
#include "printf.h"
#include "Switch.h"
#include "nRF24L01.h"
#include "RF24.h"
#include "monkeyiqrf24.h"

monkeyiqrf24 radio;

enum { RMSG_JOYXY = 1, RMSG_JOYBUTTON_DOWN = 10, RMSG_JOYBUTTON_UP = 11 } RADIOMSGTYPES;
struct radiomsg {
  int type;
  union {
    struct {
      int h;  // 0 (left) to 1023 (r ) with 512 at idle
      int v;  // 0 (down) to 1023 (up) with 512 at idle
    } joyxy;
    struct {
      int b;
    } joyb;
    int dummy;
  };
};

byte pin_a = 7;
byte pin_b = 6;
byte pin_c = 9;
byte pin_f = 8;
byte pin_e = 3;
byte pin_d = 4;
byte pin_joy = 5;
Switch swa( pin_a );
Switch swb( pin_b );
Switch swc( pin_c );
Switch swf( pin_f );
Switch swe( pin_e );
Switch swd( pin_d );
Switch swjoy( pin_joy );
byte pin_joyv = 0;
byte pin_joyh = 1;

void setup()
{
  Serial.begin(57600);
    printf_begin();

  Serial << "starting up..." << endl;
  pinMode( pin_a, INPUT );
  pinMode( pin_b, INPUT );
  pinMode( pin_c, INPUT );
  pinMode( pin_f, INPUT );
  pinMode( pin_e, INPUT );
  pinMode( pin_d, INPUT );
  pinMode( pin_joy, INPUT );
  pinMode( pin_joyv, INPUT );
  pinMode( pin_joyh, INPUT );

  Serial << "starting radio..." << endl;
  radio.setup();
  Serial << "starting loop..." << endl;

}

void loop() 
{
      swa.poll();  swb.poll();
      swc.poll();  swf.poll();
      swe.poll();  swd.poll();
      swjoy.poll(); 
      
      int b = 0;
      if( swa.pushed() )
        b = 1;
      if( swb.pushed() )
        b = 2;
      if( swc.pushed() )
        b = 3;
      if( swf.pushed() )
        b = 4;
      if( swe.pushed() )
        b = 5;
      if( swd.pushed() )
        b = 6;
      if( swjoy.pushed() )
        b = 10;        
    if( b )
    {
 //     Serial << "button pressed. b:" << b << endl;

      struct radiomsg m;
      m.type    = RMSG_JOYBUTTON_DOWN;
      m.joyb.b  = b;
      bool ok = radio.sendMessage( m );
    }
    b = 0;
      if( swa.released() )
        b = 1;
      if( swb.released() )
        b = 2;
      if( swc.released() )
        b = 3;
      if( swf.released() )
        b = 4;
      if( swe.released() )
        b = 5;
      if( swd.released() )
        b = 6;
      if( swjoy.released() )
        b = 10;     
    if( b )
    {
//      Serial << "button released. b:" << b  << endl;
      struct radiomsg m;
      m.type    = RMSG_JOYBUTTON_UP;
      m.joyb.b  = b;
      bool ok = radio.sendMessage( m );
    }
    
  
  int joyv = analogRead( pin_joyv );
  int joyh = analogRead( pin_joyh );
  
  struct radiomsg m;
  m.type    = RMSG_JOYXY;
  m.joyxy.h = joyh;
  m.joyxy.v = joyv;
  bool ok = radio.sendMessage( m );


  // debugs
  Serial << "sz:" << sizeof(radiomsg) << " send ok:" << ok << " joyh:" << joyh << " joyv:" << joyv << endl;

  // don't flood it.
  delay(20);
}

