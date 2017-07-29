
#include <MPR121.h>      //include and define necessary stuff
#include <Wire.h>
#define MPR121_ADDR 0x5C
#define MPR121_INT 4
#include <HID-Project.h> //you'll need to download this one from Arduino library manager


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);        //make the LED an output
  Wire.begin();                        //begin and set up all the libraries
  MPR121.begin(MPR121_ADDR);
  MPR121.setInterruptPin(MPR121_INT);
  MPR121.setTouchThreshold(40);         // lower numbers make the board more sensitive
  MPR121.setReleaseThreshold(20);       // make sure that the release threshold is always
                                        // lower than the touch threshold
  MPR121.updateAll();
  System.begin();
  Consumer.begin();
}

void loop() {
  // this is where we do all the real work
  // only do anything if the overall touch status has changed
  if (MPR121.touchStatusChanged()) {
    // update our touch data to ensure it is valid
    MPR121.updateTouchData();
    // run through all electrodes
    for (int i = 0; i <= 11; i++) {
      if (MPR121.isNewTouch(i)) {           //Blink the LED when any electrode is touched   
        digitalWrite(LED_BUILTIN, HIGH);
      } else if (MPR121.isNewRelease(i)) {
        digitalWrite(LED_BUILTIN, LOW);
      }
      if (MPR121.isNewTouch(0)) {
        System.write(SYSTEM_SLEEP);        //if the 0th electrode was touched, press the sleep button
      }                                    //-the same thing basicly follows the following things...
      if (MPR121.isNewTouch(11)) {
        Consumer.press(MEDIA_VOLUME_MUTE);
      } else if (MPR121.isNewRelease(11)) { //only for the volume control you have to tap, continuous hold does
        Consumer.release(MEDIA_VOLUME_MUTE);//not work, i've tried to implement it, but it is too complicated...
      }
      if (MPR121.isNewTouch(10)) {
        Consumer.press(MEDIA_VOLUME_DOWN);
      } else if (MPR121.isNewRelease(10)) {
        Consumer.release(MEDIA_VOLUME_DOWN);
      }
      if (MPR121.isNewTouch(9)) {
        Consumer.press(MEDIA_VOLUME_UP);
      } else if (MPR121.isNewRelease(9)) {
        Consumer.release(MEDIA_VOLUME_UP);
      }
      //here you could add more keys if you feel short on them,
      //i think the pattern is more than obvious enough to assemble
      //the code yourself...
    }
  }
}
