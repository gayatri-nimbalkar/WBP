#include "Arduino.h"
#include "Ext_Var.h"


//Define Variables we'll be connecting to
double Setpoint = 0, Input, Output;

//Specify the links and initial tuning parameters
float Kp=30, Ki=0.5, Kd=2.5;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
// ----------------------------------------------------------------------------------------------------------------------
//Variables
int set_temperature = 0;            //Default temperature setpoint. Leave it 0 and control it with rotary encoder
float temperature_read;
float PID_error = 0;
float previous_error = 0;
float elapsedTime, Time, timePrev;
float PID_value = 0;
// ----------------------------------------------------------------------------------------------------------------------
//PID constants
//////////////////////////////////////////////////////////
int kp = 80;   int ki = 40;   int kd = 80;
//////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------------------
int PID_p = 0;    int PID_i = 0;    int PID_d = 0;
float last_kp = 0,last_ki = 0,last_kd = 0;
int Probe = 0,set_fire;
unsigned long I_Step = 0,Step_Count=0;
// ----------------------------------------------------------------------------------------------------------------------
int WindowSize = 1000;
unsigned long windowStartTime;

Pclass::Pclass()
{}
void Pclass :: SETUP()
{
  windowStartTime = millis();

  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void Pclass::PID_Set()
{
  Input = thermo_T_Read;
  myPID.Compute();
}


void Pclass::PID_Action(float set_temp)
{
  if(Probe_error)
  {
    if (millis() - windowStartTime > WindowSize)
    { //time to shift the Relay Window
      windowStartTime += WindowSize;
    }
    if (Output < millis() - windowStartTime) digitalWrite(SSR, LOW);
    else 
    {
      if(thermo_T_Read > (set_temp + 1))
        digitalWrite(SSR, LOW);
      else
        digitalWrite(SSR, HIGH);
    }
  }
  else
    digitalWrite(SSR, LOW);

//    Serial3.print(Input); Serial3.print("  "); Serial3.print(pinchk); Serial3.print("  "); Serial3.println(Output); 
}

void Pclass :: pid_interval(float set_p, float In_put)
{
  float Error_chk = set_p - In_put;
  if(Error_chk <= 15)
  {
//    if(Error_chk < 2) 
//      Speed_PID = 200;
//    else
    Speed_PID = 200; //Error_chk * 200;   
    myPID.SetTunings(Kp, Ki, Kd); 
  }
  else 
  {
    Speed_PID = 5000;
    myPID.SetTunings(80, 0, 0);
  }
  
  pid.PID_Action(set_p);
}

int Pclass :: writeEEPROM(int deviceaddress, unsigned int eeaddress, int data ) 
{
  int dat[2];
  dat[0] = data/10;
  dat[1] = data%10;
  for(int i=0;i<2;i++)
  {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress + i >> 8));   // MSB
    Wire.write((int)(eeaddress + i & 0xFF)); // LSB
    Wire.write(dat[i]);
    Wire.endTransmission();
    delay(25);//------------------11/05/2022---Changed from 5 to 100 for new EEPROM IC
  }
  
}
 
int Pclass :: readEEPROM(int deviceaddress, unsigned int eeaddress ) 
{
  int rdata[2];
  rdata[0] = 0xFF; rdata[1] = 0xFF;
  for(int i=0;i<2;i++)
  {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress + i >> 8));   // MSB
    Wire.write((int)(eeaddress + i & 0xFF)); // LSB
    Wire.endTransmission();
   
    Wire.requestFrom(deviceaddress,1);
    if (Wire.available()) rdata[i] = Wire.read();
  }
  int r_data = (rdata[0] * 10) + rdata[1]; 
  return r_data;
}

Pclass pid = Pclass();
