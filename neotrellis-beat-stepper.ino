#include "Adafruit_NeoTrellisM4.h"
#include "trellis_flash_sampler.h"

// arduino board (from IDE): Adafruit Trellis M4 (SAMD51)

Adafruit_NeoTrellisM4 trellis = Adafruit_NeoTrellisM4();

unsigned int millisStepCount; // counter
unsigned int beatPeriod; // milliseconds between beats
uint8_t beat; // current beat
boolean lit_keys[32];
uint32_t Wheel(byte WheelPos);
uint32_t colors[4] = {Wheel(0), Wheel(191), Wheel(127), Wheel(63)};
unsigned long notesOffMils;


void setup() {
  // Serial.begin(115200);

  setBeatPeriod(375);
  millisStepCount = 0;
  beat = 0;

  trellis.begin();
  trellis.setBrightness(255);
  trellis.autoUpdateNeoPixels(false);

  for (uint8_t i=0; i<32; i++) {
    lit_keys[i] = false;
  }
  // for (uint8_t i=0; i<4; i++) colors[i] = Wheel(i * 255 / 4);

  // setup audio
  setupSampler();
}

void loop() {
  unsigned int nextMStep = millis() / beatPeriod;
  if (nextMStep != millisStepCount) {
    millisStepCount = nextMStep;
    advanceBeat();
  }

  trellis.tick();
  listen();
  // showColors();
  delay(10);
}


void setBeatPeriod(unsigned int p) {
  if (p < 100) beatPeriod = 100; // min is 0.1 sec
  else if (p > 4000) beatPeriod = 4000; // max is 4 seconds
  else beatPeriod = p;
}

void advanceBeat() {
  beat = ++beat % 8;
  showColors(); // move lit row
  playSynth(); // play sound(s)
}

void playSynth() {
  if (lit_keys[beat]) playZero();
  if (lit_keys[beat + 8]) playOne();
  if (lit_keys[beat + 16]) playTwo();
  if (lit_keys[beat + 24]) playThree();
}

void listen() {
  if(trellis.available()) {
    keypadEvent e = trellis.read();
    int key = e.bit.KEY;
    if(e.bit.EVENT == KEY_JUST_PRESSED) {
      lit_keys[key] = !lit_keys[key];
      showColors();
    }
  }
}

void showColors() {
  // showColors was causing background noise when calling in the loop function
  // it has something to do with the trellis.show() function
  // so,
  // only call it when LEDs need to be updated
  // such as on beat change and button press
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
        trellis.setPixelColor(i, dimColor(colors[row]));
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

uint32_t dimColor(uint32_t full) {
  return trellis.Color(full >> 18, (full & 0xff00) >> 10, (full & 255) >> 2);
}
