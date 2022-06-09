//include
#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TPDF.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TGaxis.h>
#include <TText.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include <TGraphErrors.h>
#include "/home/yusiang/personalLib/Style/tdrstyle.h"
#include "/home/yusiang/personalLib/RootFile/untuplizerv8_YuSiang.h"
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

//Rate vs Time for Channel
void SEAnaV2() {
  setTDRStyle();
  char FileSave[200];
  sprintf(FileSave,"%sETracksGT%d_%s_nHTH%dL%d.root",DirOperate,eventGapTcnt[28],TimeStr,TriggerS,TriggerL);

  TreeReader data(FileSave);
  ofstream out(Form("%sSEV2.dat",DirOperate));
  ofstream report(Form("%sReport_SENumDayBin_%s.dat",DirRes_LFirTra,TimeStr));
  //Data Variable
  //Int_t    frame   ;//event

  Int_t    unixtime;//abs time of event [s]
  Int_t    tYear   ;
  Int_t    tMonth  ;
  Int_t    tDate   ;
  Int_t    tHour   ;
  Int_t    tMinute ;
  Int_t    tSecond ;
  Int_t    nLayers ;//total number for hit in a frame/event
  Int_t    nH   ;//total number for hit in a frame/event
  Int_t    nH0  ;//total number for hit in a frame/event
  Int_t    nH1  ;//total number for hit in a frame/event
  Int_t    nH2  ;//total number for hit in a frame/event
  Int_t    nH3  ;//total number for hit in a frame/event
  //Int_t*    seq     ;//the board ID which if the index nth hit
  // Int_t*   dtime   ;
  // Int_t*   pcnt    ;
  // Int_t*   tcnt    ;
  Int_t   *pwidth0  ;
  Int_t   *pwidth1  ;
  Int_t   *pwidth2  ;
  Int_t   *pwidth3  ;
  
  //take time and set anlyze Constant for boundry condition


  Int_t th2fbinwidthtmp = 86400*bindays;
  Int_t BinOnTime    = int((FinDaySec-FirDaySec)/th2fbinwidthtmp)+1;
  cout<<"BinOnTime:"<<BinOnTime<<endl;

  
  //data hist create

  TH1F *hpw1      = new TH1F("Pwidth of MTB1","",160,0,160);
  TH1F *hpw3      = new TH1F("Pwidth of MTB3","",160,0,160);
  TH1F *hpw5      = new TH1F("Pwidth of MTB5","",160,0,160);
  TH1F *hpw4      = new TH1F("Pwidth of MTB4","",160,0,160);
  TH1F *hpw[4]    = {hpw1,hpw3,hpw5,hpw4};
  TH1F *hERT3     = new TH1F("hERT3","",BinOnTime,FirDaySec,FinDaySec);
  TH1F *hERT4     = new TH1F("hERT4","",BinOnTime,FirDaySec,FinDaySec);
  TH1F *hCRvT     = new TH1F("hCRvT","",BinOnTime,FirDaySec,FinDaySec);//event 
  TH1F *hERTA     = new TH1F("hERTA","",BinOnTime,FirDaySec,FinDaySec);//event 

  cout<<"bins:\t"<<BinOnTime<<endl;
  
  //set anlyze Variable


  float maxcon  = 0 , maxconA  = 0;


  int evsSE = data.GetEntriesFast();
  //Fill the data
  for (int ev = 0; ev <evsSE; ++ev) {//evs; ++ev) {
    data.GetEntry(ev);
    unixtime  = data.GetLong64("unixtime");
    if(unixtime>FinDaySec) break;
    //frame     = data.GetInt("frame");//event
    nLayers = data.GetInt("nLayers");
    nH      = data.GetInt("nLayers");
    nH0     = data.GetInt("nH0");
    nH1     = data.GetInt("nH1");
    nH2     = data.GetInt("nH2");
    nH3     = data.GetInt("nH3");
    pwidth0  = data.GetPtrInt("pwidth0");
    pwidth1  = data.GetPtrInt("pwidth1");
    pwidth2  = data.GetPtrInt("pwidth2");
    pwidth3  = data.GetPtrInt("pwidth3");
    
    //seq       = data.GetPtrInt("seq");

    bool B_CaseA=false, B_CaseB=false;
    if(nH0==1 &&nH1==1 &&nH2==1 &&nH3==1 ) B_CaseA = true;
    if(nLayers == 3 && nH0==0 && nH==3 )   B_CaseB = true;
    if(B_CaseA == false && B_CaseB == false) continue;
    
    if(B_CaseA){
      int pwidch[4] = {pwidth0[0],pwidth1[0],pwidth2[0],pwidth3[0]};
      hERT4->Fill(unixtime);
      for(int i2=0;i2<4;i2++) hpw[i2]->Fill(double(pwidch[i2]/BDPwWei[i2]));
    }
    if(B_CaseB){
     int pwidch[3] = {pwidth1[0],pwidth2[0],pwidth3[0]};
      hERT3->Fill(unixtime);
      for(int i2=1;i2<4;i2++) hpw[i2]->Fill(double(pwidch[i2-1]/BDPwWei[i2-1]));
    }
    hERTA->Fill(unixtime);
  }
  int XA[BinOnTime+1];
  int Y3[BinOnTime+1],Y4[BinOnTime+1],YA[BinOnTime+1];
  
  for(int i1 = 0; i1<BinOnTime+1 ;++i1){
    XA[i1] = th2fbinwidthtmp*(i1)+FirDaySec;
    Y3[i1] = hERT3->GetBinContent(i1+1);
    Y4[i1] = hERT4->GetBinContent(i1+1);
    YA[i1] = hERTA->GetBinContent(i1+1);
    unixTimeToHumanReadable(XA[i1], tYear, tMonth, tDate, tHour, tMinute, tSecond,timeZone);
    
    cout<<"ut:YYYYMMDDHH:Y3:Y4:YA:    "<<XA[i1]<<"\t"<<Form("%4.f%02.f%02.f-%02.f",tYear*1.,tMonth*1.,tDate*1.,tHour*1.)<<"\t"<<Y3[i1]<<"\t"<<Y4[i1]<<"\t"<<YA[i1]<<endl;
  }

  for(int i1 = 0; i1<BinOnTime ;++i1){
    if(maxconA<YA[i1]) maxconA = YA[i1];
  }

  cout<<"maxcontentALL:\t"<<maxconA<<endl;
  out<<"maxcontentALL:\t"<<maxconA<<endl;
  
  //set canvas
  TCanvas *c1 = new TCanvas("c1","",1920,1080);
  TCanvas *c1s= new TCanvas("c1s","",1920,2160);

  TCanvas *c3 = new TCanvas("c3","",1920,1080);

  
  TLegend *Lhpw[4];

  //gStyle-> SetStatH(0.1); 
  c1->cd();
  c1->Divide(2,2);
  vector <int> numMax;
  for(int i0=0;i0<NumBD;i0++){
    numMax.push_back(hpw[i0]->GetMaximum());
  }
  int maxbin = *max_element(numMax.begin(), numMax.end());
  for(int i0=0;i0<4;i0++){
    c1->cd(i0+1);
    hpw[i0]->GetXaxis()->SetTitle("PWidth(100ns) PS: scale normalized ");
    hpw[i0]->GetYaxis()->SetTitle("Number of tracks");

    hpw[i0]->SetLineColor(ColorArr9[i0]);
    hpw[i0]->SetLineWidth(2);
    hpw[i0]->GetXaxis()->SetRangeUser(0,50);
    hpw[i0]->Draw("box");
    double Mx[2]={hpw[i0]->GetMean()  , hpw[i0]->GetMean() };
    double My[2]={ 0                  , maxbin*1.2         };
    double Ex[2]={hpw[i0]->GetRMS ()  , hpw[i0]->GetRMS()  };
    double Ey[2]={maxbin*1.2          , maxbin*1.2         };
    TGraphErrors *MeanMark= new TGraphErrors(2,Mx,My,Ex,Ey);
    TGraphErrors *MeanMarkAdd = new TGraphErrors();
    Lhpw[i0] = new TLegend(0.75,0.60,0.98,0.95);
    Lhpw[i0] -> AddEntry(hpw[i0],Form("MTB%d",BD[i0]),"l");
    Lhpw[i0] -> AddEntry(MeanMark,Form("Mean: % 6.2f",hpw[i0]->GetMean()),"E2");
    Lhpw[i0] -> AddEntry(MeanMarkAdd,Form("RMS : % 6.2f",hpw[i0]->GetRMS ()),"F");
    MeanMark ->SetLineStyle(3);
    
    hpw[i0]->Draw("h");
    Lhpw[i0]->Draw("same");
    MeanMark->Draw("e2same");
    MeanMark->Draw("Esame");
    MeanMark->SetFillColorAlpha(2,0.1);
    MeanMarkAdd->SetFillColorAlpha(2,0.1);
    MeanMarkAdd->SetLineColor(0);
  }
  double numMaxtmp=0;
  for(int i=0;i<4;i++){
    if(numMax[i]>numMaxtmp) numMaxtmp = numMax[i];
  } 
  numMaxtmp*=1.2;
  for(int i=0;i<4;i++) hpw[i]->GetYaxis()->SetRangeUser(0,numMaxtmp);
  
  //Lhpw->AddEntry(hpw4,Form("4point:\t%d",nhERT4),"lp");
  //set model for TH1F and
  
  
  TH1F *hMod      = new TH1F("hMod","",BinOnTime,XA[0],XA[BinOnTime]);
  hMod->SetStats(0);
  hMod->GetXaxis()->SetTitle("Date");
  hMod->GetXaxis()->SetLabelSize(0.025);
  
  
  
  for (int i1=0;i1<BinOnTime;i1++){
    int timeBinDay = FirDaySec + 86400*i1;
    unixTimeToHumanReadable(timeBinDay, tYear, tMonth, tDate, tHour, tMinute, tSecond,timeZone);
    if(tDate==1||tDate==15||i1==0||i1==ndivise||BinOnTime<32){
      hMod->GetXaxis()->SetBinLabel((i1+1) ,Form("%s-%02.f",MonthName[tMonth],1.*tDate));
    }
  }
  
  
  int nhERT3 = hERT3->Integral(-1,-1);
  int nhERT4 = hERT4->Integral(-1,-1);
  int nhERTA = hERTA->Integral(-1,-1);
  cout<<"3H:  "<<nhERT3<<"  4H:  \t"<<nhERT4<<"  AH:  \t"<<nhERTA<<"\t"<<endl;

  TLegend *LERT = new TLegend(0.7,0.91,1,1);
  
 // TGraph *g3 = new TGraph(BinOnTime,XA,Y3);  
  // TGraph *g4 = new TGraph(BinOnTime,XA,Y4); 
  TGraph *gA = new TGraph(BinOnTime,XA,YA);  
  TH1F   *g3 = new TH1F("g3","",BinOnTime,XA[0],XA[BinOnTime]);  
  TH1F   *g4 = new TH1F("g4","",BinOnTime,XA[0],XA[BinOnTime]);  
  cout<<"BinOnTime,XA[0]\t"<<BinOnTime<<"\t"<<XA[0]<<"\t"<<XA[BinOnTime]<<endl;
  for(int i=0;i<BinOnTime;i++){
    g3->SetBinContent(i+1,hERT3->GetBinContent(i+1));
    g4->SetBinContent(i+1,hERT4->GetBinContent(i+1));
    
  }
  //float maxcon  = 0;
  int maxg3 = g3->GetMaximum();
  int maxg4 = g4->GetMaximum();
  bool b3L4 = maxg3>maxg4;
  maxcon = maxg4;
  
  if(b3L4){
    maxcon = maxg3;
  }
  
  

  g3->SetBarWidth(0.39);
  g3->SetBarOffset(0.11+0.4*0);
  
  g4->SetBarWidth(0.39);
  g4->SetBarOffset(0.11+0.4*1);
  
  c3->cd();
  
  g3->SetMarkerStyle(33);
  g4->SetMarkerStyle(33);
  g3->SetLineColor(ColorArr9[0]);
  g4->SetLineColor(ColorArr9[1]);
  g3->SetFillColor(ColorArr9[0]);
  g4->SetFillColor(ColorArr9[1]);
  g3->SetMarkerSize(4);
  g4->SetMarkerSize(3);
  
  hMod->SetStats(0);
  hMod->SetTitle(TimeStrTitle);
  hMod->GetXaxis()->SetTitle("Date");
  hMod->GetYaxis()->SetTitle("Number of tracks");
  hMod->SetTitle("");//Form("Event VS time For Case A,B"));
  hMod->GetXaxis()->SetRangeUser(FirDaySec,FinDaySec+86400);
  hMod->GetYaxis()->SetRangeUser(0,maxcon*1.2);
  hMod->GetXaxis()->SetTickLength(0);
  
  
  
  hMod->Draw();
  if(b3L4){
    g3->Draw("barsame");
    g4->Draw("barsame");
  }else{
    g4->Draw("barsame");
    g3->Draw("barsame");
  }
  TLegend *Lg = new TLegend(0.66,0.85,0.98,0.95);
  Lg->SetFillColor(0);
  Lg->SetNColumns(2);
  // Lg->SetFillColorAlpha (0, 0.00);
  Lg->AddEntry(g4,Form("Case A:  %4d",nhERT4),"F");
  Lg->AddEntry(g3,Form("Case B:  %4d",nhERT3),"F");
  Lg->Draw();
 // hMod->GetYaxis()->SetRangeUser(0,;
  c1->Print(Form("%sSEPWidth_%dday.pdf",DirRes_LFirTra,bindays));
  c1->Print(Form("%sSEPWidth_%dday.png",DirRes_LFirTra,bindays));
  c3->Print(Form("%sSEVTTG_com%dday.pdf",DirRes_LFirTra,bindays));
  c3->Print(Form("%sSEVTTG_com%dday.png",DirRes_LFirTra,bindays));
  
  

  c1s->Divide(1,2);
  
  c1s->cd(1);
  for(int i=0;i<4;i++) hpw[i]->Scale(1./hpw[i]->Integral(-1,-1));
  
  numMaxtmp=0;
  for(int i=0;i<4;i++){
    if(hpw[i]->GetMaximum()>numMaxtmp) numMaxtmp = hpw[i]->GetMaximum();
  } 
  numMaxtmp*=1.2;
  for(int i=0;i<4;i++) hpw[i]->GetYaxis()->SetRangeUser(0,numMaxtmp);
  cout<<"numMaxtmp:  "<<numMaxtmp<<endl;
  hpw[0]->Draw("h");
  for(int i=0;i<4;i++){
    hpw[i]->GetYaxis()->SetTitle("Ratio of Number of tracks");
    hpw[i]->Draw("hsame");
    Lhpw[i]->Draw("same");
    Lhpw[i]->SetX1NDC(0.48+0.125*i);
    Lhpw[i]->SetY1NDC(0.60);
    Lhpw[i]->SetX2NDC(0.605+0.125*i);
    Lhpw[i]->SetY2NDC(0.95);
  }
  c1s->cd(2);
  
  hMod->Draw();
  hMod->GetXaxis()->SetRangeUser(XA[0],XA[BinOnTime]);
  if(b3L4){
    g3->Draw("barsame");
    g4->Draw("barsame");
  }else{
    g4->Draw("barsame");
    g3->Draw("barsame");
  }
  c1s->cd(2)->SetTicks(0,0);
  //hMod->GetYaxis()->SetNdivisions(0,(maxconA+1));

  Lg->Draw();
  c1s->cd(2)->SetTicks(0,0);
  c1s->Print(Form("%sSEResult_com%dday.png",DirRes_LFirTra,bindays));
  c1s->Print(Form("%sSEResult_com%dday.pdf",DirRes_LFirTra,bindays));
    
  TPDF *pdfc1sc   = new TPDF(Form("%sSEResult_com%dday%s.pdf",DirRes_LFirTra,bindays,TimeStr));

  PDFCanPage(pdfc1sc    ,c1s ,Form("Title"));
  
  char cmdline[180];
  sprintf(cmdline,"mkdir -p %sPerDayAna/",DirRes_LFirTra);
  if (system(cmdline)!=0)  system(cmdline);
  c3->cd();
  c3->Update();
  report << "report: Selection number of muon list per binday:"<<endl;
  report<<"--------------------------------------------------"<<endl;
  report<<"YYYYMMDD\tUT\tTZone\tNumCaseA\tNumCaseB"<<endl;
  report<<"--------------------------------------------------"<<endl;
  for(int i=0;i<BinOnTime;i++){
    TLegend *Lgp = new TLegend(0.66,0.85,0.98,0.95);
    Lgp->SetNColumns(2);
    int g4b = g4->GetBinContent(i+1), g3b = g3->GetBinContent(i+1);
    cout<<"g4b:g3b:   "<<g4b<<"\t"<<g3b<<endl;
    Lgp->AddEntry(g4,Form("Case A:  %4d",g4b),"F");
    Lgp->AddEntry(g3,Form("Case B:  %4d",g3b),"F");
    Lgp->Draw();
    
    int timeBinDay = FirDaySec + 86400*i;
    unixTimeToHumanReadable(timeBinDay, tYear, tMonth, tDate, tHour, tMinute, tSecond,timeZone);
    char str_MD[30] ={},str_MD1[40] ={};
    sprintf(str_MD , "%s-%02.f",MonthName[tMonth],1.*tDate);
    // cout<<tDate<<endl;
    sprintf(str_MD1, "%04.f%02.f%02.f",tYear*1.,tMonth*1.,1.*tDate);
    // cout<<str_MD1<<endl;
    // hMod->GetXaxis()->SetBinLabel(i+1,str_MD);
    hMod->GetXaxis()->SetRangeUser(
      g4->GetBinCenter(i+1)-g4->GetBinWidth(i)/2.,g4->GetBinCenter(i+1)+g4->GetBinWidth(i)/2.);
    g3->GetXaxis()->SetRangeUser(
      g4->GetBinCenter(i+1)-g4->GetBinWidth(i)/2.,g4->GetBinCenter(i+1)+g4->GetBinWidth(i)/2.);
    g4->GetXaxis()->SetRangeUser(
      g4->GetBinCenter(i+1)-g4->GetBinWidth(i)/2.,g4->GetBinCenter(i+1)+g4->GetBinWidth(i)/2.);
    
    c3->Print(Form("%sPerDayAna/SE%dday%s.pdf",DirRes_LFirTra,bindays,str_MD1));
    cout<<(Form("%sPerDayAna/SE%dday%s.pdf",DirRes_LFirTra,bindays,str_MD1))<<endl;
    report<<str_MD1<<"\t"<<timeBinDay<<"\t"<<timeZone<<"\t"<<g4b<<"\t"<<g3b<<endl;
    if(i==BinOnTime-1) PDFCanPage(pdfc1sc,c3,str_MD,BinOnTime,BinOnTime);
    else PDFCanPage(pdfc1sc   ,c3 ,str_MD);
  }
}
