#include "Arduino.h"
#include "Ext_Var.h"

//-----------------------------------------------------------------------------------------------------------------------------------------------------
bool blankDisplayFlag = 0;
void flahDisplayFunction();
Ticker flahDisplay(flahDisplayFunction, 500, 0, MILLIS);

void flahDisplayFunction() {
  blankDisplayFlag == 1 ? blankDisplayFlag = 0 : blankDisplayFlag = 1;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
int tempArray[6];
int checkArray[5];
int incArray = 0;
bool checkTempRaised = 0;

bool processRunUpdates = 0;

void readTemp();
Ticker tempRiseCheck(readTemp, 30000, 0, MILLIS);

void readTemp() {
  // Serial3.println("tick"); //--------------change

  if (incArray < 6) {
    tempArray[incArray] = thermo_T_Read;
    incArray++;
  }
}

void lclass::tempRiseCheckUpdate() {
  tempRiseCheck.update();
}
// -----------------------------------------------------------------------------------------------------------------------------


// old pcb code
//U8G2_KS0108_128X64_F u8g2(U8G2_R0, 21, 20, 19, 18, 17, 16, 15, 14, /*enable=*/ 36, /*dc=*/ 35, /*cs0=*/ 26, /*cs1=*/ 34 , /*cs2=*/ U8X8_PIN_NONE, /* reset=*/  U8X8_PIN_NONE);   // Set R/W to low!

// new 1 pcb code
//U8G2_KS0108_128X64_F u8g2(U8G2_R0, 14, 15, 16, 17, 18, 19, 20, 21, /*enable=*/ 26, /*dc=*/ 35, /*cs0=*/ 36, /*cs1=*/ 37 , /*cs2=*/ U8X8_PIN_NONE, /* reset=*/  U8X8_PIN_NONE);   // Set R/W to low!
// End of constructor list

// new 2 pcb code
U8G2_KS0108_128X64_F u8g2(U8G2_R0, 14, 15, 16, 17, 18, 19, 20, 21, /*enable=*/10, /*dc=*/35, /*cs0=*/36, /*cs1=*/37, /*cs2=*/U8X8_PIN_NONE, /* reset=*/U8X8_PIN_NONE);  // Set R/W to low!
// End of constructor list
// -------------------------------------------------------------------------------------------------------
unsigned int minute_disp = 0;
int min_time, min_time1;
// -------------------------------------------------------------------------------------------------------
int Enter_menu_set = 0, PID_set[4], Pid_s = 0, Probe_select = 0, Probe_change_chk = 0, timm = 0;
int Set_time_flag = 0, alarm = 0, one_time = 0, flash_pb_err = 0, buz_bip = 0, curr_time = 2;
// -------------------------------------------------------------------------------------------------------
//LCD Menu Logic
int currentMenu = 0, control = 0, clear1 = 0;
int Probe_blink = 0, set_w = 7, set_w1 = 5;
int c_char[3] = { 1, 7, 13 };
int max_lmit[4] = { 99, 12, 5, 59 }, set_clear = 0;
// ---------------------------------------------------------------------------------------------------------
int last_scroll = 1, last_scroll2 = 1, cur_move1 = 0, save_presets = 1;
int all_in_one[3][12], pros = 0, menuSame = 8, pros1 = 1, cur_move = 0;
int max_lmit1[3][12] = { { 0, 460, 300, 460, 300, 460, 300, 460, 300, 0, 0, 0 },  // to limit the maximum increment of values
                         { 0, 5, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0 },
                         { 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
int start_lgc[10], preset_lgc[6][8][10], sett_lgc[6][5];  // this are the declarations of variablers for use of start, preset and settings
int set_mx_vl[6] = { 3, 3, 3, 6, 5, 3 };                  // max limit for settings current individual values
int cur_mx_vl[3] = { 2, 11, 3 };                          // currentMenu max value
//                       digestion         hold       scrubber
int S_max_lmit1[6][5] = { { 50, 0, 0, 0, 0 }, { 59, 0, 0, 0, 0 }, { 100, 0, 0, 0, 0 },
                          //                       autotune       one point    two point
                          { 50, 75, 460, 480, 0 },
                          { 99, 99, 99, 0, 0 },
                          { 100, 420, 40, 0, 40 } };  // features max values
//                       digestion         hold       scrubber

int S_min_lmit1[6][5] = { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
                          //                       autotune       one point    two point
                          { 20, 15, 400, 380, 0 },
                          { 0, 0, 0, 0, 0 },
                          { 50, 250, 0, 0, 0 } };  // features min values


int preset_mem_locatn[6][10] = {
  { 961, 977, 993, 1009, 65, 81, 97, 113, 129, 145 },    // milk and each sample have 10 presets and each preset contain 8 set values
  { 161, 177, 193, 209, 225, 241, 257, 273, 289, 305 },  // food
  { 321, 337, 353, 369, 385, 401, 417, 433, 449, 465 },  // feed
  { 481, 497, 513, 529, 545, 561, 577, 593, 609, 625 },  // soil
  { 641, 657, 673, 689, 705, 721, 737, 753, 769, 785 },  // brewary
  { 801, 817, 833, 849, 865, 881, 897, 913, 929, 945 }   // water
};


char* Preset[6] = { "Milk", "Food", "Feed", "Soil", "Brewery", "Water" };
char* settings[6] = { "Heater Slope", "Scrubber Time", "One Point Cal",
                      "Two Point Cal", "PID Parameter", "AutoLift Cal" };

int v1 = 0, Cal_addr = 5, cal_temp1[2], temp_time_pro[8], up_dwn_f = 0;
bool cal_one, S_P_curR = 0, cursorr = 0, dig_chk = 0, chk_down_1;
// --------------------------------------------------------------------------------------------------------
#define u8g_logo_width 7                                                                      // glcd box width use for cursor representation
#define u8g_logo_height 11                                                                    // glcd box height use for cursor representation
char u8g_logo_bits[] = { 0x02, 0x06, 0x0e, 0x1e, 0x3e, 0x7e, 0x3e, 0x1e, 0x0e, 0x06, 0x02 };  // arrow logo for position of settings, presets
char u8g_logo_Down[] = { 0x00, 0x7f, 0x3e, 0x1c, 0x08 };                                      // scoll up logo
char u8g_logo_Up[] = { 0x00, 0x08, 0x1c, 0x3e, 0x7f };                                        // scroll down logo

char u8g_top_arroy[] = { 0x08, 0x1c, 0x3e, 0x7f, 0x1c, 0x1c, 0x1c };                                             // this top arrow to represent the ramp current state
char u8g_right_mark[] = { 0xff, 0x7f, 0xff, 0x7b, 0xff, 0x7d, 0xef, 0x7e, 0x5f, 0x7f, 0xbf, 0x7f, 0xff, 0xff };  // right mark after the precess complition

bool u_d_arrow = 1;
String pre_list, tyu;
int pres_c1[2] = { 0, 27 }, pre_b1[2] = { 31, 42 }, edit_par1[8] = { 36, 36, 62, 62, 84, 84, 106, 106 };  // this is for the cursor positions
int pres_c[2] = { 0, 21 }, pre_b[2] = { 15, 24 }, edit_par[8] = { 41, 41, 62, 62, 83, 83, 104, 104 };
int chng_rmp = 0, time_cng = 0, chng_rmp_1 = 0, pre_show = 0;
String hour_1 = "", min_1 = "";
bool Graph_M = 0, Auto_Correct = 1, dwn_chk = 0, pre_chk1 = 0, Pre_Mem_Save = 0, process_up_d = 0, data_log = 0;
int Stop_R = 0, scrub_t = 0, pre_last = 0, Dir_Chk = 1, Pre_Loc;
int arrayrdc[] = { 4, 7, 9, 10, 9, 7, 4, 0 };
// ------------------------------------------------------------------------------------------------

lclass::lclass() {}

void lclass ::SETUP() {
  
  pinMode(safteyRelay, OUTPUT);       //-----------------------------------------------02/01/2024
  digitalWrite(safteyRelay, LOW);    //-----------------------------------------------02/01/2024
  
  Wire.begin();
  u8g2.begin();
  u8g2.enableUTF8Print();  // requires enableUTF8Print()

  sett_lgc[2][0] = EEPROM.read(1);  // 20  // one point cal saved point
  sett_lgc[0][0] = EEPROM.read(2);  // 20  // heater slope
  Lift_Pos = EEPROM.read(3);        // 20  // heater slope
  sett_lgc[1][0] = EEPROM.read(4);

  Temp_Low1 = EEPROM.get(Cal_addr + 0, sett_lgc[3][0]);  //calibration values
  Ref_Low1 = EEPROM.get(Cal_addr + 2, sett_lgc[3][1]);
  Tem_High1 = EEPROM.get(Cal_addr + 4, sett_lgc[3][2]);
  Ref_High1 = EEPROM.get(Cal_addr + 6, sett_lgc[3][3]);
  //---------------------PID values--------------------------------//
  Kp = EEPROM.get(Cal_addr + 8, sett_lgc[4][0]);
  Ki = float(EEPROM.get(Cal_addr + 10, sett_lgc[4][1])) / 10;
  Kd = float(EEPROM.get(Cal_addr + 12, sett_lgc[4][2])) / 10;
  //- ------------------------------------ eeprom location from 20 --------------------------------
  for (int x = 0; x < 6; x++)  //-------------Preset reading
  {
    for (int y = 0; y < 10; y++) {
      int size_mem = 0;
      for (int z = 0; z < 8; z++) {
        //        EEPROM.get(preset_mem_locatn[x][y] + size_mem, preset_lgc[x][z][y]);
        preset_lgc[x][z][y] = pid.readEEPROM(disk1, preset_mem_locatn[x][y] + size_mem);
        size_mem += 2;
      }
    }
  }
  //  sett_lgc[1][0] = EEPROM.read(preset_mem_locatn[5][1] + 2 + 16);  // 20  // hcp point val saved

  u8g2.clearBuffer();  // clear the internal memory
  u8g2.setFont(u8g2_font_nokiafc22_tf);
  u8g2.setDrawColor(1);
  u8g2.drawStr(20, 29, "Labquest  Borosil  ");
  u8g2.drawStr(28, 42, "       V.1.6.A       ");
  //  u8g2.drawStr(20, 42, "  KBD203  V.1.1    ");
  //  u8g2.drawStr(20, 42, "  KBD083  V.1.1    ");
  u8g2.sendBuffer();  // transfer internal memory to the display
  digitalWrite(Buzz, HIGH);
  delay(1000);
  digitalWrite(Buzz, LOW);
  delay(1000);
  all_in_one[0][1] = 20;  // min values of set temperature

  tempRiseCheck.interval(30000);
  flahDisplay.start();
  tempRiseCheck.start();

  if (!Lift_Pos) Auto_Correct = 0;  // DOWN check whether the position of trat is ok or not
}

void lclass::Auto_Corr_fun()  // this function is use to correct the position of the tray if the power cut during the process
{
  if (!Auto_Correct)  // whenever the position changes this variable will set and reset the and check every power condition
  {
    //    Dir_Chk = digitalRead(Actu_dwn_LmT);
    int Actu_dwn_LmT_count = 0;
    for (int i = 0; i < 50; i++) {
      if (digitalRead(Actu_dwn_LmT) == 0) {
        Actu_dwn_LmT_count++;
      }
    }
    if (Actu_dwn_LmT_count > 40) {
      Dir_Chk = 0;
    }

    if (Dir_Chk && !dwn_chk)  // to down till the limit switch is not triggerd
    {
      Step_Count = Dir_Change = 0;
      I_Step = 5000;                  // DOWN
    } else if (!Dir_Chk && !dwn_chk)  // once the limit switch triggerd it goes to the top position and correct the position
    {
      dwn_chk = Dir_Change = 1;  // UP
      Step_Count = 0;
      I_Step = Up_Max;
      Dir_Chk = 1;
    }
    if (!I_Step)  // and save the correct value to the memory
    {
      Auto_Correct = Lift_Pos = 1;
      EEPROM.write(3, Lift_Pos);
    }
  }
}

void lclass::main_menu() {
  if (!currentMenu && !all_in_one[pros][0])  // default condition at the power on condition
  {
    pid.Read_Thermo_P();  // temp read function

    digitalWrite(SSR, HIGH);

    
    if (incArray >= 6 && !checkTempRaised) {
      int i;
      //  Serial3.println("**************************************"); //--------------change


      for (i = 0; i < 5; i++) {
        if (tempArray[i] < tempArray[i + 1])
          checkArray[i] = 1;
        else
          checkArray[i] = 0;
        // Serial3.println(tempArray[i]); //--------------change
        // Serial3.println(checkArray[i]);
      }
      i = 0;
      if (checkArray[i] & checkArray[i + 1] & checkArray[i + 2] & checkArray[i + 3] & checkArray[i + 4]) {
          flahDisplay.interval(500);
          checkTempRaised = 1;
      } else
        checkTempRaised = 0;
      if (processRunUpdates) {
        tempRiseCheck.interval(30000);
        processRunUpdates = 0;
      }
      incArray = 0;
    }

    if (thermo_T_Read >= 480) checkTempRaised = 1;

    u8g2.clearBuffer();  // clear the internal memory

    if (!checkTempRaised) {
      u8g2.setDrawColor(1);         // color 1 for the box
      u8g2.drawBox(0, 0, 128, 11);  // top row box for the home position and the temperature
      u8g2.setFont(u8g2_font_6x10_tf);
      u8g2.setDrawColor(0);
      u8g2.drawStr(2, 9, "Home");  // color(0) means white
      u8g2.setCursor(95, 9);
      u8g2.print(str_tmp);                                                                // temperature on the top of the screen
      u8g2.setDrawColor(1); /* color 1 for the box */                                     // color(0) means black
      u8g2.drawXBM(3, 14 + (13 * pros), u8g_logo_width, u8g_logo_height, u8g_logo_bits);  //-------------Arrow mark logo
      u8g2.setFontMode(1);                                                                /* color 1 for the box */
      u8g2.drawBox(18, 14 + (13 * pros), 75, 11);
      u8g2.setDrawColor(2);  // this will invert the color
      u8g2.drawStr(22, 23, "New process");
      u8g2.drawStr(22, 36, "Preset");
      u8g2.drawStr(22, 49, "Settings");
      up_dwn_arroy(116, 14);  // function for the position of arrow cursor

    }
    else 
    {
      
//      digitalWrite(safteyRelay, HIGH);                             //-----------------------------------------------02/01/2024
      
      u8g2.setFontMode(1); /* color 1 for the box */
      u8g2.setFont(u8g2_font_nokiafc22_tf);

      flahDisplay.update();
      if (!blankDisplayFlag) {
        digitalWrite(Buzz, HIGH);
        u8g2.drawStr(10, 23, "    RELAY FAILURE        ");
        u8g2.drawStr(10, 36, "  SWITCH OFF THE UNIT    ");
        u8g2.drawStr(10, 49, "  CONTACT THE SERVICE    ");
      } else {
        digitalWrite(Buzz, LOW);
        u8g2.drawStr(15, 23, "                     ");
        u8g2.drawStr(15, 36, "                     ");
        u8g2.drawStr(15, 49, "                     ");
      }
    }

    u8g2.sendBuffer();  // transfer internal memory to the display
    LCD.InputFlags();   // button read
    pid.PID_Set();      // pid set for the reset the values
  } else {
    pid.Read_Thermo_P();  // temperature read function
    switch (pros)         // pros is gives location of new process, preset and the settings
    {
      case 0:
        start_menu();  // if proc is 0 then the new process will call
        LCD.InputFlags();
        //          lcd.setCursor(2, 0);  lcd.print("In Start Menu");
        //          Serial3.println("in start");
        break;
      case 1:
        Preset_menu();  // if proc is 1 then preset function will call
        LCD.InputFlags();
        //          lcd.setCursor(2, 0);  lcd.print("In Preset Menu");
        //          Serial3.println("in preset");
        break;
      case 2:
        Setting_menu();  // if proc 2 then the setting function will call
        LCD.InputFlags();
        //          lcd.setCursor(2, 0);  lcd.print("In Settings Menu");
        //          Serial3.println("in settings");
        break;
      default:
        break;
    }
  }
  LCD.Auto_Corr_fun();  // this is to correct the postion of tray if power fail during the process
  LCD.InputFlags();     // buttons to read the input
}

void lclass ::start_menu() {
  if (currentMenu > 0 && currentMenu <= 8)  // if the currentMenu is between the 1 and the 8 user can set the temp and time upto 4 ramps
  {
    u8g2.clearBuffer();                              // clear the internal memory
    u8g2.setDrawColor(2); /* color 1 for the box */  // if the color is 2 then it will invert the string in other color
    u8g2.drawBox(0, 0, 128, 11);                     // to draw the box to show the temp and the name of the process
    u8g2.setFont(u8g2_font_6x10_tf);                 // font defination
    u8g2.drawStr(2, 9, "New Process");               // first row will display current state and the temperature
    u8g2.setCursor(95, 9);
    u8g2.print(str_tmp);             // temperature string
    u8g2.setFont(u8g2_font_4x6_tf);  // font change to small size
    u8g2.drawStr(3, 22, "Ramp");     // will display the ramps is increases
    u8g2.drawLine(0, 26, 128, 26);   // line draw below the ramps
    u8g2.setCursor(2, 35);
    u8g2.print("(°C) ");                       // set temperatures for different ramps
    u8g2.drawStr(2, 46, "(min)");              // set times for the different ramps
    u8g2.drawLine(0, 52, 128, 52);             // draw lines below the ramps
    int tm_r[2] = { 23, 50 };                  // rows
    int tem_1[8], x[4] = { 30, 56, 78, 100 };  //coloms

    for (int j = 0; j < 8; j++)  // for 8 set parameters for the temp and time
    {
      if (j % 2 == 0)
        tem_1[j] = all_in_one[pros][j + 1];  //preset_lgc[all_in_one[1][1]][j][k];
      else
        tem_1[j] = all_in_one[pros][j + 1];  //preset_lgc[all_in_one[1][1]][j][k];
    }

    for (int i = 0; i < ((cur_mx_vl[0] - 1) / 2) + 1; ++i) {
      u8g2.setCursor(x[i] + 8, 23);
      u8g2.print(i + 1);  //---------------------ramp numbers
      u8g2.setCursor(x[i] + 8, 37);
      u8g2.print(tem_1[i * 2]);  //--------------set temperature
      u8g2.setCursor(x[i] + 8, 48);
      u8g2.print(tem_1[(i * 2) + 1]);  //--------set time
    }
    u8g2.drawBox(edit_par1[currentMenu - 1], pres_c1[all_in_one[1][2]] + pre_b1[(currentMenu - 1) % 2], 15, 7);

    u8g2.drawBox(4, 55, 25, 9);
    u8g2.drawBox(36, 55, 25, 9);  //----------------this are the four boxes for the to show the usability of keys
    u8g2.drawBox(68, 55, 25, 9);
    u8g2.drawBox(100, 55, 25, 9);

    if (abs(currentMenu - 1) % 2 != 0 && currentMenu == cur_mx_vl[0])  //------ this is for to check whether the setting of time or temp
      u8g2.drawStr(7, 62, "Start");                                    //-----------------------------------------after setting the ramps we can start the process
    else
      u8g2.drawStr(7, 62, "Next");  //------------------------------------------if we are setting last time value in ramps this will print

    u8g2.drawStr(39, 62, "+Ramp");  //------------------------------------------can add the ramps
    u8g2.drawStr(70, 62, "-Ramp");  //------------------------------------------can minus the ramps
    u8g2.drawStr(104, 62, "Back");  //------------------------------------------use to go back to previous state

    u8g2.sendBuffer();  //------------------------------------------------------transfer internal memory to the display
    LCD.InputFlags();   //------------------------------------------------------reading the button inputs
  } else {
    LCD.Main_Process();  //-----------------------------------------------------main process where process of heating will start
  }
}

void lclass ::Main_Process()  //----------------------------main process just after the settings of ramps, temp and time
{
  int hours, minutes, seconds;  //----------------------------------------------local variables use for the time conversion from hour to minute to seconds
  int remainingSeconds;
  int secondsInHour = 60 * 60;
  int secondsInMinute = 60;
  int time_R = 0;
  Butt_not_work = 1;  //--------------------------------------------------------set the flag before start the process and reset just after the process to restrict the buttons use
  delay(100);
  chng_rmp = time_cng = 0;
  chng_rmp_1 = 7;
  Count_Down = timer = chk_down_1 = 0;           //--------------------------------------initially makes the values to the zero
  windowStartTime = millis();                    //-----------------------------------------------start the windows for the pid calling
  digitalWrite(SCRUBBER, HIGH);                  //---------------------------------------------turn ON the scrubber
  Mot.up_down_lift(Start_1, u8g_logo_Down, 16);  //-----------------------------to go down the tray
  Assisgn_set_para();                            //-------------------------------------------------------whatever values we have set assign to the process variables
  Mot.Buzz_t(1200);                              //---------------------------------------------------------long beep for the process start
  up_dwn_f = 1;                                  //------------------------------------------------------------ to know the position of the tray
  Stop_timer = 1;                                //-----------------------------------------------------------set the variable high for process start acknowleggment
  Graph_M = dig_chk = 0;                         //----------------------------------------------------initially disable the graph
  process_up_d = 1;                              //---------------------------------------------------------process start set variable
  data_log = 1;
  t_count = 0;
  temp_log[0] = thermo_T_Read;
  val_p = 0;                // flags clear for data collection for graph
    flahDisplay.interval(1000);
    tempRiseCheck.stop();
  while (minute_disp != 0)  //--------------------------------------------------this loop will excute till the minute_disp goes to 0
  {
    //    digitalWrite(Limit_Dir, up_dwn_f);     // Direction out in between

    digitalWrite(SSR, HIGH);
    

    if (thermo_T_Read >= 480) {
      checkTempRaised = 1;      

      
      digitalWrite(safteyRelay, HIGH);           //-----------------------------------------------02/01/2024

      
      blankDisplayFlag = 0;
      minute_disp = 0;
    } else {
      checkTempRaised = 0;
      blankDisplayFlag = 1;
    }

    //    checkTempRaised=1;
    //    blankDisplayFlag=0;
    //    minute_disp=0;

    processRunUpdates = 1;

    pid.Read_Thermo_P();  //----------------------------------------------------reading temperature in process
    cng_tmp_time();       //---------------------------------------------------------change ramp calculation
    Setpoint = set_temperature = temp_time_pro[chng_rmp * 2];
    pid.pid_interval(Setpoint, thermo_T_Read);  //-------------------------------temperature control
    hours = (time_R / 60);                      //--------------------------------------------------coversion of sec to hour
    remainingSeconds = time_R - (hours * 60);
    minutes = remainingSeconds;  //---------------------------------------------coversion of sec to minute

    Process_Wndow(hours, minutes);      //------------------------------------------to display the hour and minute in process
    time_R = minute_disp - Count_Down;  //--------------------------------------elapse time push into this variable
    LCD.InputFlags();
    if (stopage || Stop_R == 2) minute_disp = 0;  //---------------------------user can stop the process by long pressing the RST button
                                                  //    Serial3.print(" Step_Count "); Serial3.print(Step_Count);  Serial3.print(" chk_down_1 "); Serial3.println(chk_down_1);
  }
  process_up_d = 0;
  set_temperature = timer = Count_Down = Setpoint = 0;
  Stop_R = wait_tick = scrub_t1 = 0;
  data_log = 0;
  hours = remainingSeconds = minutes = 0;
  val_p = 0;
  minute_disp = 2;
  for (int k = 0; k < 200; k++) temp_log[k] = 0;
  delay(100);
  digitalWrite(SSR, LOW);
  long time_t = millis();
  ;
  long start_t = millis();
  if (chk_down_1) {
    Mot.up_down_lift(Start_1, u8g_logo_Down, 16);
    delay(1500);
  }
  Mot.up_down_lift(Stop_0, u8g_logo_Up, 16);
  Mot.Buzz_t(1000);
  scrub_t = sett_lgc[1][0] * 60;
  while (scrub_t != 0) {
    start_t = millis();
    if (start_t - time_t > 900) {
      scrub_t1++;
      time_t = start_t;
    }
    if (scrub_t1 >= scrub_t) scrub_t = scrub_t1 = 0;

    pid.Read_Thermo_P();
    minutes = (scrub_t - scrub_t1) / 60;
    remainingSeconds = (scrub_t - scrub_t1) - (minutes * 60);
    seconds = remainingSeconds;
    String Scr_T = min_1 + minutes / 10 + minutes % 10 + ":" + seconds / 10 + seconds % 10 + "";
    Mot.Top_scrn();

    flahDisplay.update();
    if (checkTempRaised && !blankDisplayFlag) {
      u8g2.clearBuffer();
      digitalWrite(Buzz, HIGH);
      u8g2.drawStr(10, 15, "    RELAY FAILURE        ");
      u8g2.drawStr(10, 25, "  SWITCH OFF THE UNIT    ");
      u8g2.drawStr(10, 38, "  CONTACT THE SERVICE    ");
    } else {
      u8g2.clearBuffer();
      digitalWrite(Buzz, LOW);
      u8g2.drawStr(30, 28, "Scrubber Delay");
      u8g2.setCursor(50, 42);
      u8g2.print(Scr_T);
    }
    u8g2.setFont(u8g2_font_4x6_tf);
    u8g2.drawBox(100, 55, 25, 9);

    if (!Stop_R) {
      u8g2.drawStr(104, 62, "Stop");
    } else {
      u8g2.drawBox(20, 45, 89, 9);
      u8g2.drawStr(23, 52, "Do You Want to stop ?");
      u8g2.drawBox(4, 55, 25, 9);
      u8g2.drawStr(7, 62, " No ");
      u8g2.drawStr(107, 62, "Yes");
    }
    u8g2.sendBuffer();  // transfer internal memory to the display
    if (Stop_R == 2) scrub_t = 0;
    LCD.InputFlags();
    //    Serial3.print(timer); Serial3.print("  "); Serial3.println(Count_Down);
  }
  flahDisplay.interval(500);
  Mot.Top_scrn();
  u8g2.setCursor(20, 40);
  u8g2.print(" Process Complete ");  // here will be lift up code comes
  u8g2.setFont(u8g2_font_4x6_tf);
  u8g2.drawBox(100, 55, 25, 9);
  u8g2.drawStr(104, 62, "Stop");
  u8g2.sendBuffer();  // transfer internal memory to the display
  Mot.Buzz_t(1500);
  digitalWrite(SCRUBBER, LOW);

  incArray = 0;
  
  tempRiseCheck.interval(90000);
  tempRiseCheck.start();

  scrub_t = scrub_t1 = timer = last_tick = 0;
  Stop_timer = currentMenu = minute_disp = Stop_R = 0;
  all_in_one[0][0] = all_in_one[1][0] = all_in_one[2][0] = 0;
}

void lclass ::Preset_menu()  //---------------------------------------Preset Menu
{
  u8g2.clearBuffer();   // clear the internal memory
  u8g2.setDrawColor(1); /* color 1 for the box */
  u8g2.drawBox(0, 0, 128, 11);
  u8g2.setFont(u8g2_font_6x10_tf);
  if (currentMenu == 1) {
    u8g2.setDrawColor(0);
    u8g2.drawStr(2, 9, "Preset");
    u8g2.setCursor(95, 9);
    u8g2.print(str_tmp);  // temp show
    u8g2.setDrawColor(1); /* color 1 for the box */
    u8g2.drawXBM(3, 14 + (13 * cur_move), u8g_logo_width, u8g_logo_height, u8g_logo_bits);
    u8g2.setFontMode(1);                             /* color 1 for the box */
    u8g2.drawBox(18, 14 + (13 * cur_move), 50, 11);  // cursor box
    for (int i = 0; i < 3; i++) {
      u8g2.setDrawColor(2);
      u8g2.drawStr(22, 23 + (13 * i), Preset[i + cur_move1]);  // preset lists, exe. milk, food, feed, soil brewary, water
    }
    up_dwn_arroy(116, 14);
    pre_show = pre_chk1 = 0;
    u8g2.sendBuffer();  // transfer internal memory to the display
  } else if (2 <= currentMenu && currentMenu <= 10) {
    pre_list = tyu + "Preset>" + Preset[all_in_one[1][1]];  // preset with selected sample
    u8g2.setDrawColor(0);
    u8g2.setCursor(2, 9);
    u8g2.print(pre_list);
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.setCursor(95, 9);
    u8g2.print(str_tmp);

    u8g2.setFontMode(1);                             /* color 1 for the box */
    u8g2.setDrawColor(1);                            /* color 1 for the box */
    u8g2.drawBox(0, pres_c[pre_chk1] + 15, 15, 13);  // for one and two box back ground

    if (sr_Flags[0])  // edit enable
      u8g2.drawBox(edit_par[currentMenu - 3], pres_c[pre_chk1] + pre_b[(currentMenu - 3) % 2], 15, 7);

    u8g2.setDrawColor(2);
    //    u8g2.drawStr(2, 25, "01");
    //    u8g2.drawStr(2, 46, "02");

    u8g2.setCursor(2, 25);
    u8g2.print(0);
    u8g2.print(pre_show + 1);  // preset in numbers
    u8g2.setCursor(2, 46);
    if (pre_show + 2 < 10) u8g2.print(0);
    u8g2.print(pre_show + 2);

    u8g2.drawLine(0, 33, 128, 33);
    int tm_r[2] = { 21, 42 };                    // temp and time row
    int tem_1[8][2], x[4] = { 35, 56, 77, 98 };  // coloms
    for (int k = 0; k < 2; k++) {
      u8g2.setFont(u8g2_font_4x6_tf);
      u8g2.setCursor(17, tm_r[k]);
      u8g2.print("(°C) ");
      u8g2.drawStr(17, tm_r[k] + 9, "(min)");
      for (int j = 0; j < 8; j++) {
        if (j % 2 == 0)
          tem_1[j][k] = preset_lgc[all_in_one[1][1]][j][k + pre_show];  // pre_show for displaying 1to 10 presets
        else
          tem_1[j][k] = preset_lgc[all_in_one[1][1]][j][k + pre_show];
      }
    }
    for (int i = 0; i < 4; i++)  // displaying presets
    {
      u8g2.setCursor(x[i] + 8, 21);
      u8g2.print(tem_1[i * 2][0]);
      u8g2.setCursor(x[i] + 8, 30);
      u8g2.print(tem_1[(i * 2) + 1][0]);
      u8g2.setCursor(x[i] + 8, 42);
      u8g2.print(tem_1[i * 2][1]);
      u8g2.setCursor(x[i] + 8, 51);
      u8g2.print(tem_1[(i * 2) + 1][1]);
    }

    u8g2.drawBox(4, 55, 25, 9);
    u8g2.drawBox(36, 55, 25, 9);  // box for the keys usability
    u8g2.drawBox(68, 55, 25, 9);
    u8g2.drawBox(100, 55, 25, 9);

    if (sr_Flags[0]) u8g2.drawStr(7, 62, "Next");
    else u8g2.drawStr(7, 62, "Start");  // we can start the process when sr_Flags[0]
    u8g2.drawStr(39, 62, "Edit");       // "sr_Flags[0]" this variable set and reset in save and edit button functions
    u8g2.drawStr(73, 62, "Save");
    u8g2.drawStr(104, 62, "Back");
    u8g2.sendBuffer();  // transfer internal memory to the display

    LCD.preset_mem_save();  // "Save_Button()" search this function for edit and save enable disable
  } else {
    LCD.Main_Process();
  }
}

void lclass::preset_mem_save() {
  if (Pre_Mem_Save) {
    int tem_2[8][10], y_L;
    for (int j = 0; j < 8; j++) {
      tem_2[j][Pre_Loc] = preset_lgc[all_in_one[1][1]][j][Pre_Loc];
      //      Serial3.print(tem_2[j][all_in_one[pros][currentMenu]]); Serial3.print("  ");
    }
    y_L = preset_mem_locatn[all_in_one[1][1]][Pre_Loc];
    //    Serial3.println(y_L);
    for (unsigned int x = y_L; x < (y_L + 16); x += 2) {
      //    EEPROM.put(x, tem_2[(x - y_L) / 2][all_in_one[pros][currentMenu]]);
      pid.writeEEPROM(disk1, x, tem_2[(x - y_L) / 2][Pre_Loc]);
      //      Serial3.print((x-y_L)/2); Serial3.print("  "); Serial3.print(tem_2[(x-y_L)/2][Pre_Loc]); Serial3.print("  "); Serial3.println(Pre_Loc);
    }
    Pre_Mem_Save = 0;
  }
}
void lclass ::Setting_menu() {
  int ltime, mtime;
  u8g2.clearBuffer();   // clear the internal memory
  u8g2.setDrawColor(1); /* color 1 for the box */
  u8g2.drawBox(0, 0, 128, 11);
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setDrawColor(0);
  u8g2.drawStr(4, 9, "Settings");  // first row setting showing
  u8g2.setCursor(95, 9);
  u8g2.print(str_tmp);   // temperature printing on top
  if (currentMenu == 1)  // first window of setting list the different features
  {
    u8g2.setDrawColor(1); /* color 1 for the box */
    u8g2.drawXBM(3, 14 + (13 * cur_move), u8g_logo_width, u8g_logo_height, u8g_logo_bits);
    u8g2.setFontMode(1); /* color 1 for the box */
    u8g2.drawBox(18, 14 + (13 * cur_move), 85, 11);
    for (int i = 0; i < 3; i++) {
      u8g2.setDrawColor(2);
      u8g2.drawStr(22, 23 + (13 * i), settings[i + cur_move1]);
    }
    up_dwn_arroy(116, 14);
    u8g2.sendBuffer();  // transfer internal memory to the display
  }
  if ((2 <= currentMenu && currentMenu <= 3) && all_in_one[2][1] == 0)  //all_in_one[pros][currentMenu]
  {
    String heat_1, heat_2;
    heat_1 = heat_2 + sett_lgc[all_in_one[pros][1]][currentMenu - 2] + " °C/min";  // string concadenation
    u8g2.setDrawColor(2);
    if (currentMenu == 2) {
      u8g2.setCursor(30, 23 + 8);
      u8g2.print("Heater Slope");
      u8g2.setCursor(36, 23 + 21);
      u8g2.print(heat_1);
    } else {
      u8g2.drawStr(52, 44, "Saved");
      u8g2.drawBox(50, 35, 33, 11);
    }
    u8g2.setFont(u8g2_font_4x6_tf);
    u8g2.drawBox(3, 55, 25, 9);
    u8g2.drawStr(6, 62, "Save");
    u8g2.drawBox(100, 55, 25, 9);
    u8g2.drawStr(104, 62, "Back");
    u8g2.sendBuffer();  // transfer internal memory to the display

    if (currentMenu == 3) {
      EEPROM.write(2, sett_lgc[all_in_one[pros][1]][0]);
      delay(1000);
      currentMenu = 1;
    }
  } else if ((2 <= currentMenu && currentMenu <= 3) && all_in_one[2][1] == 1)  //-------------Scrubber delay
  {
    u8g2.setDrawColor(2);
    u8g2.drawStr(22, 31, "Scrubber Delay");
    //    mtime = sett_lgc[all_in_one[pros][1]][currentMenu - 2];
    if (currentMenu == 2) {
      u8g2.setCursor(59, 44);
      u8g2.print(sett_lgc[all_in_one[pros][1]][currentMenu - 2]);
      u8g2.drawBox(57, 35, 16, 11);
    } else {
      u8g2.drawStr(52, 44, "Saved");
      u8g2.drawBox(50, 35, 33, 11);
    }
    u8g2.setFont(u8g2_font_4x6_tf);
    u8g2.drawBox(3, 55, 25, 9);
    u8g2.drawStr(6, 62, "Save");
    u8g2.drawBox(100, 55, 25, 9);
    u8g2.drawStr(104, 62, "Back");
    u8g2.sendBuffer();  // transfer internal memory to the display
    if (currentMenu == 3) {
      EEPROM.write(4, sett_lgc[1][0]);
      delay(1000);
      currentMenu = 1;
    }
  } else if ((2 <= currentMenu && currentMenu <= 3) && all_in_one[2][1] == 2)  //-----------One point calobration
  {
    int m_err;
    u8g2.setDrawColor(2);
    if (2 == currentMenu) {
      u8g2.drawStr(16, 29, "Temp    Set Error");
      u8g2.setCursor(14, 45);
      u8g2.print(str_tmp);

      if (sett_lgc[all_in_one[pros][1]][currentMenu - 2] >= 50) {
        u8g2.drawStr(67, 45, " + ");
        m_err = (sett_lgc[all_in_one[pros][1]][currentMenu - 2] - 50);
      } else {
        u8g2.drawStr(67, 45, " - ");
        m_err = (50 - sett_lgc[all_in_one[pros][1]][currentMenu - 2]);
      }

      u8g2.setCursor(92, 45);
      u8g2.print(m_err);
      u8g2.drawBox(13, 21, 30, 10);
      u8g2.drawBox(62, 21, 57, 10);
      u8g2.drawBox(70, 36, 36, 10);
    } else {
      u8g2.drawStr(52, 44, "Saved");
      u8g2.drawBox(50, 35, 33, 11);
      //      delay(1000);
    }

    u8g2.setFont(u8g2_font_4x6_tf);
    u8g2.drawBox(3, 55, 25, 9);
    u8g2.drawStr(6, 62, "Save");
    u8g2.drawBox(100, 55, 25, 9);
    u8g2.drawStr(104, 62, "Back");
    u8g2.sendBuffer();  // transfer internal memory to the display

    if (currentMenu == 3) {
      EEPROM.write(1, sett_lgc[all_in_one[pros][1]][0]);
      delay(1000);
      currentMenu = 1;
    }
  } else if ((2 <= currentMenu && currentMenu <= 6) && all_in_one[2][1] == 3)  //--------------------Two Point Calibration
  {
    int x1_y1[2] = { 33, 90 }, x2_y2[2] = { 27, 41 };  // for cursor position coordinates
    bool shift_1 = 0;
    u8g2.setDrawColor(2); /* color 1 for the box */
    if (currentMenu < 6) {
      u8g2.setFontMode(1); /* color 1 for the box */
      u8g2.drawBox(24, 14 + (13 * 0), 38, 11);
      u8g2.drawBox(82, 14 + (13 * 0), 38, 11);
      u8g2.drawStr(28, 23 + 0, " LCP       HCP");

      if (currentMenu < 4) shift_1 = 0;
      else shift_1 = 1;                                              // if shift is zero then colom for temp low and ref low is same
      u8g2.drawBox(x1_y1[shift_1], x2_y2[currentMenu % 2], 21, 11);  // for cursor coordinates

      u8g2.setCursor(35, 36);
      u8g2.print(sett_lgc[all_in_one[pros][1]][0]);  // Temp_Low1
      u8g2.setCursor(92, 36);
      u8g2.print(sett_lgc[all_in_one[pros][1]][2]);  //Temp_High
      u8g2.setCursor(35, 50);
      u8g2.print(sett_lgc[all_in_one[pros][1]][1]);  // Ref_Low1
      u8g2.setCursor(92, 50);
      u8g2.print(sett_lgc[all_in_one[pros][1]][3]);  //Ref_High

      u8g2.setFont(u8g2_font_4x6_tf);
      u8g2.setCursor(4, 35);
      u8g2.print("(°C) ");
      u8g2.drawStr(2, 49, "(Ref)");

      u8g2.drawLine(0, 39, 128, 39);

    } else {
      u8g2.drawStr(52, 44, "Saved");
      u8g2.drawBox(50, 35, 33, 11);
    }

    u8g2.setFont(u8g2_font_4x6_tf);
    u8g2.drawBox(3, 55, 25, 9);
    if (currentMenu >= 5) u8g2.drawStr(8, 62, "save");
    else u8g2.drawStr(8, 62, "next");
    u8g2.drawBox(100, 55, 25, 9);
    u8g2.drawStr(104, 62, "Back");

    u8g2.sendBuffer();  // transfer internal memory to the display

    if (currentMenu == 6)  // saving the values in eeprom
    {
      Temp_Low1 = EEPROM.put(Cal_addr + 0, sett_lgc[all_in_one[pros][1]][0]);  // Temp_Low1
      Ref_Low1 = EEPROM.put(Cal_addr + 2, sett_lgc[all_in_one[pros][1]][1]);   // Ref_Low1
      Tem_High1 = EEPROM.put(Cal_addr + 4, sett_lgc[all_in_one[pros][1]][2]);  //Temp_High
      Ref_High1 = EEPROM.put(Cal_addr + 6, sett_lgc[all_in_one[pros][1]][3]);  //Ref_High

      delay(1000);
      currentMenu = 1;
    }
  } else if ((2 <= currentMenu && currentMenu <= 5) && all_in_one[2][1] == 4) {
    int p_i_d[3] = { 18, 54, 90 };  // positions for the cursor
    u8g2.setDrawColor(2);
    if (currentMenu < 5) {
      u8g2.drawBox(p_i_d[currentMenu - 2], 33, 16, 11);
      u8g2.drawStr(14, 13 + 13, " Kp    Ki    Kd ");
      u8g2.setCursor(20, 42);
      u8g2.print(sett_lgc[all_in_one[pros][1]][0]);
      u8g2.setCursor(56, 42);
      u8g2.print(sett_lgc[all_in_one[pros][1]][1]);
      u8g2.setCursor(92, 42);
      u8g2.print(sett_lgc[all_in_one[pros][1]][2]);
    } else {
      u8g2.drawStr(52, 40, "Saved");
      u8g2.drawBox(50, 31, 33, 11);
    }

    u8g2.setFont(u8g2_font_4x6_tf);
    u8g2.drawBox(3, 55, 25, 9);
    if (currentMenu >= 4) u8g2.drawStr(8, 62, "save");
    else u8g2.drawStr(8, 62, "next");
    u8g2.drawBox(100, 55, 25, 9);
    u8g2.drawStr(104, 62, "Back");
    //    }
    u8g2.sendBuffer();  // transfer internal memory to the display

    if (currentMenu == 5)  // pdd values saved in memory
    {
      Kp = EEPROM.put(Cal_addr + 8, sett_lgc[all_in_one[pros][1]][0]);
      Ki = float(EEPROM.put(Cal_addr + 10, sett_lgc[all_in_one[pros][1]][1])) / 10;
      Kd = float(EEPROM.put(Cal_addr + 12, sett_lgc[all_in_one[pros][1]][2])) / 10;

      delay(1000);
      currentMenu = 1;
    }
  } else if ((2 <= currentMenu && currentMenu <= 5) && all_in_one[2][1] == 5) {
    if (2 == currentMenu) {
      int x = 118, y = 14;
      u8g2.setDrawColor(2);
      u8g2.setFont(u8g2_font_4x6_tf);
      u8g2.drawStr(4, 21, "1. Use Up/Down Key to adjust");
      u8g2.drawStr(4, 31, "   the marking position.");
      u8g2.drawStr(4, 41, "2. Start and wait until the ");
      u8g2.drawStr(4, 51, "   calibration is done.");
      //     u8g2.drawStr(5, 61, "3. Save the settings. ");

      u8g2.drawXBM(x, y, u8g_logo_width, u8g_logo_height - 6, u8g_logo_Up);
      u8g2.drawXBM(x, (y + 32), u8g_logo_width, u8g_logo_height - 6, u8g_logo_Down);

      u8g2.drawBox(3, 55, 25, 9);
      u8g2.drawStr(6, 62, "Start");
      u8g2.drawBox(100, 55, 25, 9);
      u8g2.drawStr(104, 62, "Back");
      u8g2.sendBuffer();  // transfer internal memory to the display

    } else if (3 == currentMenu) {
      u8g2.setDrawColor(2);
      u8g2.setFont(u8g2_font_6x10_tf);
      u8g2.drawStr(32, 35, "Calibrating");
      u8g2.drawStr(32, 50, "Please Wait");
      if (!Dir_Change)  // currentmenu incremnt just after the lift reach to the top in "Stepper_Rotate" this function
      {
        Step_Count = 0;
        I_Step = 5000;
      }
      u8g2.setFont(u8g2_font_4x6_tf);
      u8g2.drawBox(100, 55, 25, 9);
      u8g2.drawStr(104, 62, "Stop");
      u8g2.sendBuffer();  // transfer internal memory to the display
    } else if (4 == currentMenu) {
      u8g2.setDrawColor(2);
      u8g2.setFont(u8g2_font_6x10_tf);
      u8g2.drawStr(32, 35, "Calibration");
      u8g2.drawStr(32, 50, "   Done    ");
      u8g2.setFont(u8g2_font_4x6_tf);
      u8g2.drawBox(100, 55, 25, 9);
      u8g2.drawStr(104, 62, "Stop");
      u8g2.sendBuffer();  // transfer internal memory to the display
      Mot.Buzz_t(1500);
      delay(2000);
      currentMenu = 1;
    }
  }
}

void lclass ::up_dwn_arroy(int x, int y) {
  if (currentMenu != 0) {
    if (!u_d_arrow) u8g2.drawXBM(x, y, u8g_logo_width, u8g_logo_height - 6, u8g_logo_Up);
    else u8g2.drawXBM(x, (y + 32), u8g_logo_width, u8g_logo_height - 6, u8g_logo_Down);
  }

  u8g2.setFont(u8g2_font_4x6_tf);
  u8g2.drawBox(3, 55, 25, 9);
  u8g2.drawStr(4, 62, "Select");
  u8g2.drawBox(100, 55, 25, 9);
  u8g2.drawStr(104, 62, "Back");
}
// ---------------------------------------------------------------------------------------------

void lclass ::InputFlags() {
  for (int i = 0; i < 4; i++) {
    if (inputFlags[i] == HIGH) {
      MenuAction(i);
      inputFlags[i] = LOW;
      if (0 <= i <= 1) {
        buz_bip = 1;
        buzz_time = 0;
      }
    }
  }
}

void lclass ::MenuAction(int input) {
  if (input == 2)  // set button
  {
    if (pros == 2 && currentMenu == 2 && all_in_one[2][1] == 5)  // Auto calibration mode
    {
      Dir_Change = Step_Count = I_Step = 0;
    }
    if (pros == 1 && currentMenu == 2) currentMenu = 11;        // to start the process from presets
    else if (pros == 1 && currentMenu == 10) currentMenu = 10;  // if edit mode is enable then max will limit and never start the process untill save the changes
    else currentMenu++;

    if ((pros <= 1) && (currentMenu > cur_mx_vl[pros])) currentMenu = 11;  //to start the process as per the ramp and preset
    else if ((pros > 1) && currentMenu > set_mx_vl[all_in_one[2][1]]) currentMenu = set_mx_vl[all_in_one[2][1]];
    u_d_arrow = 1;  // for one step up
    if (pros == 1) {
      if (currentMenu > 2) all_in_one[1][currentMenu] = all_in_one[1][currentMenu - 1];
      if ((currentMenu > 4) && preset_lgc[all_in_one[1][1]][currentMenu - 3][all_in_one[pros][2]] < preset_lgc[all_in_one[pros][1]][currentMenu - 5][all_in_one[pros][2]])
        preset_lgc[all_in_one[1][1]][currentMenu - 3][all_in_one[pros][2]] = preset_lgc[all_in_one[pros][1]][currentMenu - 5][all_in_one[pros][2]];  // in ramp and preset max set temp is the minimum temp for next temp
    } else if (!pros && currentMenu > 1 && currentMenu <= cur_mx_vl[pros] && (currentMenu % 2) != 0 && all_in_one[pros][currentMenu] <= all_in_one[pros][currentMenu - 2]) {
      all_in_one[pros][currentMenu] = all_in_one[pros][currentMenu - 2];
    } else if (!pros && currentMenu > 1 && currentMenu <= cur_mx_vl[pros] && (currentMenu % 2) == 0 && all_in_one[pros][currentMenu] <= all_in_one[pros][currentMenu - 2]) {
      if (abs(currentMenu - 1) % 2 != 0) {
        if (currentMenu == 2) {
          if (int(thermo_T_Read) < all_in_one[pros][currentMenu - 1]) {
            min_time = (all_in_one[pros][currentMenu - 1] - int(thermo_T_Read)) / sett_lgc[0][0];  // calculating the minimum time from heater slope
          }
        } else {
          if (all_in_one[pros][currentMenu - 1] > all_in_one[pros][currentMenu - 3]) {
            min_time = (all_in_one[pros][currentMenu - 1] - all_in_one[pros][currentMenu - 3]) / sett_lgc[0][0];  // calculating the minimum time from heater slope
          }
        }
      }
      if (min_time < 0) min_time = 0;
      all_in_one[pros][currentMenu] = min_time;
    }

    if (currentMenu == 11 && Stop_timer && !Stop_R) Graph_M = !Graph_M;  // graph and data in process
    Stop_R = stop_wait = 0;
  } else if (input == 3)  // reset button
  {
    if (pros == 1 && currentMenu == 3)  // if edit anabled then if currentmenu 3 nd press reset button and to disabled the edit mode
    {
      sr_Flags[0] = !sr_Flags[0];  // to disabled the edit mode
      currentMenu = 2;
    } else if (pros <= 1 && currentMenu == 11) currentMenu = 11;  // to limit the max currentMenu
    else currentMenu--;
    if (currentMenu < 0) currentMenu = 0;  // to limit the min currentMenu
    if (pros == 1) {
      if (currentMenu == 1) sr_Flags[0] = sr_Flags[1] = all_in_one[1][2] = 0;  // for default set vals
      else if (currentMenu > 1) all_in_one[1][currentMenu] = all_in_one[1][currentMenu + 1];
    }
    if (Stop_timer) {
      Stop_R++;
      if (Stop_R > 2) Stop_R = 2;
    }  // stop window popup
    else {
      if (Auto_Correct) Step_Count = I_Step = 0;
    }                                   // if process is
  } else if (input == 0) setChange(0);  // up
  else if (input == 1) setChange(1);    // down

  if (currentMenu != menuSame) {
    if (!currentMenu && !pros1) {
      u_d_arrow = pros1 = 1;
      cur_move1 = cur_move = 0;
      for (int i = 1; i < 3; i++) all_in_one[i][1] = 0;
    } else if (currentMenu && pros1) {
      //      lcd.clear();
      pros1 = 0;
    }

    //    if(pros==1 && currentMenu==1) lcd.clear();
    set_w1 = menuSame = currentMenu;
    Probe_blink = 0;
  }
}

void lclass ::setChange(int key) {
  if (key == 0)  // up
  {
    if (!currentMenu)  // main menu
    {
      pros--;
      if (pros < 0) pros = 2;
      u_d_arrow = 0;
    } else if ((pros == 1 || pros == 2) && currentMenu == 1)  //for preset and setting parameter to scroll up
    {
      u_d_arrow = 0;
      cur_move--;
      if (cur_move < 0) {
        cur_move = 0;
        cur_move1--;
      }
      all_in_one[pros][currentMenu] -= 1;
      if (all_in_one[pros][currentMenu] < 0) {
        if (pros == 1) {
          all_in_one[pros][currentMenu] = 5;
          cur_move = 2;
          cur_move1 = 3;
        }
        if (pros == 2) {
          cur_move = 2;
          cur_move1 = 3;
          all_in_one[pros][currentMenu] = 5;
        }
      }
    } else if (pros == 0 && currentMenu >= 1 && currentMenu < 11) {
      all_in_one[pros][currentMenu] += (inc2 * 1);  //(1 * inc2);//increment1[pros][currentMenu - 1];
      if (all_in_one[pros][currentMenu] > max_lmit1[pros][currentMenu]) all_in_one[pros][currentMenu] = max_lmit1[pros][currentMenu];
    } else if (pros == 1 && currentMenu > 1 && currentMenu < 11) {
      if (!sr_Flags[0]) {
        if (!pre_chk1) {
          pre_show--;
          if (pre_show < 0) pre_show = 0;
        }
        if (pre_chk1) pre_chk1 = 0;
        all_in_one[pros][currentMenu] -= inc2;  //(1 * inc2);//decrement1[pros][currentMenu-2];
        if (all_in_one[pros][currentMenu] < 0) all_in_one[pros][currentMenu] = 0;
      } else {
        preset_lgc[all_in_one[pros][1]][currentMenu - 3][all_in_one[pros][2]] += inc2;  //(1 * inc2); //increment1[0][currentMenu-3];
        if (preset_lgc[all_in_one[pros][1]][currentMenu - 3][all_in_one[pros][2]] > max_lmit1[0][currentMenu - 2]) {
          preset_lgc[all_in_one[pros][1]][currentMenu - 3][all_in_one[pros][2]] = max_lmit1[0][currentMenu - 2];
        }
      }
    } else if (pros == 2 && currentMenu > 1 && currentMenu < 11) {
      sett_lgc[all_in_one[pros][1]][currentMenu - 2] += 2;
      inc2;  //(1 * inc2);//increment1[pros][currentMenu];
      if (sett_lgc[all_in_one[pros][1]][currentMenu - 2] > S_max_lmit1[all_in_one[pros][1]][currentMenu - 2])
        sett_lgc[all_in_one[pros][1]][currentMenu - 2] = S_max_lmit1[all_in_one[pros][1]][currentMenu - 2];

      if (currentMenu == 2 && all_in_one[2][1] == 5) {
        if (!I_Step) Step_Count = 0;
        I_Step = 400;
        Dir_Change = 1;
      }
    }

    if (currentMenu == 11 && Stop_timer && process_up_d) {
      if (!dig_chk && up_dwn_f) {
        Dir_Change = chk_down_1 = 1;  // UP
        Step_Count = up_dwn_f = Lift_in_P = 0;
        I_Step = Up_Max;
      }
    }
  } else if (key == 1)  //down
  {
    //    Probe_blink = 0;
    if (!currentMenu)  //for preset and setting parameter to scroll down
    {
      pros++;
      if (pros > 2) pros = 0;
      u_d_arrow = 1;
    } else if ((pros == 1 || pros == 2) && currentMenu == 1) {
      u_d_arrow = 1;
      cur_move++;
      if (cur_move > 2) {
        cur_move = 2;
        cur_move1++;
      }
      all_in_one[pros][currentMenu] += 1;
      if (all_in_one[pros][currentMenu] > max_lmit1[pros][currentMenu]) cur_move1 = cur_move = all_in_one[pros][currentMenu] = 0;
    } else if (pros == 0 && currentMenu >= 1 && currentMenu < 11) {
      //      int min_time,min_time1;
      all_in_one[pros][currentMenu] -= (inc2 * 1);  //(1 * inc2);//decrement1[pros][currentMenu - 1];
      if (abs(currentMenu - 1) % 2 != 0) {
        if (currentMenu == 2) {
          if (int(thermo_T_Read) < all_in_one[pros][currentMenu - 1]) {
            min_time = (all_in_one[pros][currentMenu - 1] - int(thermo_T_Read)) / sett_lgc[0][0];
            if (all_in_one[pros][currentMenu] < min_time) all_in_one[pros][currentMenu] = min_time;
          } else {
            if (all_in_one[pros][currentMenu] < 0) all_in_one[pros][currentMenu] = 0;
          }
        } else {
          if (all_in_one[pros][currentMenu - 1] > all_in_one[pros][currentMenu - 3]) {
            min_time = (all_in_one[pros][currentMenu - 1] - all_in_one[pros][currentMenu - 3]) / sett_lgc[0][0];
            if (all_in_one[pros][currentMenu] < min_time) all_in_one[pros][currentMenu] = min_time;
          } else {
            if (all_in_one[pros][currentMenu] < 0) all_in_one[pros][currentMenu] = 0;
          }
        }
      } else {
        if (currentMenu == 1) {
          if (all_in_one[pros][currentMenu] < 20) all_in_one[pros][currentMenu] = 20;
        } else {
          if (all_in_one[pros][currentMenu] < all_in_one[pros][currentMenu - 2])
            all_in_one[pros][currentMenu] = all_in_one[pros][currentMenu - 2];
        }
      }
      //      if (all_in_one[pros][currentMenu] < 0) all_in_one[pros][currentMenu] = 0;
    } else if (pros == 1 && currentMenu > 1 && currentMenu < 11) {
      if (!sr_Flags[0])  // edit disable
      {
        all_in_one[pros][currentMenu] += inc2;  //(1 * inc2); //increment1[pros][currentMenu - 2];
        if (all_in_one[pros][currentMenu] > max_lmit1[pros][currentMenu]) all_in_one[pros][currentMenu] = max_lmit1[pros][currentMenu];

        if (pre_chk1) {
          pre_show++;
          if (pre_show > (all_in_one[pros][currentMenu] - 1)) pre_show = all_in_one[pros][currentMenu] - 1;
        }
        if (!pre_chk1) pre_chk1 = 1;
      } else  // edit enable
      {
        preset_lgc[all_in_one[pros][1]][currentMenu - 3][all_in_one[pros][2]] -= inc2;  //(1 * inc2); //decrement1[0][currentMenu-3];
        if (abs(currentMenu - 3) % 2 != 0) {
          if (preset_lgc[all_in_one[pros][1]][currentMenu - 3][all_in_one[pros][2]] < 0) preset_lgc[all_in_one[1][1]][currentMenu - 3][all_in_one[pros][2]] = 0;
        } else {
          if ((currentMenu - 3) > 1) {
            if (preset_lgc[all_in_one[pros][1]][currentMenu - 3][all_in_one[pros][2]] < preset_lgc[all_in_one[pros][1]][currentMenu - 5][all_in_one[pros][2]])
              preset_lgc[all_in_one[1][1]][currentMenu - 3][all_in_one[pros][2]] = preset_lgc[all_in_one[pros][1]][currentMenu - 5][all_in_one[pros][2]];
          } else if ((currentMenu - 3) == 0)
            if (preset_lgc[all_in_one[pros][1]][currentMenu - 3][all_in_one[pros][2]] < 0) preset_lgc[all_in_one[1][1]][currentMenu - 3][all_in_one[pros][2]] = 0;
        }
      }
    } else if (pros == 2 && currentMenu > 1 && currentMenu < 11) {
      sett_lgc[all_in_one[pros][1]][currentMenu - 2] -= 1;  //inc2;//(1 * inc2);//decrement1[pros][currentMenu];
      if (sett_lgc[all_in_one[pros][1]][currentMenu - 2] < S_min_lmit1[all_in_one[pros][1]][currentMenu - 2])
        sett_lgc[all_in_one[pros][1]][currentMenu - 2] = S_min_lmit1[all_in_one[pros][1]][currentMenu - 2];

      if (currentMenu == 2 && all_in_one[2][1] == 5) {
        if (!I_Step) Step_Count = 0;
        I_Step = 400;
        Dir_Change = 0;
      }
    }
    if (currentMenu == 11 && Stop_timer && process_up_d) {
      if (dig_chk && !up_dwn_f && Lift_in_P) {
        up_dwn_f = 1;
        Step_Count = Dir_Change = Lift_in_P = 0;  // Down
        I_Step = Up_Max;
      }
    }
  }
}

// ----------------------------------------- Menu Settings ---------------------------------------

void lclass ::temp_display(int val, int col, int row) {
  //  int store_t[3];
  //  lcd.setCursor(col, row);
  //  for(int i=0;i<3;i++)
  //  {
  //    store_t[i] = val%10;
  //    val = val/10;
  //  }
  //  for(int j=2;j>=0;j--)   lcd.print(store_t[j]);
  //  lcd.print((char)223);   lcd.print("C ");
}


void lclass ::cng_tmp_time() {
  if (chng_rmp_1 != chng_rmp) {
    time_cng = 0;
    for (int x = 0; x <= chng_rmp; x++) {
      time_cng += temp_time_pro[(x * 2) + 1];
    }
    chng_rmp_1 = chng_rmp;
  }

  if (Count_Down > time_cng) {
    chng_rmp++;
    if (!pros) {
      if (chng_rmp > ((cur_mx_vl[0] / 2) - 1)) chng_rmp = (cur_mx_vl[0] / 2) - 1;
    } else {
      if (chng_rmp > 3) chng_rmp = 3;
    }
  }
}

void lclass ::Assisgn_set_para() {
  for (int i = 0; i < 8; i++) temp_time_pro[i] = 0;  // clear all the set temp and time set parameters
  if (!pros) {
    for (int i = 1; i <= cur_mx_vl[0]; i++)  // as per the ramps it will show no. of columns
    {
      temp_time_pro[i - 1] = all_in_one[pros][i];
    }
  } else {
    for (int i = 0; i < 8; i++) {
      temp_time_pro[i] = preset_lgc[all_in_one[1][1]][i][all_in_one[1][2]];  //preset assignment
      arrayrdc[i] = temp_time_pro[i];
    }
  }

  for (int i = 0; i < 4; i++) {
    //      Serial3.print(temp_time_pro[i*2]); Serial3.print(" "); Serial3.println(temp_time_pro[(i*2)+1]);
    minute_disp += temp_time_pro[(i * 2) + 1];  // total time checks if ramps are setted
  }
}

void lclass ::Process_disp(int hour_2, int min_2)  // in process screen handled
{
  u8g2.setDrawColor(2); /* color 1 for the box */
  u8g2.drawBox(0, 0, 128, 11);
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setCursor(2, 9);
  if (!pros) u8g2.print("New Process");
  else u8g2.print(pre_list);
  u8g2.setCursor(95, 9);
  u8g2.print(str_tmp);

  u8g2.setFontMode(1); /* color 1 for the box */ /* color 1 for the box */
  u8g2.drawBox(0, 15, 15, 13);                   // for one and two box back ground

  if (cursorr && !last_tick)
    u8g2.drawXBM(edit_par[chng_rmp * 2] + 4, 36, 7, 7, u8g_top_arroy);

  for (int y = 1; y <= (chng_rmp + last_tick); y++) {
    u8g2.drawXBM(edit_par[(y - 1) * 2], 36, 15, 7, u8g_right_mark);
  }

  for (int x = 0; x < 2; x++) u8g2.drawBox(edit_par[chng_rmp * 2], pre_b[x], 15, 7);

  if (!all_in_one[1][2])
    u8g2.drawStr(2, 25, "01");
  else
    u8g2.drawStr(2, 25, "02");
  u8g2.drawLine(0, 33, 128, 33);
  u8g2.setFont(u8g2_font_4x6_tf);

  int tem_1[8], x[4] = { 35, 56, 77, 98 };

  u8g2.setCursor(17, 21);
  u8g2.print("(°C) ");
  u8g2.drawStr(17, 21 + 9, "(min)");
  for (int j = 0; j < 8; j++) {
    if ((chng_rmp * 2) == j) {
      tem_1[j] = temp_time_pro[j];  //thermo_T_Read;
      tem_1[j + 1] = Count_Down;
      j++;
    } else {
      if (j % 2 == 0)
        tem_1[j] = temp_time_pro[j];
      else
        tem_1[j] = temp_time_pro[j];
    }
  }

  for (int i = 0; i < 4; i++) {
    u8g2.setCursor(x[i] + 8, 21);
    u8g2.print(tem_1[i * 2]);
    u8g2.setCursor(x[i] + 8, 30);
    u8g2.print(tem_1[(i * 2) + 1]);
  }

  if (cursorr)
    hour_1 = min_1 + (hour_2 / 10) + (hour_2 % 10) + ":" + (min_2 / 10) + (min_2 % 10) + min_1;
  else
    hour_1 = min_1 + (hour_2 / 10) + (hour_2 % 10) + " " + (min_2 / 10) + (min_2 % 10) + min_1;

  if (!Stop_R) {
    //    u8g2.setCursor(74, 42); u8g2.print(Output);
    u8g2.drawStr(5, 52, "Remaining Time : ");
    u8g2.setCursor(74, 52);
    u8g2.print(hour_1);
    u8g2.drawBox(72, 45, 23, 9);
  } else {
    u8g2.drawBox(20, 45, 89, 9);
    u8g2.drawStr(23, 52, "Do You Want to stop ?");
  }
}

void lclass ::Process_Wndow(int hr1, int min_1) {
  u8g2.clearBuffer();                      // clear the internal memory
  if (!Graph_M) Process_disp(hr1, min_1);  // data mode
  else {
    if (!Stop_R)  //-------------------------------------------// if graph pressed
    {
      u8g2.setDrawColor(1);                       /* color 1 for the box */
      Mot.drawGraph(arrayrdc, sizeof(arrayrdc));  // Plot the points
      u8g2.drawLine(16, 0, 18, 0);
      u8g2.drawLine(16, 25, 18, 25);  // small lines for scale
      u8g2.drawLine(127, 49, 127, 51);
      u8g2.drawLine(72, 49, 72, 51);  // small lines for scale
      u8g2.setDrawColor(2);           /* color 1 for the box */
      u8g2.setFont(u8g2_font_6x10_tf);
      u8g2.setCursor(23, 10);
      u8g2.print("°C");  // Y axis notation
      u8g2.setFont(u8g2_font_4x6_tf);
      u8g2.setCursor(3, 5);
      u8g2.print("420");  // Y Scale value
      u8g2.setCursor(3, 30);
      u8g2.print("210");  // Y Scale value
      u8g2.setCursor(3, 50);
      u8g2.print(" 0 ");  // centre Scale value
      u8g2.setCursor(116, 48);
      u8g2.print(minute_disp);  // X Scale value
      u8g2.setCursor(68, 48);
      u8g2.print(minute_disp / 2);     // X Scale value
      u8g2.drawStr(53, 60, "Minute");  // X axis notation
    } else                             //----------------------------------------------------// if stop pressed
    {
      u8g2.setFont(u8g2_font_4x6_tf);
      u8g2.drawBox(20, 25, 89, 9);
      u8g2.drawStr(23, 32, "Do You Want to stop ?");
    }
  }
  u8g2.setFont(u8g2_font_4x6_tf);
  u8g2.drawBox(4, 55, 25, 9);
  u8g2.drawBox(100, 55, 25, 9);
  if (!Stop_R) {
    if (!Graph_M) u8g2.drawStr(7, 62, "Graph");
    else u8g2.drawStr(7, 62, "Data");
    u8g2.drawStr(104, 62, "Stop");
  } else {
    u8g2.drawStr(7, 62, " No ");
    u8g2.drawStr(107, 62, "Yes");
  }
  u8g2.sendBuffer();  // transfer internal memory to the display
}

lclass LCD = lclass();
