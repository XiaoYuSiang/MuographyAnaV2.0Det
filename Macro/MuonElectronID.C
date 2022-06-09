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
#include <THStack.h>
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

void MuonElectronID( const bool*OperMode, const int indexi=28, const int indexf=29 ) {
  bool testMode     = OperMode[0];
  bool rootFileMode = OperMode[4];
  setTDRStyle();
  double eventGap[46]={};
  char TestMarker[10]={};
  if(testMode) sprintf(TestMarker,Form("%s","TST_"));
  char DirResultPwVCase[150]={};//save path graph of Pwidth Rate
  sprintf(DirResultPwVCase,Form("%s%sBoardPwVCase/",DirResult,TestMarker));
  if (system(Form("mkdir -p %s",DirResultPwVCase))!=0) system(Form("mkdir -p %s",DirResultPwVCase));


  for(int i0=indexi;i0<indexf;i0++){
    eventGap[i0] = (1.0*eventGapTcnt[i0])/2560000000.;
    TFile *rotfile ;
    char RFNStr[200];
    sprintf(RFNStr,"%d_%s_nHTH%dL%d.root",eventGapTcnt[i0],TimeStr,TriggerS,TriggerL);
    cout<<Form("%sGapT%s",DirOperate,RFNStr)<<endl; //@@@ name by DSLAna
    TreeReader data(Form("%sGapT%s",DirOperate,RFNStr));
    cout<<Form("%sGapT%s",DirOperate,RFNStr)<<" is opened"<<endl;
    if(rootFileMode){
      rotfile = new TFile(Form("%s%sEventsGT%s",DirOperate,TestMarker,RFNStr),"RECREATE");
    }else{
      rotfile =     TFile::Open(Form("%s%sEventsGT%s",DirOperate,TestMarker,RFNStr));
    }
    cout<<Form("%s%sEventsGT%s",DirOperate,TestMarker,RFNStr)<<" is opened"<<endl;
    

    if(rootFileMode){
      
      //Data Variable
      //new Tree unit and odl tree unit
      
      Int_t      TrackIndex=0;
      Int_t      nLayers_ ;
      Int_t      iHit_;
      Int_t      eventHit_;
      bool       ElectronID=false;
      bool       MuonID=false;
      bool       NoiseID=false;
      short      IDtight=0;
      Int_t*     board_;
      Int_t*     channel_;
      Int_t*     pwidth_;
      // Int_t*     pcnt_;
      // Long64_t*  tcnt_;
      // Double_t*  dtime_;
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
      
      tree->Branch("nLayers",&nLayers_);
      // tree->Branch("pcnt",&pcnt);//@@
      tree->Branch("ElectronID",&ElectronID);
      tree->Branch("MuonID"   ,&MuonID);
      tree->Branch("NoiseID"  ,&NoiseID);
      tree->Branch("IDtight"  ,&IDtight);
      tree->Branch("board",&board);
      tree->Branch("pwidthScaleFactor",&pwidthScaleFactor);
      tree->Branch("channel",&channel);
      tree->Branch("eventHit",&eventHit_);
      tree->Branch("iHit",&iHit_);
      if(testMode!=true){
        tree->Branch("iX",&iX);
        tree->Branch("iY",&iY);
        tree->Branch("iZ",&iZ);
        //tree->Branch("pcnt",&pcnt);
        //tree->Branch("tcnt",&tcnt);
        //tree->Branch("dtime",&dtime);
      }
      tree->Branch("pwidth",&pwidth);
      
      tree->Branch("nH",&nH);
      tree->Branch("nH0",&nH0);
      tree->Branch("nH1",&nH1);
      tree->Branch("nH2",&nH2);
      tree->Branch("nH3",&nH3);
      
      //take time and set anlyze Constant for boundry condition
      Long64_t evsID = data.GetEntriesFast();
      cout <<"total event:\t"<<evsID<<endl;
      data.GetEntry(0);
      //DC.OOL=3;//@@

      //data hist create
      int ct3=0 ,ct4 = 0;
      int N3 = 0, N2 = 0;
      if(testMode) evsID = 100001;
      cout<<"evsID     "<<evsID<<endl;
      //Fill the data
      for (Long64_t ev = 0; ev <evsID; ++ev) {//evsID; ++ev) {
        data.GetEntry(ev);
        if(ev%1000 == 0) cout<<Form("\r%.5f%%",(ev*100.)/(1.*evsID))<<flush;
        
        nLayers_  = data.GetInt("nLayers");
        nH        = data.GetInt("nH");
        nH0       = data.GetInt("nH0");
        nH1       = data.GetInt("nH1");
        nH2       = data.GetInt("nH2");
        nH3       = data.GetInt("nH3");
        eventHit_ = nH;
        iHit_     = nH;
        pwidth_   = data.GetPtrInt("pwidth");
        board_    = data.GetPtrInt("board");
        if(testMode!=true){
          channel_  = data.GetPtrInt("channel");
        }
        //pcnt_     = data.GetPtrInt("pcnt");
        // tcnt_     = data.GetPtrLong64("tcnt");
        // dtime_    = data.GetPtrDouble("dtime");
        //cout<<"outLayer"<<endl;
        if(nLayers_==4){
          if(nH==4){
            MuonID =true;
            IDtight =5;
          }else if(nH==5){
            MuonID =true;
            IDtight =4;
          }else if(nH==6){
            MuonID =true;
            IDtight =3;
          }else if(nH==7){
            MuonID =true;
            IDtight =2;
          }
          if(nH>4){
            ElectronID =true;
            IDtight =2;
            if(IDtight==0) IDtight =1;
          }
          
        }else if(nLayers_==3){
          if(nH==3){
            MuonID =true;
            IDtight =4;
          }else if(nH==4){
            MuonID =true;
            IDtight =3;
          }else if(nH==5){
            MuonID =true;
            IDtight =2;
          }
          if(nH>3){
            ElectronID =true;
            if(IDtight==0) IDtight =1;
          }
        }else{
          NoiseID=true;
        }  //noise
        //cout<<"NoiseID"<<endl;
        for(int i2=0 ; i2<nH ; i2++){
          if(testMode!=true){
            board.push_back(board_[i2]);         //if(board[i3] ==hitBDA) continue;
            channel.push_back(channel_[i2]);
            iX.push_back(channel_[i2]%4);         //if(board[i2] ==hitBDA) continue;
            iY.push_back(channel_[i2]/4);         //if(board[i2] ==hitBDA) continue;
            iZ.push_back(BDcheck(board_[i2]));
          }
          pwidth.push_back(pwidth_[i2]);
          pwidthScaleFactor.push_back(BDPwWei[BDcheck(board_[i2])]);
          
        }
         // cout<<ev<<"!"<<endl;
        //cout<<"push_back"<<endl;
        tree->Fill();
        MuonID =false;
        ElectronID=false;
        NoiseID =false;
        IDtight =0;
        pwidth.clear();
        pwidthScaleFactor.clear();
        if(testMode!=true){
          board.clear();
          channel.clear();
          //pcnt.clear();
          iX.clear();
          iY.clear();
          iZ.clear();
        }
        // tcnt.clear();
        // dtime.clear();
        //cout<<"clear"<<endl;
      }
    }
    rotfile->Write();
    cout<<"\r"<<"Finished Ntuple write"<<endl;
    rotfile->Close();
    TFile *rotfile_0 = TFile::Open(Form("%s%sEventsGT%s",DirOperate,TestMarker,RFNStr));
    //cout<<"No ERR  new TH1F"<<endl;
    TTree *tf = (TTree*) rotfile_0 ->Get("t");
    TH1F *hPwidth       = new TH1F("hPwidth"      ,"hPwidth"      ,180,0,180);
    TH1F *hPwidthMuEle  = new TH1F("hPwidthMuEle" ,"hPwidthMuEle" ,180,0,180);
    TH1F *hPwidthNoiID  = new TH1F("hPwidthNoiID" ,"hPwidthNoiID" ,180,0,180);
    TH1F *hPwidthNoiID0 = new TH1F("hPwidthNoiID0","hPwidthNoiID0",180,0,180);
    TH1F *hPwidthNoiID1 = new TH1F("hPwidthNoiID1","hPwidthNoiID1",180,0,180);
    TH1F *hPwidthNoiID2 = new TH1F("hPwidthNoiID2","hPwidthNoiID2",180,0,180);
    TH1F *hPwidthNoiID3 = new TH1F("hPwidthNoiID3","hPwidthNoiID3",180,0,180);
    TH1F *hPwidthMuoID5 = new TH1F("hPwidthMuoID5","hPwidthMuoID5",180,0,180);
    TH1F *hPwidthMuoID4 = new TH1F("hPwidthMuoID4","hPwidthMuoID4",180,0,180);
    TH1F *hPwidthMuoID3 = new TH1F("hPwidthMuoID3","hPwidthMuoID3",180,0,180);
    TH1F *hPwidthMuoID2 = new TH1F("hPwidthMuoID2","hPwidthMuoID2",180,0,180);
    TH1F *hPwidthEleID  = new TH1F("hPwidthEleID" ,"hPwidthEleID" ,180,0,180);
    TH1F *hPwidthCases[11] = {hPwidth,hPwidthNoiID,hPwidthNoiID0,hPwidthNoiID1,hPwidthNoiID2,hPwidthNoiID3,hPwidthMuoID5,hPwidthMuoID4,hPwidthMuoID3,hPwidthMuoID2,hPwidthEleID};
    for(int i=0;i<11;i++){
      hPwidthCases[i]->SetLineColor(colorArr[i]);
    }
    THStack *hsPwidthVCaseWN = new THStack("hsPwidthVCaseWN","");//histrogramStachForNormalScale
    THStack *hsPwidthVCaseNN = new THStack("hsPwidthVCaseNN","");//histrogramStachForNormalScale
    THStack *hsPwidthVCase[2]={hsPwidthVCaseWN,hsPwidthVCaseNN};
    for(int inn=0;inn<2;inn++){
      hsPwidthVCase[inn]->Add(hPwidthMuoID5);
      hsPwidthVCase[inn]->Add(hPwidthMuoID4);
      hsPwidthVCase[inn]->Add(hPwidthMuoID3);
      hsPwidthVCase[inn]->Add(hPwidthMuoID2);
      hsPwidthVCase[inn]->Add(hPwidthEleID);
      if (inn==0){
        hsPwidthVCase[inn]->Add(hPwidthNoiID3);
        hsPwidthVCase[inn]->Add(hPwidthNoiID2);
        hsPwidthVCase[inn]->Add(hPwidthNoiID1);
        hsPwidthVCase[inn]->Add(hPwidthNoiID0);
      }
    }
    TLegend *LpwWN = new TLegend(0.60,0.66,0.78,0.90);
    TLegend *LpwNN = new TLegend(0.78,0.70,0.96,0.90);
    char WNNN[2][3]={"WN","NN"};
    setLegendDefault(LpwWN,1);
    setLegendDefault(LpwNN,1);
    LpwWN->AddEntry(hPwidth      ,"All hit"              ,"f");   
    LpwWN->AddEntry(hPwidthNoiID ,"Noise hit"            ,"f");   
    LpwWN->AddEntry(hPwidthNoiID0,"Noise hit(nH=1)"      ,"f");   
    LpwWN->AddEntry(hPwidthNoiID1,"Noise hit(nH=2)"      ,"f");   
    LpwWN->AddEntry(hPwidthNoiID2,"Noise hit(nH=3)"      ,"f");   
    LpwWN->AddEntry(hPwidthNoiID3,"Noise hit(nH>3)"      ,"f");   
    //cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
    LpwNN->AddEntry(hPwidthMuoID5,"Muon hit(4M+0N)"      ,"f");   
    LpwNN->AddEntry(hPwidthMuoID4,"Muon hit(4M+1N|3M)"   ,"f");   
    LpwNN->AddEntry(hPwidthMuoID3,"Muon hit(4M+2N|3M+1N)","f");   
    LpwNN->AddEntry(hPwidthMuoID2,"Muon hit(4M+3N|3M+2N)","f");   
    LpwNN->AddEntry(hPwidthEleID ,"Electron hit(>4)"     ,"f");   
    //cout<<"No ERR TCanvas"<<endl;
    TCanvas *cpwvcase = new TCanvas("cpwvcase","",3840,2160*2);
    cpwvcase->Divide(1,2);
    cpwvcase->cd(2)->SetLogy();

    // fill histrogram
    tf->Draw("pwidth/pwidthScaleFactor>>hPwidth","","hist");
    cout<<"Finish Drawing pwidth All Event "<<endl;
    tf->Draw("pwidth/pwidthScaleFactor>>hPwidthNoiID","NoiseID","histsame");
    cout<<"Finish Drawing pwidth All hPwidthNoiID "<<endl;
    tf->Draw("pwidth/pwidthScaleFactor>>hPwidthNoiID0","NoiseID&&nH==1","histsame");
    cout<<"Finish Drawing pwidth All hPwidthNoiID nH=1"<<endl;
    tf->Draw("pwidth/pwidthScaleFactor>>hPwidthNoiID1","NoiseID&&nH==2","histsame");
    cout<<"Finish Drawing pwidth All hPwidthNoiID nH=2"<<endl;
    tf->Draw("pwidth/pwidthScaleFactor>>hPwidthNoiID2","NoiseID&&nH==3","histsame");
    cout<<"Finish Drawing pwidth All hPwidthNoiID nH=3"<<endl;
    tf->Draw("pwidth/pwidthScaleFactor>>hPwidthNoiID3","NoiseID&&nH>3","histsame");
    cout<<"Finish Drawing pwidth All hPwidthNoiID nH>3"<<endl;
    tf->Draw("pwidth/pwidthScaleFactor>>hPwidthMuoID5","MuonID&&IDtight==5","histsame");
    cout<<"Finish Drawing pwidth All MuonID IDtight==5"<<endl;
    tf->Draw("pwidth/pwidthScaleFactor>>hPwidthMuoID4","MuonID&&IDtight==4","histsame");
    cout<<"Finish Drawing pwidth All MuonID IDtight==4"<<endl;
    tf->Draw("pwidth/pwidthScaleFactor>>hPwidthMuoID3","MuonID&&IDtight==3","histsame");
    cout<<"Finish Drawing pwidth All MuonID IDtight==3"<<endl;
    tf->Draw("pwidth/pwidthScaleFactor>>hPwidthMuoID2","MuonID&&IDtight==2","histsame");
    cout<<"Finish Drawing pwidth All MuonID IDtight==2"<<endl;
    tf->Draw("pwidth/pwidthScaleFactor>>hPwidthEleID","ElectronID","histsame");
    cout<<"Finish Drawing pwidth All ElectronID"<<endl;
    Long64_t Num_hPwidthCases[11] ={};
    TH1F *hPwidthCasesNor[11] = {};
    ofstream out(Form("%sCaseNumber_TG%d.txt",DirOperate,eventGapTcnt[i0]));

    for(int i=0;i<11;i++){
      hPwidthCases[i]->SetTitle("");
      hPwidthCases[i]->GetXaxis()->SetTitle("Scale Normalized pwidth(100ns)");
      hPwidthCases[i]->GetYaxis()->SetTitle("Hits Number");
      hPwidthCases[i]->GetXaxis()->SetRangeUser(-10,90);
      hPwidthCases[i]->SetStats(0);
    }
    double BinMaxD[2] = {};
    int    BinMaxI[2] = {};
    
    for(int inn=0;inn<2;inn++){
      double binmaxtem = 0;
      //cout<<"----INN----"<<inn<<endl;
      for(int i=0+inn*6;i<11;i++){
        if (inn==1)  hPwidthMuEle->Add(hPwidthCases[i]);
        Num_hPwidthCases[i]=hPwidthCases[i]->Integral(-1,-1);
        binmaxtem = hPwidthCases[i]->GetBinContent(hPwidthCases[i]->GetMaximumBin());
        if(binmaxtem>BinMaxI[inn]) BinMaxI[inn]= binmaxtem;
        // cout<<binmaxtem<<endl;
        out<<hPwidthCases[i]->GetTitle()<<"\t"<<Num_hPwidthCases[i]<<endl;
        hPwidthCasesNor[i] = (TH1F*) hPwidthCases[i]->Clone();
        hPwidthCasesNor[i] ->Scale(1./Num_hPwidthCases[i]);
        hPwidthCasesNor[i] ->SetTitle(Form("%sNor",hPwidthCases[i]->GetTitle()));
        hPwidthCasesNor[i] ->SetName(Form("%sNor",hPwidthCases[i]->GetName()));
        binmaxtem = hPwidthCasesNor[i]->GetBinContent(hPwidthCasesNor[i]->GetMaximumBin());
        // cout<<binmaxtem<<endl;
        if(binmaxtem>BinMaxD[inn]) BinMaxD[inn]= binmaxtem;
      }
      if(inn==1){
        binmaxtem = hPwidthMuEle->GetBinContent(hPwidthMuEle->GetMaximumBin());
        // cout<<"         " <<binmaxtem<<endl;
        if(binmaxtem>BinMaxI[1]) BinMaxI[1]= binmaxtem;
        // cout<<"BinMaxI"<<"\t"<<"BinMaxD"<<"\t"<<BinMaxI[inn]<<"\t"<<BinMaxD[inn]<<endl;
        for(int i=0+inn*6;i<11;i++){
          hPwidthCasesNor[i]->GetYaxis()->SetRangeUser(1,BinMaxD[inn]*1.2);
          hPwidthCases   [i]->GetYaxis()->SetRangeUser(1,binmaxtem*1.2);
        }
      }
      //UnStack histrogram for unLog and Log
      for(int i=1;i<3;i++){
        cpwvcase->cd(i);
        hPwidthCases[0+inn*6]->Draw("hist");
        
        for(int i1=1+inn*6;i1<11;i1++) hPwidthCases[i1]->Draw("histsame");
        hPwidthCases[0+inn*6]->Draw("histsame");
        if(inn==0) LpwWN->Draw("same");
        LpwNN->Draw("same");
      }
      cpwvcase->Print(Form("%sPwidthVSCaseTG%d%s.pdf",DirResultPwVCase,eventGapTcnt[i0],WNNN[inn]));
    }

    for(int inn=0;inn<2;inn++){
      for(int i=1;i<3;i++){
        cpwvcase->cd(i);
        hPwidthCasesNor[inn*6]->Draw("hist");
        for(int i1=1+inn*6;i1<11;i1++) hPwidthCasesNor[i1]->Draw("histsame");
        hPwidthCasesNor[inn*6]->Draw("histsame");
        if(inn==0) LpwWN->Draw("same");
        LpwNN->Draw("same");
        hPwidthCasesNor[inn*6]->GetXaxis()->SetRangeUser(0,60);
        hPwidthCasesNor[inn*6]->GetYaxis()->SetRangeUser(0,0.15);
        hPwidthCasesNor[inn*6]->GetYaxis()->SetTitle("Hits Number Ratio");
      }
      cpwvcase->Print(Form("%sPwidthVSCaseTG%d%sNor.pdf",DirResultPwVCase,eventGapTcnt[i0],WNNN[inn]));
    }
    for(int inn=0;inn<2;inn++){
      //Stack histrogram for unLog and Log
      for(int i=1;i<3;i++){
        //cout<<"----hPwidthCases[0+inn*6]->Draw(hist);----"<<"Fin"<<endl;
        cpwvcase->cd(i);
        hPwidthCases[0+inn*6]->Draw("hist");
        hsPwidthVCase[inn]->Draw("same");
        for(int i1=1;i1<11;i1++){
          hPwidthCases[i1]->SetFillColorAlpha(colorArr[i1],1);
        }
        if(inn==0) LpwWN->Draw("same");
        LpwNN->Draw("same");
        hPwidthCases[0+inn*6]->Draw("histsame");
      }
      //UnStack histrogram for unLog and Log Normalize
      cpwvcase->Print(Form("%sPwidthVSCaseTG%d%sStack.pdf",DirResultPwVCase,eventGapTcnt[i0],WNNN[inn]));
      //cout<<"----INN----"<<"Fin"<<endl;
    }
    /*
    for(int i1=1;i1<11;i1++){
      hPwidthCases[i1]->SetFillColor(1);
    }*/
    rotfile_0->Close();
    
    //rotfile->Close();
    
  }

}




