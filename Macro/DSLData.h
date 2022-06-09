#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>

namespace DataConst
{

  /*Time string which can use on file name*/
  const char     TimeStr[25]     = "S20220223E20220309";
  const char     TimeStrTitle[50]= "Date From  2022/02/23 to 2022/03/09";

  /*Total Event Numbers Of Input Files*/
  const Long64_t evs        = 302524;

  /*time Information of the first event*/
  const Int_t    unixtimei  = 1645608696;
  const Int_t    Yeari      = 2022;
  const Int_t    Moni       = 2;
  const Int_t    Dayi       = 23;
  const Int_t    Houri      = 17;
  const Int_t    Mini       = 31;
  const Int_t    Seci       = 36;
  const Int_t    FirDaySec  = 1645545600;

  /*time Information of the last event*/
  const Int_t    unixtimef  = 1646841597;
  const Int_t    Yearf      = 2022;
  const Int_t    Monf       = 3;
  const Int_t    Dayf       = 9;
  const Int_t    Hourf      = 23;
  const Int_t    Minf       = 59;
  const Int_t    Secf       = 57;
  const Int_t    FinDaySec  = 1646841600;

  /*Information of the time bin or chip operate*/
  const Int_t    ndivise    = 15;
  const Int_t    OOL        = 1;
  const Int_t    BinOnTime  = 342;
  const Int_t    BinOnTimeD = 360;
  const Int_t    yetSec     = 23304;
  const Float_t  RateWei    = 8.11159e-07;
  
  void Mod_DateLabel(TH1F* TH1FObj,const char* TitleY="TitleY"){
    Int_t    unixtime, tYear, tMonth, tDate ,tHour ,tMinute ,tSecond ;
    
    TH1FObj->SetStats(0);
    TH1FObj->GetXaxis()->SetTitle("Date");
    TH1FObj->GetYaxis()->SetTitle(TitleY);
    TH1FObj->GetXaxis()->SetTitleOffset(1.5);
    TH1FObj->GetYaxis()->SetTitleOffset(1.2);
    TH1FObj->GetXaxis()->SetLabelSize(0.03);
    TH1FObj->GetYaxis()->SetLabelSize(0.03);
    TH1FObj->SetMarkerStyle(0);
    TH1FObj->SetLineColor(0);
    TH1FObj->SetMarkerColor(0);
    TH1FObj->GetXaxis()->SetRangeUser(FirDaySec,FinDaySec);
    TH1FObj->GetXaxis()->SetTickLength(0);
    
    for (int i1=0;i1<(ndivise);i1++){
      unixtime = TH1FObj->GetXaxis()->GetBinCenter(1+i1)-TH1FObj->GetXaxis()->GetBinWidth(1+i1)/2.;
      unixTimeToHumanReadable(unixtime, tYear, tMonth, tDate, tHour, tMinute, tSecond,timeZone);
      char str_MD[30] ={};
      sprintf(str_MD , "%s-%02.f",MonthName[tMonth],1.*tDate);
      if(tDate==1||tDate==15||i1==0||i1==ndivise||(ndivise+1)<32){
        TH1FObj->GetXaxis()->SetBinLabel(1+i1 ,str_MD);
      }
    }
  }
  
