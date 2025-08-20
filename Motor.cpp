#include "Arduino.h"
#include "Ext_Var.h"

#define GRID  2
// ----------------------------------------------------------------------------------------------------------------------------
int Stop_timer = 0,stopage = 0,water_empty = 0,x3 = 0;
int dimtime = 0,dimtime3=0,buzz_time=0,clear2 = 0,Dir_Change;
int pins_1[9] = {Buzz, AC_Mot, DC_Mot, L_Sensor, Actu_DWN,
                   Limit_Dir, Limit_Step, SSR, SCRUBBER};

bool Lift_F = 0,Butt_not_work = 0;
int temp_log[200]={ };            // Array for temp data log
// Variables will change:
int Dir_Cng = HIGH;         // the current state of the output pin
int Dir_State;             // the current Read_Dir from the input pin
int lastDirState = LOW;   // the previous Read_Dir from the input pin

unsigned long last_Dir = 0,Up_Max = 19500;  // the last time the output pin was toggled
unsigned int Dir_Delay = 50;    // the debounce time; increase if the output flickers
// ----------------------------------------------------------------------------------------------------------------------------
Mclass::Mclass()
{}

void Mclass :: SETUP()
{
  for(int i=0;i<9;i++)
  {
    pinMode(pins_1[i], OUTPUT);
    digitalWrite(pins_1[i], LOW);
  }
  pinMode(Actu_dwn_LmT, INPUT);
}

void Mclass :: Top_scrn()
{
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setDrawColor(2); /* color 1 for the box */
  u8g2.drawBox(0, 0, 128, 11);
  u8g2.setFont(u8g2_font_6x10_tf );
  u8g2.setCursor(2, 9);
  if(!pros)    u8g2.print("New Process");
  else         u8g2.print(pre_list);
  u8g2.setCursor(95, 9); u8g2.print(str_tmp);
  u8g2.setFont(u8g2_font_nokiafc22_tf); 
}

int Mclass :: up_down_lift(int pro_state, char up_dwn[],int time_to_lift)      // call 3 for up and for down call 5
{
  int x2=20,x4=57,x5;
  Buzz_t(1200);
//  Mot.Buzz_call(1500); 
  if(pro_state && !chk_down_1) 
  {
    Mot.Top_scrn();
    u8g2.setCursor(5, 25); u8g2.print("Process Started");
    u8g2.setCursor(5, 42); u8g2.print("Scrubber ON");
    u8g2.setCursor(5, 59); u8g2.print("Autolift ON");
    u8g2.sendBuffer();          // transfer internal memory to the display
    delay(2000);
//    Mot.Buzz_t(1500); 
  }  
  Lift_F = 1;
  x3 = 0;
  
  if(pro_state) 
  {
    Dir_Change = Step_Count =  Lift_Pos = 0;                      // DOWN
    I_Step = 20000;
    EEPROM.write(3, Lift_Pos);          // to know the position of tray  if this 0(low) means tray is going down or not in default position
  }
  else 
  { 
    Dir_Change = 1;                      // UP
    Step_Count = 0; 
    I_Step = Up_Max;
  }
  
  while(x3 < time_to_lift)                           // one will be for 2 sec
  {
    Butt_not_work = 1;                               // disabled the button to handle button press
    pid.Read_Thermo_P();
    Mot.Top_scrn();
    u8g2.setCursor(35, 34);
    if(pro_state)
    {
      u8g2.print("  Lowering  ");   
      if(!Dir_Change) x3 = 0;
      else 
      { 
        Step_Count = I_Step = 0; x3 = time_to_lift + 5;
      }
    }
    else
    {
      u8g2.print("   Raising   ");
      
      if(I_Step) x3 = 0; 
      else
      {
        x3 = time_to_lift + 5;
      }
    }
    u8g2.setCursor(26, 47); u8g2.print("Test Tube Tray");
    if(pro_state) 
    {
      x2+=1; 
      if(x2 > 57) x2 = 20; x5=x2;
    }
    else 
    {
      x4-=1; 
      if(x4 < 20) x4 = 57; x5=x4;
    }
    
    u8g2.drawXBM( 2, x5, 7, 5, up_dwn); // this is arguement we are passing in function
    u8g2.drawXBM( 11, x5, 7, 5, up_dwn);
    u8g2.drawXBM( 110, x5, 7, 5, up_dwn);
    u8g2.drawXBM( 119, x5, 7, 5, up_dwn);
    
    u8g2.sendBuffer();          // transfer internal memory to the display
//    delay(1000);
//    x3++;
  } 
  
  Butt_not_work = Lift_F = 0;    // enables the buttons after lifting
  Mot.Top_scrn();
  u8g2.setCursor(35, 34); 
  if(pro_state) u8g2.print("  Lowering  ");    
  else 
  { 
    u8g2.print("   Raising   "); 
    Lift_Pos = Auto_Correct = 1; 
    EEPROM.write(3, Lift_Pos);
  }  // if process done succefully 
  u8g2.setCursor(40, 47); u8g2.print("   Done    ");
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(1000); 
  u8g2.clearBuffer(); 
}

