#include <elapsedMillis.h>
#include<BrightnessMatrix.h>

const int DELAY_TIME = 4000;

const int nPICS = 6;

BrightnessMatrix pics[nPICS];

int pn;

ElemType test[8][8] = {                               //ElemType = byte
  { 0, 32, 64,  96, 128, 160, 192, 224},
  { 4, 36, 68, 100, 132, 164, 196, 228},
  { 8, 40, 72, 104, 136, 168, 200, 232},
  {12, 44, 76, 108, 140, 172, 204, 236},
  {16, 48, 80, 112, 144, 176, 208, 240},
  {20, 52, 84, 116, 148, 180, 212, 244},
  {24, 56, 88, 120, 152, 184, 216, 248},
  {28, 60, 92, 124, 156, 188, 220, 255}};

ElemType sunset[8][8] = {
  {3, 3, 3, 3, 4, 8, 9, 5},
  {6, 6, 5, 5, 7, 8, 8, 5},
  {18, 17, 23, 32, 43, 47, 68, 33},
  {40, 46, 59, 74, 113, 160, 173, 149},
  {65, 79, 95, 118, 147, 167, 169, 148},
  {90, 97, 108, 121, 134, 142, 147, 141},
  {93, 101, 106, 114, 124, 133, 136, 135},
  {91, 96, 101, 108, 110, 109, 109, 107}};

ElemType sphere[8][8] = {
  {0, 0, 0, 3, 3, 0, 0, 0},
  {0, 5, 58, 108, 123, 86, 8, 0},
  {0, 58, 128, 163, 182, 183, 110, 0},
  {5, 103, 161, 198, 230, 222, 192, 9},
  {5, 115, 176, 222, 255, 241, 206, 9},
  {0, 78, 170, 207, 231, 227, 136, 0},
  {0, 7, 96, 173, 190, 130, 11, 0},
  {0, 0, 0, 5, 4, 0, 0, 0}};

ElemType mountain_sunset[8][8] = {
  {83, 173, 160, 76, 66, 59, 56, 52},
  {66, 129, 122, 56, 45, 40, 39, 35},
  {40, 39, 31, 24, 18, 13, 12, 6},
  {77, 86, 85, 72, 54, 36, 14, 7},
  {142, 146, 140, 108, 79, 55, 39, 18},
  {178, 210, 196, 153, 133, 118, 117, 104},
  {167, 219, 206, 164, 132, 102, 94, 102},
  {110, 130, 125, 102, 81, 66, 55, 48}};

ElemType lake[8][8] = {
  {107, 115, 114, 82, 55, 42, 36, 30},
  {135, 147, 142, 97, 61, 43, 35, 30},
  {156, 168, 160, 93, 56, 45, 44, 40},
  {54, 117, 129, 75, 63, 52, 53, 38},
  {225, 241, 238, 137, 66, 57, 48, 47},
  {237, 240, 231, 217, 148, 73, 62, 44},
  {226, 226, 227, 220, 218, 180, 114, 78},
  {213, 215, 215, 211, 209, 212, 213, 205}};

ElemType monalisa[8][8] = {
  {111, 51, 38, 53, 20, 50, 30, 54},
  {113, 81, 85, 164, 135, 60, 25, 71},
  {110, 103, 57, 190, 179, 70, 88, 134},
  {92, 115, 81, 76, 81, 40, 102, 107},
  {108, 107, 74, 165, 101, 49, 114, 134},
  {177, 192, 152, 178, 107, 77, 178, 206},
  {209, 208, 194, 143, 71, 172, 207, 209},
  {185, 181, 174, 174, 183, 181, 183, 193}};
 
void setup()
{
  //Serial.begin(9600);

  int i;
  for (i=2; i<=17; i++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  pics[0].importArray(test, 15, 0);
  pics[1].importArray(sunset, 6, 1);
  pics[2].importArray(sphere);
  pics[3].importArray(lake, 15, 1);
  pics[4].importArray(monalisa, 15, 1);
  pics[5].importArray(mountain_sunset);

  for(i=0; i<nPICS; i++)
  {
    pics[i].flipVert();         //arrays above were created from bitmaps, where lower indices correspond to lower points
    //pics[i].rotate270();
  }

  pn=0;
}

void loop()
{
  pics[pn].show(DELAY_TIME);
  pn++;
  if(pn >= nPICS) pn=0;

  /*pics[5].show(DELAY_TIME);
  pics[5].importArray(mountain_sunset, pn, 0);
  pics[5].flipAntidiag();
  pn++;*/
}

