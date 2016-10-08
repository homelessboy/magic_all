// #include <Arduino.h>
#include <FastLED.h>
#include "Keypad.h"
#include "Box.h"
#include "LightBox.h"
#include "PushBox.h"
#include "RandomBox.h"
#include "MagicBox.h"
#include "BulbBox.h"

#define LED_PIN     2
#define COLOR_ORDER RGB
#define CHIPSET     WS2811
#define NUM_LEDS    54

CRGB leds[NUM_LEDS];

const byte ROWS = 6;
const byte COLS = 9;
char keys[6][9] = {
  {1, 2, 3, 4, 5, 6, 7, 8, 9},
  {10, 11, 12, 13, 14, 15, 16, 17,18},
  {19, 20, 21, 22, 23, 24, 25, 26,27},
  {28, 29, 30, 31, 32, 33, 34, 35,36},
  {37, 38, 39, 40, 41, 42, 43, 44,45},
  {46, 47, 48, 49, 50, 51, 52, 53,54},
};
byte rowPins[ROWS] = {32, 33, 34, 35, 36, 37}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {22, 23, 24, 25, 26, 27, 28, 29, 30}; //connect to the column pinouts of the kpd
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

byte boxMap[54]={
  1,0,1,0,2,0,1,0,1,
  0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,
  0,0,0,0,3,0,0,0,0,
  0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,
};

// LightBox box=LightBox(leds,&kpd);
// PushBox box=PushBox(leds,&kpd,boxMap);
// RandomBox box=RandomBox(leds,&kpd);
// MagicBox box=MagicBox(leds,&kpd);
BulbBox box=BulbBox(leds,&kpd);
unsigned long startTimeF;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<CHIPSET,LED_PIN,COLOR_ORDER>(leds,NUM_LEDS).setCorrection(TypicalLEDStrip);
  box.setup();
  FastLED.show();
  startTimeF=millis();
}

void loop() {
  box.update();

  //更新频率为 60FPS；
  if((millis()-startTimeF)>1000/30){
    if(box.isStandBy()){
      box.standBy();
    }else{
      box.getLed();
    }
    startTimeF=millis();
    FastLED.show();
  }
}
