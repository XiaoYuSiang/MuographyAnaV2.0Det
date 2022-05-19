//include
#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TText.h>
#include "TStyle.h"
#include <TColor.h>
#include "TF2.h"
#include "TExec.h"
#include "/home/yusiang/personalLib/RootFile/untuplizerv8_YuSiang.h"
#include "/home/yusiang/personalLib/Math/UnixTranslator.h"
#include "AnaVariable.h"
#include "GobelFunctions.h"
#include "path_dir.h"
#include "DSLData.h"

using namespace std;
using namespace MuoAna_path_dir;
using namespace MuographAnaVariable;
using namespace MuographGobelFuns;
using namespace DataConst;


//Statac Variable
void Pal2()
{
   static Int_t  colors[50];
   static Bool_t initialized = kFALSE;
 
   Double_t Red[3]    = { 1.00, 0.00, 0.00};
   Double_t Green[3]  = { 0.00, 1.00, 0.00};
   Double_t Blue[3]   = { 0.00, 0.00, 1.00};
   Double_t Length[3] = { 0.00, 0.50, 0.50};
 
   if(!initialized){
      Int_t FI = TColor::CreateGradientColorTable(3,Length,Red,Green,Blue,50);
      for (int i=0; i<50; i++) colors[i] = FI+i;
      initialized = kTRUE;
      return;
   }
   gStyle->SetPalette(50,colors);
}

