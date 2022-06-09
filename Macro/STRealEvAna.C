#include "/home/yusiang/personalLib/Style/tdrstyle.C"
#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TLegend.h>
#include <TGaxis.h>
#include <TText.h>
#include <TFile.h>
#include <TPad.h>
#include <TTree.h>
#include <TColor.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include "AnaVariable.h"
#include "GobelFunctions.h"
#include "path_dir.h"
#include "DSLData.h"

using namespace std;
using namespace MuoAna_path_dir;
using namespace MuographAnaVariable;
using namespace MuographGobelFuns;
using namespace DataConst;

void STRealEvAna(){
  // setTDRStyle();


  
  TH1F  *hMod          = new TH1F("hMod"   ,"",16,0,16);
  for(int fileindex=0; fileindex<1;fileindex++){


    char RFNT[200];
    sprintf(RFNT,"%sTracksGT%d_%s_nHTH%dL%d.root",DirOperate,eventGapTcnt[28],TimeStr,TriggerS,TriggerL);
    cout<<"Root file: "<<Form("%s",RFNT)<<" is opening!"<<endl;
    TChain *t = new TChain("t");
    t->Add(Form("%s",RFNT));
    
    TH1F  *H3_X354    = new TH1F("H3_X354","",16,0,16);
    TH1F  *H3_1X54    = new TH1F("H3_1X54","",16,0,16);
    TH1F  *H3_13X4    = new TH1F("H3_13X4","",16,0,16);
    TH1F  *H3_135X    = new TH1F("H3_135X","",16,0,16);
    TH1F  *H3_1354[4] ={H3_X354,H3_1X54,H3_13X4,H3_135X};
    TH1F  *H4_1354       = new TH1F("H4_1354","",16,0,16);
    TLegend *pwdVSddt = new TLegend(0.15,.76,.96,0.89);
    pwdVSddt->SetFillColor(0);
    pwdVSddt->SetLineColor(0);
    pwdVSddt->SetNColumns(5);
    pwdVSddt->AddEntry(H3_X354,"nH==3 no MTB 1","FEl");
    pwdVSddt->AddEntry(H3_1X54,"nH==3 no MTB 3","FEl");
    pwdVSddt->AddEntry(H3_13X4,"nH==3 no MTB 5","FEl");
    pwdVSddt->AddEntry(H3_135X,"nH==3 no MTB 4","FEl");
    pwdVSddt->AddEntry(H4_1354,"nH==4 all Pass","E1X0p");
    TLegend *pwdVSddt1 = new TLegend(0.23,.76,.93,0.89);
    pwdVSddt1->SetFillColor(0);
    pwdVSddt1->SetLineColor(0);
    pwdVSddt1->SetNColumns(4);
    pwdVSddt1->AddEntry(H3_X354,"Effciency of MTB 1","FEl");
    pwdVSddt1->AddEntry(H3_1X54,"Effciency of MTB 3","FEl");
    pwdVSddt1->AddEntry(H3_13X4,"Effciency of MTB 5","FEl");
    pwdVSddt1->AddEntry(H3_135X,"Effciency of MTB 4","FEl");
    TCanvas *cevent = new TCanvas("cevent","",960,1080);
    cevent ->Divide(1,2);
    cevent ->cd(1);
    if (fileindex==0) hMod->GetYaxis()->SetRangeUser(0,550);
    if (fileindex==1) hMod->GetYaxis()->SetRangeUser(0,350);
    if (fileindex==2) hMod->GetYaxis()->SetRangeUser(0,350);
    if (fileindex==3) hMod->GetYaxis()->SetRangeUser(0,800);
    hMod->Draw();
    pwdVSddt->Draw();
    hMod->GetYaxis()->SetTitle("Event Number");
    hMod->GetXaxis()->SetTitle("Channel ID");
    hMod->SetStats(0);  
    t->Draw("channel>>H3_X354","BiZ==14 &&eventHit<=4 ","histbarSame");
    t->Draw("channel>>H3_1X54","BiZ==13 &&eventHit<=4 ","histbarSame");
    t->Draw("channel>>H3_13X4","BiZ==11 &&eventHit<=4 ","histbarSame");
    t->Draw("channel>>H3_135X","BiZ==7  &&eventHit<=4 ","histbarSame");
    t->Draw("channel>>H4_1354","BiZ==15 &&eventHit<=4 ","e1x0pSame");
    for(int bd=0;bd<4;bd++){
      H3_1354[bd]->SetFillColor(ColorArr9[bd]);
      H3_1354[bd]->SetBarWidth(0.18);
      H3_1354[bd]->SetBarOffset(0.11+0.2*bd);
      H3_1354[bd]->SetLineColor(ColorArr9[bd+4]);
      H3_1354[bd]->SetMarkerStyle(0);
      H3_1354[bd]->Scale(1/3.);
      H3_1354[bd]->Sumw2();
      double x[16]={}, y[16]={}, ex[16]={}, ey[16]={};
      for(int ch=0;ch<16;ch++){
        x[ch]=  ch+0.2+0.2*bd;
        y[ch]=  H3_1354[bd]->GetBinContent(ch+1);
        ey[ch]= H3_1354[bd]->GetBinError(ch+1);
        // cout<<x[ch]<<"  "<<H3_1354[bd]->GetBinContent(ch+1)<<"  "<<ex[ch]<<"  "<<H3_1354[bd]->GetBinError(ch+1)<<"  "<<endl;
      }
      //cout<<H1354E[bd]->GetBinContent(1);
      TGraphErrors *EFFErr = new TGraphErrors(16,x,y,ex,ey);
      EFFErr->Draw("esame");
      EFFErr->SetLineColor(ColorArr9[bd+4]);
      EFFErr->SetLineStyle(1);
      EFFErr->SetMarkerStyle(104);
      EFFErr->SetMarkerColor(ColorArr9[bd+4]);

    }
    H4_1354 ->SetMarkerStyle(4);
    H4_1354 ->SetMarkerSize(0.5);
    H4_1354 ->SetLineColor(1);
    H4_1354->Scale(1/4.);
    H4_1354->Sumw2();
    TH1F *hMod2 = (TH1F*) hMod->Clone();

    TH1F *H3_X354E  = (TH1F*) H3_X354->Clone();
    TH1F *H3_1X54E  = (TH1F*) H3_1X54->Clone();
    TH1F *H3_13X4E  = (TH1F*) H3_13X4->Clone();
    TH1F *H3_135XE  = (TH1F*) H3_135X->Clone();
    TH1F *H4_1354E1 = (TH1F*) H4_1354->Clone();
    TH1F *H4_1354E3 = (TH1F*) H4_1354->Clone();
    TH1F *H4_1354E5 = (TH1F*) H4_1354->Clone();
    TH1F *H4_1354E4 = (TH1F*) H4_1354->Clone();
    TH1F *H4_1354E  = (TH1F*) H4_1354->Clone();
    
    H3_X354E->Add(H4_1354E);
    H3_1X54E->Add(H4_1354E);
    H3_13X4E->Add(H4_1354E);
    H3_135XE->Add(H4_1354E);
    H4_1354E1->Divide(H3_X354E);
    H4_1354E3->Divide(H3_1X54E);
    H4_1354E5->Divide(H3_13X4E);
    H4_1354E4->Divide(H3_135XE);

    TH1F  *H1354E[4]={H4_1354E1,H4_1354E3,H4_1354E5,H4_1354E4};

    ofstream out(Form("%sCheff.txt",DirRes_ChEff));
    out<<"ch\t=\"1111\"\t=\"0111\"\t=\"1011\"\t=\"1101\"\t=\"1110\"\tEff.B1\tEff.B3\tEff.B5\tEff.B4"<<endl;
    for(int i=0;i<16;i++){
      hMod2->Fill(i);
      out<<"ch"<<i<<"\t";
      out<<H4_1354->GetBinContent(i+1)<<"\t";
      out<<H3_X354->GetBinContent(i+1)<<"\t";
      out<<H3_1X54->GetBinContent(i+1)<<"\t";
      out<<H3_13X4->GetBinContent(i+1)<<"\t";
      out<<H3_135X->GetBinContent(i+1)<<"\t";
      out<<H4_1354E1->GetBinContent(i+1)<<"\t";
      out<<H4_1354E3->GetBinContent(i+1)<<"\t";
      out<<H4_1354E5->GetBinContent(i+1)<<"\t";
      if(i==15) out<<H4_1354E4->GetBinContent(i+1);
      else out<<H4_1354E4->GetBinContent(i+1)<<endl;
      hMod->GetXaxis()->SetBinLabel(i+1,Form("ch.%d",i));
      hMod2->GetXaxis()->SetBinLabel(i+1,Form("ch.%d",i));
    }

    ofstream out1(Form("%sCheff.csv",DirRes_ChEff));

    cevent->cd(2);
    hMod2->GetYaxis()->SetRangeUser(0.1,1.3);
    hMod2->Draw("l");
    pwdVSddt1->Draw();
    hMod2->GetYaxis()->SetTitle("Channel Effciency");
    hMod2->SetLineStyle(2);
    //TGraphErrors *EFFErr[4]={};
    for(int bd=0;bd<4;bd++){
      double x[16]={}, y[16]={};
      double ex[16]={}, eyl[16]={}, eyh[16]={};
      for(int ch=0;ch<16;ch++){
        x[ch]=   ch+0.2+0.2*bd;
        y[ch]=   H1354E[bd]->GetBinContent(ch+1);
        eyl[ch]= H1354E[bd]->GetBinError(ch+1);
        eyh[ch]= H1354E[bd]->GetBinError(ch+1);
        if(-eyl[ch]+y[ch]<=0) eyl[ch]=y[ch];
        else if(eyh[ch]+y[ch]>=1) eyh[ch]=1-y[ch];
        out1<<BD[bd]<<"\t"<<x[ch]<<"\t"<<y[ch]<<"\t"<<eyl[ch]<<"\t"<<eyh[ch]<<"\n";
        //      cout<<x[ch]<<"  "<<H1354E[bd]->GetBinContent(ch+1)<<"  "<<ex[ch]<<"  "<<H1354E[bd]->GetBinError(ch+1)<<"  "<<endl;
      }
      //cout<<H1354E[bd]->GetBinContent(1);
      H1354E[bd]->Draw("histbarSame");
      TGraphAsymmErrors *EFFErrAsy = new TGraphAsymmErrors(16,x,y,ex,ex,eyl,eyh);
      EFFErrAsy->Draw("esame");
      EFFErrAsy->SetLineColor(ColorArr9[bd+4]);
      EFFErrAsy->SetLineStyle(1);
      EFFErrAsy->SetMarkerStyle(104);
      EFFErrAsy->SetMarkerColor(ColorArr9[bd+4]);
      H1354E[bd]->SetFillColor(ColorArr9[bd]);
      H1354E[bd]->SetBarWidth(0.18);
      H1354E[bd]->SetBarOffset(0.11+0.2*bd);
      H1354E[bd]->SetLineColor(ColorArr9[bd+4]);
    }
    hMod2->Draw("lsame");

    cevent->Print(Form("%sCheff.pdf",DirRes_ChEff));
    cevent->Close();

    out.close();
    out1.close();
    
    system(Form("cp %sCheff.txt  %sCheff.txt",DirRes_ChEff,DirOperate));
    system(Form("cp %sCheff.csv  %sCheff.csv",DirRes_ChEff,DirOperate));
    //TGraphAsymmErrors *data_phoPt_efferr = new TGraphAsymmErrors();
    //data_phoPt_efferr->BayesDivide(H3_1354[0],H4_1354);
  }
}