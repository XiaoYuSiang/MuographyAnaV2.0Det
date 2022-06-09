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
#include <TGaxis.h>
#include <TText.h>
#include <TStyle.h>
#include <TPaveStats.h>
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



void VerticalTracksAna(const int indexi=28, const int indexf=29 ) {
  double eventGap[46]={};
  
  ofstream out(Form("%sSEVertical4v3.dat",DirOperate));
  for(int i0=indexi;i0<indexf;i0++){
    eventGap[i0] = (1.0*eventGapTcnt[i0])/2560000000.;
    char RFNStr[200];
    sprintf(RFNStr,"%d_%s_nHTH%dL%d.root",eventGapTcnt[i0],TimeStr,TriggerS,TriggerL);
    TreeReader data(Form("%sGapT%s",DirOperate,RFNStr));
    cout<<Form("%sGapT%s",DirOperate,RFNStr)<<" is opened"<<endl;
        
    TFile *rotfile = new TFile(Form("%sTracksGT%s",DirOperate,RFNStr),"RECREATE");
    cout<<Form("%sTracksGT%s",DirOperate,RFNStr)<<" is opened"<<endl;

    //new Tree unit and odl tree unit
    
    Int_t      TrackIndex=0;
    Int_t      frame_   ;
    Int_t      EvIndex_ ;
    Long64_t   unixtime_;
    Int_t      tYear_   ;
    Int_t      tMonth_  ;
    Int_t      tDate_   ;
    Int_t      tHour_   ;
    Int_t      tMinute_ ;
    Int_t      tSecond_ ;
    Int_t      nLayers_ ;
    Int_t      iHit_;
    Int_t      eventHit_;
    short      BiZ_;
    Int_t*     board_;
    Int_t*     channel_;
    Int_t*     pwidth_;
    Int_t*     pwidthScaleFactor_;
    Int_t*     pcnt_;
    Long64_t*  tcnt_;
    Double_t*  dtime_;
    vector<Int_t>    board;
    vector<Int_t>    channel;
    vector<Int_t>    iX;
    vector<Int_t>    iY;
    vector<Int_t>    iZ;
    vector<Int_t>    pwidth;
    vector<Int_t>    pwidthScaleFactor;
    vector<Int_t>    pcnt;
    vector<Long64_t> tcnt;
    vector<Double_t> dtime;
    //old Tree uint
    Int_t      nH   ;//total number for hit in a frame/event
    Int_t      nH0  ;//total number for hit in a frame/event
    Int_t      nH1  ;//total number for hit in a frame/event
    Int_t      nH2  ;//total number for hit in a frame/event
    Int_t      nH3  ;//total number for hit in a frame/event
    //Int_t    nHits     = data.GetInt("nHits");
    
    TTree *tree = new TTree("t","data from analyzing file");
    tree->Branch("TrackIndex",&TrackIndex);
    tree->Branch("frame",&frame_);
    tree->Branch("EvIndex",&EvIndex_);
    tree->Branch("unixtime",&unixtime_);
    tree->Branch("tYear",&tYear_);
    tree->Branch("tMonth",&tMonth_);
    tree->Branch("tDate",&tDate_);
    tree->Branch("tHour",&tHour_);
    tree->Branch("tMinute",&tMinute_);
    tree->Branch("tSecond",&tSecond_);
    tree->Branch("nLayers",&nLayers_);
    // tree->Branch("pcnt",&pcnt);
    tree->Branch("BiZ",&BiZ_);
    tree->Branch("board",&board);
    tree->Branch("channel",&channel);
    tree->Branch("eventHit",&eventHit_);
    tree->Branch("iHit",&iHit_);
    tree->Branch("iX",&iX);
    tree->Branch("iY",&iY);
    tree->Branch("iZ",&iZ);
    tree->Branch("pwidth",&pwidth);
    tree->Branch("pwidthScaleFactor",&pwidthScaleFactor);
    tree->Branch("pcnt",&pcnt);
    tree->Branch("tcnt",&tcnt);
    tree->Branch("dtime",&dtime);
    
    //take time and set anlyze Constant for boundry condition
    Long64_t evsVT = data.GetEntriesFast();
    cout <<"total event:\t"<<evsVT<<endl;
    data.GetEntry(0);
    //OOL=3;//@@

    //data hist create
    TH1F *hMod         = new TH1F("hMod","",ndivise,FirDaySec,FinDaySec);
    TH1F *hModG        = new TH1F("hModG","",ndivise,FirDaySec,FinDaySec);
    TH1F *hN1XX4       = new TH1F("B=1XX4","",16,0,16);
    TH1F *hNX35X       = new TH1F("B=X35X","",16,0,16);
    TH1F *hN13X4       = new TH1F("B=13X4","",16,0,16);
    TH1F *hN1X54       = new TH1F("B=1X54","",16,0,16);
    TH1F *hNX354       = new TH1F("B=X354","",16,0,16);
    TH1F *hN135X       = new TH1F("B=135X","",16,0,16);
    TH1F *hN1354       = new TH1F("B=hN1354","",16,0,16);
    TH1F *hN1354One    = new TH1F("B=hN1354One","",16,0,16);
    TH2F *hNHit2D      = new TH2F("hNHit2D","",16,0,16,5,1,6);
    for(int i4=0;i4<16;i4++){
      hN1354One->Fill(i4);
    }
    int ct3=0 ,ct4 = 0;
    int N3 = 0, N2 = 0;
    //Fill the data
    for (Long64_t ev = 0; ev <evsVT; ++ev) {//evsVT; ++ev) {
      data.GetEntry(ev);
      frame_    = data.GetInt("frame");
      EvIndex_  = data.GetInt("EvIndex");
      unixtime_ = data.GetLong64("unixtime");
      if(ev%1000 == 0) cout<<Form("\r%.5f%%\t%d",(ev*100.)/(1.*evsVT),unixtime_)<<flush;
      tYear_    = data.GetInt("tYear");
      tMonth_   = data.GetInt("tMonth");
      tDate_    = data.GetInt("tDate");
      tHour_    = data.GetInt("tHour");
      tMinute_  = data.GetInt("tMinute");
      tSecond_  = data.GetInt("tSecond");
      nLayers_  = data.GetInt("nLayers");
      nH        = data.GetInt("nH");
      eventHit_ = nH;
      board_    = data.GetPtrInt("board");
      channel_  = data.GetPtrInt("channel");
      pwidth_   = data.GetPtrInt("pwidth");
      pwidthScaleFactor_   = data.GetPtrInt("pwidthScaleFactor");
      pcnt_     = data.GetPtrInt("pcnt");
      tcnt_     = data.GetPtrLong64("tcnt");
      dtime_    = data.GetPtrDouble("dtime");
      
      if(nH<2) continue;
      if(nH>62) continue;//@@
      Long64_t BoolnH = 0;
      short BoolnB = 0;
      for(int i2=0 ; i2<nH ; i2++){
        if ((BoolnH>>i2)&1) continue;
        int hitCHA = channel_[i2];
        int hitBDA = board_[i2];
        double hitdTC = dtime_[i2];
        board.push_back(board_[i2]);         //if(board[i3] ==hitBDA) continue;
        channel.push_back(channel_[i2]);
        iX.push_back(channel_[i2]%4);         //if(board[i2] ==hitBDA) continue;
        iY.push_back(channel_[i2]/4);         //if(board[i2] ==hitBDA) continue;
        iZ.push_back(BDcheck(board_[i2]));
        pwidth.push_back(pwidth_[i2]);
        pwidthScaleFactor.push_back(pwidthScaleFactor_[i2]);
        //if(pwidthScaleFactor_[i2]==2) cout<<pwidthScaleFactor_[i2]<<endl;
        pcnt.push_back(pcnt_[i2]);
        tcnt.push_back(tcnt_[i2]);
        dtime.push_back(dtime_[i2]);
        BoolnH |= 1UL << i2;
        BoolnB |= 1UL << BDcheck(board_[i2]);
        for(int i3 = i2+1 ; i3<nH ; i3++){
          //if (fabs(dtime_[i3]-hitdTC)>3*eventGap[i0]) break;
          if ((BoolnH>>i3)&1) continue;
          if ((BoolnB>>BDcheck(board_[i3]))&1) continue;
          if ( channel_[i3]==hitCHA ){
            BoolnH |= 1UL << i3;
            BoolnB |= 1UL << BDcheck(board_[i3]);
            board.push_back(board_[i3]);         //if(board[i3] ==hitBDA) continue;
            channel.push_back(channel_[i3]);
            iX.push_back(channel_[i3]%4);         //if(board[i3] ==hitBDA) continue;
            iY.push_back(channel_[i3]/4);         //if(board[i3] ==hitBDA) continue;
            iZ.push_back(BDcheck(board_[i3]));
            pwidth.push_back(pwidth_[i3]);
            pwidthScaleFactor.push_back(pwidthScaleFactor_[i3]);
            pcnt.push_back(pcnt_[i3]);
            tcnt.push_back(tcnt_[i3]);
            dtime.push_back(dtime_[i3]);
            BiZ_ = BoolnB;
          }
        }
        //for(int i2=0 ; i2<16 ; i2++){
        if((BiZ_==9||BiZ_==11||BiZ_==13||BiZ_==15 )
          ||(BiZ_==6||BiZ_==7||BiZ_==14 )){
          TrackIndex ++;
          iHit_=2;
          hN1XX4->Fill(hitCHA);
          if(BiZ_==15){
            hN1354->Fill(hitCHA);
            hN1X54->Fill(hitCHA);
            hN13X4->Fill(hitCHA);
            hN1XX4->Fill(hitCHA);
            hNX354->Fill(hitCHA);
            hN135X->Fill(hitCHA);
            hNX35X->Fill(hitCHA);
            iHit_=4;
          }else if(BiZ_==13){
            hN13X4->Fill(hitCHA);
            hN1XX4->Fill(hitCHA);
            iHit_=3;
          }else if(BiZ_==11){
            hN1X54->Fill(hitCHA);
            hN1XX4->Fill(hitCHA);
            iHit_=3;
          }else if(BiZ_==7){
            hNX354->Fill(hitCHA);
            iHit_=3;
          }else if(BiZ_==14){
            hN135X->Fill(hitCHA);
            iHit_=3;
          }else if(BiZ_==6){
            hNX35X->Fill(hitCHA);
            iHit_=2;
          }else if(BiZ_==9){
            hN1XX4->Fill(hitCHA);
            iHit_=2;
          }




          if(iHit_!=2&&false){//@@

            for(int i6=0;i6<iHit_;i6++) cout<<iZ[i6];
            cout<<"\n";
          }
          //cout<<"\n\nthe throw\n\n";
          //throw;
          
          tree->Fill();
          //cout<<"aaaaa"<<endl;
        }
        //}
        BoolnB =0;
        BiZ_   =0;
        board.clear();
        channel.clear();
        pwidth.clear();
        pwidthScaleFactor.clear();
        pcnt.clear();
        iX.clear();
        iY.clear();
        iZ.clear();
        tcnt.clear();
        dtime.clear();
        iHit_ = 0;

      }
    }

    rotfile->Write();
    cout<<"\r"<<"Finished Ntuple write"<<endl;
    TCanvas *c1 = new TCanvas("c1","",1800,1200);
    c1->Divide(3,2);
    
    c1->cd(1);
    hN13X4->Draw("box");
    hN1354->Draw("boxsame");
    c1->cd(2);
    hN1X54->Draw("box");
    hN1354->Draw("boxsame");
    c1->cd(4);
    hN135X->Draw("box");
    hN1354->Draw("boxsame");
    c1->cd(5);
    hNX354->Draw("box");
    hN1354->Draw("boxsame");
    TH1F *hNX[7]={
      hN13X4,hN1X54,hN135X,hNX354
    };
    hN13X4->SetLineColor(1);
    hN1X54->SetLineColor(2);
    hN135X->SetLineColor(1);
    hNX354->SetLineColor(2);
    hN1354->SetLineColor(3);
    hN1354One->SetLineColor(1);
    hN1354One->SetLineStyle(2);
    hN1354One->GetXaxis()->SetTitle("Channel");
    hN1354One->GetYaxis()->SetTitle("Effciency");
    hN1354One->GetYaxis()->SetRangeUser(0,1.2);
    for(int i=0;i<4;i++){
      hNX[i]->GetXaxis()->SetTitle("Channel");
      hNX[i]->GetYaxis()->SetTitle("Number of Event");
      hNX[i]->GetYaxis()->SetRangeUser(0,700);
    }
    TH1F *hNEff3 = (TH1F*) hN1354->Clone();
    hNEff3 ->SetTitle("Efficiency: MTB3");
    hNEff3 ->Divide(hN1X54);
    TH1F *hNEff5 = (TH1F*) hN1354->Clone();
    hNEff5 ->SetTitle("Efficiency: MTB5");
    hNEff5 ->Divide(hN13X4);
    TH1F *hNEff4 = (TH1F*) hN1354->Clone();
    hNEff4 ->SetTitle("Efficiency: MTB3");
    hNEff4 ->Divide(hNX354);
    TH1F *hNEff1 = (TH1F*) hN1354->Clone();
    hNEff1 ->SetTitle("Efficiency: MTB5");
    hNEff1 ->Divide(hN135X);
    
    hNEff3 ->SetStats(0);
    hNEff5 ->SetStats(0);
    hNEff4 ->SetStats(0);
    hNEff1 ->SetStats(0);
    hNEff3->SetLineColor(1);
    hNEff5->SetLineColor(2);
    hNEff4->SetLineColor(1);
    hNEff1->SetLineColor(2);
    c1->cd(3);
    hN1354One->Draw("box");
    hNEff3->Draw("boxsame");
    hNEff5->Draw("boxsame");
    c1->cd(6);
    hN1354One->Draw("box");
    hNEff4->Draw("boxsame");
    hNEff1->Draw("boxsame");
    
    c1->Print(Form("%sBoardChannelEffective/EFF_2Track%d.png",DirResult,eventGapTcnt[i0]));
    c1->Print(Form("%sBoardChannelEffective/EFF_2Track%d.pdf",DirResult,eventGapTcnt[i0]));
    int num1XX4=0,num13X4=0,num1X54=0,num1354=0;  
    int numX35X=0,num135X=0,numX354=0;  
    for(int i2=0;i2<16;i2++){
      num1XX4+=hN1XX4->GetBinContent(i2+1);
      num13X4+=hN13X4->GetBinContent(i2+1);
      num1X54+=hN1X54->GetBinContent(i2+1);
      num1354+=hN1354->GetBinContent(i2+1);
      numX35X+=hNX35X->GetBinContent(i2+1);
      numX354+=hNX354->GetBinContent(i2+1);
      num135X+=hN135X->GetBinContent(i2+1);
    }
    cout<<eventGapTcnt[i0]<<"\t";
    cout<<num1XX4<<"\t";
    cout<<num13X4<<"\t";
    cout<<num1X54<<"\t";
    cout<<num1354<<"\t";
    cout<<numX35X<<"\t";
    cout<<numX354<<"\t";
    cout<<num135X<<"\n";
    
    rotfile->Close();
  

  }
}