void IDCaseNumAna( const bool*OperMode, const int indexi=28, const int indexf=29) {
  bool testMode     = OperMode[0];
  double eventGap[46]={};

  char TestMarker[10]={};
  if(testMode) sprintf(TestMarker,"%s","TST_");
  char DirResultPwVCase[150]={};//save path graph of Pwidth Rate
  sprintf(DirResultPwVCase,"%s%sBoardPwVCase/",DirResult,TestMarker);
  if (system(Form("mkdir -p %s",DirResultPwVCase))!=0) system(Form("mkdir -p %s",DirResultPwVCase));

  for(int i0=indexi;i0<indexf;i0++){
    eventGap[i0] = (1.0*eventGapTcnt[i0])/2560000000.;
    TFile *rotfile ;
    char RFNStr[200];
    sprintf(RFNStr,"%d_%s_nHTH%dL%d.root",eventGapTcnt[i0],TimeStr,TriggerS,TriggerL);
    // if(rootFileMode){
      // rotfile = new TFile(Form("%s%sEventsGT%d_Time20211223_20220106_NL%dNl%d.root",DirOperate,TestMarker,eventGapTcnt[i0],TriggerS,TriggerL),"RECREATE");
    // }else{
      rotfile =     TFile::Open(Form("%s%sEventsGT%s",DirOperate,TestMarker,RFNStr));
    // }
    cout<<Form("%s%sEventsGT%s",DirOperate,TestMarker,RFNStr)<<" is opened"<<endl;
    TTree *tf = (TTree*) rotfile ->Get("t");
    TH1F *hCaseID       = new TH1F("hCaseID"      ,"hCaseID"      ,65,0,65);
    TH1F *hCaseIDNoiID  = new TH1F("hCaseIDNoiID" ,"hCaseIDNoiID" ,65,0,65);
    TH1F *hCaseIDNoiID0 = new TH1F("hCaseIDNoiID0","hCaseIDNoiID0",65,0,65);
    TH1F *hCaseIDNoiID1 = new TH1F("hCaseIDNoiID1","hCaseIDNoiID1",65,0,65);
    TH1F *hCaseIDNoiID2 = new TH1F("hCaseIDNoiID2","hCaseIDNoiID2",65,0,65);
    TH1F *hCaseIDNoiID3 = new TH1F("hCaseIDNoiID3","hCaseIDNoiID3",65,0,65);
    TH1F *hCaseIDMuoID5 = new TH1F("hCaseIDMuoID5","hCaseIDMuoID5",65,0,65);
    TH1F *hCaseIDMuoID4 = new TH1F("hCaseIDMuoID4","hCaseIDMuoID4",65,0,65);
    TH1F *hCaseIDMuoID3 = new TH1F("hCaseIDMuoID3","hCaseIDMuoID3",65,0,65);
    TH1F *hCaseIDMuoID2 = new TH1F("hCaseIDMuoID2","hCaseIDMuoID2",65,0,65);
    TH1F *hCaseIDEleID  = new TH1F("hCaseIDEleID" ,"hCaseIDEleID" ,65,0,65);
    TH1F *hCaseIDCases[11] = {hCaseID,hCaseIDNoiID,hCaseIDNoiID0,hCaseIDNoiID1,hCaseIDNoiID2,hCaseIDNoiID3,hCaseIDMuoID5,hCaseIDMuoID4,hCaseIDMuoID3,hCaseIDMuoID2,hCaseIDEleID};
    for(int i=0;i<11;i++){
      hCaseIDCases[i]->SetLineColor(colorArr[i]);
    }
    TLegend *LCaseNum = new TLegend(0.6,0.65,0.96,0.9);
    LCaseNum->SetFillColor(0);
    LCaseNum->SetLineColor(0);
    LCaseNum->SetNColumns(2);
    LCaseNum->AddEntry(hCaseID      ,"All hit"              ,"f");   
    LCaseNum->AddEntry(hCaseIDNoiID ,"Noise hit"            ,"f");   
    LCaseNum->AddEntry(hCaseIDNoiID0,"Noise hit(nH=1)"      ,"f");   
    LCaseNum->AddEntry(hCaseIDNoiID1,"Noise hit(nH=2)"      ,"f");   
    LCaseNum->AddEntry(hCaseIDNoiID2,"Noise hit(nH=3)"      ,"f");   
    LCaseNum->AddEntry(hCaseIDNoiID3,"Noise hit(nH>3)"      ,"f");   
    LCaseNum->AddEntry(hCaseIDMuoID5,"Muon hit(4M+0N)"      ,"f");   
    LCaseNum->AddEntry(hCaseIDMuoID4,"Muon hit(4M+1N|3M)"   ,"f");   
    LCaseNum->AddEntry(hCaseIDMuoID3,"Muon hit(4M+2N|3M+1N)","f");   
    LCaseNum->AddEntry(hCaseIDMuoID2,"Muon hit(4M+3N|3M+2N)","f");   
    LCaseNum->AddEntry(hCaseIDEleID ,"Electron hit(>4)"     ,"f");   
    TCanvas *ccaseNum = new TCanvas("ccaseNum","",3840,2160*2);
    ccaseNum->Divide(1,2);
    ccaseNum->cd(2)->SetLogy();
    hCaseID->GetXaxis()->SetTitle("number of Hits");
    hCaseID->GetYaxis()->SetTitle("Hits Number");
    hCaseID->GetXaxis()->SetRangeUser(-10,90);
    // fill histrogram
    tf->Draw("iHit>>hCaseID"      ,""                        ,"hist");
    cout<<"Finish Drawing pwidth All Event "<<endl;
    tf->Draw("iHit>>hCaseIDNoiID" ,"(NoiseID)"           ,"histsame");
    cout<<"Finish Drawing pwidth All hCaseIDNoiID "<<endl;
    tf->Draw("iHit>>hCaseIDNoiID0","(NoiseID&&nH==1)"    ,"histsame");
    cout<<"Finish Drawing pwidth All hCaseIDNoiID nH=1"<<endl;
    tf->Draw("iHit>>hCaseIDNoiID1","(NoiseID&&nH==2)"    ,"histsame");
    cout<<"Finish Drawing pwidth All hCaseIDNoiID nH=2"<<endl;
    tf->Draw("iHit>>hCaseIDNoiID2","(NoiseID&&nH==3)"    ,"histsame");
    cout<<"Finish Drawing pwidth All hCaseIDNoiID nH=3"<<endl;
    tf->Draw("iHit>>hCaseIDNoiID3","(NoiseID&&nH>3)"     ,"histsame");
    cout<<"Finish Drawing pwidth All hCaseIDNoiID nH>3"<<endl;
    tf->Draw("iHit>>hCaseIDMuoID5","(MuonID&&IDtight==5)","histsame");
    cout<<"Finish Drawing pwidth All MuonID IDtight==5"<<endl;
    tf->Draw("iHit>>hCaseIDMuoID4","(MuonID&&IDtight==4)","histsame");
    cout<<"Finish Drawing pwidth All MuonID IDtight==4"<<endl;
    tf->Draw("iHit>>hCaseIDMuoID3","(MuonID&&IDtight==3)","histsame");
    cout<<"Finish Drawing pwidth All MuonID IDtight==3"<<endl;
    tf->Draw("iHit>>hCaseIDMuoID2","(MuonID&&IDtight==2)","histsame");
    cout<<"Finish Drawing pwidth All MuonID IDtight==2"<<endl;
    tf->Draw("iHit>>hCaseIDEleID","(ElectronID)"         ,"histsame");
    cout<<"Finish Drawing pwidth All ElectronID"<<endl;
    for(int i=1;i<3;i++){
      ccaseNum->cd(i);
      hCaseID->Draw("hist");
      for(int i1=1;i1<11;i1++) hCaseIDCases[i1]->Draw("histsame");
      hCaseID->Draw("histsame");
      LCaseNum->Draw();
    }
    ccaseNum->Print(Form("%sCaseiHitTG%d.pdf",DirResultPwVCase,eventGapTcnt[i0]));
        
    Long64_t Num_hPwidthCases[11] ={};

    for(int i=0;i<11;i++){
      Num_hPwidthCases[i]=hCaseIDCases[i]->Integral(-1,-1);
      cout<<hCaseIDCases[i]->GetTitle()<<"\t"<<Num_hPwidthCases[i]<<endl;
      hCaseIDCases[i]->Reset();
      hCaseIDCases[i]->SetBinContent(i+1,Num_hPwidthCases[i]);
      hCaseID->GetXaxis()->SetBinLabel(i+1,Form("%d",Num_hPwidthCases[i]));
    }
    hCaseID->GetXaxis()->SetRangeUser(0,11);
    hCaseID->GetXaxis()->SetLabelSize(0.04);
    TH1F *hCaseIDLG = (TH1F*) hCaseID->Clone();
    TH1F *hCaseIDA[2] = {hCaseID,hCaseIDLG};
    hCaseID  ->GetYaxis()->SetRangeUser(1,Num_hPwidthCases[0]*1.2);
    hCaseIDLG->GetYaxis()->SetRangeUser(1,Num_hPwidthCases[0]*20);
    hCaseIDLG->GetXaxis()->SetTitle("Case & total Number display");
    hCaseIDLG->GetYaxis()->SetTitle("Event Number");
    
    for(int i=1;i<3;i++){
      ccaseNum->cd(i);
      hCaseIDA[i-1]->Draw("hist");
      for(int i1=1;i1<11;i1++) hCaseIDCases[i1]->Draw("histsame");
      hCaseIDA[i-1]->Draw("histsame");
      LCaseNum->Draw();
    }

    ccaseNum->cd(2)->SetGrid(0,1);
    ccaseNum->Print(Form("%sCaseNumTG%d.pdf",DirResultPwVCase,eventGapTcnt[i0]));
    
  }
  
}
void GTCaseNumAna(const char* DirOperate,const char* DirResult){

  char DirResultPwVCase[150]={};//save path graph of Pwidth Rate
  sprintf(DirResultPwVCase,"%sBoardPwVCase/",DirResult);
  if (system(Form("mkdir -p %s",DirResultPwVCase))!=0) system(Form("mkdir -p %s",DirResultPwVCase));
  double caseNum[2][11]={
    {22568997,22247727,16777216,4752971,522656,194884,67255,214346,16638,12115,39669},
    {20666423,20451886,16777216,3188545,358205,127920,43858,140860,13028, 9262,29819},
  };
  TH1F *hCaseID       = new TH1F("hCaseID"      ,"hCaseID"      ,65,0,65);
  TH1F *hCaseIDNoiID  = new TH1F("hCaseIDNoiID" ,"hCaseIDNoiID" ,65,0,65);
  TH1F *hCaseIDNoiID0 = new TH1F("hCaseIDNoiID0","hCaseIDNoiID0",65,0,65);
  TH1F *hCaseIDNoiID1 = new TH1F("hCaseIDNoiID1","hCaseIDNoiID1",65,0,65);
  TH1F *hCaseIDNoiID2 = new TH1F("hCaseIDNoiID2","hCaseIDNoiID2",65,0,65);
  TH1F *hCaseIDNoiID3 = new TH1F("hCaseIDNoiID3","hCaseIDNoiID3",65,0,65);
  TH1F *hCaseIDMuoID5 = new TH1F("hCaseIDMuoID5","hCaseIDMuoID5",65,0,65);
  TH1F *hCaseIDMuoID4 = new TH1F("hCaseIDMuoID4","hCaseIDMuoID4",65,0,65);
  TH1F *hCaseIDMuoID3 = new TH1F("hCaseIDMuoID3","hCaseIDMuoID3",65,0,65);
  TH1F *hCaseIDMuoID2 = new TH1F("hCaseIDMuoID2","hCaseIDMuoID2",65,0,65);
  TH1F *hCaseIDEleID  = new TH1F("hCaseIDEleID" ,"hCaseIDEleID" ,65,0,65);
  TH1F *hCaseIDCases[11] = {
    hCaseID,hCaseIDNoiID,hCaseIDNoiID0,hCaseIDNoiID1,hCaseIDNoiID2,hCaseIDNoiID3,hCaseIDMuoID5,hCaseIDMuoID4,hCaseIDMuoID3,hCaseIDMuoID2,hCaseIDEleID
  };
  TLegend *LpwWN = new TLegend(0.20,0.66,0.58,0.90);
  TLegend *LpwNN = new TLegend(0.58,0.70,0.96,0.90);
  char WNNN[2][3]={"WN","NN"};
  setLegendDefault(LpwWN,1);
  setLegendDefault(LpwNN,1);
  LpwWN->AddEntry(hCaseID      ,"All hit"              ,"f");   
  LpwWN->AddEntry(hCaseIDNoiID ,"Noise hit"            ,"f");   
  LpwWN->AddEntry(hCaseIDNoiID0,"Noise hit(nH=1)"      ,"f");   
  LpwWN->AddEntry(hCaseIDNoiID1,"Noise hit(nH=2)"      ,"f");   
  LpwWN->AddEntry(hCaseIDNoiID2,"Noise hit(nH=3)"      ,"f");   
  LpwWN->AddEntry(hCaseIDNoiID3,"Noise hit(nH>3)"      ,"f");   
  //cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
  LpwNN->AddEntry(hCaseIDMuoID5,"Muon hit(4M+0N)"      ,"f");   
  LpwNN->AddEntry(hCaseIDMuoID4,"Muon hit(4M+1N|3M)"   ,"f");   
  LpwNN->AddEntry(hCaseIDMuoID3,"Muon hit(4M+2N|3M+1N)","f");   
  LpwNN->AddEntry(hCaseIDMuoID2,"Muon hit(4M+3N|3M+2N)","f");   
  LpwNN->AddEntry(hCaseIDEleID ,"Electron hit(>4)"     ,"f");   
  TCanvas *ccaseNum = new TCanvas("ccaseNum","",3840,2160);
  ccaseNum->cd();

      
  Long64_t Num_hPwidthCases[11] ={};

  for(int i=0;i<11;i++){
    hCaseIDCases[i]->SetLineColor(colorArr[i]);
    cout<<Form("%.f/%.f",caseNum[0][i],caseNum[1][i])<<endl;
    hCaseIDCases[i]->SetBinContent(i+1,caseNum[0][i]/caseNum[1][i]);
    hCaseID->GetXaxis()->SetBinLabel(i+1,Form("#frac{%.f}{%.f}",caseNum[0][i],caseNum[1][i]));
    hCaseID->GetXaxis()->SetTitle("#scale[0.8]{Case Number Display (#scale[0.6]{#frac{#Deltatcnt=140 }{#Deltatcnt=1000}})}");
    hCaseID->GetYaxis()->SetTitle("Case Number Ratio");
  }
  hCaseID->GetXaxis()->SetRangeUser(0,12);
  hCaseID->GetYaxis()->SetRangeUser(0.6,2.2);
  hCaseID->GetXaxis()->SetLabelSize(0.04);
  int gx[2]={-5,20},gy[2]={1,1};
  TGraph *g1 = new TGraph(2,gx,gy);
  
  hCaseIDCases[0]->Draw("hist");
  for(int i1=1;i1<11;i1++) hCaseIDCases[i1]->Draw("histsame");
  hCaseIDCases[0]->Draw("histsame");
  g1->SetLineWidth(0);
  g1->SetLineStyle(3);
  LpwWN->Draw();
  LpwNN->Draw();
  g1->Draw("lsame");
  ccaseNum->Print(Form("%sNumRatio140VS1000.pdf",DirResultPwVCase));

  
  
  
}