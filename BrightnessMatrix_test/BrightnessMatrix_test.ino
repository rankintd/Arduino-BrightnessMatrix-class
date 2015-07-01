#include <elapsedMillis.h>
#include<BrightnessMatrix.h>

const int CONTRAST = 4;
const int DELAY_TIME = 500;

ElemType sphere[8][8] = {                       //ElemType = byte
  {0, 0, 0, 5, 4, 0, 0, 0},
  {0, 7, 96, 173, 190, 130, 11, 0},
  {0, 78, 170, 207, 231, 227, 136, 0},
  {5, 115, 176, 222, 255, 241, 206, 9},
  {5, 103, 161, 198, 230, 222, 192, 9},
  {0, 58, 128, 163, 182, 183, 110, 0},
  {0, 5, 58, 108, 123, 86, 8, 0},
  {0, 0, 0, 3, 3, 0, 0, 0}};

BrightnessMatrix bm(sphere);

int count = 0;

void setup()
{
  //Serial.begin(9600);

  int i;
  for (i=2; i<=17; i++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

    bm.importArray(sphere);
    bm.show(DELAY_TIME);
    bm.clear();
    bm.show(DELAY_TIME);
    
    bm.importArray(sphere);
    bm.show(DELAY_TIME);
    
    bm.rotate270();
    bm.show(DELAY_TIME);
    
    bm.transpose();
    bm.show(DELAY_TIME);
    
    bm.flipVert();
    bm.show(DELAY_TIME);

    for(i=0; i<nROWS; i++)
    {
        bm.wrap(-1, 2);
        bm.show(DELAY_TIME);
    }
    
    bm.clear();
    bm.show(DELAY_TIME);

    for(i=0; i<8; i++)
    {
        bm.importArray(sphere);
        bm.reorient(i);
        bm.show(DELAY_TIME);
    }
    
    bm.clear();
    bm.show(DELAY_TIME);
    
    bm.importArray(sphere);
    bm(0, 0) = 99;
    bm(7, 7) = 1;
    bm(2, 11) = 1;

    bm.show(DELAY_TIME);
}

void loop()
{
    int i;
    for(i=0; i<nROWS; i++)
    {
        bm.swapCols((i+1)%nCOLS, i);
        bm.show(DELAY_TIME/4);
        bm.swapRows((i+1)%nROWS, i);
        bm.show(DELAY_TIME/4);
    }
}

