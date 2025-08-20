#ifndef Pid_h
#define Pid_h

#define ZCD_PIN 20

class Pclass
{
  public :
  Pclass();
  void SETUP();
  void PID_Set();
  void Temp_Read1();
  void pid_interval(float,float);
  void PID_Action(float);
  void Read_Thermo_P();
  void Stepper_Rotate(unsigned long t_dly, unsigned long step_s);
  int writeEEPROM(int deviceaddress, unsigned int eeaddress, int data );
  int readEEPROM(int deviceaddress, unsigned int eeaddress ) ;
};

extern Pclass pid;

#endif

//#define AC_Mot 28         // AC_Mot pin output
//#define DC_Mot 39         // DC_Mot pin output
//
//#define Limit_Dir 30      // Stepper direction pin output        Lift Up
//#define Limit_Step 31     // Stepper step pin output        Lower


//if(!Auto_Correct)
//    {
//      Dir_Chk = digitalRead(Actu_dwn_LmT);
//      if(Dir_Chk && !dwn_chk)
//      {
//        Step_Count = Dir_Change = 0; I_Step = 5000;  // DOWN
//      }
//      else if(!Dir_Chk && !dwn_chk)
//      {
//        dwn_chk = Dir_Change = Lift_Pos = Auto_Correct = 1;   // UP     
//        Step_Count = 0; 
//        I_Step = 90000;
//        EEPROM.write(3, Lift_Pos);
//      }
//    }

//int preset_mem_locatn[6][10] = {{1,17,33,49,65,81,97,113,129,145},
//                                {161,177,193,209,225,241,257,273,289,305},
//                                {321,337,353,369,385,401,417,433,449,465},
//                                {481,497,513,529,545,561,577,593,609,625},
//                                {641,657,673,689,705,721,737,753,769,785},
//                                {801,817,833,849,865,881,897,913,929,945}
//                                };


//if(!Auto_Correct)
//  {
////    Dir_Chk = digitalRead(Actu_dwn_LmT);
//    int Actu_dwn_LmT_count = 0;
//    for(int i = 0; i< 50; i++)
//    {
//      if(digitalRead(Actu_dwn_LmT) == 0)
//      {
//        Actu_dwn_LmT_count++;
//      }
//    }
//    if(Actu_dwn_LmT_count > 40)
//    {
//      Dir_Chk = 0;
//    }
//
//    if(Dir_Chk && !dwn_chk)
//    {
//      Step_Count = Dir_Change = 0; I_Step = 5000;  // DOWN
//    }
//    else if(!Dir_Chk && !dwn_chk)
//    {
//      dwn_chk = Dir_Change = 1;   // UP     
//      Step_Count = 0; 
//      I_Step = 90000;
//      Dir_Chk = 1;
//    }
//    if(!I_Step)
//    {
//      Auto_Correct = Lift_Pos = 1;
//      EEPROM.write(3, Lift_Pos);
//    }
//  }
