/* Chris Telford 18/05/2018.
 * Wheelchair LED effects code. This is designed to allow someone to fit their wheelchair with WS2812 LEDs.
 * Different patterns can be cycled through using a button. Brightness is controlled by a potentiometer. 
 * Some of the patterns are for show and some (the strobes) might be useful in terms of visibility at night. 
 * Please note; effects and effects switcher code taken from here; https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/ 
 * https://www.tweaking4all.com/forums/topic/arduino-all-led-effects-in-one-sketch/
 * https://www.tweaking4all.com/downloads/arduino/AllEffects_FastLED.ino.zip
 * Altered slightly with pattern changes, additions, and the dimmer switch.
 */



#include "FastLED.h"        // FastLED Library.
#include <EEPROM.h>         // EEPROM Library.
#define NUM_LEDS 14         // Change here for longer strings of LEDs
CRGB leds[NUM_LEDS];

#define PIN 6               // Pin for LED data line.
#define BUTTON 3            // Button for mode changes. Has to be either 2 or 3 on most arduinos for the code to work properly.

int buttonState = LOW;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 15  ;    // the debounce time; increase if the output flickers

byte selectedEffect=0;      // Keeps track of display mode.

//Code for brightness potentiometer knob
int potPin = A0;            // select the input pin for the potentiometer for Overall Brightness.
int potValue = 0;           // variable to store the value coming from the potentiometer.
int brightnessValue = 0;    // variable for brightness value (remapped pot value for 255).

void setup()
{
  FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );       // Standard FastLED setup for WS2812.
  //digitalWrite (BUTTON, HIGH);                                                            // Internal pull-up resistor.
  pinMode(BUTTON, INPUT);
  attachInterrupt (digitalPinToInterrupt (BUTTON), changeEffect, CHANGE);                   // pressed
  //Serial.begin(9600);                                                                     // open the serial port at 9600 bps:
}



