#ifndef MAGICACTIONCREATOR_H
#define MAGICACTIONCREATOR_H
#include "MagicAction.h"
#include "P.h"

class MagicActionCreator{
private:
  int position;
  byte frontSide=0,downSide=4;

  byte keys[54];
  byte keyNum=0;
  bool doubleSame=true,doubleDis=true,singleWay=true;

protected:
  byte getNum(P p,int position=-1,int frontSide=-1){
    if(position<0)
      position=this->position;
    if(frontSide<0)
      frontSide=this->frontSide;
    return SURFACE[0][(surfaceD[frontSide][position][p.face]+p.getNum())%8].getNum();
  }

  //0:右；  1：下；  2：左；  3：上； 4：对面；5：自己；
  int getSide(int side){
    if(side==4||side == 5)
      return SIDE[frontSide][side];
    return SIDE[frontSide][(side-position+4)%4];
  }

  Action getActionSingle(byte index){
    P p=P(index);
    if(p.getNum()==8) return Action(-1);//单按下中心点，无操作；
    //操作中间层
    if(p.getNum()%2==1) return Action(SIDE[p.face][p.getNum()/2],true,false);
    if(!singleWay) return Action(-1);
    int num=getNum(p);
    if(num<0) return Action(-1);

    for(int i=0;i<6;i++){
      if(p.face==getSide(i)){
        int action=actionSingle[i][num/2];
        return Action(getSide(action/2),action%2);
      }
    }
    return Action(-1);
  }

public:
  void pushKey(byte kcode){
    keys[keyNum++]=kcode;
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

  MagicAction getAction(){
    switch(keyNum){
      case 1:
        return MagicAction(getActionSingle(keys[0]));
        break;
    }
    return MagicAction();
  }


};

#endif //MAGICACTIONCREATOR_H
