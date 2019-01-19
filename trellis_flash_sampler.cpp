/* See trellis_flash_sampler example for inspiration
 * The first 6 keys play samples.
 */

#include <Audio.h>
#include "trellis_flash_sampler.h"

// WAV files converted to code by wav2sketch
#include "AudioSampleSnare.h"        // http://www.freesound.org/people/KEVOY/sounds/82583/
#include "AudioSampleTomtom.h"       // http://www.freesound.org/people/zgump/sounds/86334/
#include "AudioSampleHihat.h"        // http://www.freesound.org/people/mhc/sounds/102790/
#include "AudioSampleKick.h"         // http://www.freesound.org/people/DWSD/sounds/171104/
// #include "AudioSampleGong.h"         // http://www.freesound.org/people/juskiddink/sounds/86773/
// #include "AudioSampleCashregister.h" // http://www.freesound.org/people/kiddpark/sounds/201159/



// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//
AudioPlayMemory    sound0;
AudioPlayMemory    sound1;
AudioPlayMemory    sound2;
AudioPlayMemory    sound3;
AudioMixer4        mix1;    // two 4-channel mixers are needed in
AudioOutputAnalogStereo  audioOut;

// Create Audio connections between the components
//
AudioConnection c1(sound0, 0, mix1, 0);
AudioConnection c2(sound1, 0, mix1, 1);
AudioConnection c3(sound2, 0, mix1, 2);
AudioConnection c4(sound3, 0, mix1, 3);
AudioConnection c5(mix1, 0, audioOut, 0);
AudioConnection c6(mix1, 0, audioOut, 1);


void setupSampler() {
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(10);

  // reduce the gain on mixer channels, so more than 1
  // sound can play simultaneously without clipping
  mix1.gain(0, 0.4);
  mix1.gain(1, 0.4);
  mix1.gain(2, 0.4);
  mix1.gain(3, 0.4);
}


void playZero() {
  sound0.play(AudioSampleHihat);
}
void playOne() {
  sound1.play(AudioSampleKick);
}
void playTwo() {
  sound2.play(AudioSampleTomtom);
}
void playThree() {
  sound3.play(AudioSampleSnare);
}
// void playFour() {
//   sound4.play(AudioSampleGong);
// }
// void playFive() {
//   sound5.play(AudioSampleCashregister);
// }
