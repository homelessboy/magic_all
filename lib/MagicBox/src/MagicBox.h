#ifndef MAGICBOX_H
#define MAGICBOX_H
#define randomx(x) (rand()%x)

#include "Box.h"
#include "MagicActionCreator.h"
#include "Magic.h"

class MagicBox:public Box{
private:
  MagicActionCreator creator=MagicActionCreator();
  Magic magic=NULL;//=Magic();
  bool haveNewPush=false;
public:
    MagicBox(CRGB *led,Keypad *kpd)
    :Box(led,kpd){
      magic=Magic(led);
    }

    virtual void setup(){
      standByFlag=false;
      magic.setup();
      // position=getPosition(0,4);
    }

    virtual void update(){
      if(kpd->getKeys()){
        if(standByFlag)
          standByFlag=false;
        else{
          bool haveReleased=false;
          for(int i=0;i<LIST_MAX;i++){
            if(kpd->key[i].stateChanged){
              if(kpd->key[i].kstate==PRESSED){
                haveNewPush=true;
                creator.pushKey(kpd->key[i].kcode);
              }else if(kpd->key[i].kstate==RELEASED){
                haveReleased=true;
              }
            }
          }
          if(haveReleased && haveNewPush){
            magic.addAction(creator.getAction());
          }
          if(haveReleased){
            haveNewPush=false;
            for(int i=0;i<LIST_MAX;i++){
              if(kpd->key[i].kstate==RELEASED&&kpd->key[i].stateChanged){
                creator.removeKey(kpd->key[i].kcode);
              }
            }
          }
        }
      }
      // magic.update()
    }

    virtual void getLed(){
      magic.getLed();
    }

    virtual bool isStandBy(){
      if(magic.isStandBy()!=standByFlag){
        standByFlag=magic.isStandBy();
        if(standByFlag){
          standByT=millis();
        }
      }
      return standByFlag;
    }

};

