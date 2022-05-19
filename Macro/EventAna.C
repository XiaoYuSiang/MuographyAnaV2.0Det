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

//channel vs rate combine

void EventAna(const int indexi=28, const int indexf=29 ) {
  double eventGap[46]={};
  vector<string> vRootFiles = RawRootList(DirOperate);
  // for(int i=0;i<100;i++) cout<<vRootFiles[i]<<endl;
  TreeReader data(vRootFiles);
  ofstream out(Form("%sSEVertical.dat",DirOperate));

  //Data Variable
  for(int i0=indexi;i0<indexf;i0++){
    eventGap[i0] = (1.0*eventGapTcnt[i0])/2560000000.;
    char rtfN[200]; sprintf(rtfN,"%sGapT%d_%s_nHTH%dL%d.root",DirOperate,eventGapTcnt[i0],TimeStr,TriggerS,TriggerL);
    TFile *rotfile = new TFile(rtfN,"RECREATE");//@@ by DSL
    cout<<Form("Root Save: %s",rtfN)<<endl;
    Int_t      EvIndex=0;
    Int_t      frame_;
    Long64_t   unixtime_;
    Int_t      tYear;
    Int_t      tMonth;
    Int_t      tDate;
    Int_t      tHour;
    Int_t      tMinute;
    Int_t      tSecond;
    
    //PS:2560000000 Hz
    Int_t      nLayers;
    Int_t      nHits ;//total number for hit in a frame/event
    Int_t      nH  ;//total number for hit in a frame/event
    Int_t      nH0  ;//total number for hit in a frame/event
    Int_t      nH1  ;//total number for hit in a frame/event
    Int_t      nH2  ;//total number for hit in a frame/event
    Int_t      nH3  ;//total number for hit in a frame/event
    Double_t*  dtime_;
    Int_t*     channel_;
    Int_t*     board_;
    Int_t*     pwidth_;
    Int_t*     pcnt_;
    Long64_t*  tcnt_;
    vector<Double_t>  dtime;
    vector<Int_t>     channel;
    vector<Int_t>     board;
    vector<Int_t>     pwidth;
    vector<Int_t>     pwidthScaleFactor;
    vector<Int_t>     pcnt;
    vector<Long64_t>  tcnt;
    vector<Long64_t>  dtcnt;
    Double_t   dTimeOfEv ;
    
    TTree *tree = new TTree("t","data from analyzing file");
    tree->Branch("frame",&frame_);
    tree->Branch("EvIndex",&EvIndex);
    tree->Branch("unixtime",&unixtime_);
    tree->Branch("tYear",&tYear);
    tree->Branch("tMonth",&tMonth);
    tree->Branch("tDate",&tDate);
    tree->Branch("tHour",&tHour);
    tree->Branch("tMinute",&tMinute);
    tree->Branch("tSecond",&tSecond);
    // tree->Branch("pcnt",&pcnt);
    tree->Branch("nH",&nH);
    tree->Branch("nH0",&nH0);
    tree->Branch("nH1",&nH1);
    tree->Branch("nH2",&nH2);
    tree->Branch("nH3",&nH3);
    
    tree->Branch("nLayers",&nLayers);
    tree->Branch("channel",&channel);
    tree->Branch("board",&board);
    tree->Branch("pwidth",&pwidth);
    tree->Branch("pwidthScaleFactor",&pwidthScaleFactor);
    tree->Branch("tcnt",&tcnt);
    tree->Branch("pcnt",&pcnt);
    tree->Branch("dtcnt",&dtcnt);
    tree->Branch("dtime",&dtime);
    tree->Branch("dTimeOfEv",&dTimeOfEv);
    

    
    TH1F *hboard       = new TH1F("hboard ","",5,1,6);
    TH1F *hBoard       = new TH1F("hBoard ","",5,1,6);
    TH1F *hChannel     = new TH1F("hChannel ","",16,0,16);
    Int_t Index =0;
    double t0,tct0;
    dTimeOfEv=0;
    int nHit = 0;
    int evs = data.GetEntriesFast();
    for (Long64_t ev = 0; ev < evs ; ++ev) {//@@@
      int i=0;
      data.GetEntry(ev);
      frame_   = data.GetInt("frame");
      unixtime_= data.GetLong64("unixtime");//fabs time of event [s]
      if(ev%1000 == 0) cout<<Form("\r%.5f%%\t%d",(ev*100.)/(1.*evs),unixtime_)<<flush;
      nHits    = data.GetInt("nHits");
      dtime_   = data.GetPtrDouble("dtime");
      board_   = data.GetPtrInt("board");
      channel_ = data.GetPtrInt("channel");
      pwidth_  = data.GetPtrInt("pwidth");
      pcnt_    = data.GetPtrInt("pcnt");
      tcnt_    = data.GetPtrLong64("tcnt");
      

      if (tct0 == 0) tct0 = dtime_[i];
      // t0 = tcnt+pcnt/2560000000.;
      for(; i<nHits; i++){
        if (fabs(tct0-dtime_[i])>eventGap[i0]){
          tYear   = data.GetInt("tYear");
          tMonth  = data.GetInt("tMonth");
          tDate   = data.GetInt("tDate");
          tHour   = data.GetInt("tHour");
          tMinute = data.GetInt("tMinute");
          tSecond = data.GetInt("tSecond");
          nH      = nHit;//total number for hit in a frame/event
          nH0     = hBoard->GetBinContent(BD[0]);//total number for hit in a frame/event
          nH1     = hBoard->GetBinContent(BD[1]);//total number for hit in a frame/event
          nH2     = hBoard->GetBinContent(BD[2]);//total number for hit in a frame/event
          nH3     = hBoard->GetBinContent(BD[3]);//total number for hit in a frame/event
          if(nH!=nH0+nH1+nH2+nH3) cout<<EvIndex<<"  Meow"<<endl;
          nLayers=0 ;//total number for hit in a frame/event
          for(int i1=0; i1<4;i1++){
            if (hBoard->GetBinContent(BD[i1])!=0) nLayers++;
          }
          if (nHit > TriggerS&&nHit < TriggerL){
            if(unixtime_>1600000000){
              dTimeOfEv = fabs(tct0-dtime_[i]);
              tree->Fill();
              //if(Index%1000000==0) 
              //cout<<"Index:\t"<<Index<<endl;
              Index++;
              EvIndex = Index;
            }          
            //cout<<"dTimeOfEv\t"<<dTimeOfEv<<"\tnHit:\t"<<nHit<<endl;
            board.clear();
            channel.clear();
            pwidth.clear();
            pwidthScaleFactor.clear();
            tcnt.clear();
            pcnt.clear();
            dtcnt.clear();
            dtime.clear();
            //cout<<fabs(tct0-dtime_[i])<<endl;
            // cout<<"dTimeOfEv"<<dTimeOfEv<<endl;
            //dTimeOfEv = dtime_[i];
            nHit = 0;
            hBoard->Reset();
            hChannel->Reset();
            //cout<<"EvIndex:\t"<<EvIndex<<endl;
          }
        }
        board.push_back(board_[i]);
        channel.push_back(channel_[i]);
        pwidth.push_back(pwidth_[i]);
        pwidthScaleFactor.push_back(BDPwWei[BDcheck(board_[i])]);
        tcnt.push_back(tcnt_[i]);
        pcnt.push_back(pcnt_[i]);
        dtime.push_back(dtime_[i]);
        dtcnt.push_back(fabs(tct0-dtime_[i]));
        hBoard  ->Fill(board_[i]);
        hboard  ->Fill(board_[i]);
        hChannel->Fill(channel_[i]);
        nHit++;
        tct0 = dtime_[i];
      }
    }
    //tree->Scan("Index:tcnt");
    rotfile->Write();
    cout<<"\n"<<"Finished Ntuple write"<<endl;
  }
}