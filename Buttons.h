#ifndef Buttons_h
#define Buttons_h
#include "OneButton.h"

#define Button_Set  A0   // Select Key 22
#define Button_up  A5    // Up Key 27
#define Button_down  A7  // Down Key 29
#define Button_Reset  A3 // Exit Key 25

#define edit_buttn  A1   // Edit Key 23
#define save_buttn A2    // Save Key 24

class Eclass
{
  public :
  Eclass();
  void SETUP();
  void setInputFlags();
  void resolveInputFlags();
  void save_editB();
  int Save_preset_parameters(int);
  void Save_Button();
  void Edit_Button();
  void Ramp_up(int val1);
};
extern OneButton button1;
extern OneButton button2;
extern Eclass Butt_P;

#endif
