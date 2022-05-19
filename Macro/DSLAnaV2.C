//include
#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TStyle.h>
#include "/home/yusiang/personalLib/RootFile/untuplizerv8_YuSiang.h"
#include "/home/yusiang/personalLib/Math/UnixTranslator.h"
#include "path_dir.h"
#include "AnaVariable.h"
#include "GobelFunctions.h"

using namespace std;
using namespace MuoAna_path_dir;
using namespace MuographAnaVariable;
using namespace MuographGobelFuns;

struct timeSt{
  Long64_t    unixtime;//abs time of event [s]
  Int_t    tY, tM, tD, th, tm, ts;
  timeSt(Long64_t ut ){
    unixTimeToHumanReadable(ut, tY, tM, tD, th, tm, ts, timeZone);
    unixtime = ut;
  };

  
};

//channel vs rate combine
void DSLAnaV2() {
  //
  vector<string> vssssss = RawRootList(DirOperate);
  // for(int i=0;i<100;i++) cout<<vssssss[i]<<endl;
  TreeReader data(vssssss);
  ofstream outh(Form("%sDSLData.h",DirMacros ));
  ofstream out(Form("%sDSLAna.txt",DirOperate));
  for(int i=0;i<int(vssssss.size());i++){
    cout<<vssssss[i]<<endl;
  }
  //Data Variable
  //Int_t    frame   ;//event
  
  //take time and set anlyze Constant for boundry condition
  Long64_t evs = data.GetEntriesFast();//total event
  cout <<"total event:\t"<<evs<<endl;
  data.GetEntry(0);
  timeSt timeiT(data.GetLong64("unixtime"));
  Int_t unixtimei    = data.GetLong64("unixtime");
  Int_t Yeari        = timeiT.tY;
  Int_t Moni         = timeiT.tM;
  Int_t Dayi         = timeiT.tD;
  Int_t Houri        = timeiT.th;
  Int_t Mini         = timeiT.tm;
  Int_t Seci         = timeiT.ts;
  cout <<"unixtimei:\t"<<unixtimei<<endl;
  data.GetEntry(evs-1);
  timeSt timefT(data.GetLong64("unixtime"));
  Int_t unixtimef    = data.GetLong64("unixtime");
  Int_t Yearf        = timefT.tY;
  Int_t Monf         = timefT.tM;
  Int_t Dayf         = timefT.tD;
  Int_t Hourf        = timefT.th;
  Int_t Minf         = timefT.tm;
  Int_t Secf         = timefT.ts;
  cout <<"unixtimef:\t"<<unixtimef<<endl;
  char  TimeStr[20];
  sprintf(TimeStr,"S%04d%02d%02dE%04d%02d%02d",Yeari,Moni,Dayi,Yearf,Monf,Dayf);
  /*     Example      sprintf(StatrStr,"SYYYYMMSSEYYYYMMSS")*/
  cout <<"dt:\t"<<unixtimef-unixtimei<<"\t"<<"dday:\t"<<Form("%.2f",(unixtimef-unixtimei)/86400.)<<endl;
  cout<<"TimeStr:    "<<TimeStr<<endl;
  out<<"TimeStr:    "<<TimeStr<<endl;
  Int_t dday = (unixtimef-unixtimei)/86400 + 1;
  Int_t ndivise = dday;
  Int_t OOL=0;//OrderOfLabel

  for (;OOL<12;OOL++){
    if(ndivise<1+(32)*pow(2,OOL)){
      OOL=pow(2,OOL);
      cout<<"Trigger:\t"<<(32)*pow(2,OOL)<<"\tod:\t"<<OOL<<endl;
      break;
    }
  }
  cout<<"Trigger:\t"<<(32)*pow(2,OOL)<<"\tOrder Of Date Label:\t"<<OOL<<endl;
  Int_t BinOnTime    = int((unixtimef-unixtimei)/th2fbinwidth);
  Int_t FirDaySec    = unixtimei-Seci-Mini*60-Houri*3600;
  Int_t FinDaySec    = unixtimef-Secf-Minf*60-Hourf*3600+86400;

  Int_t yetSec       = 86400-Seci-Mini*60-Houri*3600;
  Int_t yetTimeBin   = int(yetSec/th2fbinwidth);

  // outT<<unixtimei<<Yeari<<Moni<<Dayi<<FirDaySec<<endl;
  // outT<<unixtimef<<Yearf<<Monf<<Dayf<<FinDaySec<<endl;
  // outT<<OOL<<BinOnTimeS<<yetSec<<BinOnTimeS<<endl;
  
  Float_t RateWei= 0;//1./(DC.unixtimef-DC.unixtimei);

  int chipt0 = 0, chipt = 0, ev0 = 0, ev1 = 0;
  int dt = 0, gap = 0;
  float chipfre  = 0;
  int chipNum = 0;
  int Day0 = 0, Month0 = 0, Year0 = 0;
  data.GetEntry(0);
  int unixtime, tYear, tMonth, tDate, tHour, tMinute, tSecond;
  for (Long64_t ev = 0; ev <evs; ++ev) {
    data.GetEntry(ev);
    unixtime  = data.GetLong64("unixtime");
    tYear     = data.GetInt("tYear");
    tMonth    = data.GetInt("tMonth");
    tDate     = data.GetInt("tDate");
    tHour     = data.GetInt("tHour");
    tMinute   = data.GetInt("tMinute");
    tSecond   = data.GetInt("tSecond");
    if (gap == 0 && dt == 0){
      ev0 = ev;
      dt = unixtime-chipt;
      chipt0 = unixtime;
      // cout<<"chipt0:\t"<<chipt0<<"\tdt:\t"<<dt<<"gap:\t"<<gap<<endl;
      // cout<<Form("ST:%d_%d_%d_%d:%d'%d\"",tYear,tMonth,tDate,tHour,tMinute,tSecond)<<endl;
      out<<"chipt0:\t"<<chipt0<<"\tdt:\t"<<dt<<"gap:\t"<<gap<<endl;
      out<<Form("ST:%d_%d_%d_%d:%d'%d\"",tYear,tMonth,tDate,tHour,tMinute,tSecond)<<endl;
      chipt  = unixtime;
      Day0 = tDate;
      Month0 = tMonth;
      Year0 = tYear;
      continue;
    }
    dt = unixtime-chipt;
    gap = unixtime-chipt0;
    chipt = unixtime;
    //cout<<"\tdt:\t"<<dt<<endl;
    if (dt>breaktime||ev+1==evs||Day0!=tDate){
      if (Day0!=tDate) ev--;
      chipfre = 1./gap;
      // cout<<"chipt0:\t"<<chipt0<<"\tchipt1:\t"<<chipt<<endl;
      // cout<<"dt:\t"<<dt<<"\tgap:\t"<<gap<<"\tFre:\t"<<chipfre<<endl;
      out<<"chipt0:\t"<<chipt0<<"\tchipt1:\t"<<chipt<<endl;
      out<<"dt:\t"<<dt<<"\tgap:\t"<<gap<<"\tFre:\t"<<chipfre<<endl;
      ev1 = ev;
      RateWei+=gap;

      gap = 0;
      dt = 0;
      chipt = 0;
      chipt0 = 0;
      chipNum++;

      out<<Form("ED:%d_%d_%d_%d:%d'%d\"",tYear,tMonth,tDate,tHour,tMinute,tSecond)<<endl<<"------------------------------"<<endl;

    }
    continue;
  }
  if(RateWei==0) RateWei=0;
  else RateWei = 1.0/RateWei;
  
  cout<<"chipNum:\t"<<chipNum<<endl;
  cout<<"OPtime:\t"<<RateWei<<"\tfrequency:\t"<<1.0/RateWei<<endl;
  out<<"chipNum:\t"<<chipNum<<endl;
  out<<"OPtime:\t"<<RateWei<<"\tfrequency:\t"<<1.0/RateWei<<endl;
  
  
  cout<<"Saving the data Information at "<<DirMacros<<"DSLData.h"<<endl;
  
  outh<<"#include <iostream>"<<endl;
  outh<<"#include <vector>"<<endl;
  outh<<"#include <fstream>"<<endl;
  outh<<"#include <typeinfo>"<<endl;
  outh<<"#include <algorithm>"<<endl;
  outh<<""<<endl;
  outh<<"namespace DataConst"<<endl;
  outh<<"{"<<endl;
  outh<<""<<endl;
  outh<<"  /*Time string which can use on file name*/"<<endl;
  outh<<"  const char     TimeStr[25]= \""<< TimeStr   <<"\";\n";
  outh<<endl<<"  /*Total Event Numbers Of Input Files*/"<<endl;
  outh<<"  const Long64_t evs        = "<< evs       <<";\n";
  outh<<endl<<"  /*time Information of the first event*/"<<endl;
  outh<<"  const Int_t    unixtimei  = "<< unixtimei <<";\n";
  outh<<"  const Int_t    Yeari      = "<< Yeari     <<";\n";
  outh<<"  const Int_t    Moni       = "<< Moni      <<";\n";
  outh<<"  const Int_t    Dayi       = "<< Dayi      <<";\n";
  outh<<"  const Int_t    Houri      = "<< Houri     <<";\n";
  outh<<"  const Int_t    Mini       = "<< Mini      <<";\n";
  outh<<"  const Int_t    Seci       = "<< Seci      <<";\n";
  outh<<"  const Int_t    FirDaySec  = "<< FirDaySec <<";\n";
  outh<<endl<<"  /*time Information of the last event*/"<<endl;
  outh<<"  const Int_t    unixtimef  = "<< unixtimef <<";\n";
  outh<<"  const Int_t    Yearf      = "<< Yearf     <<";\n";
  outh<<"  const Int_t    Monf       = "<< Monf      <<";\n";
  outh<<"  const Int_t    Dayf       = "<< Dayf      <<";\n";
  outh<<"  const Int_t    Hourf      = "<< Hourf     <<";\n";
  outh<<"  const Int_t    Minf       = "<< Minf      <<";\n";
  outh<<"  const Int_t    Secf       = "<< Secf      <<";\n";
  outh<<"  const Int_t    FinDaySec  = "<< FinDaySec <<";\n";
  outh<<endl<<"  /*Information of the time bin or chip operate*/"<<endl;
  outh<<"  const Int_t    ndivise    = "<< ndivise   <<";\n";
  outh<<"  const Int_t    OOL        = "<< OOL       <<";\n";
  outh<<"  const Int_t    BinOnTime  = "<< BinOnTime <<";\n";
  outh<<"  const Int_t    yetSec     = "<< yetSec    <<";\n";
  outh<<"  const Float_t  RateWei    = "<< RateWei   <<";\n";
  outh<<"  "<<endl;
  outh<<"};"<<endl;
  
  
  outh.close();

  //data hist create



}
