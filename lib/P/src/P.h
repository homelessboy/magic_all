//
// Created by java-xuhui on 2016/3/31.
//

#ifndef P_H
#define P_H
#include "Arduino.h"
static byte NUM[3][3]={
  {0,1,2},
  {7,8,3},
  {6,5,4},
};

struct P {
    byte face;
    byte x;
    byte y;
public:
    P(byte face, byte y, byte x) {
        this->face = face;
        this->y = y;
        this->x = x;
    }
    P(byte index=0){
        this->face=index/9;
        this->y=(index%9)/3;
        this->x=(index%9)%3;
    }
    byte getIndex() const {
        return face * 9 + y * 3 + x;
    }
    byte getNum() const{
      return NUM[y][x];
    }
};

static int getIndex(int face,int x,int y){
  return face * 9 + x * 3 + y;
}
static int getIndex(int face,int i){
  return face * 9 + i;
}

static P CIRCLE[6][12]={
        {P(3, 2, 2), P(3, 1, 2), P(3, 0, 2), P(5, 2, 0), P(5, 2, 1), P(5, 2, 2), P(1, 0, 0), P(1, 1, 0), P(1, 2, 0), P(4, 0, 2), P(4, 0, 1), P(4, 0, 0)},
        {P(0, 2, 2), P(0, 1, 2), P(0, 0, 2), P(5, 2, 2), P(5, 1, 2), P(5, 0, 2), P(2, 0, 0), P(2, 1, 0), P(2, 2, 0), P(4, 2, 2), P(4, 1, 2), P(4, 0, 2)},
        {P(1, 2, 2), P(1, 1, 2), P(1, 0, 2), P(5, 0, 2), P(5, 0, 1), P(5, 0, 0), P(3, 0, 0), P(3, 1, 0), P(3, 2, 0), P(4, 2, 0), P(4, 2, 1), P(4, 2, 2)},
        {P(2, 2, 2), P(2, 1, 2), P(2, 0, 2), P(5, 0, 0), P(5, 1, 0), P(5, 2, 0), P(0, 0, 0), P(0, 1, 0), P(0, 2, 0), P(4, 0, 0), P(4, 1, 0), P(4, 2, 0)},
        {P(0, 2, 0), P(0, 2, 1), P(0, 2, 2), P(1, 2, 0), P(1, 2, 1), P(1, 2, 2), P(2, 2, 0), P(2, 2, 1), P(2, 2, 2), P(3, 2, 0), P(3, 2, 1), P(3, 2, 2)},
        {P(0, 0, 2), P(0, 0, 1), P(0, 0, 0), P(3, 0, 2), P(3, 0, 1), P(3, 0, 0), P(2, 0, 2), P(2, 0, 1), P(2, 0, 0), P(1, 0, 2), P(1, 0, 1), P(1, 0, 0)},
};

static P SURFACE[6][8]={
        {P(0,0,0),P(0,0,1),P(0,0,2),P(0,1,2),P(0,2,2),P(0,2,1),P(0,2,0),P(0,1,0)},
        {P(1,0,0),P(1,0,1),P(1,0,2),P(1,1,2),P(1,2,2),P(1,2,1),P(1,2,0),P(1,1,0)},
        {P(2,0,0),P(2,0,1),P(2,0,2),P(2,1,2),P(2,2,2),P(2,2,1),P(2,2,0),P(2,1,0)},
        {P(3,0,0),P(3,0,1),P(3,0,2),P(3,1,2),P(3,2,2),P(3,2,1),P(3,2,0),P(3,1,0)},
        {P(4,0,0),P(4,0,1),P(4,0,2),P(4,1,2),P(4,2,2),P(4,2,1),P(4,2,0),P(4,1,0)},
        {P(5,0,0),P(5,0,1),P(5,0,2),P(5,1,2),P(5,2,2),P(5,2,1),P(5,2,0),P(5,1,0)}
};

