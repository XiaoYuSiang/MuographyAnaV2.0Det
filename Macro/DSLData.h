#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>

namespace DataConst
{

  /*Time string which can use on file name*/
  const char     TimeStr[25]= "S20220126E20220207";

  /*Total Event Numbers Of Input Files*/
  const Long64_t evs        = 777394;

  /*time Information of the first event*/
  const Int_t    unixtimei  = 1643185845;
  const Int_t    Yeari      = 2022;
  const Int_t    Moni       = 1;
  const Int_t    Dayi       = 26;
  const Int_t    Houri      = 16;
  const Int_t    Mini       = 30;
  const Int_t    Seci       = 45;
  const Int_t    FirDaySec  = 1643126400;

  /*time Information of the last event*/
  const Int_t    unixtimef  = 1644234285;
  const Int_t    Yearf      = 2022;
  const Int_t    Monf       = 2;
  const Int_t    Dayf       = 7;
  const Int_t    Hourf      = 19;
  const Int_t    Minf       = 44;
  const Int_t    Secf       = 45;
  const Int_t    FinDaySec  = 1644249600;

  /*Information of the time bin or chip operate*/
  const Int_t    ndivise    = 13;
  const Int_t    OOL        = 1;
  const Int_t    BinOnTime  = 291;
  const Int_t    yetSec     = 26955;
  const Float_t  RateWei    = 9.53809e-07;
  
};
