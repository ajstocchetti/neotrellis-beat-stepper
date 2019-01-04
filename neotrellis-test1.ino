#include "Adafruit_NeoTrellisM4.h"
#include "sequencer.h"

// arduino board (from IDE): Adafruit Trellis M4 (SAMD51)

// The NeoTrellisM4 object is a keypad and neopixel strip subclass
// that does things like auto-update the NeoPixels and stuff!
Adafruit_NeoTrellisM4 trellis = Adafruit_NeoTrellisM4();
Sequencer sequencer;

boolean lit_keys[32];
uint8_t beat;

void setup(){
  Serial.begin(115200);

  trellis.begin();
  trellis.setBrightness(80);
  trellis.autoUpdateNeoPixels(false);
  sequencer.begin();


  Serial.println("Chris-mas present");
  beat = 7; // start at max

  for (uint8_t i=0; i<32; i++) {
    lit_keys[i] = false;
  }


}

void loop() {
  sequencer.run();

  // put your main code here, to run repeatedly:
  beat = ++beat % 8;
  // Serial.print("the beat is: ");
  // Serial.println(beat);


  trellis.tick();
  trellis.fill(0);
  // trellis.setPixelColor(beat, 0x800000);
  listen();
  showColors();
  trellis.show();

  delay(10);
  // delay(250);
}





void listen() {
  while(trellis.available()) {
    keypadEvent e = trellis.read();
    int key = e.bit.KEY;
    if(e.bit.EVENT == KEY_JUST_PRESSED) {
      lit_keys[key] = !lit_keys[key];
      if (lit_keys[key]) Serial.print("Activating");
      else Serial.print("De-Activating");
      Serial.print(" key "); Serial.println(key);
      break;
    }
  }
}

void showColors() {
  for (uint8_t i = 0; i <32; i++) {
    if (lit_keys[i]) {
      trellis.setPixelColor(i, Wheel(i * 255 / 32)); // rainbow!
    }
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return trellis.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return trellis.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return trellis.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


//
// static inline void acknowledgeInterrupt()
// {
//   Serial.println("ack interrupt");
// 	if (SEQ_TC->COUNT16.INTFLAG.bit.MC0 == 1) {
// 		SEQ_TC->COUNT16.INTFLAG.bit.MC0 = 1;
// 	}
// }
//
// // this gets called when the timer fires
// void SEQ_Handler()
// {
// 	acknowledgeInterrupt();
// }
