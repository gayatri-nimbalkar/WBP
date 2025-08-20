#include "Arduino.h"
#include "Ext_Var.h"

//Adafruit_MAX31865 max_1 = Adafruit_MAX31865(4);
//Adafruit_MAX31865 max_2 = Adafruit_MAX31865(3);
//
//#define RREF      470
//#define RNOMINAL  100

#define PERIOD_EXAMPLE_VALUE (32767)
#define TCB_CMP_EXAMPLE_VALUE (32767)

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

float Ref_High1 = 430,  Ref_Low1 = 25;  // 222  // 440
float Tem_High1 = 450, Temp_Low1 = 25;  // 200  // 400

void Read_thermo();
void cur_time();
void Buzz1();
void sec_time();

Ticker timer1(Read_thermo, 1500, 0);
Ticker timer2(cur_time, 50, 0);
Ticker Buzz_g(Buzz1, 1000, 1);
Ticker Second_t(sec_time, 1000, 0);
// -----------------------------------------------------------------------------------
String str_tmp = "30°C";
bool probe_buzz=0,Probe_error,Lift_Pos;
int On_Off;
int val_p=0;
// -----------------------------------------------------------------------------------
int Probe_check = 0,Probe1[2],temp_red = 0,power_on_temp=0,zz=0;
long timer = 0,Count_Down= 0,Speed_PID = 0;
float correct_val,temp_current=0,thermo_Read,temp_read1,thermo_T_Read,thermo_T_Cal;
int Err_1_point,stop_wait=0,scrub_t1,last_tick = 0,wait_tick = 0, Lift_in_P=0,Step_Mot=0;
bool Read_t1 = 0, Read_t2 = 1;
int t_count=0;
// -----------------------------------------------------------------------------------------------------------------------------

void Buzz1()
{
  digitalWrite(Buzz, LOW);
}

void Mclass :: Buzz_call(int b_tm)         // We have called the buzzer in ticker and need to pass the turn on interval for beeping 
{
  Buzz_g.interval(b_tm);
  digitalWrite(Buzz, HIGH);
  Buzz_g.start();
}

void setup() 
{
  // put your setup code here, to run once:
  pinMode(LCD_RS, OUTPUT);             // GLCD pins needs to push low intially
  digitalWrite(LCD_RS, LOW);
  Serial3.begin(9600);         // UART3 Default we use for the KBD
  pid.SETUP();      // this functions use for the pin definations
  LCD.SETUP();
  Butt_P.SETUP();
  Mot.SETUP();
  
  timer1.start(); // ticker start function to call infinite time
  timer2.start();
  Second_t.start();

//  max_1.begin(MAX31865_3WIRE);  // set to 2WIRE or 4WIRE as necessary
//  max_2.begin(MAX31865_2WIRE);
  cli();                                                  //stop interrupts //set timer1 interrupt at 1Hz  
   TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;  /* enable overflow interrupt */
   TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;   /* set Normal mode */
   TCA0.SINGLE.EVCTRL &= ~(TCA_SINGLE_CNTEI_bm);  /* disable event counting */
   TCA0.SINGLE.PER = PERIOD_EXAMPLE_VALUE;  /* set the period */
   TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc /* set clock source (sys_clk/256) */
   | TCA_SINGLE_ENABLE_bm; /* start timer */

   TCB0.CCMP = TCB_CMP_EXAMPLE_VALUE;                              //Load the Compare or Capture register with the timeout value
   TCB0.CTRLA = TCB_CLKSEL_CLKDIV1_gc | TCB_ENABLE_bm;             //Enable TCB and set CLK_PER divider to 1 (No Prescaling) 
   TCB0.INTCTRL = TCB_CAPT_bm;                                     // Enable Capture or Timeout interrupt
  sei();//allow 
  
}

void loop() 
{
  // put your main code here, to run repeatedly:

  LCD.InputFlags();          // button reading function
  LCD.main_menu();           // main function where all function call in this function
//    Serial3.print(pros); Serial3.print("  "); Serial3.print(currentMenu); Serial3.print("  "); Serial3.print(all_in_one[pros][currentMenu]); Serial3.println("  "); 
//  Serial3.print(sett_lgc[all_in_one[pros][1]][currentMenu - 2]); Serial3.print("  "); Serial3.println(preset_lgc[all_in_one[pros][1]][currentMenu-3][all_in_one[pros][currentMenu]]/*preset_lgc[all_in_one[1][1]][currentMenu-3][all_in_one[1][currentMenu]*/);
//  Serial3.print("  "); Serial3.println(all_in_one[1][currentMenu]); 
//  Mot.Limit_Dir_D();
}

void cur_time()
{
  if(accel)  // this use for the increment of set values when long press
  {
    inc1++; 
    if(inc1 > 8) {inc1 = accel = 0; inc2 = 1;}
  }
  if(Stop_timer)        // In process
  {
    static int chk_spd = 0;
    chk_spd += 50;
    if(chk_spd >= Speed_PID)      // time to call the PID function
    {
      pid.PID_Set();             // reading pid function when process will start
      chk_spd = 0;
    }
  }
  if((pros == 2 && currentMenu == 3 && all_in_one[2][1] == 5) || currentMenu > 8) Mot.Limit_Dir_D();  // limit switch reading in preset and process time
}