  TH1F *Mod_DateLabel(const char* TH1FName="Mod_TH1F_UTVYZ",const char* TH1FTitle="",const float YMin=0, const float YMax=1,const char* TH1FTitleY="TitleY"){
    TH1F *Mod_TH1F_UTVYZ = new TH1F(TH1FName,TH1FTitle,ndivise,FirDaySec,FinDaySec);
    Mod_DateLabel(Mod_TH1F_UTVYZ,TH1FTitleY);
    Mod_TH1F_UTVYZ->GetYaxis()->SetRangeUser(YMin,YMax);
    return Mod_TH1F_UTVYZ;
  }
  void Mod_DateLabel(TH2F* TH2FObj,const char* TitleY="TitleY",const char* TitleZ="TitleZ"){
    Int_t    unixtime, tYear, tMonth, tDate ,tHour ,tMinute ,tSecond ;
    TH2FObj->SetStats(0);
    TH2FObj->GetXaxis()->SetTitle("Date");
    TH2FObj->GetYaxis()->SetTitle(TitleY);
    TH2FObj->GetYaxis()->SetTitle(TitleZ);
    TH2FObj->GetXaxis()->SetTitleOffset(1.5);
    TH2FObj->GetYaxis()->SetTitleOffset(1.2);
    TH2FObj->GetZaxis()->SetTitleOffset(1.3);
    TH2FObj->GetXaxis()->SetLabelSize(0.03);
    TH2FObj->GetYaxis()->SetLabelSize(0.03);
    TH2FObj->GetZaxis()->SetLabelSize(0.025);
    TH2FObj->SetMarkerStyle(0);
    TH2FObj->SetLineColor(0);
    TH2FObj->SetMarkerColor(0);
    TH2FObj->GetXaxis()->SetRangeUser(FirDaySec,FinDaySec);
    TH2FObj->GetXaxis()->SetTickLength(0);
    
    for (int i1=0;i1<(ndivise);i1++){
      unixtime = TH2FObj->GetXaxis()->GetBinCenter(1+i1)-TH2FObj->GetXaxis()->GetBinWidth(1+i1)/2.;
      unixTimeToHumanReadable(unixtime, tYear, tMonth, tDate, tHour, tMinute, tSecond,timeZone);
      char str_MD[30] ={};
      sprintf(str_MD , "%s-%02.f",MonthName[tMonth],1.*tDate);
      if(tDate==1||tDate==15||i1==0||i1==ndivise||(ndivise+1)<32){
        TH2FObj->GetXaxis()->SetBinLabel(1+i1 ,str_MD);
      }
    }
  }

  TH2F *Mod_DateLabel(const char* TH2FName="Mod_TH2F_UTVYVZ",const char* TH2FTitle="",const int NYBin = 10,const float YMin=0, const float YMax=1,const char* TH2FTitleY="TitleY"){
    TH2F *Mod_TH2F_UTVYVZ = new TH2F(TH2FName,TH2FTitle,ndivise,FirDaySec,FinDaySec,NYBin,YMin,YMax);
    Mod_DateLabel(Mod_TH2F_UTVYVZ,TH2FTitleY);
    Mod_TH2F_UTVYVZ->GetYaxis()->SetRangeUser(YMin,YMax);
    return Mod_TH2F_UTVYVZ;
  }
  TGraphErrors *Mod_DateGrid(const float YMin, const float YMax){
  
    TH1F *Mod_TH1F_UTVYZ      = new TH1F("Mod_TH1F_UTVYZ","",ndivise,FirDaySec,FinDaySec);
    float DGx[ndivise]={},DGy[ndivise]={},DGex[ndivise]={},DGey[ndivise]={};
    for(int i0=0;i0<ndivise;i0++) {
      DGx[i0] = Mod_TH1F_UTVYZ->GetBinCenter(i0+1)+Mod_TH1F_UTVYZ->GetBinWidth(i0+1)/2.;
      DGy[i0] = YMin;  DGey[i0] = fabs(YMax-YMin)*2;  DGex[i0] = 0;
      // cout<<DGx[i0]<<"	"<<DGy[i0]<<"	"<<DGex[i0]<<"	"<<DGey[i0]<<endl;
    }
    TGraphErrors *TDayGrid = new TGraphErrors(ndivise,DGx,DGy,DGex,DGey);
    TDayGrid->SetLineStyle(3);
    TDayGrid->SetLineColor(1);
    TDayGrid->SetMarkerStyle(1);
    TDayGrid->SetMarkerColor(0);
    delete Mod_TH1F_UTVYZ;
    return TDayGrid;
  }
};
