#ifndef MAGICACTIONCREATOR_H
#define MAGICACTIONCREATOR_H
#include "MagicAction.h"
#include "P.h"

class MagicActionCreator{
private:
  int position=0;
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

  bool isAllCenter(){
     P p;
     for(int i=0;i<keyNum;i++){
       if(P(keys[i]).getNum()!=8)
         return false;
     }
     return true;
   }

  //0:右；  1：下；  2：左；  3：上； 4：对面；5：自己；
  int getSide(int side){
    if(side==4||side == 5)
      return SIDE[frontSide][side];
    return SIDE[frontSide][(side-position+4)%4];
  }

  int getPosition(int frontSide,int downSide){
    int tmpPosition=-1;
    if(SIDE[frontSide][1]==downSide)
      tmpPosition=0;
    else if(SIDE[frontSide][0]==downSide)
      tmpPosition=1;
    else if(SIDE[frontSide][3]==downSide)
      tmpPosition=2;
    else if(SIDE[frontSide][2]==downSide)
      tmpPosition=3;
    return tmpPosition;
  }



  MagicAction getAction(byte index){
    Action ans=Action(-1);
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
        ans = Action(getSide(action/2),action%2);
      }
    }

    return MagicAction(ans);
  }

  Action getActionSameWay(int index1,int index2){
     if(!doubleSame) return Action(-1);
     int side=getSameSide(index1, index2);
     int t1=0,t2=0;
     if(side>=0){
       t1=getNumInCircle(side, index1);
       t2=getNumInCircle(side, index2);
       if (t1%3==t2%3 && t1%3 != 1) {
         return Action(side,t1%3==2);
       }
     }else {
       side=getSameMiddle(index1, index2);
       if(side < 0) return Action(-1);
       t1=getNumInMiddle(side, index1);
       t2=getNumInMiddle(side, index2);
       if(t1%3==t2%3 && t1%3!=1){
         return Action(side,t1%3==2,false);
       }
     }
     return Action(-1);
  }

  Action getActionDisWay(int index1,int index2){
    if(!doubleDis) return Action(-1);
    int side=getSameSide(index1,index2);
    int t1=0,t2=0;
    int d1,d2;
    if(side>=0){
      t1=getNumInCircle(side, index1);
      t2=getNumInCircle(side, index2);
      if(t2>t1){
        d1=t2-t1;
        d2=12+t1-t2;
      }else{
        d2=t1-t2;
        d1=12+t2-t1;
      }
      if(d1==d2)
        return Action(-1);
        return Action(side,d1<d2);
    }else {
      side=getSameMiddle(index1, index2);
      if(side < 0) return Action(-1);
      t1=getNumInMiddle(side, index1);
      t2=getNumInMiddle(side, index2);
      if(t2>t1){
        d1=t2-t1;
        d2=12+t1-t2;
      }else{
        d2=t1-t2;
        d1=12+t2-t1;
      }
      if(d1==d2)
        return Action(-1);
      return Action(side,d1<d2,false);
    }
  }

  MagicAction getAction(byte index1,byte index2){
    if(isAllCenter()){
      return MagicAction(2,frontSide,downSide);
    }
    Action ans=getActionSameWay(index1,index2);
    if(ans.surface<0){
      ans=getActionDisWay(index1,index2);
      return MagicAction(ans);
    }
  }

  MagicAction showFace(){
    if(isAllCenter())
      return MagicAction(2,frontSide,downSide);
    return MagicAction();
  }
  MagicAction setFace(){
    int side=-1,tmpPosition,down,front;
    P plist[3]={
      P(keys[0]),
      P(keys[1]),
      P(keys[2]),
    };
    for(int i=0;i<3;i++){
      for(int j=0;j<3;j++){
        if(i!=j){
          tmpPosition=getPosition(plist[i].face,plist[j].face);
          if(tmpPosition>=0 &&
            getNum(plist[i],tmpPosition,plist[i].face)==3 &&
            getNum(plist[3-i-j],tmpPosition,plist[i].face)==7){
              this->position=tmpPosition;
              this->frontSide=plist[i].face;
              this->downSide=plist[j].face;
              return MagicAction(2,plist[i].face,plist[j].face);
          }
        }
      }
    }
    return MagicAction();
  }

  MagicAction goBack(){
    if(isAllCenter())
      return GO_BACK;
    return MagicAction();
  }
  MagicAction save(){
    if(isAllCenter())
      return SAVE;
    return MagicAction();
  }
  MagicAction clear(){
    int side=P(keys[0]).face;
    for(int i=0;i<4;i++){
      P tmpP=P(keys[i]);
      if(tmpP.face!=side)
        return MagicAction();
      if(tmpP.getNum()%2!=0 || tmpP.getNum()==8)
        return MagicAction();
    }
    return CLEAR;
  }
  MagicAction randomBox(){
    int side=P(keys[0]).face;
    for(int i=0;i<4;i++){
      P tmpP=P(keys[i]);
      if(tmpP.face!=side || tmpP.getNum()%2!=1)
        return MagicAction();
    }
    return RANDOM_BOX;
  }
  MagicAction resetAll(){
    if(isAllCenter())
      return RESET_ALL;
    return MagicAction();
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
        return getAction(keys[0]);
        break;
      case 2:{
        MagicAction ans =getAction(keys[0],keys[1]);
        if(ans.code==0)
          ans=showFace();
        return ans;
        break;
      }

      case 3:{
        MagicAction ans=setFace();
        if(ans.code==0)
          ans=goBack();
        return ans;
        break;
      }
      case 4:{
        MagicAction ans=save();
        if(ans.code==0)
          ans=clear();
        if(ans.code==0)
          ans=randomBox();
        return ans;
        break;
      }
      case 5:{
        return resetAll();
        break;
      }
      default:
        return MagicAction();

    }
    return MagicAction();
  }


};

#endif //MAGICACTIONCREATOR_H