int Mclass :: getXCoord(int x, int XMAX)
{
//  return (((x * 106)/XMAX)+17);
    float G_x=(((x * 110)/XMAX)+17);     // value scaling calculation for time axis 
//    G_x=G_x+0.5;
    return G_x;
}

int Mclass :: getYCoord(int y, int YMAX)
{
//  return 50-((y * 45.0)/YMAX);
   float G_y=(50-((y * 50)/YMAX));   // value scaling calculation for temp axis 
//   G_y=G_y+0.5;
  return G_y;
}

void Mclass :: drawGraph(int val[], int sizeArray)
{
  int leN = arrayLength(sizeArray);
//  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_6x12_tr);  // choose a suitable font

  u8g2.drawLine(17, 0, 17, 53);
  u8g2.drawLine(14, 50, 127, 50);
//  for(int p = 35; p < 126; p+=18)
//  {
//    drawRVLine(p,0,p,50,GRID);                               // Grid lines are excluded
//  }
//  for(int p = 1; p < 38; p+=16)
//  {
//    drawRHLine(17,p,127,p,GRID);                               // Grid lines are excluded
//  }
  String strX = String(leN);
  String strY = String(getMaxVal(val,leN));

  int xVal,yVal,PxVal=17,PyVal=50;
  for(int i = 0; i <= Count_Down; i++)            // No. of points ploted will be same as the time elapsed in min
  {
    xVal = getXCoord(i,minute_disp);              //Calculate X cordinate
    yVal = getYCoord(temp_log[i],420);            //Calculate Y cordinate
    u8g2.drawPixel(xVal, yVal);                   // Draw the point
//    Serial3.println(temp_log[i]);
//   xVal = getXCoord(i,leN);
//   yVal = getYCoord(val[i-1],getMaxVal(val,leN));
//   u8g2.drawLine(PxVal,PyVal,xVal,yVal);
////   u8g2.sendBuffer(); 
//   PxVal = xVal;
//   PyVal = yVal;
  }
}

int Mclass :: getMaxVal(int arrayC[], int len)
{
  int mxm = arrayC[0];
  for (int i=0; i<len; i++)
  {
    if (arrayC[i]>mxm)
    {
      mxm = arrayC[i];
    }
  }
  return mxm;
}

int Mclass :: arrayLength(int arraySize)
{
  return arraySize/sizeof(int);
}

//graph vertical line and horizontal lines
void Mclass :: drawRHLine(int x1, int y1, int x2, int y2, int space)
{
  for(;x1<x2;x1+=space)
  {
    u8g2.drawPixel(x1, y1);
  }
}

void Mclass :: drawRVLine(int x1, int y1, int x2, int y2, int space)
{
  for(;y1<y2;y1+=space)
  {
    u8g2.drawPixel(x1, y1);
  }
}

void Mclass :: Buzz_t(int t)
{
  digitalWrite(Buzz, HIGH);
  delay(t);  
  digitalWrite(Buzz, LOW);
}

void Mclass :: Limit_Dir_fun()
{
//  Dir_Change = digitalRead(Actu_dwn_LmT);
//  if(Dir_Change) digitalWrite(Limit_Dir,LOW); else digitalWrite(Limit_Dir,HIGH);
}

void Mclass :: Limit_Dir_D() //--------------------------Limit switch detcetion
{  
  int Read_Dir = digitalRead(Actu_dwn_LmT);   // read the state of the switch into a local variable:
  if (Read_Dir != lastDirState) // If the switch changed, due to noise or pressing:
  {
    last_Dir = millis();  // reset the debouncing timer
  }
  if ((millis() - last_Dir) > Dir_Delay + 50) 
  {
    // if the button state has changed:
    if (Read_Dir != Dir_State) 
    {
      Dir_State = Read_Dir;      // only toggle the LED if the new button state is HIGH
      if (Dir_State != HIGH) 
      {
        if((pros == 2 && currentMenu == 3 && all_in_one[2][1] == 5))  // calibration mode
        {
          if(!Dir_Change)
          {
            Step_Count = 0;
            I_Step = Up_Max;   // up motor steps for the default state
          }
        }
        Dir_Change = 1;
      }
    }
  }
  lastDirState = Read_Dir;     // save the Read_Dir. Next time through the loop, it'll be the lastDirState:
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

Mclass Mot = Mclass();