// class MagicBox:public Box{
// private:
//   Action *actions=new Action[40];
//   int actionIndex=0;
//
//   byte* cells;
//
//   int position;//通过设置faceSide,downSide来确定position的值，
//   bool actionEnd;//当按下新的按钮时，则认为之前的动作已经记录，可以开始记录新的动作
//   byte keys[54];//当前按下的按键
//   byte keyNum=0;//记录共有多少按键按下
//   byte frontSide=0,downSide=4;//正面及底面编号
//   bool doubleSame=true,doubleDis=true,singleWay=true; //操作方式
//
//   void(* resetFunc) (void) = 0;
//
//   //
//   byte getNum(P p,int position=-1,int frontSide=-1){
//     if(position<0)
//       position=this->position;
//     if(frontSide<0)
//       frontSide=this->frontSide;
//     return SURFACE[0][(surfaceD[frontSide][position][p.face]+p.getNum())%8].getNum();
//   }
//
//   //0:右；  1：下；  2：左；  3：上； 4：对面；5：自己；
//   int getSide(int side){
//     if(side==4||side == 5)
//       return SIDE[frontSide][side];
//       return SIDE[frontSide][(side-position+4)%4];
//   }
//
//   int getPosition(int frontSide,int downSide){
//     int tmpT=-1;
//     if(SIDE[frontSide][1]==downSide)
//       tmpT=0;
//     else if(SIDE[frontSide][0]==downSide)
//       tmpT=1;
//     else if(SIDE[frontSide][3]==downSide)
//       tmpT=2;
//     else if(SIDE[frontSide][2]==downSide)
//       tmpT=3;
//     return tmpT;
//   }
//
//   bool isAllCenter(){
//     P p;
//     for(int i=0;i<keyNum;i++){
//       if(P(keys[i]).getNum()!=8)
//         return false;
//     }
//     return true;
//   }
//
// protected:
//   void addAction(Action action){
//     Serial.println("addAction");
//     Serial.print(action.surface);
//     Serial.print(";");
//     Serial.print(action.cw);
//     Serial.print(";");
//     Serial.println(action.isSide);
//     if(action.surface>=0){
//       actions[actionIndex++]=action;
//     }
//   }
//
//   Action getActionSameWay(int index1,int index2){
//     if(!doubleSame) return Action(-1);
//     int side=getSameSide(index1, index2);
//     int t1=0,t2=0;
//     if(side>=0){
//       t1=getNumInCircle(side, index1);
//       t2=getNumInCircle(side, index2);
//       if (t1%3==t2%3 && t1%3 != 1) {
//         return Action(side,t1%3==2);
//       }
//     }else {
//       side=getSameMiddle(index1, index2);
//       if(side < 0) return Action(-1);
//       t1=getNumInMiddle(side, index1);
//       t2=getNumInMiddle(side, index2);
//       if(t1%3==t2%3 && t1%3!=1){
//         return Action(side,t1%3==2,false);
//       }
//     }
//     return Action(-1);
//   }
//
//   Action getActionDisWay(int index1,int index2){
//     if(!doubleDis) return Action(-1);
//     int side=getSameSide(index1,index2);
//     int t1=0,t2=0;
//     int d1,d2;
//     if(side>=0){
//       t1=getNumInCircle(side, index1);
//       t2=getNumInCircle(side, index2);
//       if(t2>t1){
//         d1=t2-t1;
//         d2=12+t1-t2;
//       }else{
//         d2=t1-t2;
//         d1=12+t2-t1;
//       }
//       if(d1==d2)
//         return Action(-1);
//         return Action(side,d1<d2);
//     }else {
//       side=getSameMiddle(index1, index2);
//       if(side < 0) return Action(-1);
//       t1=getNumInMiddle(side, index1);
//       t2=getNumInMiddle(side, index2);
//       if(t2>t1){
//         d1=t2-t1;
//         d2=12+t1-t2;
//       }else{
//         d2=t1-t2;
//         d1=12+t2-t1;
//       }
//       if(d1==d2)
//         return Action(-1);
//       return Action(side,d1<d2,false);
//     }
//   }
//
//   Action getAction(byte index){
//     P p=P(index);
//     if(p.getNum()==8) return Action(-1);//单按下中心点，无操作；
//     //操作中间层
//     if(p.getNum()%2==1) return Action(SIDE[p.face][p.getNum()/2],true,false);
//     if(!singleWay) return Action(-1);
//     int num=getNum(p);
//     if(num<0) return Action(-1);
//
//     for(int i=0;i<6;i++){
//       if(p.face==getSide(i)){
//         int action=actionSingle[i][num/2];
//         return Action(getSide(action/2),action%2);
//       }
//     }
//     return Action(-1);
//   }
//
//   Action getAction(byte index1,byte index2){
//     if(isAllCenter()){
//       showFace(frontSide,downSide);
//       return Action(-1);
//     }
//     Action ans=getActionSameWay(index1,index2);
//     if(ans.surface<0)
//       ans=getActionDisWay(index1,index2);
//     return ans;
//   }
//
//   int showFace(byte frontSide,byte downSide){
//
//   }
//
//   int setFace(){
//
//   }
//
//   int goBack(){
//
//   }
//
//   int save(){
//
//   }
//   int clear(){
//
//   }
//
//   int random(int randomNum=20,unsigned long timeRandom=200){
//     int side=P(keys[0]).face;
//     for(int i=0;i<4;i++){
//       P tmpP=P(keys[i]);
//       if(tmpP.face!=side||tmpP.getNum()%2!=1)
//         return -1;
//     }
//
//     srand(millis());
//     for(int i=0;i<randomNum;i++){
//       int t=randomx(18);
//       int side=(t/2)%6;
//       if(t>15)
//         side=5;
//       addAction(Action(side,t%2==1,t/2>=6));
//     }
//
//   }
//
//   int resetAll(){
//     if(!isAllCenter())
//       return -1;
//     resetFunc();
//     return 0;
//   }
//
//   int removeKey(byte keyCode){
//     for(int i=0;i<keyNum;i++){
//       if(keys[i]==keyCode){
//         for(int j=i;j<keyNum-1;j++){
//           keys[j]=keys[j+1];
//         }
//         keyNum--;
//         return 0;
//       }
//     }
//   }
//
// public:
//   MagicBox(CRGB *led,Keypad *kpd)
//     :Box(led,kpd){
//       delete(cells);
//       cells=new byte[54];
//       for(int i=0;i<6;i++){
//         for(int j=0;j<9;j++)
//           cells[i*9+j]=i;
//       }
//     }
//
//   virtual void setup(){
//     standByFlag=false;
//     position=getPosition(0,4);
//   }
//
//   virtual void update(){
//     if(kpd->getKeys()){
//       if(standByFlag)
//         standByFlag=false;
//       else{
//         bool haveReleased=false;
//         for(int i=0;i<LIST_MAX;i++){
//           if(kpd->key[i].stateChanged){
//             if(kpd->key[i].kstate==PRESSED){
//               actionEnd=true;
//               keys[keyNum++]=kpd->key[i].kcode;
//             }else if(kpd->key[i].kstate==RELEASED){
//               haveReleased=true;
//             }
//           }
//         }
//         if(haveReleased && actionEnd){
//           switch(keyNum){
//             case 1:
//               addAction(getAction(keys[0]));
//               break;
//             case 2:
//               addAction(getAction(keys[0],keys[1]));
//               break;
//             case 3:
//               if(setFace()<0)
//                 goBack();
//               break;
//             case 4:
//               if(save()<0)
//                 if(clear()<0)
//                   random();
//               break;
//             case 5:
//               resetAll();
//             default:
//               break;
//           }
//         }
//         if(haveReleased){
//           actionEnd=false;
//           for(int i=0;i<LIST_MAX;i++)
//             if(kpd->key[i].kstate==RELEASED)
//               removeKey(kpd->key[i].kcode);
//         }
//
//       }
//
//     }
//   }
//
//   virtual void getLed(){
//     for(int i=0;i<54;i++){
//       led[i]=color[cells[i]];
//     }
//   }
// };
//
#endif //MAGICBOX_H
