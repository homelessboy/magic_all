#include "EEPROMOperator.h"


void EEPROMOperator::setBool(bool value, int address){
 EEPROM.write(address, value);
}

bool EEPROMOperator::getBool(int address){
  return EEPROM.read(address);
}

void EEPROMOperator::setByte(byte value, int address){
 EEPROM.write(address, value);
}

byte EEPROMOperator::getByte(int address){
  return EEPROM.read(address);
}

void EEPROMOperator::setCRGBs(CRGB *value, int size, int address){
  for(int i=0;i<size;i++){
    EEPROM.write(address+i*3, value[i].r);
    EEPROM.write(address+i*3+1, value[i].g);
    EEPROM.write(address+i*3+2, value[i].b);
  }
}

CRGB* EEPROMOperator::getCRGBs(int size, int address){
  CRGB* read=new CRGB[size];
  for(int i=0;i<size;i++){
    read[i]=CRGB(EEPROM.read(address+i*3), EEPROM.read(address+i*3+1), EEPROM.read(address+i*3+2));
  }
  return read;
}

void EEPROMOperator::setCells(byte* value,int address){
  char tmp;
  for(int i=0;i<18;i++){
    tmp=value[i*3]*36+value[i*3+1]*6+value[i*3+2];
    EEPROM.write(address+i, tmp);
  }
}

byte* EEPROMOperator::getCells(int address){
  byte* read=new byte[54];
  byte tmp;
  for(int i=0;i<18;i++){
    tmp=EEPROM.read(address+i);
    read[i*3+2]=tmp%6;
    tmp=tmp/6;
    read[i*3+1]=tmp%6;
    read[i*3]=tmp/6;
  }
  return read;
}

void EEPROMOperator::setLong(unsigned long value,int address){
  for(int i=3;i>=0;i--){
    byte tmp=value&0xFF;
    value=value>>8;
    EEPROM.write(address+i, tmp);
  }
}

unsigned long EEPROMOperator::getLong(int address){
  unsigned long read=0;
  for(int i=0;i<4;i++){
    read=read<<8;
    byte tmp=EEPROM.read(address+i);
    read=tmp|read;
  }
  return read;
}

void EEPROMOperator::setNewone(bool newone){
  setBool(newone, NEW_ONE);
}

bool EEPROMOperator::isNewone(){
//  return false;
  return getBool(NEW_ONE);
}

void EEPROMOperator::setDoubleSame(bool doubleSame){
  setBool(doubleSame, DOUBLE_SAME);
}

bool EEPROMOperator::getDobuleSame(){
  return getBool(DOUBLE_SAME);
}

void EEPROMOperator::setDoubleDis(bool doubleDis){
  setBool(doubleDis,DOUBLE_DIS);
}

bool EEPROMOperator::getDoubleDis(){
  return getBool(DOUBLE_DIS);
}

void EEPROMOperator::setSingleWay(bool singleWay){
  setBool(singleWay,SINGLE_WAY);
}

bool EEPROMOperator::getSingleWay(){
  return getBool(SINGLE_WAY);
}

void EEPROMOperator::setFrontSide(byte frontSide){
  setByte(frontSide, FRONT_SIDE);
}

byte EEPROMOperator::getFrontSide(){
  return getByte(FRONT_SIDE);
}

void EEPROMOperator::setDownSide(byte downSide){
  setByte(downSide, DOWN_SIDE);
}

byte EEPROMOperator::getDownSide(){
  return getByte(DOWN_SIDE);
}

void EEPROMOperator::setColor(CRGB *color){
  setCRGBs(color, 6 , COLOR);
}

CRGB* EEPROMOperator::getColor(){
  return getCRGBs(6, COLOR);
}

void EEPROMOperator::setCell(byte *cell){
  setCells(cell, CELL);
}

byte* EEPROMOperator::getCell(){
  return getCells(CELL);
}

void EEPROMOperator::setMem(byte *mem, int index){
  setCells(mem,MEM+index*18);
}

byte* EEPROMOperator::getMem(int index){
  return getCells(MEM+index*18);
}

void EEPROMOperator::setMemNum(byte num){
  setByte(num, MEM_NUM);
}

byte EEPROMOperator::getMemNum(){
  return getByte(MEM_NUM);
}

void EEPROMOperator::setCirclePS(byte circlePS){
  setByte(circlePS,CIRCLE_PS);
}

byte EEPROMOperator::getCirclePS(){
  return getByte(CIRCLE_PS);
}

void EEPROMOperator::setMiddlePS(byte middlePS){
  setByte(middlePS,MIDDLE_PS);
}

byte EEPROMOperator::getMiddlePS(){
  return getByte(MIDDLE_PS);
}

void EEPROMOperator::setSurfacePS(byte surfacePS){
  setByte(surfacePS,SURFACE_PS);
}

byte EEPROMOperator::getSurfacePS(){
  return getByte(SURFACE_PS);
}

void EEPROMOperator::setMaskRound(byte maskRound){
  setByte(maskRound,MASK_ROUND);
}

byte EEPROMOperator::getMaskRound(){
  return getByte(MASK_ROUND);
}

void EEPROMOperator::setTimeP(unsigned long timeP){
  setLong(timeP, TIME_P);
}

unsigned long EEPROMOperator::getTimeP(){
  return getLong(TIME_P);
}

void EEPROMOperator::setMask8(CRGB* mask8){
  setCRGBs(mask8, 8, MASK_8);
}

CRGB* EEPROMOperator::getMask8(){
  return getCRGBs(8,MASK_8);
}

void EEPROMOperator::setMask12(CRGB* mask12){
  setCRGBs(mask12, 12, MASK_12);
}

CRGB* EEPROMOperator::getMask12(){
  return getCRGBs(12,MASK_12);
}

void EEPROMOperator::setStandbyTime(unsigned long standbyTime){
  setLong(standbyTime,STANDBY_TIME);
}

unsigned long EEPROMOperator::getStandbyTime(){
  return getLong(STANDBY_TIME);
}

// void EEPROMOperator::saveCells(byte *cells){
//   byte tmp=0;
//   byte *save=new byte[18];
//   byte index=0;
//   for(int i=0;i<54;i++){
//     if(i%3==0){
//       tmp=cells[i];
//     }else if(i%3==1){
//       tmp=tmp*6+cells[i];
//     }else {
//       tmp=tmp*6+cells[i];
//       save[index]=tmp;
//       index++;
//     }
//   }
//   for(int i=0;i<18;i++){
//     EEPROM.write(CELLS_START+i, save[i]);
//   }
//   delete(save);
// }
//
// byte* EEPROMOperator::getCells(){
//   byte* get=new byte[54];
//   byte tmp;
//   for(int i=0;i<18;i++){
//     tmp=EEPROM.read(CELLS_START+i);
//     get[i*3+2]=tmp%6;
//     tmp=tmp/6;
//     get[i*3+1]=tmp%6;
//     get[i*3]=tmp/6;
//   }
//   return get;
// }
