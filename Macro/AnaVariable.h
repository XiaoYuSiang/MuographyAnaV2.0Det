#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>

using namespace std;


namespace MuographAnaVariable
{
  /*Detector Information variable*/
  const int  BD[4] ={1,3,5,4};
  const Int_t BDPwWei[4] = {2,1,1,1};
  const double bx=5.,by=5.,bz=1.;
  const double bx_2=bx/2.;
  const double by_2=by/2.;
  const double bz_2=bz/2.;
  
  
  /*↘Ana constant don't change!!!↙*/
  //the setting of Ev gap 
  const Int_t eventGapTcnt[46]= {2,5,8,10,13,16,20,24,28,30,31,32,34,36,38,40,42,46,50,55,60,70,80,90,100,90,100,120,140,160,180,200,250,300,350,400,500,600,700,800,1000,1200,1400,1600,1800,2000};
  /*↗Ana constant don't change!!!↖*/
  
  
  /*Analysis variable */
  //th2fbinwidth is tim bin in Analysis
  const double th2fbinwidth = 3600;
  //timeZone of Taiwan is UTC+8
  const int    timeZone     = +8 ;
  
  //breaktime is the time detector sleep
  const Int_t  breaktime = 120;//(s)
  
  //nHit Triggers from S -> L
  //nH of event in Trigger would be record
  const Int_t TriggerS = 0;
  const Int_t TriggerL = 10000;
  
  
  
  /*Art Option*/
  // the color for 16 kind color
  const int colorArr[16]={1,90,93,96,99,205,51,60,65,70,80};
  
  
  
};
