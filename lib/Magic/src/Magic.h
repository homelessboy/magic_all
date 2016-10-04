#ifndef MAGIC_H;
#define MAGIC_H;

#include "MagicAction.h"

class Magic{
private:
  MagicAction actions[100];
  byte actionIndex;
public:
  Magic(){
    actionIndex=0;
  }

  void addAction(MagicAction action){
    if(action.code>0){
      actions[actionIndex++]=action;
    }
  }

  void update(){

  }

  void getLed(){
    if(actionIndex>0){
      Serial.println(actions[0].code);
      if(actions[0].code==2){
        Serial.print(actions[0].arg1);
        Serial.print(";;;;");
        Serial.println(actions[0].arg2);
      }
      actionIndex--;
    }
  }

  bool isStandBy(){

  }
};

#endif //MAGIC_H
