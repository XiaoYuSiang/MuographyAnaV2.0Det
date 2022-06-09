#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TF1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TText.h>
#include <TGaxis.h>
#include "TStyle.h"
#include "/home/yusiang/personalLib/RootFile/untuplizerv8_YuSiang.h"
#include "/home/yusiang/personalLib/Style/tdrstyle.h"
#include "AnaVariable.h"
#include "GobelFunctions.h"
#include "path_dir.h"
#include "DSLData.h"

using namespace std;
using namespace MuoAna_path_dir;
using namespace MuographAnaVariable;
using namespace MuographGobelFuns;
using namespace DataConst;


//channel vs rate combine
void R4VTAna() {

  vector<string> vRootFiles = RawRootList(DirOperate);
  TreeReader data(vRootFiles);
  ofstream out(Form("%sR4VT.dat",DirOperate));
  //Data Variable
  //Int_t    frame   ;//event
  Int_t    unixtime;//abs time of event [s]
  Int_t    tYear   ;
  Int_t    tMonth  ;
  Int_t    tDate   ;
  Int_t    tHour   ;
  Int_t    tMinute ;
  Int_t    tSecond ;
  Int_t    nHits   ;//total number for hit in a frame/event
  Int_t*   board   ;//the board ID which if the index nth hit
  Int_t*   channel ;//the board ID which if the index nth hit

  //take time and set anlyze Constant for boundry condition
  data.GetEntry(0);

  //data hist create
  
  TH1F *hBT[NumBD];
  TH2F *hRT[NumBD];
  for(int i0=0;i0<NumBD;i0++){
    hBT[i0] = new TH1F(Form("hBT%d",BD[i0]),Form("MTB-%d iZ: %d",BD[i0],i0),NumCh,0,NumCh);
    hRT[i0] = new TH2F(Form("hRT%d",BD[i0]),Form("MTB-%d iZ: %d",BD[i0],i0),BinOnTimeD,FirDaySec,FinDaySec,NumCh,0,NumCh);//10min
  } 
  
  float maxcon = 0, mincon = 100;
  float maxconl = 0,minconl = 100;

  //evs = 1600;
  data.GetEntry(0);
  unixtime  = data.GetLong64("unixtime");
  tYear     = data.GetInt("tYear");
  tMonth    = data.GetInt("tMonth");
  tDate     = data.GetInt("tDate");
  tHour     = data.GetInt("tHour");
  tMinute   = data.GetInt("tMinute");
  tSecond   = data.GetInt("tSecond");
  
  for (Long64_t ev = 0; ev <evs; ++ev) {//evs; ++ev) {
    data.GetEntry(ev);
    unixtime  = data.GetLong64("unixtime");
    //frame     = data.GetInt("frame");//event
    tYear     = data.GetInt("tYear");
    tMonth    = data.GetInt("tMonth");
    tDate     = data.GetInt("tDate");
    tHour     = data.GetInt("tHour");
    tMinute   = data.GetInt("tMinute");
    tSecond   = data.GetInt("tSecond");
    nHits     = data.GetInt("nHits");
    board     = data.GetPtrInt("board");
    channel   = data.GetPtrInt("channel");
    vector<int> ihits;
    int i1 = 0;
    for(int i1 = 0; i1<nHits ;++i1){
      for(int i2=0;i2<NumBD;i2++){
        if(board[i1]==BD[i2]){
          hBT[i2]->Fill(channel[i1],RateWei);
          hRT[i2]->Fill(unixtime,channel[i1],1/th2fbinwidth);

          continue;
        }
      }
    }
  }
  /*Find Max and Min*/
  for(int i0 = 0; i0<NumBD ;++i0){
    for(int i1 = 0; i1<BinOnTime ;++i1){
      float conltmp = hBT[i0]->GetBinContent(i1+1);
      if(maxconl<conltmp) maxconl = conltmp;
      if(minconl>conltmp&&conltmp!=0) minconl = conltmp;
      for(int i2 = 0; i2<NumCh ;++i2){
        float contmp = hRT[i0]->GetBinContent(i1+1,i2+1);
        if(maxcon<contmp) maxcon = contmp;
        if(mincon>contmp&&conltmp!=0) mincon = contmp;
      }
    }
  }
    
  cout<<"maxcontent:\t"<<maxcon<<endl;
  out<<"maxcontent:\t"<<maxcon<<endl;
  cout<<"mincontentLa:\t"<<minconl<<"\tmaxcontentLa:\t"<<maxconl<<endl;
  out<<"mincontentLa:\t"<<minconl<<"\tmaxcontentLa:\t"<<maxconl<<endl;
  float maxSetUserB2D = MaxSetUserRatio(maxconl,0.9,2);
  float minSetUserB2D = MinSetUserRatio(minconl,1.2,2);
  float maxSetUserB1D = MaxSetUserRatio(maxconl,1.2,2);
  float minSetUserB1D = MinSetUserRatio(minconl,1.2,2);
  float maxSetUserRT = MaxSetUserRatio(maxcon,1.2,1);
  float minSetUserRT = MinSetUserRatio(mincon,1,1);
    
  
  TCanvas *c1 = new TCanvas("c1","",1080,1080);
  TCanvas *c2 = new TCanvas("c2","",1080,1080);
  TCanvas *c3 = new TCanvas("c3","",1200*2*2,1000*2*2);
  
  TH1F *hMod  = new TH1F("hMod","",NumCh,0,NumCh);
  hMod->SetStats(0);
  hMod->SetTitle("");
  hMod->GetXaxis()->SetTitle("Channel Index");
  hMod->GetYaxis()->SetTitle("Rate(s^{-1} ) & Ratio");
  hMod->GetYaxis()->SetTitleOffset(1.4);
  
  for(int i0=0;i0<NumBD;i0++){
    hBT[i0]->SetStats(0);
    hBT[i0]->GetXaxis()->SetTitle("Channel");
    hBT[i0]->GetYaxis()->SetTitle("Rate(s^{-1} )");
    hBT[i0]->GetYaxis()->SetTitleOffset(1.4);
    hBT[i0]->GetXaxis()->SetLabelSize(0.05);
    hBT[i0]->GetYaxis()->SetLabelSize(0.04);
    for (int i1=0;i1<NumCh;i1++){
      hBT[i0]->GetXaxis()->SetBinLabel(i1+1 ,Form("%d",i1));
    }
    hBT[i0]->GetXaxis()->SetRangeUser(0,NumCh);
    hBT[i0]->GetYaxis()->SetRangeUser(minSetUserB1D,maxSetUserB1D);
  }
  
  
  TLegend *L[NumBD];
  float RateMean[NumBD] = {},RateRMS[NumBD] = {};
  int nHErr[NumBD]={}, nLErr[NumBD]={};
  float *xerrH[NumBD], *yerrH[NumBD], *xerrL[NumBD], *yerrL[NumBD];
  TGraph *gAVRateCh[NumBD];
  for(int i0=0;i0<NumBD;i0++){
    for(int i1=0;i1<NumCh;i1++){
      RateMean[i0] += hBT[i0]->GetBinContent(i1+1);
    }
    RateMean[i0] /= NumCh;
    for(int i1=0;i1<NumCh;i1++){
      RateRMS[i0] += pow(hBT[i0]->GetBinContent(i1+1)-RateMean[i0],2);
    }
    RateRMS[i0] /= NumCh;
    RateRMS[i0] = sqrt(RateRMS[i0]);
    vector<float> vxerrH,vyerrH,vxerrL,vyerrL;
    for(int i1=0;i1<NumCh;i1++){
      if( hBT[i0]->GetBinContent(i1+1) > RateMean[i0]+RateRMS[i0]*Trigger_RateRMS ){
        vxerrH .push_back( hBT[i0]->GetBinCenter(i1+1));
        vyerrH .push_back( hBT[i0]->GetBinContent(i1+1));
      }
      else if( hBT[i0]->GetBinContent(i1+1) < RateMean[i0]-RateRMS[i0]*Trigger_RateRMS ){
        vxerrL .push_back( hBT[i0]->GetBinCenter(i1+1));
        vyerrL .push_back( hBT[i0]->GetBinContent(i1+1));
      }
    }
    
    nHErr[i0] = vxerrH.size();
    nLErr[i0] = vxerrL.size();
    xerrH[i0] = new float[nHErr[i0]];
    yerrH[i0] = new float[nHErr[i0]];
    xerrL[i0] = new float[nLErr[i0]];
    yerrL[i0] = new float[nLErr[i0]];
    for(int i1=0;i1<nHErr[i0];i1++) {
      xerrH[i0][i1] = vxerrH[i1];
      yerrH[i0][i1] = vyerrH[i1];
    }
    for(int i1=0;i1<nLErr[i0];i1++) {
      xerrL[i0][i1] = vxerrL[i1];
      yerrL[i0][i1] = vyerrL[i1];
    }
    float avgx[2]={0,NumCh}, avgy[2]={RateMean[i0],RateMean[i0]};
    gAVRateCh[i0]= new TGraph(2, avgx, avgy);
    gAVRateCh[i0]->SetFillColor(0);
    gAVRateCh[i0]->SetLineStyle(2);
    gAVRateCh[i0]->SetMarkerColor(0);
  }
  
  
  TH2F *h2DMod  = new TH2F("h2DMod","",NumnX,0,NumnX,NumnY,0,NumnY);
  h2DMod->SetStats(0);
  h2DMod->SetTitle("");
  h2DMod->GetXaxis()->SetTitle("iX");
  h2DMod->GetYaxis()->SetTitle("iY");
  h2DMod->GetXaxis()->CenterTitle(true);
  h2DMod->GetYaxis()->CenterTitle(true);
  h2DMod->GetZaxis()->SetTitle("Rate(s^{-1} )");
  h2DMod->GetYaxis()->SetTitleOffset(1.4);
  h2DMod->GetZaxis()->SetTitleOffset(0);
  h2DMod->GetXaxis()->SetLabelSize(0.07);
  h2DMod->GetYaxis()->SetLabelSize(0.07);
  h2DMod->GetZaxis()->SetLabelSize(0.03);
  h2DMod->SetMarkerSize(3);
  
  
  c3->Divide(2,2); 
  c3->cd(1);
  c3->SetGridx(); 
  
  TText *ZeroZero = new TText(3.5,.5,"0");
  ZeroZero->SetTextAlign(22);  ZeroZero->SetTextColor(1); 
  ZeroZero->SetTextSize(0.06);
  TText *ThreThre = new TText(.5,.5,"3");
  ThreThre->SetTextAlign(22);  ThreThre->SetTextColor(1); 
  ThreThre->SetTextSize(0.06);
  
  for(int i0=0;i0<NumBD;i0++){
    if(NumBD>9){
      cout<<"Error: Please ask Yu Siang Xiao to add the color palette, now color is large than 9"<<endl;
      cout<<"Macro/R4VTAna.C/~230sent."<<endl;
      throw;
    } 
    hBT[i0]->SetLineColor(ColorArr9[i0]);
    hBT[i0]->SetLineWidth(2);
  }
  
  for (int i1=0;i1<NumCh;i1++){
    hMod->GetXaxis()->SetBinLabel(i1+1 ,Form("%d",i1));
  }
  cout<<"The during days is"<<(Dayf-Dayi)<<endl;
  out<<"The during days is"<<(Dayf-Dayi)<<endl;
  c1->Divide(2,2); 
  
  TGraph *ModErrPL = new TGraph();
  ModErrPL->SetMarkerStyle(22); ModErrPL->SetMarkerColor(2); ModErrPL->SetMarkerSize(2);
  TGraph *ModErrPH = new TGraph();
  ModErrPH->SetMarkerStyle(23); ModErrPH->SetMarkerColor(4); ModErrPH->SetMarkerSize(2);
  
  
  for(int i0=0;i0<NumBD;i0++){
    c1->cd(i0+1)->SetTicks(0,1);
    //hMod->Draw();
    hBT[i0]->Draw("h");
    gAVRateCh[i0]->Draw("lsame");
    TLegend *L = new TLegend(0.15,0.75,0.85,0.9);
    L->SetNColumns(2);
    L->SetLineColorAlpha (0, 0);
    L->SetFillColorAlpha (0, 0);
    L->AddEntry(hBT[i0],Form("MTB-%d (GChID: %02.f~%02.f)",BD[i0],i0*NumCh*1.,(i0+1)*NumCh*1.-1),"F");
    L->AddEntry(ModErrPL,Form("High Rate(>%2.1fRMS) Ch.",Trigger_RateRMS),"P");
    L->AddEntry(gAVRateCh[i0],Form("Mean: %.2f  RMS: %.2f",RateMean[i0],RateRMS[i0]),"l");
    L->AddEntry(ModErrPH,Form("Low  Rate(<%2.1fRMS) Ch.",Trigger_RateRMS),"P");
    L->Draw("same");
    
    if(nHErr[i0]!=0){
      TGraph *ErrPH = new TGraph(nHErr[i0],xerrH[i0],yerrH[i0]);
      ErrPH->SetMarkerStyle(22);
      ErrPH->SetMarkerColor(2);
      ErrPH->SetMarkerSize(2);
      ErrPH->Draw("psame");
    } 
    if(nLErr[i0]!=0){
      TGraph *ErrPL = new TGraph(nLErr[i0],xerrL[i0],yerrL[i0]);
      ErrPL->SetMarkerStyle(23);
      ErrPL->SetMarkerColor(4);
      ErrPL->SetMarkerSize(2);
      ErrPL->Draw("psame");
    }
    
  }
  hMod->GetXaxis()->SetRangeUser(0,NumCh);
  hMod->GetYaxis()->SetRangeUser(0,maxSetUserRT);
  

  
  c1->SetGrid(1,1);
  c1->Print(Form("%sR4VChBox_%s.png",DirRes_RatVCh,TimeStr));
  c1->Print(Form("%sR4VChBox_%s.pdf",DirRes_RatVCh,TimeStr));
  
  c1->Close();
  
  TH2F *hBRvT2D[NumBD];
  for(int i0=0;i0<NumBD;i0++){
    hBRvT2D[i0] = new TH2F(Form("hB%dRvT2D",BD[i0]),"",NumnX,0,NumnX,NumnY,0,NumnY);
  }
  for(int i0=0;i0<4;i0++){
    hBRvT2D[i0]->SetStats(0);
    hBRvT2D[i0]->SetTitle(Form("MTB-%d iZ: %d",BD[i0],i0));
    hBRvT2D[i0]->GetXaxis()->SetTitle("iX");
    hBRvT2D[i0]->GetYaxis()->SetTitle("iY");
    hBRvT2D[i0]->GetXaxis()->CenterTitle(true);
    hBRvT2D[i0]->GetYaxis()->CenterTitle(true);
    hBRvT2D[i0]->GetZaxis()->SetTitle("Rate(s^{-1} )");
    hBRvT2D[i0]->GetZaxis()->RotateTitle(true);
    hBRvT2D[i0]->GetYaxis()->SetTitleOffset(1.4);
    hBRvT2D[i0]->GetZaxis()->SetTitleOffset(1.1);
    hBRvT2D[i0]->GetXaxis()->SetLabelSize(0.07);
    hBRvT2D[i0]->GetYaxis()->SetLabelSize(0.07);
    hBRvT2D[i0]->GetZaxis()->SetLabelSize(0.03);
    for (int i1=0;i1<NumCh;i1++){
      h2DMod->SetBinContent(4-i1%4,i1/4+1,i1);//@@
      hBRvT2D[i0]->SetBinContent(4-i1%4,i1/4+1,hBT[i0]->GetBinContent(i1+1));
    }
    for (int i1=0;i1<NumnX;i1++){
      hBRvT2D[i0]->GetXaxis()->SetBinLabel(1+i1 ,Form("%d",i1));
    }
    for (int i1=0;i1<NumnY;i1++){
      hBRvT2D[i0]->GetYaxis()->SetBinLabel(1+i1 ,Form("%d",i1));
    }
  }

  //cout<<"-------------"<<h2DMod->GetBinContent(1,1)<<endl;
  h2DMod->SetBinContent(1,1,0.0);
  for (int i1=0;i1<NumBD;i1++){
    h2DMod->GetXaxis()->SetBinLabel(1+i1 ,Form("%d",i1));
    h2DMod->GetYaxis()->SetBinLabel(1+i1 ,Form("%d",i1));
  }
  c2->cd();
  h2DMod->Draw("text");
  ZeroZero->Draw();
  ThreThre->Draw();
  c2->SetGrid(1,1);
  c2->SetTicks(1,1);
  c2->Print(Form("%sChID&XYZ.png",DirIMFdat));
  c2->Print(Form("%sChID&XYZ.pdf",DirIMFdat));
  c2->Clear();
  c2->Divide(2,2); 
  c2->SetFrameBorderMode(0); 
  for(int i0=0;i0<NumBD;i0++){
    c2->cd(i0+1)->SetLeftMargin(0.9);
    c2->cd(i0+1)->SetRightMargin(0.13);
    c2->cd(i0+1);
    hBRvT2D[i0]->Draw("colz");
    h2DMod->Draw("textsame");
    ZeroZero->Draw();
    ThreThre->Draw();

    hBRvT2D[i0]->GetZaxis()->SetRangeUser(minSetUserB2D,maxSetUserB2D);//maxconl);
    
  }
  h2DMod->GetXaxis()->SetRangeUser(0,4);
  h2DMod->GetYaxis()->SetRangeUser(0,4);
  //h2DMod->GetZaxis()->SetRangeUser(minconl,maxconl);
  //gStyle->SetPalette(51);
  
  c2->Print(Form("%sR4VCh2D_%s.png",DirRes_RatVCh,TimeStr));
  c2->Print(Form("%sR4VCh2D_%s.pdf",DirRes_RatVCh,TimeStr));
  
  // c2->Close();
  
  
  
  TH2F *hRTLego[NumBD];
  int rotateRate = 6;
  for(int i0=0;i0<NumBD;i0++){
    hRT[i0]->SetStats(0);
    hRT[i0]->SetTitle(Form("MTB-%d iZ: %d",BD[i0]));
    hRT[i0]->GetXaxis()->SetTitle("Date");
    hRT[i0]->GetYaxis()->SetTitle("Channel ID");
    hRT[i0]->GetZaxis()->SetTitle("Rate(s^{-1})");
    hRT[i0]->GetXaxis()->SetTitleOffset(1.8);
    hRT[i0]->GetYaxis()->SetTitleOffset(1.1);
    hRT[i0]->GetZaxis()->SetTitleOffset(1.1);
    hRT[i0]->GetZaxis()->SetLabelOffset(0.003);
    hRT[i0]->GetXaxis()->SetLabelOffset(0.002);
    hRT[i0]->GetXaxis()->SetTitleSize(0.04);
    hRT[i0]->GetYaxis()->SetTitleSize(0.04);
    hRT[i0]->GetZaxis()->SetTitleSize(0.04);
    hRT[i0]->GetZaxis()->SetLabelSize(0.025);
    hRT[i0]->GetXaxis()->SetLabelSize(0.03);
    hRT[i0]->GetYaxis()->SetLabelSize(0.035);
    hRT[i0]->GetXaxis()->SetNdivisions(-ndivise+2);
    hRT[i0]->GetXaxis()->SetRangeUser(Dayi,Dayf);
    hRT[i0]->GetZaxis()->SetRangeUser(0,maxSetUserRT);
    hRT[i0]->GetXaxis()->CenterTitle(true);
    hRT[i0]->GetYaxis()->CenterTitle(true);
    hRT[i0]->GetZaxis()->CenterTitle(true);
    hRT[i0]->GetZaxis()->SetNdivisions(-005);
    hRT[i0]->SetLineColor(1);
    hRT[i0]->GetZaxis()->SetRangeUser(minSetUserB2D,maxSetUserB2D);//maxcon*1.2);
    for (int i1=0;i1<NumCh;i1++){
      hRT[i0]->GetYaxis()->SetBinLabel(i1+1 ,Form("%d",i1));
    }
    
    hRTLego[i0] = (TH2F*) hRT[i0]->Clone();
    hRTLego[i0]->GetXaxis()->SetLabelSize(0.02);
    hRTLego[i0]->GetZaxis()->SetTitleOffset(1.5);
    hRT[i0] ->GetZaxis()->RotateTitle(true);
    for (int i3=0;i3<ndivise+1;i3++){
      unixtime = hRT[i0]->GetXaxis()->GetBinCenter(1+86400/th2fbinwidth*i3);
      unixTimeToHumanReadable(unixtime, tYear, tMonth, tDate, tHour, tMinute, tSecond,timeZone);
      char str_MD[30] ={};
      sprintf(str_MD , "%s-%02.f",MonthName[tMonth],1.*tDate);
      if(tDate==1||tDate==15||i3==0||i3==ndivise||(ndivise+1)<32){
        if(i3 == ndivise){
               hRT[i0]->GetXaxis()->SetBinLabel(86400/th2fbinwidth*i3 ,str_MD);
               hRTLego[i0]->GetXaxis()->SetBinLabel(86400/th2fbinwidth*i3/rotateRate ,str_MD);
        }else{
          hRT[i0]->GetXaxis()->SetBinLabel(1+86400/th2fbinwidth*i3 ,str_MD);
          hRTLego[i0]->GetXaxis()->SetBinLabel(1+86400/th2fbinwidth*i3/rotateRate ,str_MD);
        }
      }
      // cout<<"i3:    "<<i3<<"    "<<unixtime<<"    "<<str_MD<<endl;
    }
  }
  

  
  for(int i0=0;i0<NumBD;i0++){
    c3->cd(i0+1);
    c3->cd(i0+1)->SetLeftMargin(0.09);
    c3->cd(i0+1)->SetRightMargin(0.13);
    c3->cd(i0+1)->SetBottomMargin(0.15);
    //hXYZMod->Draw("colz");
    hRT[i0]->Draw("colz");
    
  }


  c3->Print(Form("%sR4VTcolz_%s.png",DirRes_RatVT,TimeStr));
  c3->Print(Form("%sR4VTcolz_%s.pdf",DirRes_RatVT,TimeStr));

  

  
  for(int i0=0;i0<NumBD;i0++){
    c3->cd(i0+1);
    c3->cd(i0+1)->SetLeftMargin(0.13);
    c3->cd(i0+1)->SetRightMargin(0.09);
    hRTLego[i0]->Draw("lego2");

  }


  c3->Print(Form("%sR4VTLego2_%s.png",DirRes_RatVT,TimeStr));
  c3->Print(Form("%sR4VTLego2_%s.pdf",DirRes_RatVT,TimeStr));
  c3->Close();
  




  
}