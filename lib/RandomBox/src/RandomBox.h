#ifndef RANDOMBOX_H
#define RANDOMBOX_H

#include "Box.h"

class RandomBox:public Box{
private:
  int gameState;//游戏状态；0:standBy;1:ready;2:gameing;3:showScore;
  byte scores;//游戏分数；
  byte *level;//不同级别下，游戏进行的次数；
  byte levelNum;//总共有多少种级别
  unsigned long levelTime;//每一次的游戏时间；默认2000；

  bool light[9];//当前游戏点亮哪些灯；
  byte lightFace;//当前点亮的面；
  byte roundNum,roundIndex;//roundNum：当前游戏级别；roundIndex：当前级别下的游戏次数；
  int roundScores;//当前round分数

  unsigned long startTime;//当前状态的开始时间

protected:
  void ready(){
    if(millis()-startTime>3000){
      gameState++;
      startTime=millis();
      roundNum=0;
      roundIndex=0;
      scores=0;
      roundScores=0;
      return;
    }

    int index=(millis()-startTime)/300%10;
    for(int i=0;i<6;i++){
      for(int j=0;j<9;j++){
        led[i*9+j]=CRGB(0,0,0);
      }
      if(index>0){
        led[i*9+4]=color[i];
      }
      for(int j=0;j<index-1;j++){
        led[SURFACE[i][j].getIndex()]=color[i];
      }
    }
  }

  int newRound(){
    if(roundScores>0)
      scores+=roundScores;
    roundScores=0;
    while(roundIndex>=level[roundNum]){
      roundNum++;
      roundIndex=0;
      if(roundNum>levelNum){
        return -1;
      }
    }
    roundIndex++;

    lightFace=SIDE[lightFace][random(4)];
    for(int i=0;i<9;i++)
      light[i]=false;
    int j=0;
    for(int i=0;i<roundNum;i++){
      j=random(9);
      while(light[j])
        j=random(9);
      light[j]=true;
    }

    Serial.print("new Round:");
    Serial.print(roundIndex);
    Serial.print(" : ");
    Serial.println(roundNum);
    Serial.print("face:");
    Serial.println(lightFace);
    for(int i=0;i<9;i++){
      if(light[i])
        Serial.println(i);
    }
    Serial.println("---------------");
    return 0;
  }

  void gameing(){
    if(millis()-startTime>levelTime){
      startTime=millis();
      if(newRound()<0){
        gameState++;
        startTime=millis();
        Serial.println("show s");
        return;
      }
    }

    for(int i=0;i<6;i++){
      for(int j=0;j<9;j++){
        led[i*9+j]=(i==lightFace && light[j])?color[lightFace]:OFF;
      }
    }
  }

  void showScores(){
    Serial.println(scores);
    for(int i=0;i<=scores;i++){
      showScores(i);
      delay(20);
    }
    delay(5000);
    gameState=0;
    startTime=millis();
    standByT=millis();
  }

  void showScores(int s){
    for(int i=0;i<54;i++)
      led[i]=OFF;
    for(int i=0;i<s/10;i++){
      led[i+9]=color[1];
    }
    for(int i=0;i<s%10;i++){
      led[i]=color[0];
    }
    FastLED.show();
  }

  void click(byte index){
    if(gameState==2){
      if(index/9==lightFace){
        if(light[index%9]){
          light[index%9]=false;
          roundScores++;
        }else{
          roundScores--;
          Serial.println("fail");
        }
      }
    }else if(gameState==0){
      if(index%9==4){
        gameState++;
        startTime=millis();
      }
    }
  }

public:
  RandomBox(CRGB *led,Keypad *kpd,byte *level=NULL,byte levelNum=0
    ,unsigned long levelTime=2000):Box(led,kpd){
      if(level==NULL){
        level=new byte[6];
        level[0]=0;
        level[1]=5;
        level[2]=5;
        level[3]=5;
        level[4]=5;
        level[5]=5;
        level[6]=4;
        levelNum=6;
      }
      this->level=level;
      this->levelNum=levelNum;
      this->levelTime=levelTime;
    }

    virtual void setup(){
      randomSeed(analogRead(0));
      gameState=0;
      startTime=millis();
    }

    virtual bool isStandBy(){
      return gameState==0;
    }

    virtual void getLed(){
      switch(gameState){
        case 1://ready
          ready();
          break;
        case 2:
          gameing();
          break;
        case 3:
          showScores();
          break;
        default:
          break;
      }
    }

    virtual void update(){
      if(kpd->getKeys()){
        for(int i=0;i<LIST_MAX;i++){
          if(kpd->key[i].stateChanged && kpd->key[i].kstate==PRESSED){
              click(kpd->key[i].kcode);
          }
        }

      }
    }

};

#endif //RANDOMBOX_H