static P MIDDLE[6][12]={
        {P(3,2,1),P(3,1,1),P(3,0,1),P(5,1,0),P(5,1,1),P(5,1,2),P(1,0,1),P(1,1,1),P(1,2,1),P(4,1,2),P(4,1,1),P(4,1,0)},//正对0面中心层顺时针
        {P(0,2,1),P(0,1,1),P(0,0,1),P(5,2,1),P(5,1,1),P(5,0,1),P(2,0,1),P(2,1,1),P(2,2,1),P(4,2,1),P(4,1,1),P(4,0,1)},//正对1面中心层顺时针
        {P(4,1,0),P(4,1,1),P(4,1,2),P(1,2,1),P(1,1,1),P(1,0,1),P(5,1,2),P(5,1,1),P(5,1,0),P(3,0,1),P(3,1,1),P(3,2,1)},//正对2面中心层顺时针
        {P(4,0,1),P(4,1,1),P(4,2,1),P(2,2,1),P(2,1,1),P(2,0,1),P(5,0,1),P(5,1,1),P(5,2,1),P(0,0,1),P(0,1,1),P(0,2,1)},//正对3面中心层顺时针
        {P(0,1,0),P(0,1,1),P(0,1,2),P(1,1,0),P(1,1,1),P(1,1,2),P(2,1,0),P(2,1,1),P(2,1,2),P(3,1,0),P(3,1,1),P(3,1,2)},//正对4面中心层顺时针
        {P(3,1,2),P(3,1,1),P(3,1,0),P(2,1,2),P(2,1,1),P(2,1,0),P(1,1,2),P(1,1,1),P(1,1,0),P(0,1,2),P(0,1,1),P(0,1,0)},//正对5面中心层顺时针
};

//0:右；  1：下；  2：左；  3：上； 4：对面；5：自己；
static byte SIDE[6][6]={
  {1,4,3,5,2,0},
  {2,4,0,5,3,1},
  {3,4,1,5,0,2},
  {0,4,2,5,1,3},
  {1,2,3,0,5,4},
  {1,0,3,2,4,5},
};

// static int LEFT[6]={3,0,1,2,3,3};
// static int RIGHT[6]={1,2,3,0,1,1};
// static int UP[6]={5,5,5,5,0,2};
// static int DOWN[6]={4,4,4,4,2,0};
// static int BACK[6]={2,3,0,1,5,4};


//d=surfaceD[frontSide][t][p.face];
//t:
//  0:DOWN[frontSide]=downside;
//  1:RIGHT[frontSide]=downSide;
//  2:UP[frontSide]=downSide;
//  3:LEFT[frontSide]=downSide;
static byte surfaceD[6][4][6]={
  {
    {0,0,0,0,0,0},
    {2,2,6,2,2,2},
    {4,4,4,4,4,4},
    {6,6,6,6,6,6},
  },
  {
    {0,0,0,0,6,2},
    {2,2,2,4,0,4},
    {4,4,4,4,2,6},
    {6,6,6,6,4,2},
  },
  {
    {0,0,0,0,4,4},
    {6,2,2,2,6,6},
    {4,4,4,4,0,0},
    {2,6,6,6,2,2},
  },
  {
    {0,0,0,0,2,6},
    {2,6,2,2,4,0},
    {4,4,4,4,6,2},
    {6,2,6,6,0,4},
  },
  {
    {0,2,4,6,0,4},
    {2,4,6,0,2,2},
    {4,6,0,2,4,0},
    {6,0,2,4,6,6},
  },
  {
    {0,6,4,2,4,0},
    {2,0,6,4,2,2},
    {4,2,0,6,0,4},
    {6,4,2,0,6,6},
  },
};

//0:右；  1：下；  2：左；  3：上； 4：对面；5：自己；
//值/2，对应操作的面，值%2==0对应顺时针；
static byte actionSingle[6][4]={
  {3*2,3*2+1,1*2,1*2+1},
  {5*2,5*2+1,4*2,4*2+1},
  {3*2,3*2+1,1*2,1*2+1},
  {4*2,4*2+1,5*2,5*2+1},
  {0*2+1,2*2,2*2+1,0*2},
  {2*2+1,0*2,0*2+1,2*2},
};


static int getSameSide(int index1,int index2){
  int side=-1;
  for(int i=0;i<6;i++){
    int t=0;
    for(int j=0;j<12;j++){
      if(CIRCLE[i][j].getIndex()==index1)
        t++;
      if(CIRCLE[i][j].getIndex()==index2)
        t++;
    }
    if(t==2){
      if(side!=-1)
        return -1;
      side=i;
    }
  }
  return side;
}

static int getSameMiddle(int index1, int index2){
  for(int i=0;i<6;i++){
    int t=0;
    for(int j=0;j<12;j++){
      if(MIDDLE[i][j].getIndex()==index1)
        t++;
      if(MIDDLE[i][j].getIndex()==index2)
        t++;
    }
    if(t==2){
      return i;
    //  if(side>=0)
      //  return -1;
    }
  }
  return -1;
}

static int getNumInCircle(int side,int index){
  if(side < 0) return -1;
  for(int i=0;i<12;i++){
    if(CIRCLE[side][i].getIndex()==index)
      return i;
  }
  return -1;
}

static int getNumInMiddle(int side, int index){
  if(side < 0) return -1;
  for(int i=0;i<12;i++){
    if(MIDDLE[side][i].getIndex()==index)
      return i;
  }
  return -1;
}

#endif //P_H
