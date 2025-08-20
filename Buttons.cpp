#include "Arduino.h"
#include "Ext_Var.h"
// ---------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------
OneButton button1(Button_Set, true);                // Setup a new OneButton on pin A1. 
OneButton button2(Button_Reset, true);              // Setup a new OneButton on pin A2. 
// --------------------------------------------------------------------------------------------
const int numOfInputs = 4;
const int numOfInputs1 = 4;
const int inputPins[numOfInputs1] = {Button_up,Button_down,Button_Set,Button_Reset};
const int save_rst_pin[numOfInputs1] = {edit_buttn,save_buttn};
int edit_enable = 0,change_ramp = 0;
int inputState[numOfInputs1],s_r_State[numOfInputs1];
int lastInputState[numOfInputs1] = {LOW,LOW,LOW,LOW},sr_State[2] = {LOW,LOW};
bool inputFlags[numOfInputs] = {LOW,LOW,LOW,LOW},sr_Flags[2] = {LOW,LOW};
long lastDebounceTime[numOfInputs1] = {0,0,0,0};
long lastDebounce_sr_Time[2] = {0,0};
//long debounceDelay = 5;                             // the debounce time; increase if the output flickers
int min_speed = 5500,long_pp=0,edit_pwr[2] ={0,0},edit_pwr1;
int old_dimetime = min_speed ,ramp_val=0;
// ---------------------------------------------------------------------------------------------
// Variables will change:
int buttonState,buttonStat;             // the current reading from the input pin
int lastButtonState = LOW,lastButtonStat = LOW;   // the previous reading from the input pin
long lastDebounceTim = 0,lastDebounceTi = 0;  // the last time the output pin was toggled  
int powerbu[2]={0,0};
int Water_fill = 0, inc1=0, inc2=1, accel=0;
// ---------------------------------------------------------------------------------------------

Eclass::Eclass()
{}
void Eclass :: SETUP()
{
  for(int i = 0; i < numOfInputs1; i++) 
  {
    pinMode(inputPins[i], INPUT_PULLUP);                    // using array define the input pin with internal pull up
    digitalWrite(inputPins[i], HIGH);                       // pull-up 10k
  }
   pinMode(save_buttn, INPUT_PULLUP);                       // internal pull up for the save button
   pinMode(edit_buttn, INPUT_PULLUP);                       // internal pull up for the edit button
}

void Eclass :: setInputFlags()                       // function for the up down buttons
{
  for(int i = 0; i < numOfInputs1; i++) 
  {
    int reading = digitalRead(inputPins[i]);
    if (reading != lastInputState[i]) 
      lastDebounceTime[i] = millis();   
    if ((millis() - lastDebounceTime[i]) > 5) 
    {
      if (reading != inputState[i]) 
      {
        inputState[i] = reading;
        if (inputState[i] == HIGH) 
        {
          if(!powerbu[i])  powerbu[i] = 1;      // this use for to avoid the power condition
          else     inputFlags[i] = HIGH;        
          long_pp = 0;
          Mot.Buzz_call(50);
        }
      }
      else 
      {
        if(!reading)                 // if button is press for long time then the this will call till button is press
        {
          long_pp++;
          if(long_pp > 15)             // this delay for the time to increment the values
          {
            inputFlags[i] = HIGH; 
            inc1 = long_pp=0;
            Mot.Buzz_call(50);         // buzzer beep
            accel = 1; 
            inc2+=1; 
            if(inc2 > 15) inc2 = 15;
          }            
        }  
      }
    }
    lastInputState[i] = reading;
  }
}

void Eclass :: save_editB()
{
  Save_Button();   // this button is used to minus the ramps and in preset to save changes
  Edit_Button();  // this button isused to add ramps in new process and enable the edit mode
}

int Eclass :: Save_preset_parameters(int b1_b2)
{
  if(currentMenu==2)                         // this will use in preset menu to change the edit mode
  {
    sr_Flags[b1_b2] =! sr_Flags[b1_b2];
    if(sr_Flags[0]) { currentMenu = 3; sr_Flags[1]=0;}
  }
  else if(currentMenu > 2 && currentMenu <= 10)
  {
    sr_Flags[b1_b2] =! sr_Flags[b1_b2];
    if(sr_Flags[1]) { currentMenu = 2; sr_Flags[0]=sr_Flags[1]=0; }
  }      
}

