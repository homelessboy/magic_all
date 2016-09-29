#ifndef PUSHBOX_H
#define PUSHBOX_H

#include "Box.h"

static byte WALL=1;
static byte BOX=2;
static byte TARGET=3;
static byte CLICK=4;
static byte BLANK=0;

class PushBox:public Box{
private:
  byte *map;//地图
  byte *showIndex;//不同点显示的颜色
  byte game[54];//当前游戏状态
  byte box;//箱子所在坐标
public:
  PushBox(CRGB *led,Keypad *kpd,byte *map,byte *showIndex=NULL):Box(led,kpd){
    setMap(map);
    if(showIndex==NULL){
      showIndex=new byte[4];
      showIndex[0]=0;
      showIndex[1]=3;
      showIndex[2]=0;
      showIndex[3]=5;
      showIndex[4]=4;
    }
    this->showIndex=showIndex;
  }

  void setMap(byte *map){
    this->map=map;
    for(int i=0;i<54;i++){//获取箱子所在位置
      game[i]=map[i];
      if(map[i]==BOX)
        box=i;
    }
  }

  virtual void update(){
    if(kpd->getKeys()){
      if(standByFlag)
        standByFlag=false;
      else{
        for(int i=0;i<LIST_MAX;i++){
          if(kpd->key[i].stateChanged){
            //按下点亮按下的点，放开则推箱子
            if(kpd->key[i].kstate==PRESSED){
              light(kpd->key[i].kcode);
            }else if(kpd->key[i].kstate==RELEASED){
              move(kpd->key[i].kcode);
            }
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
    if(game[index]==BLANK)
      game[index]=CLICK;
  }

  //推箱子
  void move(byte index){
    if(game[index]!=CLICK)
      return;
    game[index]=BLANK;
    for(int i=0;i<6;i++){
      for(int j=0;j<12;j++){
        if(CIRCLE[i][j].getIndex()==index){
          if(CIRCLE[i][(j+1)%12].getIndex()==box){
            moveTo(CIRCLE[i][(j+2)%12].getIndex());
            return;
          }else if(CIRCLE[i][(j+11)%12].getIndex()==box){
            moveTo(CIRCLE[i][(j+10)%12].getIndex());
            return;
          }
        }
      }
    }

    for(int i=0;i<6;i++){
      for(int j=0;j<12;j++){
        if(MIDDLE[i][j].getIndex()==index){
          if(MIDDLE[i][(j+1)%12].getIndex()==box){
            moveTo(MIDDLE[i][(j+2)%12].getIndex());
            return;
          }
        }
      }
    }
  }

  //推到指定点，判断是否可达，或者已经推到目的地；
  void moveTo(byte to){
    if(game[to]==TARGET){
      Serial.println("success");
    }
    if(game[to]!=BLANK) return;
    game[box]=0;
    game[to]=BOX;
    box=to;
  }

  //根据当前游戏状态，更新led；
  virtual void getLed(){
    for(int i=0;i<54;i++){
      if(game[i]==BLANK){
        led[i]=CRGB(0,0,0);
      }else{
        led[i]=color[showIndex[game[i]]];
      }
    }
  }
};

#endif //PUSHBOX_H
