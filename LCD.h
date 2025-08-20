#ifndef LCD_h
#define LCD_h
#include "LiquidCrystal.h"

#define safteyRelay 38                      //--------------------------02/01/2024

class lclass
{
  public :
  lclass();
  void SETUP();
  void main_menu();
  void Settings_mode();
  void MenuAction(int input);
  void setChange(int key);
  void InputFlags() ;
  void temp_display(int, int ,int);
  
  void tempRiseCheckUpdate();

  void start_menu();
  void Preset_menu();
  void Setting_menu();
  void preset_mem_save();
  void Main_Process();
  void up_dwn_arroy(int, int);
  void cng_tmp_time();
  void Assisgn_set_para();
  void Process_disp(int, int);
  void Process_Wndow(int hr1, int min_1);
  void Auto_Corr_fun();
  
};
extern LiquidCrystal lcd;
extern U8G2_KS0108_128X64_F u8g2;
extern lclass LCD;

#endif
