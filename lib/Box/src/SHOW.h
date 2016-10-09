#ifndef SHOW_H
#define SHOW_H

#include "FastLED.h"
#include "P.h"

CRGB color[6]={CRGB(0,0,10),CRGB(0,10,0),CRGB(0,10,10),
                CRGB(10,0,0),CRGB(10,0,10),CRGB(10,10,0)};
CRGB rainbow12[12];

unsigned long farmIndex=0;

typedef void (SHOW)(CRGB *led,CRGB choice);

static byte SHOW_NUM=5;

void setColor(CRGB *led,CRGB color=CRGB(0,0,0)){
  for(int i=0;i<54;i++)
    led[i]=color;
}

//常亮
void show1(CRGB *led,CRGB choice){
  for(int i=0;i<54;i++){
    led[i]=choice;
  }
}

//呼吸灯
void show2(CRGB *led,CRGB choice){
  CHSV color=rgb2hsv_approximate(choice);
  color.v=scale8(cubicwave8(farmIndex),150);
  for(int i=0;i<54;i++){
    led[i]=color;
  }
  farmIndex++;
}

// void show(CRGB *led,CRGB choice,unsigned long *startTime){}

//面旋转
void show3(CRGB *led,CRGB choice){
  for(int i=0;i<6;i++){
    led[i*9+4]=CRGB(0,0,0);
    for(int j=0;j<8;j++){
      if(((farmIndex/2)+j+i*2)%8<3){
        led[SURFACE[i][j].getIndex()]=choice;
      }else{
        led[SURFACE[i][j].getIndex()]=CRGB(0,0,0);
      }
    }
  }
  farmIndex++;
}
//面旋转
void show4(CRGB *led,CRGB choice){
  setColor(led);
  for(int i=0;i<12;i++){
    if((i+0)%12<3){
      led[CIRCLE[5][(i+farmIndex)%12].getIndex()]=choice;
    }
    if((i+1)%12<3){
      led[MIDDLE[5][(i+1+10+farmIndex)%12].getIndex()]=choice;
    }
    if((i+2)%12<3){
      led[CIRCLE[4][(12-(i+2+farmIndex)%12)%12].getIndex()]=choice;

    }
  }
  farmIndex++;
}

//彩虹圈
void show5(CRGB *led,CRGB choice){
  fill_rainbow(rainbow12, 12, 0,12);
  int v=rgb2hsv_approximate(choice).v;
  unsigned long index=farmIndex;
  for(int i=0;i<12;i++){
    led[CIRCLE[5][(i+index)%12].getIndex()]=rainbow12[i];
    led[MIDDLE[5][(i+9+index)%12].getIndex()]=rainbow12[i];
    led[CIRCLE[4][(12-(i+index-2)%12)%12].getIndex()]=rainbow12[i];
  }

  for(int i=0;i<8;i++){
    led[SURFACE[5][(i+4)%8].getIndex()]=led[CIRCLE[5][i*3/2].getIndex()];
    led[SURFACE[4][(18-i%8)%8].getIndex()]=led[CIRCLE[5][i*3/2].getIndex()];
  }
  led[P(5,1,1).getIndex()]=choice;
  led[P(4,1,1).getIndex()]=choice;

  for(int i=0;i<54;i++){
    CHSV tmp=rgb2hsv_approximate(led[i]);
    led[i]=CHSV(tmp.h,tmp.s,v);
  }
  farmIndex++;
}


#endif //SHOW_H
