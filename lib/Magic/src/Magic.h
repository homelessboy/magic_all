#ifndef MAGIC_H;
#define MAGIC_H;

#include "MagicAction.h"
#include "FastLED.h"
#include "P.h"

static byte CIRCLE_INDEX=0;
static byte SURFACE_INDEX=1;
static byte MIDDLE_INDEX=2;

class Magic{
private:
  CRGB *led;
  MagicAction actions[100];
  byte actionIndex;
  unsigned long lastUpdate;
  byte cells[54];
  CRGB color[6]={
    CRGB(0,0,10),CRGB(0,10,0),CRGB(0,10,10),
    CRGB(10,0,0),CRGB(10,0,10),CRGB(10,10,0)};
  CRGB mask8[8]={
    CRGB(3,0,0),CRGB(6,0,0),CRGB(9,0,0),CRGB(0,0,0),
    CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0)
  };
  CRGB mask12[12]={
    CRGB(3,0,0),CRGB(6,0,0),CRGB(9,0,0),CRGB(0,0,0),
    CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),
    CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0)
  };

  byte maskRound=2;
  byte circlePS=3,middlePS=3,surfacePS=2;
  byte PS[3]={3,2,3};//依次为circle；surface；middle
  byte step[3]={0,0,0};
  byte maskNum[3]={12,8,12};

  //旋转相关的参数；
  byte circleStep,middleStep,surfaceStep,operatSide;
  bool cw;

  unsigned long timeP=500;//每次转动的时间
  unsigned long standbyTime=30000;//进入待机状态的时间

  unsigned long startActionTime=0;//开始执行操作时间；

protected:
  void rotationP(P *ps,bool cw,int size,int step){
    while(step-- > 0){
      P p = ps[0];
      byte tmp=cells[p.getIndex()];
      for(int i=1;i<size;i++){
        P next=cw?ps[size-i]:ps[i];
        cells[p.getIndex()]=cells[next.getIndex()];
        p=next;
      }
      cells[p.getIndex()]=tmp;
    }
  }

  void rotation(int index,int side,int cw,int step){
    if(index==CIRCLE_INDEX){
      rotationP(CIRCLE[side],cw,12,step);
    }else if(index==SURFACE_INDEX){
      rotationP(SURFACE[side],cw,8,step);
    }else if(index==MIDDLE_INDEX){
      rotationP(MIDDLE[side],cw,12,step);
    }
  }

  CRGB getMask(CRGB ledi,CRGB maski){
    if(maskRound==0)
      return ledi;
    if(maski.r==0 && maski.g==0 && maski.b==0)
        return ledi;
    return maski;
  }

  void rotation(){
    if(startActionTime==0){
      startActionTime=millis();
      Serial.println("rotation");
      for(int i=0;i<3;i++){
        step[i]=PS[i];
      }
      if(actions[0].action.isSide){
        step[MIDDLE_INDEX]=0;
      }else{
        step[CIRCLE_INDEX]=0;
        step[SURFACE_INDEX]=0;
      }
      cw=actions[0].action.cw;
      operatSide=actions[0].action.surface;
    }

    unsigned long nowTime=millis();
    if(startActionTime!=0){
      for(int i=0;i<3;i++){
        if(step[i]<=0)
          continue;
        int tmp=PS[i]-(nowTime-startActionTime)*PS[i]/timeP;
        if(tmp<0)
          tmp=0;
        if(tmp<step[i]){
          rotation(i,operatSide,cw,step[i]-tmp);
          step[i]=tmp;
        }
      }
    }
    showDefault();

    if(startActionTime!=0 && maskRound>0){
      int maskIndex=0;
      CRGB maskColor;
      for(int i=0;i<3;i++){
        if(step[i]<=0)
          continue;
        maskIndex=(nowTime-startActionTime)*maskNum[i]*maskRound/timeP;
        for(int j=0;j<maskNum[i];j++){
          int index=cw?(maskNum[i]+maskIndex-j)%maskNum[i]:(j+maskNum[i]+maskIndex)%maskNum[i];
          if(maskNum[i]==8){
            maskColor=mask8[index];
          }else if(maskNum[i]==12){
            maskColor=mask12[index];
          }
          if(i==CIRCLE_INDEX){
            led[CIRCLE[operatSide][j].getIndex()]=getMask(led[CIRCLE[operatSide][j].getIndex()],maskColor);
          }else if(i==SURFACE_INDEX){
            led[SURFACE[operatSide][j].getIndex()]=getMask(led[SURFACE[operatSide][j].getIndex()],maskColor);
          }else if(i==MIDDLE_INDEX){
            led[MIDDLE[operatSide][j].getIndex()]=getMask(led[MIDDLE[operatSide][j].getIndex()],maskColor);
          }
        }
      }
    }

    if(startActionTime!=0){
      for(int i=0;i<3;i++){
        if(step[i]!=0)
          return;
      }
      actionEnd();
    }
  }

  void showFace(){

  }

  void goBack(){

  }

  void save(){

  }

  void clear(){

  }

  void randomBox(){

  }

  void resetAll(){

  }

  void showDefault(){
    for(int i=0;i<54;i++){
      led[i]=color[cells[i]];
    }
  }

public:
  Magic(CRGB *led){
    this->led=led;
    actionIndex=0;
  }
  void setup(){
    for(int i=0;i<6;i++){
      for(int j=0;j<9;j++){
        cells[i*9+j]=i;
      }
    }
    lastUpdate=millis();
  }

  void addAction(MagicAction action){
    if(isStandBy()){
      lastUpdate=millis();
      return;
    }
    if(action.code>0){
      actions[actionIndex++]=action;
    }
  }

  void actionEnd(){
    if(actionIndex>0){
      for(int i=0;i<actionIndex;i++){
        actions[i]=actions[i+1];
      }
      startActionTime=0;
      actionIndex--;
    }
  }

  void getLed(){
    if(actionIndex>0){
      switch(actions[0].code){
        case 1:
          rotation();
          return;
          break;
        case 2:
          showFace();
          return;
          break;
        case 3:
          goBack();
          return;
          break;
        case 4:
          save();
          return;
          break;
        case 5:
          clear();
          return;
          break;
        case 6:
          randomBox();
          return;
          break;
        case 7:
          resetAll();
          return;
          break;
        default:
          break;
      };
    }
    showDefault();
  }

  bool isStandBy(){
    return false;//millis()-lastUpdate>=standbyTime;
  }
};

#endif //MAGIC_H
