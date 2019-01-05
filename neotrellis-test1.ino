#include "Adafruit_NeoTrellisM4.h"

// arduino board (from IDE): Adafruit Trellis M4 (SAMD51)

// The NeoTrellisM4 object is a keypad and neopixel strip subclass
// that does things like auto-update the NeoPixels and stuff!
Adafruit_NeoTrellisM4 trellis = Adafruit_NeoTrellisM4();

unsigned int millisStepCount; // counter
unsigned int beatPeriod; // milliseconds between beats
uint8_t beat; // current beat
boolean lit_keys[32];
uint32_t colors[4];

void setup() {
  Serial.begin(115200);

  setBeatPeriod(400);
  millisStepCount = 0;
  beat = 0;

  trellis.begin();
  trellis.setBrightness(255);
  trellis.autoUpdateNeoPixels(false);


  Serial.println("Chris-mas present");
  for (uint8_t i=0; i<32; i++) {
    lit_keys[i] = false;
  }
  for (uint8_t i=0; i<4; i++) {
    colors[i] = Wheel(i * 255 / 4);
    Serial.print(i);
    Serial.print(" - ");
    Serial.print(colors[i]);
    Serial.print(" - ");
    Serial.print(halfColor(colors[i]));
  }
}

void loop() {
  unsigned int nextMStep = millis() / beatPeriod;
  if (nextMStep != millisStepCount) {
    millisStepCount = nextMStep;
    advanceBeat();
  }

  trellis.tick();
  listen();
  showColors();
  delay(5);
}


void setBeatPeriod(unsigned int p) {
  if (p < 100) beatPeriod = 100; // min is 0.1 sec
  else if (p > 4000) beatPeriod = 4000; // max is 4 seconds
  else beatPeriod = p;
}

void advanceBeat() {
  beat = ++beat % 8;
  Serial.print("the beat is now: ");
  Serial.println(beat);

  // TODO: Play sound(s)
}

void listen() {
  // while(trellis.available()) {
  if(trellis.available()) {
    keypadEvent e = trellis.read();
    int key = e.bit.KEY;
    if(e.bit.EVENT == KEY_JUST_PRESSED) {
      lit_keys[key] = !lit_keys[key];
      if (lit_keys[key]) Serial.print("Activating");
      else Serial.print("De-Activating");
      Serial.print(" key "); Serial.println(key);
      // break;
    }
  }
}

void showColors() {
  for (uint8_t i = 0; i <32; i++) {
    byte row = i / 8;
    if (i % 8 == beat) {
      if (lit_keys[i]) {
        // trellis.setPixelColor(i, Wheel(i * 255 / 32));
        trellis.setPixelColor(i, colors[row]);
      } else {
        // trellis.setPixelColor(i, 0x800000);
        trellis.setPixelColor(i, 0x444444);
      }
    } else {
      // key is not on current beat
      // light up if key is selected
      if (lit_keys[i]) {
        trellis.setPixelColor(i, halfColor(colors[row]));
      } else {
        trellis.setPixelColor(i, 0);
      }
    }
  }
  trellis.show();
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

uint32_t halfColor(uint32_t full) {
  return trellis.Color(full >> 18, (full & 0xff00) >> 10, (full & 255) >> 2);
}
