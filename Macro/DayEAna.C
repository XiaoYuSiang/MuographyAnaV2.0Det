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
#include "DSLData.h"
#include "/home/yusiang/personalLib/RootFile/untuplizerv8_YuSiang.h"
#include "/home/yusiang/personalLib/Math/UnixTranslator.h"
using namespace std;


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
void DayEAna(vector<string> files,const char* sapaths) {
  //Printf("Files found: %i", (int) files.size());
  TreeReader data(files);
  ofstream out(Form("./OperateResule/%sDayEffAna.txt",sapaths));
  DSLDataInitialize();
  //Data Variable
  //Int_t    frame   ;//event
  Int_t    unixtime;//abs time of event [s]
  Int_t    tYear   ;
  Int_t    tMonth  ;
  Int_t    tDate   ;
  Int_t    tHour   ;
  Int_t    tMinute ;
  Int_t    tSecond ;
  Int_t    nHits   ;
  Int_t*   board   ;
  Int_t*   channel ;
  Int_t*   seq     ;
    
  //take time and set anlyze Constant for boundry condition
  Long64_t evs = data.GetEntriesFast();
  cout <<"total event:\t"<<evs<<endl;
  data.GetEntry(0);
  cout <<"DC.unixtimei:\t"<<DC.unixtimei<<endl;
  data.GetEntry(evs-1);
  cout <<"DC.unixtimef:\t"<<DC.unixtimef<<endl;
  cout <<"dt:\t"<<DC.unixtimef-DC.unixtimei<<"\t"<<"dday:\t"<<DC.Dayf-DC.Dayi<<endl;
  DC.ndivise = DC.Dayf-DC.Dayi+2;
  cout<<"DC.ndivise:\t"<<DC.ndivise<<endl;
  
  
  //data hist create
  TH2F *htmp  = new TH2F("htmp","",20448,0,20448,5,1,6);//10min
  TH2F *htmpc = new TH2F("htmpc","",20448,0,20448,80,0,80);//10min
  TH2F *hBT  = new TH2F("hBT","",DC.ndivise,DC.Dayi-1,DC.Dayf+1,4,0,4);//10min
  TH2F *hCT  = new TH2F("hBT","",DC.ndivise,DC.Dayi-1,DC.Dayf+1,64,0,64);//10min
  //TH2F *hBT  = new TH2F("hBT","",DC.Dayf-DC.Dayi+4,DC.Dayi-1,DC.Dayf+3,4,0,4);//10min
  int t1=0;
  int oiev=0, iev=0, XBins=0;
  for(int iev = 0; iev <evs; iev++){
    data.GetEntry(iev);
    tMinute   = data.GetInt("tMinute");
    tHour     = data.GetInt("tHour");
    // tMonth    = data.GetInt("tMonth");
    // tDate     = data.GetInt("tDate");
    t1        = data.GetLong64("unixtime");
    board     = data.GetPtrInt("board");
    channel   = data.GetPtrInt("channel");
    tMinute   = data.GetInt("tMinute");
    nHits     = data.GetInt("nHits");
    XBins     = tMinute/10+tHour*6+(UTtDGMT2021(t1)-UTtDGMT2021(DC.unixtimei))*144;
    //cout<<(UTtDGMT2021(t1)-UTtDGMT2021(DC.unixtimei))*144<<"\t"<<tMinute<<"\t"<<tMinute/5<<"\t"<<tHour*12<<"\tXBins"<<"\t"<<XBins<<endl;
    for(int iHit = 0; iHit<nHits ; iHit++){
      htmp->Fill(XBins,board[iHit]);
      htmpc->Fill(XBins,(board[iHit]-1)*16+channel[iHit]);
    }
  }
  cout<<UTtDGMT2021(DC.unixtimei)<<endl;
  
  for(int iBin = 0; iBin<20448 ; iBin++){
    t1 = iBin/144+UTtDGMT2021(DC.unixtimei);
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
  int imonth=0, idate=0;
  c1->cd();
  htmpc->Draw("colz");
  c1->Print(Form("./Result/htmpc%d_%d_%dto%d_%d_%dcom.png",2021,UTMGMT2021(DC.unixtimei),UTDGMT2021(DC.unixtimei),2021,UTMGMT2021(DC.unixtimef),UTDGMT2021(DC.unixtimef)));

  for (int i1=1;i1<300;i1+=1){
    imonth = LDTMGMT2021(DC.Dayi+(i1));
    idate = LDTDGMT2021(DC.Dayi+(i1));
    //if(DUTGMT2021(imonth,idate)>DC.Dayf) break;
    if(idate==1||idate==15){
      hBT->GetXaxis()->SetBinLabel((i1) ,Form("%s-%d",MonthName[imonth],idate));
      hCT->GetXaxis()->SetBinLabel((i1) ,Form("%s-%d",MonthName[imonth],idate));
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
    hCT->GetYaxis()->SetBinLabel(1+ic  ,Form("MTB1-%d",ic));
    hCT->GetYaxis()->SetBinLabel(17+ic ,Form("MTB3-%d",ic));
    hCT->GetYaxis()->SetBinLabel(33+ic ,Form("MTB5-%d",ic));
    hCT->GetYaxis()->SetBinLabel(49+ic ,Form("MTB4-%d",ic));
  }
  hCT->SetTitle("");
  //TExec *ex2 = new TExec("ex2","Pal2();");
  //ex2->Draw();
  hBT->SetStats(0);
  hBT->GetXaxis()->SetTitle("Date");
  hBT->GetYaxis()->SetTitle("Readout board");
  hBT->GetXaxis()->SetTitleOffset(1.2);
  hBT->GetYaxis()->SetTitleOffset(1.);
  hBT->GetZaxis()->SetLabelOffset(0.003);
  hBT->GetXaxis()->SetLabelOffset(0.002);
  hBT->GetZaxis()->SetTitleSize(0.04);
  hBT->GetYaxis()->SetTitleSize(0.04);
  
  hCT->SetStats(0);
  hCT->GetXaxis()->SetTitle("Date");
  hCT->GetYaxis()->SetTitle("Readout board");
  hCT->GetXaxis()->SetTitleOffset(1.2);
  hCT->GetYaxis()->SetTitleOffset(1.2);
  hCT->GetZaxis()->SetLabelOffset(0.003);
  hCT->GetXaxis()->SetLabelOffset(0.002);
  hCT->GetZaxis()->SetTitleSize(0.04);
  hCT->GetYaxis()->SetTitleSize(0.04);
  
  hBT->GetZaxis()->SetTitle("Data collection efficiency");
  hBT->GetZaxis()->RotateTitle(true);
  hBT->GetZaxis()->SetLabelSize(0.025);
  hBT->GetXaxis()->SetLabelSize(0.04);
  hBT->GetXaxis()->SetNdivisions(-DC.ndivise+2);
  hBT->GetXaxis()->SetRangeUser(DC.Dayi,DC.Dayf);
  hBT->GetZaxis()->SetRangeUser(0,1);
  hBT->GetZaxis()->SetNdivisions(-005);
  hCT->GetZaxis()->SetTitle("Data collection efficiency");
  hCT->GetZaxis()->RotateTitle(true);
  hCT->GetZaxis()->SetLabelSize(0.025);
  hCT->GetXaxis()->SetLabelSize(0.03);
  hCT->GetYaxis()->SetLabelSize(0.025);
  hCT->GetXaxis()->SetNdivisions(-DC.ndivise+2);
  hCT->GetXaxis()->SetRangeUser(DC.Dayi,DC.Dayf);
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
  

  c1->Print(Form("./Result/DayEffBoardcolz_%d_%d_%dto%d_%d_%dcom.png",2021,UTMGMT2021(DC.unixtimei),UTDGMT2021(DC.unixtimei),2021,UTMGMT2021(DC.unixtimef),UTDGMT2021(DC.unixtimef)));
  c1->Print(Form("./Result/DayEffBoardcolz_%d_%d_%dto%d_%d_%dcom.pdf",2021,UTMGMT2021(DC.unixtimei),UTDGMT2021(DC.unixtimei),2021,UTMGMT2021(DC.unixtimef),UTDGMT2021(DC.unixtimef)));
  c2->cd();
  hCT->Draw("colz");
  
  c2->Print(Form("./Result/DayEffChanncolz_%d_%d_%dto%d_%d_%dcom.png",2021,UTMGMT2021(DC.unixtimei),UTDGMT2021(DC.unixtimei),2021,UTMGMT2021(DC.unixtimef),UTDGMT2021(DC.unixtimef)));
  c2->Print(Form("./Result/DayEffChanncolz_%d_%d_%dto%d_%d_%dcom.pdf",2021,UTMGMT2021(DC.unixtimei),UTDGMT2021(DC.unixtimei),2021,UTMGMT2021(DC.unixtimef),UTDGMT2021(DC.unixtimef)));
  
  
  out.close();
  
  
}