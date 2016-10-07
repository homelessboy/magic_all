#ifndef MAGIC_H;
#define MAGIC_H;

#include "MagicAction.h"
#include "FastLED.h"
#include "P.h"
#include "EEPROMOperator.h"

static byte CIRCLE_INDEX=0;
static byte SURFACE_INDEX=1;
static byte MIDDLE_INDEX=2;

class Magic{
private:
  CRGB *led;
  EEPROMOperator rom=EEPROMOperator();

  MagicAction actions[100];
  byte actionIndex;
  unsigned long lastUpdate;
  byte *cells;
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

  byte memNum=0;
  byte* cellMem[5];
  int memChoice=-1;

  byte maskRound=1;
  byte circlePS=3,middlePS=3,surfacePS=2;
  byte PS[3]={3,2,3};//依次为circle；surface；middle
  byte step[3]={0,0,0};
  byte maskNum[3]={12,8,12};

  //旋转相关的参数；
  byte circleStep,middleStep,surfaceStep,operatSide;
  bool cw;

  unsigned long timePRotation=500;//每次转动的时间
  unsigned long timePRandom=200;
  unsigned long standbyTime=30000;//进入待机状态的时间
  unsigned long OKTime=500;//操作完成后动作
  unsigned long showFaceTime=2000;

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

  void saveMem(){
    rom.setMemNum(memNum);
    for(int i=0;i<memNum;i++){
      rom.setMem(cellMem[i],i);
    }
  }

  void setOK(){
    for(int i=0;i<54;i++){
      led[i]=color[1];
    }
  }

  void rotation(){
    if(startActionTime==0){
      startActionTime=millis();
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
    unsigned long timeP=actions[0].code==ROTATION_CODE?timePRotation:timePRandom;

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
    if(startActionTime==0){
      startActionTime=millis();
      for(int i=0;i<54;i++)
        led[i]=CRGB(0,0,0);
      for(int i=0;i<9;i++){
        led[actions[0].arg2*9+i]=color[0];
      }
      for(int i=0;i<4;i++){
        led[actions[0].arg1*9+i*2+1]=color[0];
      }
      led[actions[0].arg1*9+4]=color[0];
    }
    if(millis()-startActionTime>showFaceTime){
      actionEnd();
    }
  }

  void goBack(){
    if(startActionTime==0){
      startActionTime=millis();
      if(memNum>0)
        memChoice++;

      if(memChoice>=memNum){
        memChoice=0;
      }
      setOK();
    }
    if(millis()-startActionTime>OKTime){
      actionEnd();
    }
  }

  void save(){
    if(startActionTime==0){
      startActionTime=millis();
      memNum++;
      if(memNum>5){
        memNum=5;
        delete(cellMem[4]);
      }
      for(int i=memNum;i>1;i--){
        cellMem[i-1]=cellMem[i-2];
      }
      cellMem[0]=new byte[54];
      for(int i=0;i<54;i++){
        cellMem[0][i]=cells[i];
      }
      saveMem();
      setOK();
    }
    if(millis()-startActionTime>OKTime){
      actionEnd();
    }
  }

  void clear(){
    if(startActionTime==0){
      startActionTime=millis();
      for(int i=0;i<54;i++){
        cells[i]=i/9;
      }
      setOK();
    }
    if(millis()-startActionTime>OKTime){
      actionEnd();
    }
  }

  void randomBox(){
    if(actions[0].action.surface>=0){
      rotation();
      return;
    }

    if(startActionTime==0){
      randomSeed(analogRead(0));
      startActionTime=millis();
      for(int i=0;i<20;i++){
        int t=random(18);
        int side=(t/2%6);
        if(t>15)
          side=5;
        addAction(MagicAction(RANDOM_BOX_CODE,0,0,Action(side,t%2,t/2>=6)));
      }
      setOK();
    }

    if(millis()-startActionTime>OKTime){
      actionEnd();
    }
  }

  void resetAll(){

  }

  void showDefault(){
    for(int i=0;i<54;i++){
      led[i]=memChoice<0?color[cells[i]]:color[cellMem[memChoice][i]];
    }
  }

public:
  Magic(CRGB *led){
    this->led=led;
    actionIndex=0;
  }
  void setup(){
    rom.setNewone();
    if(rom.isNewone()){
      cells=new byte[54];
      for(int i=0;i<6;i++){
        for(int j=0;j<9;j++){
          cells[i*9+j]=i;
        }
      }
      rom.setNewone(false);
      rom.setCell(cells);
      memNum=0;
      saveMem();
    }else{
      cells=rom.getCell();
      memNum=rom.getMemNum();
      for(int i=0;i<memNum;i++){
        cellMem[i]=rom.getMem(i);
      }
    }
    lastUpdate=millis();
  }

  void addAction(MagicAction action){
    if(isStandBy()){
      lastUpdate=millis();
      return;
    }
    if(action.code<=0)
      return;

    actions[actionIndex++]=action;

    if(memChoice>=0 && action.code!=GO_BACK_CODE){
      for(int i=0;i<54;i++){
        cells[i]=cellMem[memChoice][i];
      }
      delete(cellMem[memChoice]);
      for(int i=memChoice;i>0;i--){
        cellMem[i]=cellMem[i-1];
      }
      cellMem[0]=new byte[54];
      for(int i=0;i<54;i++){
        cellMem[0][i]=cells[i];
      }
      memChoice=-1;
      saveMem();
    }
  }

  void actionEnd(){
    if(actionIndex>0){
      for(int i=0;i<actionIndex;i++){
        actions[i]=actions[i+1];
      }
      startActionTime=0;
      actionIndex--;
      rom.setCell(cells);
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
