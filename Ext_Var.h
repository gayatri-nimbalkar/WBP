#ifndef Ext_Var_h
#define Ext_Var_h
#include <U8g2lib.h>
#include "LCD.h"
#include "Pid.h"
//#include "OneButton.h"
#include "Buttons.h" 
#include "max6675.h"
#include "Adafruit_MAX31865.h"
#include "Motor.h"
#include <EEPROM.h>
#include <Wire.h>  
#include "Ticker.h"
#include <PID_v1.h>

#define disk1 0x50    //Address of 24LC256 eeprom chip

#define Start_1 1    // macro defination for variables
#define Stop_0  0
#define Lift_up 5
#define Lift_dwn  3
// -------------------------------------------------------this are the extern variables use for the visibility of variables acros the files-------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
extern int currentMenu,set_w1,zz, Read_time,Lift_in_P;
extern int Stop_timer,stopage,c_char[3],Probe,dimtime,dimtime3,menu[4],set_temperature;
extern int PID_set[4],Probe1[2],Probe_select,buz_bip,buzz_time;
// --------------------------------------------------------------------------------------------------------------------
extern int Probe_blink,power_state,inc1, inc2, accel,last_tick,x3,Dir_Change,temp_log[200], val_p;
extern int preset_lgc[6][8][10],sett_lgc[6][5],scrub_t1,wait_tick,up_dwn_f,Pre_Loc;
extern int temp_red,all_in_one[3][12],pros,cur_move,save_presets,edit_enable;
// --------------------------------------------------------------------------------------------------------------------
extern bool inputFlags[4],sr_Flags[2],cursorr,Probe_error,Lift_F,Lift_Pos,Auto_Correct,Pre_Mem_Save,data_log;
extern bool Butt_not_work, dig_chk, chk_down_1;
extern long Count_Down,timer,Speed_PID;
extern int change_ramp,old_dimetime,cur_mx_vl[3],arrayrdc[],stop_wait,Stop_R,scrub_t,cur_move1;
// --------------------------------------------------------------------------------------------------------------------
extern float thermo_T_Cal,PID_error;
extern float Ref_High[2], Ref_Low[2],  Ref_Diff[2], Temp_High[2], Temp_Low[2],  Temp_diff[2];
extern float thermo_T_Read;
extern float Ref_Hi_Lo[2],Ref_Diffe,Ref_High1,Ref_Low1,Tem_High1,Temp_Low1;
extern float Temp_Hi_Lo[2],Temp_Diffe;
extern String str_tmp,pre_list;
extern char u8g_logo_Down[],u8g_logo_Up[];
extern unsigned long windowStartTime, Step_Count, I_Step,Up_Max;
extern double Setpoint, Input, Output;
extern float Kp, Ki, Kd,temp_read1;
extern int t_count;
// --------------------------------------------------------------------------------------------------------------------
extern unsigned int minute_disp;
//extern byte Degree[8];

class Vclass
{
  public :
  Vclass();
};

extern Vclass var;
#endif