void loop() 
{ 
  potValue = analogRead(potPin);                                                            // Read the potentiometer values.
  brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
  FastLED.setBrightness(brightnessValue);                                                   // Set master brightness control for fastled.
  
  EEPROM.get(0,selectedEffect);                                                             // Read mode from EEPROM (for some reason).
  
  if(selectedEffect>20) {                                                                   // Change number here when adding effects. Loops back to 0.
    selectedEffect=0;
    EEPROM.put(0,0);    
  } 
 
     
// *********************************************************************
// ** List / Order of Effects. Add new ones here. Add functions below.**
// *********************************************************************
 
  switch(selectedEffect) {
    
    case 0  : {
                // RGBLoop - no parameters
                RGBLoop();
                changeEffect();
                break;
                
              }
              
    case 1  : {
                Black();
                changeEffect();
                break;
                
              }          

    case 2  : {
                // FadeInOut - Color (red, green. blue)
                FadeInOut(0xff, 0x00, 0x00); // red
                FadeInOut(0x00, 0xff, 0x00); // green 
                FadeInOut(0x00, 0x00, 0xff); // blue
                changeEffect();
                break;
              }
              
    case 3  : {
                // Strobe - Color (red, green, blue), number of flashes, flash speed, end pause
                //Strobe(0xff, 0xff, 0xff, 3, 250, 400);
                WhiteStrobe();
                changeEffect();
                break;
              }

    case 4  : {
               // Strobe - Color (red, green, blue), number of flashes, flash speed, end pause
                //Strobe(0xff, 0x00, 0x00, 3, 250, 400);
                RedStrobe();
                changeEffect();
                break;
              }
              
    case 5  : {
                // CylonBounce - Color (red, green, blue), eye size, speed delay, end pause
                CylonBounce(0xff, 0x00, 0x00, 4, 20, 50);
                changeEffect();
                break;
              }
              
    case 6  : {
                // NewKITT - Color (red, green, blue), eye size, speed delay, end pause
                NewKITT(0xff, 0x00, 0x00, 8, 10, 50);
                changeEffect();
                break;
              }
              
    case 7  : {
                // Twinkle - Color (red, green, blue), count, speed delay, only one twinkle (true/false) 
                Twinkle(0xff, 0x00, 0x00, 10, 100, false);
                changeEffect();
                break;
              }
              
    case 8  : { 
                // TwinkleRandom - twinkle count, speed delay, only one (true/false)
                TwinkleRandom(25, 100, false);
                changeEffect();
                break;
              }
              
    case 9  : {
                // Sparkle - Color (red, green, blue), speed delay
                Sparkle(0xff, 0x00, 0xff, 0);
                changeEffect();
                break;
              }
               
    case 10  : {
                // SnowSparkle - Color (red, green, blue), sparkle delay, speed delay
                SnowSparkle(0x10, 0x10, 0x10, 20, random(100,1000));
                changeEffect();
                break;
              }
              
    case 11 : {
                // Running Lights - Color (red, green, blue), wave dealy
                RunningLights(0xff,0x00,0x00, 50);  // red
                RunningLights(0x00,0xff,0x00, 50);  // green
                RunningLights(0x00,0x00,0xff, 50);  // blue
                changeEffect();
                break;
              }
              
    case 12 : {
                // colorWipe - Color (red, green, blue), speed delay
                colorWipe(0x00,0xff,0x00, 50);
                colorWipe(0xff,0x00,0xff, 50);
                changeEffect();
                break;
              }

    case 13 : {
                // rainbowCycle - speed delay
                rainbowCycle(2);
                changeEffect();
                break;
              }
    
    case 14 : {
                // rainbowCycle - speed delay
                rainbowCycle(15);
                changeEffect();
                break;
              }
              

    case 15 : {
                // theatherChase - Color (red, green, blue), speed delay
                theaterChase(0xff,0,0,50);
                theaterChase(0,0xff,0,50);
                theaterChase(0,0,0xff,50);
                changeEffect();
                break;
              }

    case 16 : {
                // theaterChaseRainbow - Speed delay
                theaterChaseRainbow(50);
                changeEffect();
                break;
              }

    case 17 : {
                // Fire - Cooling rate, Sparking rate, speed delay
                Fire(55,120,15);
                changeEffect();
                break;
              }


              // simple bouncingBalls not included, since BouncingColoredBalls can perform this as well as shown below
              // BouncingColoredBalls - Number of balls, color (red, green, blue) array, continuous
              // CAUTION: If set to continuous then this effect will never stop!!! 
              
    case 18 : {
                // mimic BouncingBalls
                byte onecolor[1][3] = { {0xff, 0x00, 0x00} };
                BouncingColoredBalls(1, onecolor, false);
                changeEffect();
                break;
              }

    case 19 : {
                // multiple colored balls
                byte colors[3][3] = { {0xff, 0x00, 0x00}, 
                                      {0xff, 0xff, 0xff}, 
                                      {0x00, 0x00, 0xff} };
                BouncingColoredBalls(3, colors, false);
                changeEffect();
                break;
              }

    case 20 : {
                // meteorRain - Color (red, green, blue), meteor size, trail decay, random trail decay (true/false), speed delay 
                meteorRain(0xff,0xff,0xff,10, 64, true, 30);
                changeEffect();
                break;
              }
  }
}



// Change modes / effects. When button is pressed selected effect is incrimented and stored to EEPROM for some reason.
void changeEffect() {
    int reading = digitalRead(BUTTON);
    if (reading != lastButtonState) {
      lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > debounceDelay) {
       if (reading != buttonState) {
        buttonState = reading;

        if (buttonState == HIGH) {
   
    selectedEffect++;
    EEPROM.put(0, selectedEffect);
    asm volatile ("  jmp 0");
    
   }
    }
    }
    lastButtonState = reading;
  
}

// ******************************************
// ** LED Effect Functions from here down. **
// ******************************************
void Black(){
  fill_solid( leds, NUM_LEDS, CRGB::Black);
  showStrip();
}

