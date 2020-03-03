#include "FastLED.h"
#define NUM_LEDS 20
CRGB leds[NUM_LEDS];
#define PIN 2
int Shifting = 0;

void setup()
{
  FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
}

//shift convertion. Allow to move start of LED sequence arround the hat and not at the begining of the strip
int shift(int pixel) {
  int newpixel=pixel+Shifting;
  if (newpixel>NUM_LEDS){newpixel=newpixel-NUM_LEDS;}
  return newpixel;
}

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}


//*********************  Effects section  *********************

//FBI warning
void FBI(){
  for (int turn=0;turn<3;turn++){
    for (int a=0; a<NUM_LEDS/2;a++) {setPixel(shift(a), 0xff, 0, 0);}
    showStrip();
    delay(35);
    setAll(0,0,0);
    delay(35);
    }
    
  for (int turn=0;turn<3;turn++){
    for (int a=NUM_LEDS/2; a<NUM_LEDS;a++) {setPixel(shift(a), 0, 0, 0xff);}
    showStrip();
    delay(35);
    setAll(0,0,0);
    delay(35);
    }
}

//Gyrophare
void Gyro() {
  int delayval = 10; // timing delay in milliseconds
  int nbled = 7;
  int nbTours = random(10, 25);
  for (int tours = 0; tours < nbTours; tours++) {
      int red=random(0, 255);
      int green=random(0, 255);
      int blue=random(0, 255);
    // début d'un aller
    for (int i = 0; i < NUM_LEDS + nbled + 1; i++) {
      if (i < NUM_LEDS) {setPixel(i, red, green, blue);}
        
      if (i >= nbled) {setPixel(i - nbled, 0, 0, 0);}
      showStrip();
      delay(delayval);
    }
  }
}


//Cyclone
void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){

  for(int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for(int i = NUM_LEDS-EyeSize-2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
 
  delay(ReturnDelay);
}

//Sparkle
void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel,red,green,blue);
  showStrip();
  delay(SpeedDelay);
  setPixel(Pixel,0,0,0);
}

//Running Light
void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position=0;
 
  for(int j=0; j<NUM_LEDS*2; j++)
  {
      Position++; // = 0; //Position + Rate;
      for(int i=0; i<NUM_LEDS; i++) {
        // sine wave, 3 offset waves make a rainbow!
        //float level = sin(i+Position) * 127 + 128;
        //setPixel(i,level,0,0);
        //float level = sin(i+Position) * 127 + 128;
        setPixel(i,((sin(i+Position) * 127 + 128)/255)*red,
                   ((sin(i+Position) * 127 + 128)/255)*green,
                   ((sin(i+Position) * 127 + 128)/255)*blue);
      }
     
      showStrip();
      delay(WaveDelay);
  }
}

//Rainbow Cycle
void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS; i++) {
      c=Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
    }
    showStrip();
    delay(SpeedDelay);
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];
 
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}

//Meteor rain
void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {  
  setAll(0,0,0);
 
  for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {
   
   
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
   
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
        setPixel(i-j, red, green, blue);
      }
    }
   
    showStrip();
    delay(SpeedDelay);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
   
    oldColor = strip.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
   
    strip.setPixelColor(ledNo, r,g,b);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue );
 #endif  
}


// Let's start the Loop!
void loop() {
  for (int cycle=0;cycle<2;cycle++){RunningLights(0xff,0xA2,0x03, 50);}        // orange
  setAll(0,0,0);
  delay(1000);
  for (int cycle=0;cycle<20;cycle++){FBI();}
  setAll(0,0,0);
  delay(1000);
  for (int cycle=0;cycle<5;cycle++){CylonBounce(0xff, 0, 0, 4, 10, 50);}
  setAll(0,0,0);
  delay(1000);
  for (int cycle=0;cycle<2000;cycle++){Sparkle(255, 255, 255, 0);}
  setAll(0,0,0);
  delay(1000);
  //for (int cycle=0;cycle<2;cycle++){RunningLights(0,0,0xff, 50);}        // blue
  Gyro();
  setAll(0,0,0);
  delay(1000);
  rainbowCycle(10); //~25 sec
  setAll(0,0,0);
  delay(1000);
  for (int cycle=0;cycle<6;cycle++){meteorRain(0xff,0xff,0xff,10, 64, true, 30);}
  setAll(0,0,0);
  delay(1000);

}
