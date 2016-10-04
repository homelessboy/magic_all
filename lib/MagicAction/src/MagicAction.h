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

static MagicAction GO_BACK=MagicAction(3);
static MagicAction SAVE=MagicAction(4);
static MagicAction CLEAR=MagicAction(5);
static MagicAction RANDOM_BOX=MagicAction(6);
static MagicAction RESET_ALL=MagicAction(7);

/*
  code:
    0:无效操作
    1:旋转
    2:显示方位：arg1：前面，arg2：底面
    3:回退
    4:保存
    5:打乱
    6:重置
*/

#endif //MAGICACTION_H