void Eclass :: Save_Button()      // save button calling function
{
  int reading = digitalRead(save_rst_pin[1]);
    if (reading != sr_State[1]) 
      lastDebounce_sr_Time[1] = millis();   
    if ((millis() - lastDebounce_sr_Time[1]) > 5) 
    {
      if (reading != s_r_State[1]) 
      {
        s_r_State[1] = reading;
        if (s_r_State[1] == HIGH) 
        { 
          Mot.Buzz_call(50);
          if(!edit_pwr[0])  edit_pwr[1] = 1;    // to handle the power on condition to avoid useless calling
          else  
          {           
            if(currentMenu > 2 && currentMenu <= 10 && pros==1)
            {
              sr_Flags[1] =! sr_Flags[1] ;   // if edit is enabled then this flag will be high and save all the changes
              if(sr_Flags[1]) 
              {
//                LCD.preset_mem_save();
                Pre_Mem_Save = 1;                                // just after the preset edits, save button to save the edit with this values
                Pre_Loc = all_in_one[1][currentMenu];            // locations of the preset values
                currentMenu = 2;                                 // after saving the results screen will back to the currentmenu second location
                all_in_one[1][currentMenu] =  all_in_one[1][currentMenu+1];  
                Probe_blink=sr_Flags[0]=0;
              }
            }      
            set_w1 = sr_Flags[1];
            if(cur_mx_vl[0] > 2 && currentMenu <= 8  && !pros)          // this is for the changing the sursor position, if ramps are added
            {
              cur_mx_vl[0]-=2;                                          // if added ramp is removed then the position back to the previous menu
              if((cur_mx_vl[0] <= currentMenu)) currentMenu = cur_mx_vl[0];    // to restrict the max position of the cursor
              all_in_one[0][cur_mx_vl[0]+2] = 0;                        // this is to reset the position of the cursor
              all_in_one[0][cur_mx_vl[0]+1] = 0;       
            }           
          }
        }
      }
    }
    sr_State[1] = reading;
}

void Eclass :: Edit_Button()   // edit button calling
{
    int reading = digitalRead(save_rst_pin[0]);
    if (reading != sr_State[0]) 
      lastDebounce_sr_Time[0] = millis();   
    if ((millis() - lastDebounce_sr_Time[0]) > 5) 
    {
      if (reading != s_r_State[0]) 
      {
        s_r_State[0] = reading;
        if (s_r_State[0] == HIGH) 
        {
          Mot.Buzz_call(50);
          if(!edit_pwr[0])  edit_pwr[0] = 1;             // handle the power on condition to avoid calling
          else  
          {
            if(currentMenu==2 && pros==1)                //  to enter into the edit mode to edit the presets            
            {
              sr_Flags[0] =! sr_Flags[0];              
              if(sr_Flags[0]) 
              { 
                currentMenu = 3;                         // presets start from the third location of the currentmenu
                all_in_one[1][currentMenu] =  all_in_one[1][currentMenu-1];   // to assign the previous value
                sr_Flags[1]=0;                   // enable the save mode
              }
            }
          }
          if(currentMenu > 0 && cur_mx_vl[0] < 7 && !pros) cur_mx_vl[0]+=2;    // to change the cursor position // ramp add 
        }
      }
    }
    sr_State[0] = reading;
}
 
void Eclass::Ramp_up(int val1)           // this function is not used in this code 
{
  if(old_dimetime <= dimtime)
  {
    ramp_val++;
    if(ramp_val >= val1)
    {
      ramp_val = 0;
      if(old_dimetime < dimtime)      old_dimetime += 2;
      else    old_dimetime = dimtime;
    }
  }
  else if(old_dimetime >= dimtime)
  {
    ramp_val++;
    if(ramp_val >= val1)
    {
      ramp_val = 0;
      if(old_dimetime > dimtime)   old_dimetime -= 2;
      else     old_dimetime = dimtime;
    }
  }
}


Eclass Butt_P = Eclass();