//channel vs rate combine
void DayEAnaV2() {
  //Printf("Files found: %i", (int) files.size());
  vector<string> vRootFiles = RawRootList(DirOperate);
  // for(int i=0;i<100;i++) cout<<vRootFiles[i]<<endl;
  TreeReader data(vRootFiles);
  
  ofstream out(Form("%sDayEffAna.txt",DirOperate));
  //Data Variable
  //Int_t    frame   ;//event
  Int_t    unixtime;//abs time of event [s]
  Int_t    nHits   ;
  Int_t*   board   ;
  Int_t*   channel ;
  Int_t    tYear   ;
  Int_t    tMonth  ;
  Int_t    tDate   ;
  Int_t    tHour   ;
  Int_t    tMinute ;
  Int_t    tSecond ;
  //take time and set anlyze Constant for boundry condition

  int nd10min = (ndivise+2)*24*6, nd1day = ndivise+2;
  const int StartBinUnixt = FirDaySec-86400;
  const int EndBinUnixt = FinDaySec+86400;
  cout<<"nd10min:  "<<nd10min<<endl;
  //data hist create
  TH2F *htmp  = new TH2F("htmp","",nd10min,0,nd10min,5,1,6);//10min
  TH2F *htmpc = new TH2F("htmpc","",nd10min,0,nd10min,80,0,80);//10min
  TH2F *hBT  = new TH2F("hBT","",nd1day,0,nd1day,4,0,4);//10min
  TH2F *hCT  = new TH2F("hCT","",nd1day,0,nd1day,64,0,64);//10min
  //TH2F *hBT  = new TH2F("hBT","",Dayf-Dayi+4,Dayi-1,Dayf+3,4,0,4);//10min
  int t1=0;
  int oiev=0, iev=0, XBins=0;
  
  for(int iev = 0; iev <evs; iev++){
    data.GetEntry(iev);
    t1        = data.GetLong64("unixtime");
    board     = data.GetPtrInt("board");
    channel   = data.GetPtrInt("channel");
    nHits     = data.GetInt("nHits");
    XBins     = (t1-StartBinUnixt)/600.;
    if(iev%10000==0) cout<<XBins<<endl;
    for(int iHit = 0; iHit<nHits ; iHit++){
      htmp->Fill(XBins,board[iHit]);
      htmpc->Fill(XBins,(board[iHit]-1)*16+channel[iHit]);
    }
    // if(iev==0||iev==evs-1)
      // cout<<"t1-StartBinUnixt: "<<t1<<" - "<<StartBinUnixt<<" = "<<t1-StartBinUnixt<<endl;
  }
  
  cout<<"unixtimei:    "<<unixtimei<<endl;
  cout<<"unixtimef:    "<<unixtimef<<endl;
  
  for(int iBin = 0; iBin<nd10min ; iBin++){
    t1 = iBin/144;
    //cout<<iBin/144<<endl;
    if (htmp->GetBinContent(iBin+1,1)!=0) hBT->Fill(t1,0.5,1/144.);
    if (htmp->GetBinContent(iBin+1,3)!=0) hBT->Fill(t1,1.5,1/144.);
    if (htmp->GetBinContent(iBin+1,5)!=0) hBT->Fill(t1,2.5,1/144.);
    if (htmp->GetBinContent(iBin+1,4)!=0) hBT->Fill(t1,3.5,1/144.);
    for(int ic = 0; ic<16;ic++){
      if (htmpc->GetBinContent(iBin+1,1+ic)!=0)  hCT->Fill(t1,ic,1/144.);
      if (htmpc->GetBinContent(iBin+1,33+ic)!=0) hCT->Fill(t1,ic+16,1/144.);
      if (htmpc->GetBinContent(iBin+1,65+ic)!=0) hCT->Fill(t1,ic+32,1/144.);
      if (htmpc->GetBinContent(iBin+1,49+ic)!=0) hCT->Fill(t1,ic+48,1/144.);
    }
  }
  TCanvas *c1 = new TCanvas("c1","c1",1200,800);
  TCanvas *c2 = new TCanvas("c2","c2",1200,800);
  c1->cd();
  htmpc->Draw("colz");
  c1->Print(Form("%shtmpc%scom.png",DirRes_DayEff,TimeStr));//@@


  for (int i1=1;i1<nd1day;i1++){
    int timeBinDay = StartBinUnixt + 86400*i1;
    unixTimeToHumanReadable(timeBinDay, tYear, tMonth, tDate, tHour, tMinute, tSecond,timeZone);
    if(tDate==1||tDate==15||i1==1||i1==ndivise){
      hBT->GetXaxis()->SetBinLabel((i1) ,Form("%s-%02.f",MonthName[tMonth],1.*tDate));
      hCT->GetXaxis()->SetBinLabel((i1) ,Form("%s-%02.f",MonthName[tMonth],1.*tDate));
    }
  }
  
  //channel label
  hBT->SetStats(0);
  hBT->GetYaxis()->SetBinLabel(1 ,"MTB1");
  hBT->GetYaxis()->SetBinLabel(2 ,"MTB3");
  hBT->GetYaxis()->SetBinLabel(3 ,"MTB5");
  hBT->GetYaxis()->SetBinLabel(4 ,"MTB4");
  hBT->SetTitle("");
  hCT->SetStats(0);
  for(int ic=0;ic<16;ic++){
    hCT->GetYaxis()->SetBinLabel(1+ic  ,Form("MTB%1.d-%02.f",1,ic*1.));
    hCT->GetYaxis()->SetBinLabel(17+ic ,Form("MTB%1.d-%02.f",3,ic*1.));
    hCT->GetYaxis()->SetBinLabel(33+ic ,Form("MTB%1.d-%02.f",5,ic*1.));
    hCT->GetYaxis()->SetBinLabel(49+ic ,Form("MTB%1.d-%02.f",4,ic*1.));
  }
  hCT->SetTitle("");
  //TExec *ex2 = new TExec("ex2","Pal2();");
  //ex2->Draw();
  hBT->SetStats(0);
  hBT->GetXaxis()->SetTitle("Date");
  hBT->GetYaxis()->SetTitle("Readout board");
  hBT->GetXaxis()->SetTitleOffset(1.1);
  hBT->GetYaxis()->SetTitleOffset(1.3);
  hBT->GetXaxis()->SetLabelOffset(0.002);
  hBT->GetZaxis()->SetLabelOffset(0.003);
  hBT->GetZaxis()->SetTitleSize(0.04);
  hBT->GetYaxis()->SetTitleSize(0.04);
  
  hCT->SetStats(0);
  hCT->GetXaxis()->SetTitle("Date");
  hCT->GetYaxis()->SetTitle("Readout Channel");
  hCT->GetXaxis()->SetTitleOffset(1.1);
  hCT->GetYaxis()->SetTitleOffset(1.3);
  hCT->GetZaxis()->SetLabelOffset(0.003);
  hCT->GetXaxis()->SetLabelOffset(0.002);
  hCT->GetZaxis()->SetTitleSize(0.04);
  hCT->GetYaxis()->SetTitleSize(0.04);
  
  hBT->GetZaxis()->SetTitle("Data collection efficiency");
  hBT->GetZaxis()->RotateTitle(true);
  hBT->GetZaxis()->SetLabelSize(0.025);
  hBT->GetXaxis()->SetLabelSize(0.04);
  hBT->GetXaxis()->SetNdivisions(-ndivise+2);
  hBT->GetXaxis()->SetRangeUser(Dayi,Dayf);
  hBT->GetZaxis()->SetRangeUser(0,1);
  hBT->GetZaxis()->SetNdivisions(-005);
  hCT->GetZaxis()->SetTitle("Data collection efficiency");
  hCT->GetZaxis()->RotateTitle(true);
  hCT->GetZaxis()->SetLabelSize(0.025);
  hCT->GetXaxis()->SetLabelSize(0.04);
  hCT->GetYaxis()->SetLabelSize(0.025);
  hCT->GetXaxis()->SetNdivisions(-ndivise+2);
  hCT->GetXaxis()->SetRangeUser(Dayi,Dayf);
  hCT->GetZaxis()->SetRangeUser(0,1);
  hCT->GetZaxis()->SetNdivisions(-005);
  hBT->GetZaxis()->SetTitleOffset(0.6);
  hCT->GetZaxis()->SetTitleOffset(0.6);
  c1->SetTicks(0,1);
  c1->SetGrid(0,1);
  c2->SetTicks(0,1);
  c2->SetGrid(0,1);
  //L->Draw();
  Pal2();

  
  hBT->Draw("colz");
  

  c1->Print(Form("%sDayEffBoardcolz_%scom.png",DirRes_DayEff,TimeStr));
  c1->Print(Form("%sDayEffBoardcolz_%scom.pdf",DirRes_DayEff,TimeStr));
  c2->cd();
  hCT->Draw("colz");
  
  c2->Print(Form("%sDayEffChanncolz_%scom.png",DirRes_DayEff,TimeStr));
  c2->Print(Form("%sDayEffChanncolz_%scom.pdf",DirRes_DayEff,TimeStr));
  
  
  out.close();
  
  
}