//Custom Visibility Strobe in White (Chris Telford).
void WhiteStrobe()  {
        fill_solid( leds, NUM_LEDS, CRGB::White);
        potValue = analogRead(potPin);                                                            // Read the potentiometer values.
        brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
        FastLED.setBrightness(brightnessValue);  
        showStrip();
        delay(150);
        fill_solid( leds, NUM_LEDS, CRGB::Black);
        potValue = analogRead(potPin);                                                            // Read the potentiometer values.
        brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
        FastLED.setBrightness(brightnessValue);  
        showStrip();
        delay(50);
        fill_solid( leds, NUM_LEDS, CRGB::White);
        potValue = analogRead(potPin);                                                            // Read the potentiometer values.
        brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
        FastLED.setBrightness(brightnessValue);  
        showStrip();
        delay(150);
        fill_solid( leds, NUM_LEDS, CRGB::Black);
        potValue = analogRead(potPin);                                                            // Read the potentiometer values.
        brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
        FastLED.setBrightness(brightnessValue); 
        showStrip();
        delay(50);
        fill_solid( leds, NUM_LEDS, CRGB::White);
        potValue = analogRead(potPin);                                                            // Read the potentiometer values.
        brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
        FastLED.setBrightness(brightnessValue); 
        showStrip();
        delay(150);
        fill_solid( leds, NUM_LEDS, CRGB::Black);
        potValue = analogRead(potPin);                                                            // Read the potentiometer values.
        brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
        FastLED.setBrightness(brightnessValue); 
        showStrip();
        delay(50);
        fill_solid( leds, NUM_LEDS, CRGB::White);
        potValue = analogRead(potPin);                                                            // Read the potentiometer values.
        brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
        FastLED.setBrightness(brightnessValue); 
        showStrip();
        delay(200);
        fill_solid( leds, NUM_LEDS, CRGB::Black);
        potValue = analogRead(potPin);                                                            // Read the potentiometer values.
        brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
        FastLED.setBrightness(brightnessValue); 
        showStrip();
        potValue = analogRead(potPin);                                                            // Read the potentiometer values.
        brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
        FastLED.setBrightness(brightnessValue);                                                   // Set master brightness control for fastled.
        delay(200);                                                                               // long end delay.
       
}

//Custom Visibility Strobe in Red (Chris Telford).
void RedStrobe()  {
        fill_solid( leds, NUM_LEDS, CRGB::Red);
        potValue = analogRead(potPin);                                                            // Read the potentiometer values.
        brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
        FastLED.setBrightness(brightnessValue);  
        showStrip();
        delay(150);
        fill_solid( leds, NUM_LEDS, CRGB::Black);
        potValue = analogRead(potPin);                                                            // Read the potentiometer values.
        brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
        FastLED.setBrightness(brightnessValue);  
        showStrip();
        delay(50);
        fill_solid( leds, NUM_LEDS, CRGB::Red);
        potValue = analogRead(potPin);                                                            // Read the potentiometer values.
        brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
        FastLED.setBrightness(brightnessValue);  
        showStrip();
        delay(150);
        fill_solid( leds, NUM_LEDS, CRGB::Black);
        potValue = analogRead(potPin);                                                            // Read the potentiometer values.
        brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
        FastLED.setBrightness(brightnessValue); 
        showStrip();
        delay(50);
        fill_solid( leds, NUM_LEDS, CRGB::Red);
        potValue = analogRead(potPin);                                                            // Read the potentiometer values.
        brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
        FastLED.setBrightness(brightnessValue); 
        showStrip();
        delay(150);
        fill_solid( leds, NUM_LEDS, CRGB::Black);
        potValue = analogRead(potPin);                                                            // Read the potentiometer values.
        brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
        FastLED.setBrightness(brightnessValue); 
        showStrip();
        delay(50);
        fill_solid( leds, NUM_LEDS, CRGB::Red);
        potValue = analogRead(potPin);                                                            // Read the potentiometer values.
        brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
        FastLED.setBrightness(brightnessValue); 
        showStrip();
        delay(200);
        fill_solid( leds, NUM_LEDS, CRGB::Black);
        potValue = analogRead(potPin);                                                            // Read the potentiometer values.
        brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
        FastLED.setBrightness(brightnessValue); 
        showStrip();
        potValue = analogRead(potPin);                                                            // Read the potentiometer values.
        brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
        FastLED.setBrightness(brightnessValue);                                                   // Set master brightness control for fastled.
        delay(200);                                                                               // long end delay.
       
}

