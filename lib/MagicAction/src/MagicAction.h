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

static byte ROTATION_CODE=1;
static byte SHOW_FACE_CODE=2;
static byte GO_BACK_CODE=3;
static byte SAVE_CODE=4;
static byte CLEAR_CODE=5;
static byte RANDOM_BOX_CODE=6;
static byte RESET_ALL_CODE=7;

static MagicAction GO_BACK=MagicAction(GO_BACK_CODE);
static MagicAction SAVE=MagicAction(SAVE_CODE);
static MagicAction CLEAR=MagicAction(CLEAR_CODE);
static MagicAction RANDOM_BOX=MagicAction(RANDOM_BOX_CODE);
static MagicAction RESET_ALL=MagicAction(RESET_ALL_CODE);

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
