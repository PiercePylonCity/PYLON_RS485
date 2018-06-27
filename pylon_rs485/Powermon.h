
#ifndef Powermon_h
#define Powermon_h
#define bytesQty 2 
#include "ModbusMaster.h"
typedef struct Powermons {
   float Va; int addrVa = 0; 
   float Vb; int addrVb = 2; 
   float Vc; int addrVc = 4; 
   float Ia; int addrIa = 8;
   float Ib; int addrIb = 10; 
   float Ic; int addrIc = 12; 
   float P;  int addrP  = 16; 
   float Pa; int addrPa = 18; 
   float Pb; int addrPb = 20;
   float Pc; int addrPc = 22; 
   float pfa; int addrpfa = 42; 
   float pfb; int addrpfb = 44; 
   float pfc; int addrpfc = 46; 
   float freq; int addrfreq = 54; 
   float Ein;  int addrEin = 512;
   float Eout; int addrEout = 768;
} Powermon;
extern void powermonStart(void);
extern void powermonGo(void); 
extern Powermon powermonGet(void); 

#endif