void sec_time()                     // time function for every sec
{
  if(Stop_timer)                     // if the process start when ever the process will start "Stop_timer" this variable will become 1
  {
    cursorr = !cursorr;              // this is to blink the arroy every sec in the process runnig window
    if(minute_disp > 0)              // whenver the user set time minute_disp greater than 0 
    {
      timer++;
      if(timer >= (1 * 59) && !last_tick)  // for every minute count_down will increment
      { 
        timer = 0; 
        Count_Down++;   // for every minute this will increment
      }
    }
    if(Count_Down >= (minute_disp))     
    {
      last_tick = 1; wait_tick++;
      if(wait_tick > 1)
      {     
        Count_Down = timer = 0;   // count down for minute timer
        wait_tick = minute_disp = 0;
      }
    }
    if(Stop_R) { stop_wait++;     if(stop_wait > 5)  stop_wait = Stop_R = 0; } // wait time for the user exit from the process
    if(!up_dwn_f)                                          // in process user can lift the tray and after 2min tray will reset the botton for the heating
    {
      Lift_in_P++;
      if(Lift_in_P > 120)           // in process user can lift the tray and after 2 min it will automaticaly goed down
      {
        up_dwn_f = 1;
        Step_Count = Dir_Change = Lift_in_P = 0;                      // Down
        I_Step = Up_Max;
      }
    }
  }
  if(Lift_F) x3++;
}

ISR(TCA0_OVF_vect)
{                                                      // updating all the tickers in the ISR routin
  timer1.update(); 
  LCD.tempRiseCheckUpdate();
  timer2.update();
  Second_t.update();
  Buzz_g.update();
  if(!Butt_not_work) Butt_P.setInputFlags();           // button update in ISR
  if(pros < 2 && !Butt_not_work) Butt_P.save_editB();  // Save and Edit button
  TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;            // The interrupt flag has to be cleared manually 
}

void Pclass :: Stepper_Rotate(unsigned long t_dly, unsigned long step_s)  // stepper motor function needs to pass the step delay and the number of steps 
{
  if(Step_Count < step_s)     // check the steps
  {
    Step_Mot++;  
    if(Step_Mot >= t_dly-5) 
    {
      Step_Mot = 0; 
      Step_Count++; 
      if(Step_Count >= I_Step)                  // this is to check the path provided done or not
      {
        I_Step = 0; x3 = 20;
        if(pros == 2 && currentMenu == 3  && all_in_one[2][1] == 5) currentMenu = 4;  // once the steps are done after calibration done window will appear
        if(Stop_timer) 
        {
          if(!up_dwn_f) dig_chk = 1; else dig_chk = chk_down_1 = 0;
        }
      }
    }
    digitalWrite(Limit_Dir, Dir_Change);  // direction pin use to channge the direction of rotation
    digitalWrite(Limit_Step, (Step_Mot < (t_dly / 2)) ? !On_Off : On_Off); // step pin need to provide the steps to the motor for rotation
  }
}

ISR(TCB0_INT_vect)
{
  pid.Stepper_Rotate(70,I_Step);   // 70usec is the delay between two steps and the I_step is the variable use to give the total path steps
  TCB0.INTFLAGS = TCB_CAPT_bm;              /* Clear the interrupt flag */
}

void Read_thermo()         // to read the function every 1500milli sec interval
{
  Read_t1 =! Read_t1;
}

// --------------------------------------------------------------------------------------------------------------------------------

void Pclass::Read_Thermo_P()  // function is use to read the temperature
{
  String uio = "";
  if(Read_t1 != Read_t2)
  {
    temp_read1 = thermocouple.readCelsius();
    thermo_Read = (((temp_read1 - Temp_Low1)*(Ref_High1 - Ref_Low1))/(Tem_High1 - Temp_Low1))+Ref_Low1;   // two point cal values passed to the formulae
    Err_1_point = sett_lgc[2][0] - 50;                    // one point calibration
    thermo_T_Read = thermo_Read + Err_1_point;            // add or subtract the one point claibrated value
    
    if(int(thermo_T_Read ) < 5 || int(thermo_T_Read ) > 500)  // probe error finding condition
    {                                                           //// If temp is less than 5 or >500 , will display error
      Probe_error = 0;                                            // If shows error check the thermocouple connected or not
      probe_buzz = ! probe_buzz;
       if(!probe_buzz)//-----------------------------------------with this if else we are displaying the error condition
       {
         str_tmp = uio + "Error";  // requires enableUTF8Print() for °C
         Mot.Buzz_call(200);                             // beeping for the error condition
       }
       else
       {
        str_tmp = uio + "     ";  // requires enableUTF8Print() for °C           // flipping the error and blank screen for error occurs
       }
    }
    else                     // if temperature is greater than the 99 then display condition is handle by this statement
    {
      if(data_log==1)            // Starts temp data collection after process starts    
      {       
        t_count++;
        if(t_count==40)         //Add temp value in array for graph after every minute
        {
//          static int val_p=0;
          t_count=0;
          val_p++;                   // Array location for temp value   
          temp_log[val_p]= thermo_T_Read;
        }
      }
      Probe_error = 1;
      if(int(thermo_T_Read) > 99) uio = ""; else uio = " ";//-----------for screen print handling of temp display
      str_tmp = uio + int(thermo_T_Read) + "°C";  // requires enableUTF8Print() for °C    
    }
//    Step_Count = 0;
    Read_t2 = Read_t1;
  }
}
// ----------------------------------------------------------End--------------------------------------------------------------------------------------------
