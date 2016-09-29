#ifndef MAGICACTION_H
#define MAGICACTION_H

#include "Action.h"

struct MagicAction {
  byte code;
  Action action;
  int arg1;
  int arg2;
public:
    MagicAction(Action action){
      if(action.surface<0){
        code=0;
      }else{
        code=1;
        this->action=action;
      }
    }
    MagicAction(byte code=0,int arg1=0,int arg2=0){
      this->code=code;
      this->arg1=arg1;
      this->arg2=arg2;
    }
};



#endif //MAGICACTION_H