void RGBLoop(){
   for(int j = 0; j < 3; j++ ) { 
    // Fade IN
    for(int k = 0; k < 256; k++) { 
      switch(j) { 
        case 0: setAll(k,0,0); break;
        case 1: setAll(0,k,0); break;
        case 2: setAll(0,0,k); break;
      }
      potValue = analogRead(potPin);                                                            // Read the potentiometer values.
      brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
      FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
      showStrip();
      delay(3);
    }
    // Fade OUT
    for(int k = 255; k >= 0; k--) { 
      switch(j) { 
        case 0: setAll(k,0,0); break;
        case 1: setAll(0,k,0); break;
        case 2: setAll(0,0,k); break;
      }
      potValue = analogRead(potPin);                                                            // Read the potentiometer values.
      brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
      FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
      showStrip();
      delay(3);
    }
  }
}

void FadeInOut(byte red, byte green, byte blue){
  float r, g, b;
      
  for(int k = 0; k < 256; k=k+1) { 
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    potValue = analogRead(potPin);                                                            // Read the potentiometer values.
    brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
    FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
    showStrip();
  }
     
  for(int k = 255; k >= 0; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    potValue = analogRead(potPin);                                                            // Read the potentiometer values.
    brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
    FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
    showStrip();
  }
}

void Strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++) {
    setAll(red,green,blue);
    showStrip();
    delay(FlashDelay);
    setAll(0,0,0);
    potValue = analogRead(potPin);                                                            // Read the potentiometer values.
    brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
    FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
    showStrip();
    delay(FlashDelay);
  }
 
 delay(EndPause);
}

void HalloweenEyes(byte red, byte green, byte blue, 
                   int EyeWidth, int EyeSpace, 
                   boolean Fade, int Steps, int FadeDelay,
                   int EndPause){
  randomSeed(analogRead(0));
  
  int i;
  int StartPoint  = random( 0, NUM_LEDS - (2*EyeWidth) - EyeSpace );
  int Start2ndEye = StartPoint + EyeWidth + EyeSpace;
  
  for(i = 0; i < EyeWidth; i++) {
    setPixel(StartPoint + i, red, green, blue);
    setPixel(Start2ndEye + i, red, green, blue);
  }
  
  showStrip();
  
  if(Fade==true) {
    float r, g, b;
  
    for(int j = Steps; j >= 0; j--) {
      r = j*(red/Steps);
      g = j*(green/Steps);
      b = j*(blue/Steps);
      
      for(i = 0; i < EyeWidth; i++) {
        setPixel(StartPoint + i, r, g, b);
        setPixel(Start2ndEye + i, r, g, b);
      }
      
      showStrip();
      delay(FadeDelay);
    }
  }
  
  setAll(0,0,0); // Set all black
  
  delay(EndPause);
}

void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){

  for(int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    potValue = analogRead(potPin);                                                            // Read the potentiometer values.
    brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
    FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
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
    potValue = analogRead(potPin);                                                            // Read the potentiometer values.
    brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
    FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
    showStrip();
    delay(SpeedDelay);
  }
  
  delay(ReturnDelay);
}

void NewKITT(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){
  RightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  LeftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  OutsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  CenterToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  LeftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  RightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  OutsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  CenterToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
}

