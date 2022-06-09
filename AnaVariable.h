#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TColor.h>
#include "lineFit3D.h"

using namespace std;


namespace MuographAnaVariable
{
  
  /*Analysis variable */
  //th2fbinwidth is tim bin in Analysis
  const double th2fbinwidth = 3600;
  Int_t bindays = 1;
  //timeZone of Taiwan is UTC+8
  const int    timeZone     = +8 ;
  
  //breaktime is the time detector sleep
  const Int_t  breaktime = 120;//(s)
  
  //nHit Triggers from S -> L
  //nH of event in Trigger would be record
  const Int_t TriggerS = 0;
  const Int_t TriggerL = 10000;
  
  //Rate ana. trigger
  //Trigger for how much RMS sigma would be error
  const Float_t Trigger_RateRMS = 1.5;
  
  //Rate similarity on sunny day
  //select the sunny day simple
  const Int_t UT_SelectDay = 1643212800; //ex: 2022/01/27 UTC+8
  const Int_t Num_SelectDay=3;
  Int_t SelectDaySec[Num_SelectDay]={
    1643212800+86400*0,
    1643212800+86400*1,
    1643212800+86400*2,
  };
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  /*Art Option*/
  // the color for 16 kind color
  const int colorArr[16]={1,90,93,96,99,205,51,60,65,70,80};
  const int ColorArr9[9]={
    TColor::GetColor("#7882A4"),
    TColor::GetColor("#FF7979"),
    TColor::GetColor("#C0A080"),
    TColor::GetColor("#FFB770"),
    TColor::GetColor("#586284"),
    TColor::GetColor("#DF5959"),
    TColor::GetColor("#A08060"),
    TColor::GetColor("#DD9750"),
    1
  };
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  /* pwidth fitting parameters */
  const double SPLminP[3][7] ={//SetParLimitmin
    {0.15,-3.0,1.5                     },//Landau--muon data
    {0.20,-6.0,0.00,0.00,0.00,10.0,0.00},//Landau+Landau--0
    {0.60,-10.,2.00,.055,0.00,10.0,0.00},//Landau+gau--85
  };
  const double SPLmaxP[3][7] ={//SetParLimitmax
    {0.65,17.0,10.0                    },//Landau--muon data
    {2.00,10.0,0.25,3.0 ,0.5 ,25.0,5.00},//Landau+Landau--0
    {2.00,15.0,3.00,3.0 ,6.0 ,25.0,3.00},//Landau+gau--85
  };
  const double SPSP[3][7] ={
    {0.40,10.0,3.33                    },
    {0.18,4.00,3.7 ,0.2 ,14. ,3.7 ,    },
    {.194,4.00,0.0 ,2.3 ,3.0           },
  };
  const double SPP[5]=
    {.194,4.00,0.0 ,2.3 ,3.0           
  };
  
  char WeatherData[][5]={
    "sn","Su","Su",//0318~20
    "c","cr","c","st","su","su","su","su","su","c","cr",//21~0431
    "cr","c","su","c","cr","cr","r","cr","cr","w",//0401~10
    "cr","su","su","cr","su","su","su","c","cr","cr",//11~20
    "c","su","cr","c","su","su","cr","cr","cr","su",//21~0430
    "su","su","C","C","C","su","C","c","su","su",//0501~10
    "su","su","su","su","su","su","su","su","cr","su",//11~20
    "su","su","su","R","su","cr","su","cr","cr","r","r",//21~0531
    "r","cr","su"/*,"r","r","r","r","cr","r","r","su","cr"*///0601~0610
    
  };
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  /*Detector Information variable*/
  const int    NumBD = 4;
  const int    NumCh = 16;
  const int    NumnX = 4, NumnY = 4, NumnZ = 4;
  const int    BinNumOnCha = NumBD*NumCh;
  const int    BD[4]    = {1,3,5,4};
  const int    BDINV[6] = {0,0,0,1,3,2};
  const int    BDPwWei[4] = {2,1,1,1};
  const double bx = 5., by = 5., bz = 1.;
  const double bx_2 = bx/2.;
  const double by_2 = by/2.;
  const double bz_2 = bz/2.;
  const int    binpw = 90;// for 90 pwidth = 9ms
  
  
  
  /*↘Ana constant don't change!!!↙*/
  //the setting of Ev gap 
  const int eventGapTcnt[46]= {
    2,5,8,10,13,16,20,24,28,
    30,31,32,34,36,38,40,42,
    46,50,55,60,70,80,90,100,
    90,100,120,140,160,180,
    200,250,300,350,400,500,
    600,700,800,1000,1200,
    1400,1600,1800,2000};
  /*↗Ana constant don't change!!!↖*/
  
  
  
  
};
