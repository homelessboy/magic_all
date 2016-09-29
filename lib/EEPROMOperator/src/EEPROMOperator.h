#ifndef EEPROMOPERATOR_H
#define EEPROMOPERATOR_H

#include <EEPROM.h>
#include <FastLED.h>
#include <Arduino.h>

class EEPROMOperator{

public:
  void setBool(bool value,int address);
  bool getBool(int address);

  void setByte(byte value,int address);
  byte getByte(int address);

  void setCRGBs(CRGB* value,int size,int address);
  CRGB* getCRGBs(int size,int address);

  void setCells(byte* value,int address);
  byte* getCells(int address);

  void setLong(unsigned long value,int address);
  unsigned long getLong(int address);

  void setNewone(bool newone=true);
  bool isNewone();

  void setDoubleSame(bool doubleSame);
  bool getDobuleSame();

  void setDoubleDis(bool doubleDis);
  bool getDoubleDis();

  void setSingleWay(bool singleWay);
  bool getSingleWay();

  void setFrontSide(byte frontSide);
  byte getFrontSide();

  void setDownSide(byte downSide);
  byte getDownSide();

  void setColor(CRGB *color);
  CRGB* getColor();

  void setCell(byte *cell);
  byte* getCell();

  void setMem(byte *mem,int index);
  byte* getMem(int index);

  void setMemNum(byte num=0);
  byte getMemNum();

  void setCirclePS(byte circlePS);
  byte getCirclePS();

  void setMiddlePS(byte middlePS);
  byte getMiddlePS();

  void setSurfacePS(byte surfacePS);
  byte getSurfacePS();

  void setMaskRound(byte maskRound);
  byte getMaskRound();

  void setTimeP(unsigned long timeP);
  unsigned long getTimeP();

  void setMask8(CRGB* mask8);
  CRGB* getMask8();

  void setMask12(CRGB* mask12);
  CRGB* getMask12();

  void setStandbyTime(unsigned long standbyTime);
  unsigned long getStandbyTime();
};

static int NEW_ONE=0;
static int DOUBLE_SAME=1;
static int DOUBLE_DIS=2;
static int SINGLE_WAY=3;
static int FRONT_SIDE=4;
static int DOWN_SIDE=5;
static int COLOR=6;//size=9*3*6=162
static int CELL=168;//size=18
static int MEM=186;//size=90
static int MEM_NUM=276;
static int CIRCLE_PS=278;
static int MIDDLE_PS=279;
static int SURFACE_PS=280;
static int MASK_ROUND=281;
static int TIME_P=282;//size=4
static int MASK_8=286;//size=8*3=24;
static int MASK_12=310;//size=12*3=36
static int STANDBY_TIME=346;//size=4;


#endif //EEPROMOPERATOR_H
