#ifndef Motor_h
#define Motor_h

#define Buzz  0          // Buzzer pin output
#define SCRUBBER  1      // SCRUBBER pin output
#define SSR  7 

#define thermoDO   5
#define thermoCLK  6
#define thermoCS   11

//#define Max_CS   10

// 14 to 21 pins are allocated to GLCD data pins
// 26, 34, 35, 36, 37 pins are allocated to GLCD Control pins

#define AC_Mot 28         // AC_Mot pin output
#define DC_Mot 39         // DC_Mot pin output

#define Limit_Dir 30      // Stepper direction pin output        Lift Up
#define Limit_Step 31     // Stepper step pin output        Lower

#define Actu_dwn_LmT 33    // Actu_dwn_LmT pin output
#define Actu_DWN 32    // Actu_DWN pin output

//#define LCD_RS 37      // LCD_RS pin output old pcb

#define LCD_RS 34      // LCD_RS pin output
#define L_Sensor 38     // L_Sensor pin output


class Mclass
{
  public :
  Mclass();
  void SETUP();
  int up_down_lift(int,char[],int);
  void Top_scrn();
  void Buzz_call(int);

  int getXCoord(int x, int XMAX);
  int getYCoord(int y, int YMAX);
  void drawGraph(int val[], int sizeArray);
  int getMaxVal(int arrayC[], int len);
  int arrayLength(int arraySize);
  void drawRHLine(int x1, int y1, int x2, int y2, int space);
  void drawRVLine(int x1, int y1, int x2, int y2, int space);
  void Buzz_t(int t);
  void Limit_Dir_fun();
  void Limit_Dir_D();
};

extern Mclass Mot;

#endif
