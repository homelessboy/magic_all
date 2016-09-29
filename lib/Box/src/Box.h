#ifndef BOX_H
#define BOX_H

#include "Arduino.h"
#include "P.h"
#include "FastLED.h"
#include "Keypad.h"

class Box{
protected:
  Keypad *kpd;
  CRGB *led;
  CRGB color[6]={CRGB(0,0,10),CRGB(0,10,0),CRGB(0,10,10),
                  CRGB(10,0,0),CRGB(10,0,10),CRGB(10,10,0)};
  unsigned long standByT,lastUpdateT,wait4StandByT=30000;
  bool standByFlag=false;

public:
  Box(CRGB *led,Keypad *kpd){
    this->led=led;
    this->kpd=kpd;
  }

  //初始化设置
  void setup(){
  }

  //更新，每次loop执行。
  void update(){
  }

  //返回是否在待机状态；待机状态执行standBy，否则执行getled；
  bool isStandBy(){
    return standByFlag;
  }

  void standBy(){
    for(int i=0;i<6;i++){
      for(int j=0;j<9;j++){
        led[i*9+j]=color[(i+(millis()-standByT)/500)%6];
        // led[i*9+j]=color[(i+7)%6];
      }
    }
  }

  void getLed(){}
};
static CRGB OFF=CRGB(0,0,0);

#endif //BOX_H