// used by NewKITT
void CenterToOutside(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i =((NUM_LEDS-EyeSize)/2); i>=0; i--) {
    setAll(0,0,0);
    
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    
    setPixel(NUM_LEDS-i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(NUM_LEDS-i-j, red, green, blue); 
    }
    setPixel(NUM_LEDS-i-EyeSize-1, red/10, green/10, blue/10);
    potValue = analogRead(potPin);                                                            // Read the potentiometer values.
    brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
    FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

// used by NewKITT
void OutsideToCenter(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i = 0; i<=((NUM_LEDS-EyeSize)/2); i++) {
    setAll(0,0,0);
    
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    
    setPixel(NUM_LEDS-i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(NUM_LEDS-i-j, red, green, blue); 
    }
    setPixel(NUM_LEDS-i-EyeSize-1, red/10, green/10, blue/10);
    potValue = analogRead(potPin);                                                            // Read the potentiometer values.
    brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
    FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

// used by NewKITT
void LeftToRight(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    potValue = analogRead(potPin);                                                            // Read the potentiometer values.
    brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
    FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

// used by NewKITT
void RightToLeft(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i = NUM_LEDS-EyeSize-2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    potValue = analogRead(potPin);                                                            // Read the potentiometer values.
    brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
    FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void Twinkle(byte red, byte green, byte blue, int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0,0,0);
  
  for (int i=0; i<Count; i++) {
     setPixel(random(NUM_LEDS),red,green,blue);
     potValue = analogRead(potPin);                                                            // Read the potentiometer values.
     brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
     FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
     showStrip();
     delay(SpeedDelay);
     if(OnlyOne) { 
       setAll(0,0,0); 
     }
   }
  
  delay(SpeedDelay);
}

void TwinkleRandom(int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0,0,0);
  
  for (int i=0; i<Count; i++) {
     setPixel(random(NUM_LEDS),random(0,255),random(0,255),random(0,255));
     potValue = analogRead(potPin);                                                            // Read the potentiometer values.
     brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
     FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
     showStrip();
     delay(SpeedDelay);
     if(OnlyOne) { 
       setAll(0,0,0); 
     }
   }
  
  delay(SpeedDelay);
}

void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel,red,green,blue);
  potValue = analogRead(potPin);                                                            // Read the potentiometer values.
  brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
  FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
  showStrip();
  delay(SpeedDelay);
  setPixel(Pixel,0,0,0);
}

void SnowSparkle(byte red, byte green, byte blue, int SparkleDelay, int SpeedDelay) {
  setAll(red,green,blue);
  
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel,0xff,0xff,0xff);
  potValue = analogRead(potPin);                                                            // Read the potentiometer values.
  brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
  FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
  showStrip();
  delay(SparkleDelay);
  setPixel(Pixel,red,green,blue);
  potValue = analogRead(potPin);                                                            // Read the potentiometer values.
  brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
  FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
  showStrip();
  delay(SpeedDelay);
}

void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position=0;
  
  for(int i=0; i<NUM_LEDS*2; i++)
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
      potValue = analogRead(potPin);                                                            // Read the potentiometer values.
      brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
      FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
      showStrip();
      delay(WaveDelay);
  }
}

void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for(uint16_t i=0; i<NUM_LEDS; i++) {
      setPixel(i, red, green, blue);
      potValue = analogRead(potPin);                                                            // Read the potentiometer values.
      brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
      FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
      showStrip();
      delay(SpeedDelay);
  }
}

void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS; i++) {
      c=Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
    }
    potValue = analogRead(potPin);                                                            // Read the potentiometer values.
    brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
    FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
    showStrip();
    delay(SpeedDelay);
  }
}

// used by rainbowCycle and theaterChaseRainbow
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

void theaterChase(byte red, byte green, byte blue, int SpeedDelay) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < NUM_LEDS; i=i+3) {
        setPixel(i+q, red, green, blue);    //turn every third pixel on
      }
      potValue = analogRead(potPin);                                                            // Read the potentiometer values.
      brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
      FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
      showStrip();
     
      delay(SpeedDelay);
     
      for (int i=0; i < NUM_LEDS; i=i+3) {
        setPixel(i+q, 0,0,0);        //turn every third pixel off
      }
    }
  }
}

