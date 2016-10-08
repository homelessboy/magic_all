#ifndef BULBBOX_H
#define BULBBOX_H

#include "Box.h"
#include "SHOW.h"


class BulbBox:public Box{
private:
  CRGB showColor=CRGB(0,0,0);
  CRGB memColor=CRGB(0,0,0);
  unsigned long startTime;
  bool haveReleased=true;
  byte showIndex=0;


  byte keys[4];
  byte keyNum=0;

  int opFace=-1;
  CRGB opFaceColor[9]={
    CRGB(0,0,0),CRGB(0,30,0),CRGB(10,10,10),
    CRGB(30,0,0),CRGB(0,0,0),CRGB(0,0,30),
    CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),
  };
public:
  BulbBox(CRGB *led,Keypad *kpd):Box(led,kpd){

  }

  virtual void setup(){
    startTime=millis();
    standByFlag=true;
    Serial.println(rgb2hsv_approximate(CRGB(10,0,0)).v);

  }

  virtual bool isStandBy(){
    return false;
  }

  virtual void update(){
    if(kpd->getKeys()){
      lastUpdateT=millis();
      if(standByFlag){
        for(int i=0;i<LIST_MAX;i++){
          if(kpd->key[i].stateChanged && kpd->key[i].kstate==PRESSED){
            opFace=P(kpd->key[i].kcode).face;
            standByFlag=false;
            return ;
          }
        }
      }else{
        for(int i=0;i<LIST_MAX;i++){
          if(kpd->key[i].stateChanged){
            if(kpd->key[i].kstate==PRESSED){
              pushKey(kpd->key[i].kcode);
            }else if(kpd->key[i].kstate==RELEASED){
              removeKey(kpd->key[i].kcode);
            }
          }
        }
      }
    }else{
      if(!standByFlag && millis()-lastUpdateT>10000){
        standByFlag=true;
        opFace=-1;
      }
    }
  }

  void set(){
    if(keyNum==0){
      return;
    }
    for(int i=0;i<keyNum;i++){
      if(P(keys[i]).face!=opFace){
        return;
      }
    }
    lastUpdateT=millis();
    if(keyNum<=2){
      int incValue=1;
      int opIndex=0;
      if(keyNum==2){
        incValue=-1;
        if(P(keys[0]).getNum()==8){
          opIndex=1;
        }else if(P(keys[0]).getNum()==8){
          opIndex=0;
        }else{
          return;
        }
      }
      switch(P(keys[opIndex]).getNum()){
        case 0:
          if(keyNum==1){
            showColor=CRGB(0,0,0);
          }
          break;
        case 1:
          showColor.g+=incValue;
          break;
        case 2:
          if(keyNum==1){
            showColor=memColor;
          }else if(keyNum==2){
            memColor=showColor;
          }
          break;
        case 3:
          showColor.b+=incValue;
          break;
        case 4:{
            if(keyNum>1)
              break;
            showIndex++;
            if(showIndex>=SHOW_NUM)
              showIndex=0;
            removeKey(keys[0]);
            Serial.println(showIndex);
          }
          break;
        case 5:{
            keyNum=0;
            opFace=-1;
            standByFlag=true;
          }
          break;
        case 6:{
            if(keyNum>1)
              break;
            showIndex--;
            if(showIndex<0)
              showIndex=SHOW_NUM-1;

            removeKey(keys[0]);
            Serial.println(showIndex);
          }
          break;
        case 7:
          showColor.r+=incValue;
          break;
        default:
          break;
      }
    }
  }

  virtual void getLed(){
    int a=1,b=2;
    switch (showIndex) {
      case 0:
        display(show1,showColor);
        break;
      case 1:
        display(show2,showColor);
        break;
      case 2:
        display(show3,showColor);
        break;
      case 3:
        display(show4,showColor);
        break;
      default:
        break;
    };

    set();
    if(opFace>=0){
      for(int i=0;i<9;i++){
        led[opFace*9+i]=opFaceColor[i];
      }
      led[opFace*9+4]=showColor;
      led[opFace*9+2]=memColor;
    }
  }

  void pushKey(byte kcode){
    if(keyNum<=4){
      keys[keyNum++]=kcode;
    }
  }
  void removeKey(byte kcode){
    for(int i=0;i<keyNum;i++){
      if(keys[i]==kcode){
        for(int j=i;j<keyNum-1;j++){
          keys[j]=keys[j+1];
        }
        keyNum--;
        return;
      }
    }
  }
};

#endif //BULBBOX_H
