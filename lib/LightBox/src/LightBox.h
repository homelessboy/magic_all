#ifndef LIGHTBOX_H
#define LIGHTBOX_H

#include "Box.h"

class LightBox:public Box{
private:
  CRGB checked;
  CRGB blank;
  bool cell[54];
  byte middleIndex[3]={0,1,5};
public:
  LightBox(CRGB *led,Keypad *kpd,CRGB checked=CRGB(0,0,10),CRGB blank=CRGB(0,0,0))
    :Box(led,kpd){
      this->checked=checked;
      this->blank=blank;
    }

  virtual void setup(){
    for(int i=0;i<54;i++){
      cell[i]=false;
    }
  }

  virtual void update(){
    // Serial.println("in light");
    if(kpd->getKeys()){
      if(standByFlag)//待机状态第一按键结束待机状态，不进行操作
        standByFlag=false;
      else{
        for(int i=0;i<LIST_MAX;i++){
          if(kpd->key[i].stateChanged && kpd->key[i].kstate==PRESSED){
            light(kpd->key[i].kcode);//点亮按下的点
          }
        }
      }
      lastUpdateT=millis();
    }else{
      if(!standByFlag && millis()-lastUpdateT>wait4StandByT){
        standByFlag=true;
        standByT=millis();
      }
    }
  }

  void light(byte index){
    int getNum=0;//找到的圈数，两圈即找全。
    cell[index]=!cell[index];
    for(int i=0;i<6;i++){
      for(int j=0;j<12;j++){
        if(CIRCLE[i][j].getIndex()==index){
          getNum++;
          cell[CIRCLE[i][(j+11)%12].getIndex()]=!cell[CIRCLE[i][(j+11)%12].getIndex()];
          cell[CIRCLE[i][(j+13)%12].getIndex()]=!cell[CIRCLE[i][(j+13)%12].getIndex()];
          if(getNum==2) return;
        }
      }
    }

    for(int i=0;i<3;i++){
      for(int j=0;j<12;j++){
        if(MIDDLE[middleIndex[i]][j].getIndex()==index){
          getNum++;
          cell[MIDDLE[middleIndex[i]][(j+11)%12].getIndex()]=!cell[MIDDLE[middleIndex[i]][(j+11)%12].getIndex()];
          cell[MIDDLE[middleIndex[i]][(j+13)%12].getIndex()]=!cell[MIDDLE[middleIndex[i]][(j+13)%12].getIndex()];
          if(getNum==2)
            return;
        }
      }
    }
  }

  //根据点亮与否，设置颜色；
  virtual void getLed(){
    for(int i=0;i<54;i++){
      led[i] = cell[i]? checked:blank;
    }
  }
};

#endif //LIGHTBOX_H