void theaterChaseRainbow(int SpeedDelay) {
  byte *c;
  
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < NUM_LEDS; i=i+3) {
          c = Wheel( (i+j) % 255);
          setPixel(i+q, *c, *(c+1), *(c+2));    //turn every third pixel on
        }
        potValue = analogRead(potPin);                                                            // Read the potentiometer values.
        brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
        FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
        showStrip();
       
        delay(SpeedDelay);
       
        for (int i=0; i < NUM_LEDS; i=i+3) {
          setPixel(i+q, 0,0,0);        //turn every third pixel off
        }
    }
  }
}

void Fire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[NUM_LEDS];
  int cooldown;
  
  // Step 1.  Cool down every cell a little
  for( int i = 0; i < NUM_LEDS; i++) {
    cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);
    
    if(cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }
  
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
    
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = 0; j < NUM_LEDS; j++) {
    setPixelHeatColor(j, heat[j] );
  }
  potValue = analogRead(potPin);                                                            // Read the potentiometer values.
  brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
  FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
  showStrip();
  delay(SpeedDelay);
}

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    setPixel(Pixel, 255, 255, heatramp);
  } else if( t192 > 0x40 ) {             // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}

void BouncingColoredBalls(int BallCount, byte colors[][3], boolean continuous) {
  float Gravity = -9.81;
  int StartHeight = 1;
  
  float Height[BallCount];
  float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
  float ImpactVelocity[BallCount];
  float TimeSinceLastBounce[BallCount];
  int   Position[BallCount];
  long  ClockTimeSinceLastBounce[BallCount];
  float Dampening[BallCount];
  boolean ballBouncing[BallCount];
  boolean ballsStillBouncing = true;
  
  for (int i = 0 ; i < BallCount ; i++) {   
    ClockTimeSinceLastBounce[i] = millis();
    Height[i] = StartHeight;
    Position[i] = 0; 
    ImpactVelocity[i] = ImpactVelocityStart;
    TimeSinceLastBounce[i] = 0;
    Dampening[i] = 0.90 - float(i)/pow(BallCount,2);
    ballBouncing[i]=true; 
  }

  while (ballsStillBouncing) {
    for (int i = 0 ; i < BallCount ; i++) {
      TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];
      Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i]/1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i]/1000;
  
      if ( Height[i] < 0 ) {                      
        Height[i] = 0;
        ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
        ClockTimeSinceLastBounce[i] = millis();
  
        if ( ImpactVelocity[i] < 0.01 ) {
          if (continuous) {
            ImpactVelocity[i] = ImpactVelocityStart;
          } else {
            ballBouncing[i]=false;
          }
        }
      }
      Position[i] = round( Height[i] * (NUM_LEDS - 1) / StartHeight);
    }

    ballsStillBouncing = false; // assume no balls bouncing
    for (int i = 0 ; i < BallCount ; i++) {
      setPixel(Position[i],colors[i][0],colors[i][1],colors[i][2]);
      if ( ballBouncing[i] ) {
        ballsStillBouncing = true;
      }
    }
    potValue = analogRead(potPin);                                                            // Read the potentiometer values.
    brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
    FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
    showStrip();
    setAll(0,0,0);
  }
}

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
    potValue = analogRead(potPin);                                                            // Read the potentiometer values.
    brightnessValue = map(potValue, 0, 1023, 15, 255);                                        // Map potentiometer to brightness and set limits.
    FastLED.setBrightness(brightnessValue);                                                   // Set master brightness.
    showStrip();
    delay(SpeedDelay);
  }
}

// used by meteorrain
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

// *** REPLACE TO HERE ***



// ***************************************
// ** FastLed/NeoPixel Common Functions **
// ***************************************

// Apply LED color changes
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

// Set a LED color (not yet visible)
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

// Set all LEDs to a given color and apply it (visible)
void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}
