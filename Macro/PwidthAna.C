#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TMath.h>
#include <TF1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TRandom.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TText.h>
#include <TPDF.h>
#include "TH2.h"
#include "TProfile.h"
#include <TProfile2D.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include <TGaxis.h>
#include <TChain.h>
#include <TObject.h>
#include <TStopwatch.h>
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


//channel vs rate combine
void PwidthAna() {
  cout<< "Please use the true version"<<endl;
  throw;
}


void PwidthAnaV2(const bool*OperMode, const int itcntgap=28) {
  bool testMode        =true;  testMode         = OperMode[0];
  bool normalizeMode   =true;  normalizeMode    = OperMode[1];
  bool NegHiRatChMode  =true;  NegHiRatChMode   = OperMode[2];
  bool lowstatisticMode=false; lowstatisticMode = OperMode[3];
  bool rootFileMode    =true;  rootFileMode     = OperMode[4];
  bool SETightMode     =true;  SETightMode      = OperMode[5];
  vector<string> vRootFiles = RawRootList(DirOperate);
  // for(int i=0;i<100;i++) cout<<vRootFiles[i]<<endl;
  
  int binrt = 0;
  int Starttime = unixtimei, Endtime = unixtimef;
  char TestMarker[15]={};
  int NumBDt = NumBD, NumCht = NumCh;
  if (testMode){
    sprintf(TestMarker,"TST_");
    // NumBDt = 1;
    // NumCht = 1;
  }
  
  if(SETightMode==false&&testMode) sprintf(TestMarker,"TST_Loose_");
  else if(SETightMode==false) sprintf(TestMarker,"Loose_");
  
  TFile *SelectRfile;
  //cout<<"000000"<<endl;
  char RFNStr[200];
  sprintf(RFNStr,"%d_%s_nHTH%dL%d.root",eventGapTcnt[itcntgap],TimeStr,TriggerS,TriggerL);
  
  if(SETightMode) SelectRfile = TFile::Open(Form("%sTracksGT%s",DirOperate,RFNStr));
  else SelectRfile = TFile::Open(Form("%s%sEventsGT%s",DirOperate,TestMarker,RFNStr));
  TTree *tR  = (TTree*) SelectRfile ->Get("t");
  cout<<"opening....."<<SelectRfile->GetName()<<endl;
  
  int colorArr[16] = {};
  for(int i=0;i<16;i++){
    if(i<12) colorArr[i]=205+i;
    else if(i>11) colorArr[i]=209+i;
  }
  

  char DirResultGPw[150]={""}, DirResultGRt[150]={""};//save path graph of Pwidth Rate

  sprintf(DirResultGPw,"%s%sBoardGPwNor/",DirResult,TestMarker);
  if (system(Form("mkdir -p %s",DirResultGPw))!=0) system(Form("mkdir -p %s",DirResultGPw));
  sprintf(DirResultGRt,"%s%sBoardGRtNor/",DirResult,TestMarker);
  if (system(Form("mkdir -p %s",DirResultGRt))!=0) system(Form("mkdir -p %s",DirResultGRt));
  
  TStopwatch optime;
  optime.Start();
  binrt = (Endtime-Starttime)*1.0/th2fbinwidth;
  cout<<"binrt number:   "<<binrt<<endl;
  int fileVecSize = int(vRootFiles.size());
  cout<<"File number:   "<<fileVecSize<<endl;
  char *infilename[200]={};
  TChain *t = new TChain("t");
  if(testMode) fileVecSize = 1;
  //fileVecSize = 10;//@@@@
  for(int i=0;i<fileVecSize;i++){
    infilename[i] = &vRootFiles[i][0];
    t->Add(infilename[i]);
    ////cout<<infilename[i]<<endl;
  } 
  /*
  TH2F *hPWVTB3Ch15 = new TH2F("hPWVTB3Ch15","hPWVTB3Ch15",binpw,0,binpw,binrt,Starttime,Endtime);//ch64, pw: 0~180
  TH2F *hPWVTB3Ch15N= (TH2F*) hPWVTB3Ch15->Clone();//ch64, pw: 0~180

  gStyle->SetLineScalePS(0.1);
  TCanvas *cPWVTB3Ch15 = new TCanvas("cPWVTB3Ch15","board==3&&channel==15",960*4,540*4);
  cPWVTB3Ch15->cd();
  cPWVTB3Ch15->SetPhi(-30);
  cPWVTB3Ch15->SetTheta(70);
  t->Draw("unixtime:pwidth>>hPWVTB3Ch15","board==3&&channel==15","Lego2");
  if(normalizeMode){
    for(int ibt=0;ibt<binrt;ibt++){
      double ibtIntegralNum = hPWVTB3Ch15->Integral(0,binpw,ibt,ibt+1);
      
      for(int ibinpw=0;ibinpw<binpw;ibinpw++){
        double ibtOriginNum =0;
        ibtOriginNum = hPWVTB3Ch15->Integral(ibinpw,(ibinpw+1),ibt,ibt+1);
        double interData = 0;
        if(ibtIntegralNum==0) interData=0;
        else interData=ibtOriginNum/ibtIntegralNum;
        hPWVTB3Ch15N->SetBinContent(ibinpw,ibt,interData);
        
        //cout<<ibinpw<<"    "<<ibtOriginNum/ibtIntegralNum<<endl;
        //cout<<ibinpw<<"    "<<hPWVTB3Ch15N->GetBinContent(ibinpw,ibt)<<endl;
      }
    }
  }
  hPWVTB3Ch15N->GetXaxis()->SetTitle("Pwidth(100ns)");
  hPWVTB3Ch15N->GetYaxis()->SetTitle("unixtime(s)");
  hPWVTB3Ch15N->GetZaxis()->SetTitle("Number of Hit");
  hPWVTB3Ch15N->GetXaxis()->SetLabelSize(0.03);
  hPWVTB3Ch15N->GetYaxis()->SetLabelSize(0.03);
  hPWVTB3Ch15N->GetZaxis()->SetLabelSize(0.025);
  hPWVTB3Ch15N->GetXaxis()->SetTitleSize(0.035);
  hPWVTB3Ch15N->GetYaxis()->SetTitleSize(0.035);
  hPWVTB3Ch15N->GetZaxis()->SetTitleSize(0.04);
  hPWVTB3Ch15N->GetXaxis()->SetTitleOffset(1.6);
  hPWVTB3Ch15N->GetYaxis()->SetTitleOffset(1.6);
  hPWVTB3Ch15N->GetZaxis()->SetTitleOffset(1.4);
  if(normalizeMode) hPWVTB3Ch15N->GetZaxis()->SetTitle("Ratio");
  hPWVTB3Ch15N->Draw("Lego2");
  cPWVTB3Ch15->Print(Form("%s%sNormal.pdf",DirResult,"hPWvTLego2"));
  cPWVTB3Ch15->Print(Form("%s%sNormal.png",DirResult,"hPWvTLego2"));
  hPWVTB3Ch15N->Draw("colz");
  cPWVTB3Ch15->Print(Form("%s%sNormal.pdf",DirResult,"hPWvTcolz"));
  cPWVTB3Ch15->Print(Form("%s%sNormal.png",DirResult,"hPWvTcolz"));
  */
  
  

  
  
  
  gStyle->SetLineScalePS(0.3);
  //vector<string> vRootFiles = find_files(inpaths);
  //Printf("vRootFiles found: %i", (int) vRootFiles.size());
  //TreeReader data(inpaths);
  
  ofstream outPwfit  (Form("%s%soutPwfit.txt"  ,DirOperate,TestMarker));
  ofstream outRt(Form("%sRtBinsTooLarge.txt",DirOperate));
  TCanvas *cpwch = new TCanvas("cpwch","",4160,4160);
  TCanvas *cpwchSE = new TCanvas("cpwchSE","",4160,4160);
  TCanvas *cpwrt = new TCanvas("cpwrt","",4160,4160);
  TCanvas *cpw[4]={},*cpwAll[4]={};
  TCanvas *crt[4]={},*crtAll[4]={};
  TPDF *pdfcpw = new TPDF(Form("%sPwidthCompareNormalize.pdf",DirResultGPw));
  TPDF *pdfcrt = new TPDF(Form("%sRateCompareNormalize.pdf",DirResultGRt));
  TPDF *pdfcpwA = new TPDF(Form("%sPwidthACompareNormalize.pdf",DirResultGPw));
  TPDF *pdfcrtA = new TPDF(Form("%sRateACompareNormalize.pdf",DirResultGRt));
  TH1F *hPw[64]={},*hPwSE[64]={};
  TH1F *hRt[64]={};
  // TF1  *fPw[64]={},*fPwSE[64]={};
  TH2F *hchPw = new TH2F("hchPw","hchPw",binpw,0,binpw,80,0,80);//ch64, pw: 0~180
  TH2F *hchPw_0 = new TH2F("hchPw_0","hchPw_0",binpw,0,binpw,80,0,80);//ch64, pw: 0~180
  TH2F *hchPwSE = new TH2F("hchPwSE","hchPwSE",binpw,0,binpw,80,0,80);//ch64, pw: 0~180
  TH2F *hchPwDS[2] = {hchPw,hchPwSE};//ch64, pw: 0~180
  TH2F *hchRt = new TH2F("hchRt","hchRt",binrt,Starttime,Endtime,80,0,80);//ch64, pw: 0~180
  cpwch->SetPhi(-30);
  cpwch->SetTheta(70);
  cpwchSE->SetPhi(-30);
  cpwchSE->SetTheta(70);
  FastStyleStats(.9,.9);
  cpwchSE->cd();
  char PWDDataName[3][200]={};
  sprintf(PWDDataName[0],"%s%soutPwHIData.txt"  ,DirOperate,TestMarker);
  sprintf(PWDDataName[1],"%s%soutPwSEData.txt"  ,DirOperate,TestMarker);
  if(rootFileMode){
    if(SETightMode) tR->Draw("(channel+(board-1)*16):pwidth/pwidthScaleFactor>>hchPwSE","BiZ==15 &&eventHit==4","colz");
    else tR->Draw("(channel+(board-1)*16):pwidth/pwidthScaleFactor>>hchPwSE","IDtight>=4","colz");
    cpwch->cd();
    t->Draw(Form("(channel+(board-1)*16):pwidth/2.>>hchPw_0"),"board==1","colz");
    t->Draw(Form("(channel+(board-1)*16):pwidth>>hchPw"),"board!=1","colz");
    hchPw->Add(hchPw_0);
    //cpwch->Update();
    char devoteCondition[40]={};
    if(NegHiRatChMode){
      sprintf(devoteCondition,"channel!=15||board!=3");
      t->Draw(Form("(channel+(board-1)*16):pwidth>>hchPw"),Form("%s",devoteCondition),"colz");
    }
    for(int i=0;i<2;i++){
      ofstream outPwData(PWDDataName[i]);
      
      if(normalizeMode){
        for(int bd=0;bd<NumBDt;bd++){
          for(int ch=0;ch<NumCht;ch++){
            if(NegHiRatChMode){
              if(BD[bd]==3&&ch==15) continue;
            }
            double ChIntegralNum  = hchPwDS[i]->Integral(-1,-1,(BD[bd]-1)*16+ch+1,(BD[bd]-1)*16+ch+1);
            outPwData<<BD[bd]<<"\t"<<ch;
            //cout<<ch+bd*16<<"      "<<ChIntegralNum<<endl;
            for(int ibinpw=0;ibinpw<binpw;ibinpw++){
              double ChOriginNum =hchPwDS[i]->Integral(ibinpw+1,ibinpw+1,(BD[bd]-1)*16+ch+1,(BD[bd]-1)*16+ch+1);
              double interData = 0;
              if(ChOriginNum==0) interData=0;
              else interData=ChOriginNum/ChIntegralNum;
              hchPwDS[i]->SetBinContent(ibinpw+1,(BD[bd]-1)*16+ch+1,interData);
              outPwData<<"\t"<<interData;
              // cout<<ibinpw<<"    "<<(BD[bd]-1)*16+ch+1<<"    "<<ChOriginNum/ChIntegralNum<<endl;
              // cout<<ibinpw<<"    "<<(BD[bd]-1)*16+ch+1<<"    "<<hchPw->GetBinContent(2*ibinpw+2,(BD[bd]-1)*16+ch+1)<<endl;
            } 
            outPwData<<"\n";
            //cout<<ch+bd*16<<"      "<<hchPwDS[i]->Integral(-1,-1,(BD[bd]-1)*16+ch+1,(BD[bd]-1)*16+ch+1)<<endl;
          }
        }
      }
      outPwData.close();
      setTitle(hchPwDS[i],"Pwidth/pwidthScaleFactor(100ns)","board*16+channel","Number of Hit");
      if(normalizeMode) hchPwDS[i]->GetZaxis()->SetTitle("Ratio");
    }
  }
  
  double PWDTmp[2][4][16][90] ={};//[SE | Hit][bd][ch][pwd]
  for(int ic=0;ic<2;ic++){
      //"/data4/YuSiang/S4_11F_4lay/OperateResule/outRTHIData.txt",
    //cout<<PWDDataName[1]<<endl;
    ifstream in(PWDDataName[ic]);
    for(int i0=0,ich=0,ibd=0;i0<NumCht*NumBDt;i0++){
      in>>ibd>>ich;
      ibd = BDINV[ibd];
      //cout<<ibd<<"\t"<<ich;
      for(int ibin=0;ibin<binpw;ibin++){
        double pwtmp=0;
        in>>pwtmp;
        PWDTmp[ic][ibd][ich][ibin] = pwtmp;
        //cout<<"\t"<<ic<<"\t"<<ibd<<"\t"<<ich<<"\t"<<ibin;
        //cout<<"\t"<<PWDTmp[ic][ibd][ich][ibin];
        //cout<<"\t"<<pwtmp;
      }
      //cout<<endl;
    }
    in.close();
  }
  
  DrawPdfPng(cpwch  ,Form("%shchPwColzNormal"  ,DirResultGPw));
  DrawPdfPng(cpwchSE,Form("%shchPwSEColzNormal",DirResultGPw));
  FastStyleStats(0.95,0.95);
  TH2F *hchPwL   = new TH2F(*hchPw);
  TH2F *hchPwLSE = new TH2F(*hchPwSE);
  TH2F    *hchPwLDS[2] ={hchPwL,hchPwLSE};//all data and selection data 
  TCanvas *cpwchDS[2]  ={cpwch,cpwchSE};//all data and selection data 
  for(int i=0;i<2;i++){
    cpwchDS[i]->cd();
    hchPwLDS[i]->Draw("Lego2");
    hchPwLDS[i]->GetXaxis()->SetRangeUser(0,60);
    setAxisLabelSize(hchPwLDS[i],0.03,0.03,0.025);
    setAxisTitleSize(hchPwLDS[i],0.035,0.035,0.04);
    setAxisTitleOffset(hchPwLDS[i],1.6,1.6,1.4);
    setAxisCenterTitle(hchPwLDS[i],true,true,true);
  }
  cpwchDS[0]->Print(Form("%s%sNormal.pdf",DirResultGPw,"hchPwLego"));
  cpwchDS[1]->Print(Form("%s%sNormal.pdf",DirResultGPw,"hchPwSELego"));
  cpwchDS[0]->Close();
  cpwchDS[1]->Close();

  
  cpwrt->cd();
  cpwrt->SetPhi(-30);
  cpwrt->SetTheta(70);
  setTitle(hchRt,"unixtime","board*16+channel","Data tacking Rate(Hz)");
  FastStyleStats(0.85,0.44);
  
  t->Draw(Form("(channel+(board-1)*16):unixtime>>hchRt"),"","colz");
  
  cpwrt->Print(Form("%s%sNormal.pdf",DirResultGRt,"hchRtColz"));
  FastStyleStats(0.95,0.95);
  TH2F *hchRtL = (TH2F*) hchRt->Clone();
  hchRtL->Draw("Lego2");
  setAxisLabelSize(hchRtL,0.03,0.03,0.025);
  setAxisTitleSize(hchRtL,0.035,0.035,0.040);
  setAxisTitleOffset(hchRtL,1.6,1.6,1.4);
  setAxisCenterTitle(hchRtL,true,true,true);
  
  cpwrt->Print(Form("%s%sNormal.pdf",DirResultGRt,"hchRtLego"));
  cpwrt->Close();
  FastStyleStats(0.95,0.6);
  for(int bd=0;bd<NumBDt;bd++){
    
    //cout<<DirResultGPwaRt<<"      A         "<<endl;
    // char cpwname[30]={};
    char crtname[30]={};
    // sprintf(cpwname ,Form("cpwB%d",BD[bd]));
    sprintf(crtname ,Form("crtB%d",BD[bd]));
    // cpw[bd] = new TCanvas(cpwname,"",2160,2160);
    crt[bd] = new TCanvas(crtname,"",2160,2160);
    // cpwAll[bd] = new TCanvas(Form("%sAll",cpwname),"",2160,2160);
    crtAll[bd] = new TCanvas(Form("%sAll",crtname),"",2160,2160);
    // cpw[bd]->Divide(4,4);
    crt[bd]->Divide(4,4);
    
    // TLegend *LChpw  = new TLegend(0.4,0.60,0.93,0.9);
    // setLegendDefault(LChpw,4);
    TLegend *LChrt  = new TLegend(0.4,0.75,0.93,0.9);
    setLegendDefault(LChrt,4);

    //cpw[bd]->cd();
    // cout<<"\ncv"<<bd<<"\t";
    for(int ch=0;ch<NumCht;ch++){
      //cout<<DirResultGPwaRt<<"      B         "<<endl;
      char hpwname[30]={},hrtname[30]={};
      // char fpwname[30]={},fpwsename[30]={};
      // sprintf(hpwname  ,"hPwB%dCh%d",BD[bd],ch);
      sprintf(hrtname  ,"hRtB%dCh%d",BD[bd],ch);
      // sprintf(fpwname  ,"fPwB%dCh%d",BD[bd],ch);
      // sprintf(fpwsename,"fpwseB%dCh%d",BD[bd],ch);
       
      ////cout<<hpwname<<"   "<<hpwselectobject<<"   "<<selectcondition<<endl;

      // hPw[ch+bd*16]   = new TH1F(hpwname,hpwname,binpw,0,binpw);
      // hPwSE[ch+bd*16] = new TH1F(hpwname,hpwname,binpw,0,binpw);
      //fPw[ch+bd*16]   = new TF1(fpwname  ,pwidthHitfit,0,90,7);
      
      // int hitcase = 0;
      // if(lowstatisticMode) hitcase = 3;
      // int casePnum[5] = {9,8,7,7,3};
      // if(hitcase==0){
        // fPw[ch+bd*16] = new TF1(fpwname,HitFit2lgFun_0,0,50,9);
        // HitFit2lgFun_0(fPw[ch+bd*16]);
      // }else if(hitcase==1){
        // fPw[ch+bd*16] = new TF1(fpwname,N_HitFitFun_0 ,0,50,8);
        // N_HitFitFun_0(fPw[ch+bd*16]);
      // }else if(hitcase==2){
        // fPw[ch+bd*16] = new TF1(fpwname,HitFitFun_0   ,0,50,7);
        // HitFitFun_0(fPw[ch+bd*16]);
      // }else if(hitcase==3){
        // fPw[ch+bd*16] = new TF1(fpwname,HitFitFun_85  ,0,50,7);
        // HitFitFun_85(fPw[ch+bd*16]);
      // }
      // fPwSE[ch+bd*16] = new TF1(fpwsename,pwidthSEfit,0,90,3);

      // TH1F *hPwAll[2]={hPw[ch+bd*16],hPwSE[ch+bd*16]};
      // TF1  *fPwAll[2]={fPw[ch+bd*16],fPwSE[ch+bd*16]};
      hRt[ch+bd*16] = new TH1F(hrtname,hrtname,binrt,Starttime,Endtime);
      //outPwfit<<bd<<"\t"<<ch;
      // for(int ibinpw=0; ibinpw<binpw;ibinpw++){
        // hPwAll[0]->SetBinContent(ibinpw+1,PWDTmp[0][bd][ch][ibinpw]);
        // hPwAll[1]->SetBinContent(ibinpw+1,PWDTmp[1][bd][ch][ibinpw]);
         // cout<<PWDTmp[0][bd][ch][ibinpw]<<"   "<<PWDTmp[1][bd][ch][ibinpw]<<endl;
      // }


      // landaufun(fPwAll[1]);
      
      // for(int icasehit=0;icasehit<casePnum[hitcase];icasehit++) {
        // fPwAll[0]->SetParLimits (icasehit,SPLmin[hitcase][icasehit],SPLmax[hitcase][icasehit]);
        // fPwAll[0]->SetParameter(icasehit,SPS[hitcase][icasehit]);
      // }
      //fPwAll[0]->FixParameter(casePnum[hitcase]-1,0);
      
      // for(int ise=0;ise<3;ise++){
        // fPwAll[1]->SetParLimits (ise,SPLmin[4][ise],SPLmax[4][ise]);
        // fPwAll[1]->SetParameter(ise,SPS[4][ise]);
      // }
      
      
      // for(int i1=0;i1<2;i1++){
        
      //cout<<DirResultGPwaRt<<"      C         "<<endl;
        //if(ch==3&&BD[bd]==4) int aaaa=1;
        //else continue;
        // hPwAll[i1]->Fit(fPwAll[i1],"NQ");
        // fPwAll[i1]->SetLineWidth(i1+2);
        /*
        if(fPwAll[0]->GetChisquare()>0.07){
          int Chi2MinIndex=0;
          float Chi2tmp[25]={},chi2Min=1,Bestfit[9]={};
          for(int iP=0;iP<50;iP++){
            cout<<DirResultGPwaRt<<"      D         "<<endl;
            fPwAll[0]->SetParameter(1,-10+iP/2);
            fPwAll[0]->SetParameter(1,-10+iP/2);
            hPwAll[0]->Fit(fPwAll[0],"NQ");
            Chi2tmp[iP]=fPwAll[0]->GetChisquare();
            if(Chi2tmp[iP]<chi2Min){
              chi2Min = Chi2tmp[iP];
              Chi2MinIndex = iP;
              for(int ips=0;ips<casePnum[hitcase];ips++) Bestfit[ips]=fPwAll[0]->GetParameter(ips);
            }
            if(fPwAll[0]->GetChisquare()<0.07) break;
          }
          fPwAll[0]->SetParameters(Bestfit[0],Bestfit[1],Bestfit[2],Bestfit[3],Bestfit[4],Bestfit[5],Bestfit[6],Bestfit[7],Bestfit[8]);
          hPwAll[0]->Fit(fPwAll[0],"NQ");
          fPwAll[0]->SetLineWidth(i1+2);
        }
        if(fPwAll[1]->GetChisquare()>0.1){
          int Chi2MinIndex=0;
          float Chi2tmp[25]={},chi2Min=1,Bestfit[3]={};
          for(int iP=0;iP<25;iP++){
            fPwAll[1]->SetParameters(0.4,0+iP,0.3);
            hPwAll[1]->Fit(fPwAll[1],"NQ");
            Chi2tmp[iP]=fPwAll[1]->GetChisquare();
            // cout<<Chi2tmp[iP]<<endl;
            if(Chi2tmp[iP]<chi2Min){
              chi2Min = Chi2tmp[iP];
              Chi2MinIndex = iP;
              for(int ips=0;ips<3;ips++) Bestfit[ips]=fPwAll[1]->GetParameter(ips);
            }
            if(fPwAll[1]->GetChisquare()<0.1) break;
          }
          // cout<<Chi2MinIndex<<"    "<<Chi2tmp[Chi2MinIndex]<<endl;
          //Chi2MinIndex++;
          fPwAll[1]->SetParameters(Bestfit[0],Bestfit[1],Bestfit[2]);
          hPwAll[1]->Fit(fPwAll[1],"NQ");
          fPwAll[1]->SetLineWidth(1+2);
        }
        */
        // if(lowstatisticMode&&SETightMode!=true) break;
        
      // }
      
      // TLegend *LChs  = new TLegend(0.6,0.8,0.95,0.9);
      // setLegendDefault(LChs);
      // LChs->AddEntry(hPw[ch+bd*16]  ,Form("Ch-%d All  event",ch),"l");
      // LChs->AddEntry(hPwSE[ch+bd*16],Form("Ch-%d Muon event",ch),"F");
      // TLegend *LFitHit  = new TLegend(0.6,0.8-0.03*8-0.05,0.93,0.8);
      // setLegendDefault(LFitHit);
      // TLegend *LFitSE   = new TLegend(0.6,0.8-0.03*12-0.05*2,0.93,0.8-0.03*8-0.05);
      // setLegendDefault(LFitSE);
      // TLegend *LFit[2]  = {LFitHit,LFitSE};
      // int fitParNum[2]={casePnum[hitcase],3};
      //outPwfit  <<BD[bd]<<"\t"<<ch;
      //outPwSEfit<<BD[bd]<<"\t"<<ch;
      // LFit[0]->AddEntry(fPwSE[ch+bd*16],Form("Ch-%d Hits fit",ch),"l");
      // LFit[1]->AddEntry(fPwSE[ch+bd*16],Form("Ch-%d Muon fit",ch),"l");
      // for(int ifit=0;ifit<2;ifit++){
        // LFit[ifit]->AddEntry("",Form("#chi^{2}  =  %.4f\t",fPwAll[ifit]->GetChisquare()),"h");
        // for(int iL=0;iL<fitParNum[ifit];iL++){
          // outPwfit  <<"\t"<<fPwAll[ifit]->GetParameter(iL);
          // LFit[ifit]->AddEntry("",Form("%s\t%.4f    #pm  %.4f",fPwAll[ifit]->GetParName(iL),fPwAll[ifit]->GetParameter(iL),fPwAll[ifit]->GetParError(iL)),"h");
        // }
        // if(lowstatisticMode&&SETightMode!=true) break;
      // }
      //outPwfit<<endl;
      //outPwSEfit<<endl;
        //cout<<BD[bd]<<"\t"<<ch<<"\t";
        
        
      // LChpw->AddEntry(hPw[ch+bd*16]  ,Form("Ch-%d All  event",ch),"l");
      // LChpw->AddEntry(hPwSE[ch+bd*16],Form("Ch-%d Muon event",ch),"F");
      LChrt->AddEntry(hRt[ch+bd*16]  ,Form("Ch-%d All  hits ",ch),"l");
      // cout<<ch+bd*16<<"  "<<hPw[ch+bd*16]->Integral(-1,-1)<<endl;
      // cout<<ch+bd*16<<"  "<<hPwSE[ch+bd*16]->Integral(-1,-1)<<endl;

      //cout<<(BD[bd]-1)*16+ch+1<<endl;


      
      // cpw[bd]->cd(2*(ch%4)+13-ch);
      // hPw[ch+bd*16]  ->Draw("hist");
      // hPw[ch+bd*16]  ->SetStats(0);
      // hPwSE[ch+bd*16]->Draw("histsame");
      // fPw[ch+bd*16]  ->Draw("lsame");
      // if(lowstatisticMode==false||SETightMode==false) fPwSE[ch+bd*16]->Draw("lsame");
      // LChs->Draw("same");
      // LFitHit->Draw("same");
      // if(lowstatisticMode==false||SETightMode==false) LFitSE->Draw("same");
      // cpwAll[bd]->cd();
      // if (ch==0){
        // hPw[ch+bd*16]->Draw("hist");
        // LChpw->Draw("same");
      // }
      // else       hPw[ch+bd*16]->Draw("whistsame");
      // hPwSE[ch+bd*16]->Draw("histsame");
      // cout<<"load data hrt"<<endl;
      double RtValue[2] = {};
      RtValue[0] = 1.0*(hchRt->GetBinContent(1,(BD[bd]-1)*16+ch+1));
      for(int ibinrt=1; ibinrt<binrt+1;ibinrt++){
        RtValue[1] = 1.0*(hchRt->GetBinContent(ibinrt,(BD[bd]-1)*16+ch+1));
        hRt[ch+bd*16]->SetBinContent(ibinrt,RtValue[1]);
        double RtRate = (RtValue[1]/RtValue[0]);
        if(ibinrt>20){
          if     ( RtRate<1.E+8&&RtRate>1.165) outRt<<BD[bd]<<"\t"<<ch<<"\t"<<ibinrt<<"\t"<<RtValue[1]/RtValue[0]<<endl;
          else if(RtValue[1]/RtValue[0]<0.835) outRt<<BD[bd]<<"\t"<<ch<<"\t"<<ibinrt<<"\t"<<RtValue[1]/RtValue[0]<<endl;
        }
        
        RtValue[0] = RtValue[1];
      }
      // cout<<"draw data hrt"<<endl;
      crt[bd]->cd(2*(ch%4)+13-ch);
      hRt[ch+bd*16]->Scale(1/th2fbinwidth);
      hRt[ch+bd*16]->Draw("hist");
      crtAll[bd]->cd();
      
      if (ch==0){
        hRt[ch+bd*16]->Draw("hist");
        LChrt->Draw();
      }else       hRt[ch+bd*16]->Draw("histsame");
      // cout<<"finish draw data hrt"<<endl;
      //cout<<"ch"<<ch<<"    "<<hPw[ch+bd*16]->GetBinContent(1)<<"\n";
      // setTitle(hPw[ch+bd*16]  ,hpwname,"pwidth/pwidthScaleFactor(100ns)","Number of Hit");
      // setTitle(hPwSE[ch+bd*16],hpwname,"pwidth/pwidthScaleFactor(100ns)","Number of Hit");
      // if(normalizeMode){
        // hPw[ch+bd*16]  ->GetYaxis()->SetTitle("Ratio");
        // hPwSE[ch+bd*16]->GetYaxis()->SetTitle("Ratio");
      // }
      setTitle(hRt[ch+bd*16],hrtname,"unixtime(s) half hour/bin","Rate of channel(Hz)");
      // cout<<"setTitle(hRt[ch+bd*16],hrtnam"<<endl;

      // hRt[ch+bd*16]  ->SetTitle(hrtname);
      // hRt[ch+bd*16]  ->GetXaxis()->SetTitle("unixtime(s) half hour/bin");
      // hRt[ch+bd*16]  ->GetYaxis()->SetTitle("Rate of channel(Hz)");
      // setAxisRangeUser(hPw[ch+bd*16],0,60,0,0.2);
      hRt[ch+bd*16]  ->GetYaxis()->SetRangeUser(0,1.2);
      // hPw[ch+bd*16]  ->SetLineWidth(0.5);
      // hPwSE[ch+bd*16]->SetLineWidth(0.5);
      hRt[ch+bd*16]  ->SetLineWidth(0.5);
      // cout<<"hRt[ch+bd*16]  ->SetLineWidth(0.5"<<endl;
      // hPw[ch+bd*16]  ->SetLineColor(colorArr[ch]);
      // fPw[ch+bd*16]  ->SetLineColor(colorArr[ch]);
      // fPwSE[ch+bd*16]->SetLineColor(colorArr[ch]);
      // hPwSE[ch+bd*16]->SetLineColorAlpha(colorArr[ch],0.1);
      // hPwSE[ch+bd*16]->SetFillColorAlpha(colorArr[ch],0.2);
      hRt[ch+bd*16]  ->SetLineColor(colorArr[ch]);
      hRt[ch+bd*16]  ->SetStats(true);
      // cout<<"finish draw hrt"<<endl;
      
    }
    //cout<<DirResultGPwaRt<<"      E         "<<endl;
    //cout<<cpwname<<"      E         "<<endl;
    char namessss[200]={};
    // sprintf(namessss,"%s%sNormal",DirResultGPwaRt,cpwname);
    // DrawPdfPng(cpw[bd],namessss);
    char namessss1[200]={};
    sprintf(namessss1,"%s%sNormal",DirResultGRt,crtname);
      // cout<<"intf(namessss1,%s%"<<endl;
    //cout<<DirResultGPwaRt<<"      E         "<<endl;
    // cout<<cpwname<<"      E         "<<endl;
    DrawPdfPng(crt[bd],namessss1);
    // cout<<DirResultGPwaRt<<"      E         "<<endl;
    // PDFCanPage(pdfcpw,cpw[bd],Form("MTB-%d",BD[bd]),bd+1,NumBDt);
    PDFCanPage(pdfcrt,crt[bd],Form("MTB-%d",BD[bd]),bd+1,NumBDt);
    // PDFCanPage(pdfcpwA,cpwAll[bd],Form("MTB-%d",BD[bd]),bd+1,NumBDt);
    PDFCanPage(pdfcrtA,crtAll[bd],Form("MTB-%d",BD[bd]),bd+1,NumBDt);
      // cout<<"PDFCanPage(pdfcrtA,crtAll[bd]"<<endl;
    // cout<<DirResultGPwaRt<<"      E         "<<endl;
  }


  optime.Stop();
  optime.Print();
}

/* 

void PwidthAnaV3(const bool*OperMode){
  bool testMode        =true;  testMode         = OperMode[0];
  bool normalizeMode   =true;  normalizeMode    = OperMode[1];
  bool NegHiRatChMode  =true;  NegHiRatChMode   = OperMode[2];
  bool lowstatisticMode=false; lowstatisticMode = OperMode[3];
  bool rootFileMode    =true;  rootFileMode     = OperMode[4];
  bool SETightMode     =true;  SETightMode      = OperMode[5];
  TStopwatch optime;
  optime.Start();
  int colorArr[16] = {};
  for(int i=0;i<16;i++){
    if(i<12) colorArr[i]=205+i;
    else if(i>11) colorArr[i]=209+i;
  }
  char TestMarker[15]={};
  if (testMode){
    sprintf(TestMarker,"TST_");
    NumBDt = 1;
    NumCht = 1;
  }
  if(SETightMode==false&&testMode) sprintf(TestMarker,"TST_Loose_");
  else if(SETightMode==false) sprintf(TestMarker,"Loose_");
  
  char DirResultGPwaRt[150]={""};//save path graph of Pwidth Rate
  sprintf(DirResultGPwaRt,"%s%sBoardGPwaRtNor/",DirResult,TestMarker);
  if (system(Form("mkdir -p %s",DirResultGPwaRt))!=0) system(Form("mkdir -p %s",DirResultGPwaRt));
  TCanvas *cpw[4]={},*cpwAll[4]={},*cIntpw[4]={};
  TCanvas *c= new TCanvas("QQ","QQ",500,500);
  TH1F *hPw[64]={},*hPwSE[64]={};
  TH1F *hIntPw[64]={},*hIntPwSE[64]={};
  TH1F *SNR[64]={};
  TF1  *fPw[64]={},*fPwSE[64]={};
  TPDF *pdfcpw   = new TPDF(Form("%sPwidthCompareNormalize.pdf",DirResultGPwaRt));
  TPDF *pdfcpwA  = new TPDF(Form("%sPwidthACompareNormalize.pdf",DirResultGPwaRt));
  TPDF *pdfcSNRpw= new TPDF(Form("%sIntPwidthCompareNormalize.pdf",DirResultGPwaRt));
  PDFCanPage(pdfcpw    ,c ,Form("Title"),1,NumBDt);
  PDFCanPage(pdfcpwA   ,c ,Form("Title"),1,NumBDt);
  PDFCanPage(pdfcSNRpw ,c ,Form("Title"),1,NumBDt);
  double PWDTmp[2][4][16][90] ={};//[SE | Hit][bd][ch][pwd]
  char PWDDataName[3][200]={};
  sprintf(PWDDataName[0],"%s%soutPwHIData.txt"  ,DirOperate,TestMarker);
  sprintf(PWDDataName[1],"%s%soutPwSEData.txt"  ,DirOperate,TestMarker);
  char PWDFitResul[2][200]={};
  sprintf(PWDFitResul[0],"%s%soutPwHIfit.txt"  ,DirOperate,TestMarker);
  sprintf(PWDFitResul[1],"%s%soutPwSEfit.txt"  ,DirOperate,TestMarker);
  ofstream outFH(PWDFitResul[0]);
  ofstream outFS(PWDFitResul[1]);
  for(int ic=0;ic<2;ic++){
    ifstream in(PWDDataName[ic]);
    for(int i0=0,ich=0,ibd=0;i0<NumCht*NumBDt;i0++){
      in>>ibd>>ich;
      ibd = BDINV[ibd];
      for(int ibin=0;ibin<binpw;ibin++){
        double pwtmp=0;
        in>>pwtmp;
        PWDTmp[ic][ibd][ich][ibin] = pwtmp;
      }
    }
    in.close();
  }
  
  //FastStyleStats(0.95,0.95);
  double MPVs[64][3]={};

  
  for(int bd=0;bd<NumBDt;bd++){
    //cout<<DirResultGPwaRt<<"      A         "<<endl;
    char cpwname[30]={};
    char cIntpwname[40]={};
    sprintf(cpwname    ,Form("cpwB%d",BD[bd]));
    sprintf(cIntpwname ,Form("cIntpwB%d",BD[bd]));
    cIntpw[bd] = new TCanvas(cIntpwname  ,"",2160,2160);
    cpw[bd]    = new TCanvas(cpwname     ,"",2160,2160);

    cpwAll[bd] = new TCanvas(Form("%sAll",cpwname),"",2160,2160);
    cpw[bd]->Divide(4,4);
    cIntpw[bd]->Divide(4,4);
    TLegend *LChpw  = new TLegend(0.4,0.60,0.93,0.9);
    setLegendDefault(LChpw,4);

    
    for(int ch=0;ch<NumCht;ch++){
      //cout<<DirResultGPwaRt<<"      B         "<<endl;
      char hpwname[30]={};
      char INTpwname[40]={},INTpwsename[40]={};
      char fpwname[30]={},fpwsename[30]={};
      char SNRname[30]={};
      sprintf(hpwname  ,"hPwB%dCh%d",BD[bd],ch);
      sprintf(INTpwname  ,"hIntPwB%dCh%d",BD[bd],ch);
      sprintf(INTpwsename,"hIntPwseB%dCh%d",BD[bd],ch);
      sprintf(fpwname  ,"fPwB%dCh%d",BD[bd],ch);
      sprintf(fpwsename,"fpwseB%dCh%d",BD[bd],ch);
      sprintf(SNRname,"SNR%dCh%d",BD[bd],ch);
       
      ////cout<<hpwname<<"   "<<hpwselectobject<<"   "<<selectcondition<<endl;

      hPw[ch+bd*16]      = new TH1F(hpwname,hpwname,binpw,0,binpw);
      hPwSE[ch+bd*16]    = new TH1F(hpwname,hpwname,binpw,0,binpw);
      hIntPw[ch+bd*16]   = new TH1F(INTpwname  ,INTpwname  ,binpw,0,binpw);
      hIntPwSE[ch+bd*16] = new TH1F(INTpwsename,INTpwsename,binpw,0,binpw);
      SNR[ch+bd*16]      = new TH1F(SNRname    ,SNRname    ,binpw,0,binpw);
      //fPw[ch+bd*16]   = new TF1(fpwname  ,pwidthHitfit,0,90,7);
      
      int hitcase = 2;
      if(lowstatisticMode) hitcase = 3;
      int casePnum[5] = {9,8,7,7,3};
      if(hitcase==0){
        fPw[ch+bd*16] = new TF1(fpwname,HitFit2lgFun_0,0,50,9);
        HitFit2lgFun_0(fPw[ch+bd*16]);
      }else if(hitcase==1){
        fPw[ch+bd*16] = new TF1(fpwname,N_HitFitFun_0 ,0,50,8);
        N_HitFitFun_0(fPw[ch+bd*16]);
      }else if(hitcase==2){
        fPw[ch+bd*16] = new TF1(fpwname,HitFitFun_0   ,0,50,7);
        HitFitFun_0(fPw[ch+bd*16]);
      }else if(hitcase==3){
        fPw[ch+bd*16] = new TF1(fpwname,HitFitFun_85  ,0,50,7);
        HitFitFun_85(fPw[ch+bd*16]);
      }
      fPwSE[ch+bd*16] = new TF1(fpwsename,landaufun,0,90,3);
      TH1F *hPwAll[2]={hPw[ch+bd*16],hPwSE[ch+bd*16]};
      TH1F *hIntPwAll[2]={hIntPw[ch+bd*16],hIntPwSE[ch+bd*16]};
      TF1  *fPwAll[2]={fPw[ch+bd*16],fPwSE[ch+bd*16]};
      //outPwfit<<bd<<"\t"<<ch;
      for(int ibinpw=0; ibinpw<binpw;ibinpw++){
        hPwAll[0]->SetBinContent(ibinpw+1,PWDTmp[0][bd][ch][ibinpw]);
        hPwAll[1]->SetBinContent(ibinpw+1,PWDTmp[1][bd][ch][ibinpw]);
         //cout<<PWDTmp[0][bd][ch][ibinpw]<<"   "<<PWDTmp[1][bd][ch][ibinpw]<<endl;
      }
      //fPwAll[1]->SetParameters(0.15,0.15,0.23,0.06,17,0.17);
      landaufun(fPwAll[1]);
      
      

      for(int ise=0;ise<3;ise++){
        if(SETightMode) fPwAll[1]->SetParLimits (ise,SPLmin[4][ise],SPLmax[4][ise]);
        fPwAll[1]->SetParameter(ise,SPS[4][ise]);
      }
      if(SETightMode!=true){
        fPwAll[1]->SetParLimits (0,0.14,0.50);
        fPwAll[1]->SetParLimits (1,-2.0,15.0);
        fPwAll[1]->SetParLimits (2,2.00,6.00);
      }
      hPwAll[1]->Fit(fPwAll[1],"NQ");
      fPwAll[1]->SetLineWidth(0+2);
      double BestfitSEMPV =fPwAll[1]->GetParameter(1);
      if(fPwAll[1]->GetChisquare()>0.02){
        int Chi2MinIndex=0;
        float Chi2tmp[25]={},chi2Min=1,Bestfit[3]={};
        for(int iP=0;iP<25;iP++){
          fPwAll[1]->SetParameters(0.4,0+iP,0.3);
          hPwAll[1]->Fit(fPwAll[1],"NQ");
          Chi2tmp[iP]=fPwAll[1]->GetChisquare();
          //cout<<Chi2tmp[iP]<<endl;
          if(Chi2tmp[iP]<chi2Min){
            chi2Min = Chi2tmp[iP];
            Chi2MinIndex = iP;
            for(int ips=0;ips<3;ips++) Bestfit[ips]=fPwAll[1]->GetParameter(ips);
          }
          // if(fPwAll[1]->GetChisquare()<0.05){
            // for(int ips=0;ips<3;ips++) Bestfit[ips]=fPwAll[1]->GetParameter(ips);
            // break;
          // }
        }
        // cout<<Chi2MinIndex<<"    "<<Chi2tmp[Chi2MinIndex]<<endl;
        //Chi2MinIndex++;
        fPwAll[1]->SetParameters(Bestfit[0],Bestfit[1],Bestfit[2]);
        BestfitSEMPV = Bestfit[1];
        hPwAll[1]->Fit(fPwAll[1],"NQ");
      }
      
      for(int icasehit=0;icasehit<casePnum[hitcase];icasehit++) {
        fPwAll[0]->SetParLimits (icasehit,SPS[hitcase][icasehit]/2.,SPS[hitcase][icasehit]*2.);
      }
      fPwAll[0]->SetParLimits (1,BestfitSEMPV-15,BestfitSEMPV-9);
      fPwAll[0]->SetParLimits (4,BestfitSEMPV-3,BestfitSEMPV+3);
      fPwAll[0]->SetParLimits (6,0,SPS[hitcase][6]+3);
      if(lowstatisticMode&&SETightMode){
        BestfitSEMPV = 18;
        fPwAll[0]->SetParLimits (1,BestfitSEMPV-17,BestfitSEMPV-11);
        fPwAll[0]->SetParLimits (4,BestfitSEMPV-3,BestfitSEMPV+3);
      }
      
      hPwAll[0]->Fit(fPwAll[0],"NQ");
      fPwAll[0]->SetLineWidth(0+2);
      int iPMax =10;
      if(lowstatisticMode) iPMax = 20;
      if(fPwAll[0]->GetChisquare()>0.01){
        int Chi2MinIndex=0;
        float Chi2tmp[10]={},chi2Min=1,Bestfit[7]={};
        for(int iP=0;iP<iPMax;iP++){
          fPwAll[0]->SetParLimits (1,BestfitSEMPV-14+iP/2.,BestfitSEMPV-10+iP/2.);
          fPwAll[0]->SetParLimits (4,BestfitSEMPV-2+iP/2.,BestfitSEMPV+2+iP/2.);
          if(lowstatisticMode){
            fPwAll[0]->SetParLimits (1,BestfitSEMPV-26+iP/2.,BestfitSEMPV-22+iP/2.);
            fPwAll[0]->SetParLimits (4,BestfitSEMPV-10+iP/2.  ,BestfitSEMPV-6+iP/2.);
            fPwAll[0]->SetParLimits (6,1,3);
          }
          hPwAll[0]->Fit(fPwAll[0],"NQ");
          Chi2tmp[iP]=fPwAll[0]->GetChisquare();//@@@
          //cout<<Chi2tmp[iP]<<endl;
          if(Chi2tmp[iP]<chi2Min){
            chi2Min = Chi2tmp[iP];
            Chi2MinIndex = iP;
            for(int ips=0;ips<7;ips++) Bestfit[ips]=fPwAll[0]->GetParameter(ips);
          }
        }
        // cout<<Chi2MinIndex<<"    "<<Chi2tmp[Chi2MinIndex]<<endl;
        //Chi2MinIndex++;
        fPwAll[0]->SetParameters(Bestfit[0],Bestfit[1],Bestfit[2],Bestfit[3],Bestfit[4],Bestfit[5],Bestfit[6]);
        hPwAll[0]->Fit(fPwAll[0],"NQ");
      }
      if(fPwAll[0]->GetChisquare()>0.017&&lowstatisticMode){
        
        iPMax = 20;
        int Chi2MinIndex=0;
        float Chi2tmp[10]={},chi2Min=1,Bestfit[7]={};
        for(int iP=0;iP<iPMax;iP++){
          fPwAll[0]->SetParLimits (1,BestfitSEMPV-14+iP/2.,BestfitSEMPV-10+iP/2.);
          fPwAll[0]->SetParLimits (4,BestfitSEMPV-2+iP/2.,BestfitSEMPV+2+iP/2.);
          if(lowstatisticMode){
            fPwAll[0]->SetParLimits (1,BestfitSEMPV-26+iP/2.,BestfitSEMPV-22+iP/2.);
            fPwAll[0]->SetParLimits (4,BestfitSEMPV-10+iP/2.  ,BestfitSEMPV-6+iP/2.);
            fPwAll[0]->SetParLimits (6,1,3);
            fPwAll[0]->SetParLimits (2,3.56/1.4,3.56*1.4);
          }
          hPwAll[0]->Fit(fPwAll[0],"NQ");
          Chi2tmp[iP]=fPwAll[0]->GetChisquare();//@@@
          //cout<<Chi2tmp[iP]<<endl;
          if(Chi2tmp[iP]<chi2Min){
            chi2Min = Chi2tmp[iP];
            Chi2MinIndex = iP;
            for(int ips=0;ips<7;ips++) Bestfit[ips]=fPwAll[0]->GetParameter(ips);
          }
        }
        fPwAll[0]->SetParameters(Bestfit[0],Bestfit[1],Bestfit[2],Bestfit[3],Bestfit[4],Bestfit[5],Bestfit[6]);
        hPwAll[0]->Fit(fPwAll[0],"NQ");
      }
      MPVs[bd*16+ch][0]=fPwAll[0]->GetParameter(1);
      MPVs[bd*16+ch][1]=fPwAll[0]->GetParameter(5);
      MPVs[bd*16+ch][2]=fPwAll[1]->GetParameter(1);
      int fitParNum[2]={casePnum[hitcase],3};
      
      TLegend *LChs  = new TLegend(0.55,0.8,1,0.9);
      setLegendDefault(LChs);
      LChs->AddEntry(hPw[ch+bd*16]  ,Form("Ch-%d All  event",ch),"l");
      LChs->AddEntry(hPwSE[ch+bd*16],Form("Ch-%d Muon event",ch),"F");
      float LFHpy1m,LFHpy1M,LFHpy2m,LFHpy2M;
      LFHpy1M = 0.8;
      LFHpy1m = LFHpy1M-0.04*(fitParNum[0]+2);
      LFHpy2M = LFHpy1m;
      LFHpy2m = LFHpy2M-0.04*(fitParNum[1]+2);
      TPad *LFitHit = drawTF1LegendsCan(fPwAll[0],Form("Ch-%d Hits fit",ch),0.55,LFHpy1m,1,LFHpy1M,c);
      TPad *LFitSE = drawTF1LegendsCan(fPwAll[1],Form("Ch-%d Muon fit",ch),0.55,LFHpy2m,1,LFHpy2M,c);
      
      for(int ifit=0;ifit<2;ifit++){
        if(ifit==0)outFH<<BD[bd]<<"\t"<<ch<<"\t"<<fPwAll[ifit]->GetChisquare();
        else       outFS<<BD[bd]<<"\t"<<ch<<"\t"<<fPwAll[ifit]->GetChisquare();
        for(int iL=0;iL<fitParNum[ifit];iL++){
          //outPwfit  <<"\t"<<fPwAll[ifit]->GetParameter(iL);
          if(ifit==0) outFH<<"\t"<<fPwAll[ifit]->GetParameter(iL)<<"\t"<<fPwAll[ifit]->GetParError(iL);
          else        outFS<<"\t"<<fPwAll[ifit]->GetParameter(iL)<<"\t"<<fPwAll[ifit]->GetParError(iL);
        }if(ifit==0) outFH<<endl;
         else        outFS<<endl;
        //if(lowstatisticMode&&SETightMode) break;
      }
        
      LChpw->AddEntry(hPw[ch+bd*16]  ,Form("Ch-%d All  event",ch),"l");
      LChpw->AddEntry(hPwSE[ch+bd*16],Form("Ch-%d Muon event",ch),"F");
      
      cpw[bd]->cd(2*(ch%4)+13-ch);
      hPw[ch+bd*16]  ->Draw("hist");
      hPw[ch+bd*16]  ->SetStats(0);
      hPwSE[ch+bd*16]->Draw("histsame");
      fPw[ch+bd*16]  ->Draw("lsame");
      if(lowstatisticMode==false||SETightMode==false) fPwSE[ch+bd*16]->Draw("lsame");
      LChs->Draw("same");
      LFitHit->Draw("same");
      
      if(lowstatisticMode==false||SETightMode==false) LFitSE->Draw("same");
      
      cIntpw[bd]->cd(2*(ch%4)+13-ch);
      double percentgeThershold = 0.035;
      for(int ibinpw=0; ibinpw<binpw;ibinpw++){
        double tmpInt[2]={};
        tmpInt[0]=hPw[ch+bd*16]  ->Integral(ibinpw,binpw);
        tmpInt[1]=hPwSE[ch+bd*16]->Integral(ibinpw,binpw);
        hIntPwAll[0]            ->Fill(ibinpw,tmpInt[0]);
        hIntPwAll[1]            ->Fill(ibinpw,tmpInt[1]);
        if(tmpInt[0]>percentgeThershold||
           tmpInt[1]>percentgeThershold)
            SNR[ch+bd*16]    ->Fill(ibinpw,tmpInt[1]/tmpInt[0]);//@@
      }
      double xsnrMax=SNR[ch+bd*16]->GetMaximumBin()-0.5;
      double xsnr[2]={xsnrMax, xsnrMax}; double ysnr[2]={100, -100};
      TGraph *SNRMax = new TGraph(2,xsnr,ysnr);
      
      SNR[ch+bd*16]->Draw("histsame");
      hIntPwAll[0]->Draw("histsame");
      hIntPwAll[1]->Draw("histsame");
      SNRMax->Draw("lsame");
      hIntPwAll[0]->SetLineColor(1);
      hIntPwAll[1]->SetLineColor(2);
      SNR[ch+bd*16]->SetLineColor(3);
      SNRMax->SetLineStyle(2);
      SNRMax->SetLineColorAlpha(1,0.5);
      setTitle(SNR[ch+bd*16]  ,SNRname,"pwidth cut(100ns)","Ratio");
      setAxisRangeUser(SNR[ch+bd*16],0,50,0,3);
      TLegend *LSNR  = new TLegend(0.3,0.7,0.95,0.9);
      LSNR->SetLineColor(0);
      LSNR->AddEntry(hIntPwAll[9] ,Form("Ch-%d Cumulative rate of Hit ",ch),"l");
      LSNR->AddEntry(hIntPwAll[1] ,Form("Ch-%d Cumulative rate of Muon",ch),"l");
      LSNR->AddEntry(SNR[ch+bd*16],Form("Ch-%d S/N (Normolize scale)  ",ch),"l");
      LSNR->AddEntry(SNRMax       ,Form("Ch-%d S/N Max: %.f #pm 50ns ",ch,xsnrMax*100),"l");
      LSNR->AddEntry(""           ,Form("    PS: In case S or N rate>%.1f%% ",percentgeThershold*100),"h");
      LSNR->Draw("same");
      cpwAll[bd]->cd();
      if (ch==0){
        hPw[ch+bd*16]->Draw("hist");
        LChpw->Draw("same");
      }
      else       hPw[ch+bd*16]->Draw("histsame");
      hPwSE[ch+bd*16]->Draw("histsame");
        
      setTitle(hPw[ch+bd*16]  ,hpwname,"pwidth/pwidthScaleFactor(100ns)","Ratio");
      setTitle(hPwSE[ch+bd*16],hpwname,"pwidth/pwidthScaleFactor(100ns)","Ratio");
      
      setAxisRangeUser(hPw[ch+bd*16],0,60,0,0.2);
      hPw[ch+bd*16]  ->SetLineWidth(0.5);
      hPwSE[ch+bd*16]->SetLineWidth(0.5);
      hPw[ch+bd*16]  ->SetLineColor(colorArr[ch]);
      fPw[ch+bd*16]  ->SetLineColor(colorArr[ch]);
      fPwSE[ch+bd*16]->SetLineColor(colorArr[ch]);
      fPwAll[1]->SetLineWidth(1);
      fPwAll[0]->SetLineWidth(1.5);
      hPwSE[ch+bd*16]->SetLineColorAlpha(colorArr[ch],0.1);
      hPwSE[ch+bd*16]->SetFillColorAlpha(colorArr[ch],0.2);
      
      //cpw[bd]->cd(2*(ch%4)+13-ch);
      //c->cd();
    }
    PDFCanPage(pdfcSNRpw,cIntpw[bd],Form("MTB-%d",BD[bd]),bd+1,NumBDt);
    //cout<<DirResultGPwaRt<<"      E         "<<endl;
    //cout<<cpwname<<"      E         "<<endl;
    char namessss[200]={},namesintsss[200]={};
    sprintf(namessss,"%s%sNormal",DirResultGPwaRt,cpwname);
    sprintf(namesintsss,"%sInt%sNormal",DirResultGPwaRt,cpwname);
    DrawPdfPng(cpw[bd],namessss);
    DrawPdfPng(cIntpw[bd],namesintsss);
    
    PDFCanPage(pdfcpw   ,cpw[bd]   ,Form("MTB-%d",BD[bd]),bd+1,NumBDt);
    PDFCanPage(pdfcpwA  ,cpwAll[bd],Form("MTB-%d",BD[bd]),bd+1,NumBDt);
    // cout<<DirResultGPwaRt<<"      E         "<<endl;
  }
  
  for(int i=0;i<64;i++){
    //cout<<MPVs[i][0]<<"\t"<<MPVs[i][1]<<"\t"<<MPVs[i][2]<<endl;
  }
  
  optime.Stop();
  optime.Print();
  
  
  
} */

/*
  //data hist create
  TH1F *hPw3c  = new TH1F("hPw3c","",180,0,180);
  TH1F *hPw5c  = new TH1F("hPw5c","",180,0,180);
  TH1F *hPw4c  = new TH1F("hPw4c","",180,0,180);
  TH2F *hPw1   = new TH2F("hPw1","",16,0,16,180,0,180);//10min
  TH2F *hPw3   = new TH2F("hPw3","",16,0,16,180,0,180);//10min
  TH2F *hPw5   = new TH2F("hPw5","",16,0,16,180,0,180);//10min
  TH2F *hPw4   = new TH2F("hPw4","",16,0,16,180,0,180);//10min
  TH1F *hPWB1      = new TH1F("PwidthOfMTB1","",180,0,180);
  TH1F *hPWB3      = new TH1F("PwidthOfMTB3","",180,0,180);
  TH1F *hPWB5      = new TH1F("PwidthOfMTB5","",180,0,180);
  TH1F *hPWB4      = new TH1F("PwidthOfMTB4","",180,0,180);
  TH2F *hPWMod     = new TH2F("hPWMod","",BinOnTime,unixtimei,unixtimef,60,0,180);
  TH2F *hPWvT1   = new TH2F("hPWvT1","",BinOnTime,unixtimei,unixtimef,60,0,180);
  TH2F *hPWvT3   = new TH2F("hPWvT3","",BinOnTime,unixtimei,unixtimef,60,0,180);
  TH2F *hPWvT5   = new TH2F("hPWvT5","",BinOnTime,unixtimei,unixtimef,60,0,180);
  TH2F *hPWvT4   = new TH2F("hPWvT4","",BinOnTime,unixtimei,unixtimef,60,0,180);
  TH1F *hPWB2921      = new TH1F("hPWB2921","",180,0,180);
  TH1F *hPWB2923      = new TH1F("hPWB2923","",180,0,180);
  TH1F *hPWB2925      = new TH1F("hPWB2925","",180,0,180);
  TH1F *hPWB2924      = new TH1F("hPWB2924","",180,0,180);
  TH1F *hPWB2881      = new TH1F("hPWB2881","",180,0,180);
  TH1F *hPWB2883      = new TH1F("hPWB2883","",180,0,180);
  TH1F *hPWB2885      = new TH1F("hPWB2885","",180,0,180);
  TH1F *hPWB2884      = new TH1F("hPWB2884","",180,0,180);
  TH1F *hPWB2901      = new TH1F("hPWB2901","",180,0,180);
  TH1F *hPWB2903      = new TH1F("hPWB2903","",180,0,180);
  TH1F *hPWB2905      = new TH1F("hPWB2905","",180,0,180);
  TH1F *hPWB2904      = new TH1F("hPWB2904","",180,0,180);
  TH1F *hPwc[4]    ={hPw1c,hPw3c,hPw5c,hPw4c};
  TH2F *hPw [4]    ={hPw1,hPw3,hPw5,hPw4};//10min
  TH1F *hPWB[4]    ={hPWB1,hPWB3,hPWB5,hPWB4};
  TH2F *hPWvT[4]= {hPWvT1,hPWvT3,hPWvT5,hPWvT4};
  TH1F *hPWB292[4]   = {hPWB2921,hPWB2923,hPWB2925,hPWB2924};
  TH1F *hPWB290[4]   = {hPWB2901,hPWB2903,hPWB2905,hPWB2904};
  TH1F *hPWB288[4]   = {hPWB2881,hPWB2883,hPWB2885,hPWB2884};
  //cout<<"bins:\t"<<BinOnTime<<endl;
  float maxcon = 0;
  float maxconl = 0,minconl = 10;
  int chipt0 = 0;
  int chipt = 0;
  int ev0 = 0, ev1 = 0;
  int dt = 0;
  int gap = 0;
  float chipfre  = 0;
  int chipNum = 0;
  int Day0 = 0;
  int Month0=0;
  int Year0=0;
  //evs = 1600;
  data.GetEntry(0);
  unixtime  = data.GetLong64("unixtime");
  tYear     = data.GetInt("tYear");
  tMonth    = data.GetInt("tMonth");
  tDate     = data.GetInt("tDate");
  tHour     = data.GetInt("tHour");
  tMinute   = data.GetInt("tMinute");
  tSecond   = data.GetInt("tSecond");
  for (Long64_t ev = 0; ev <evs; ++ev) {
    data.GetEntry(ev);
    unixtime = data.GetLong64("unixtime");
    tYear    = data.GetInt("tYear");
    tMonth   = data.GetInt("tMonth");
    tDate    = data.GetInt("tDate");
    tHour    = data.GetInt("tHour");
    tMinute  = data.GetInt("tMinute");
    tSecond  = data.GetInt("tSecond");
    nHits    = data.GetInt("nHits");
    board    = data.GetPtrInt("board");
    channel  = data.GetPtrInt("channel");
    pwidth   = data.GetPtrInt("pwidth");

    if (gap == 0 && dt == 0){
      ev0 = ev;
      dt = unixtime-chipt;
      chipt0 = unixtime;
      // //cout<<"chipt0:\t"<<chipt0<<"\tdt:\t"<<dt<<"gap:\t"<<gap<<endl;
      // //cout<<Form("ST:%d_%d_%d_%d:%d'%d\"",tYear,tMonth,tDate,tHour,tMinute,tSecond)<<endl;
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
    ////cout<<"\tdt:\t"<<dt<<endl;
    if (dt>breaktime||ev+1==evs||Day0!=tDate){
      if (Day0!=tDate) ev--;
      chipfre = 1./gap;
      // //cout<<"chipt0:\t"<<chipt0<<"\tchipt1:\t"<<chipt<<endl;
      // //cout<<"dt:\t"<<dt<<"\tgap:\t"<<gap<<"\tFre:\t"<<chipfre<<endl;
      out<<"chipt0:\t"<<chipt0<<"\tchipt1:\t"<<chipt<<endl;
      out<<"dt:\t"<<dt<<"\tgap:\t"<<gap<<"\tFre:\t"<<chipfre<<endl;
      ev1 = ev;
      wei+=gap;
      gap = 0;
      dt = 0;
      chipt = 0;
      chipt0 = 0;
      chipNum++;
      out<<Form("ED:%d_%d_%d_%d:%d'%d\"",tYear,tMonth,tDate,tHour,tMinute,tSecond)<<endl<<"------------------------------"<<endl;

    }
    continue;
  }
  //cout<<"chipNum:\t"<<chipNum<<endl;
  //cout<<"OPtime:\t"<<wei<<"\tfrequency:\t"<<1.0/wei<<endl;
  out<<"chipNum:\t"<<chipNum<<endl;
  out<<"OPtime:\t"<<wei<<"\tfrequency:\t"<<1.0/wei<<endl;
  wei = 1.0/wei;
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
    //seq       = data.GetPtrInt("seq");
    pwidth    = data.GetPtrInt("pwidth");
    if(timelastset){
      if(unixtime>unixtimef){
        //cout<<"oJ8K"<<endl;
        //throw;
        break;
        
      }
    }
    vector<int> ihits;
    int i1 = 0;
    for(int i1 = 0; i1<nHits ;++i1){
      for(int i5=0;i5<4;i5++){
        if(board[i1]==BD[i5]){
          hPw[i5]->Fill(channel[i1],pwidth[i1]);
          hPWB[i5]->Fill(pwidth[i1]);
          hPWvT[i5]->Fill(unixtime,pwidth[i1]);
          if(unixtime<TS[1]-1)                  hPWB292[i5]->Fill(pwidth[i1]);
          if(unixtime>TS[1]-1|unixtime<TS[2]-1) hPWB288[i5]->Fill(pwidth[i1]);
          if(unixtime>TS[2]-1)                  hPWB290[i5]->Fill(pwidth[i1]);
          break;
        }
      }
    }
  }
  for(int i1 = 0; i1<BinNumOnCha ;++i1){
    for(int i2 = 0; i2<100 ;++i2){
      if(maxcon<hPw1->GetBinContent(i2,i1)) maxcon = hPw1->GetBinContent(i2,i1);
      if(maxcon<hPw3->GetBinContent(i2,i1)) maxcon = hPw3->GetBinContent(i2,i1);
      if(maxcon<hPw5->GetBinContent(i2,i1)) maxcon = hPw5->GetBinContent(i2,i1);
      if(maxcon<hPw4->GetBinContent(i2,i1)) maxcon = hPw4->GetBinContent(i2,i1);
    }
  }
  for(int i5=0;i5<4;i5++){
    for(int i1 = 0; i1<BinNumOnCha/4 ;++i1){
      if(maxconl<hPw[i5]->GetBinContent(i1+1)) maxconl = hPw[i5]->GetBinContent(i1+1);
      if(minconl<hPw[i5]->GetBinContent(i1+1)) minconl = hPw[i5]->GetBinContent(i1+1);
    }
    for(int i1 = 1; i1<181 ;++i1){
      hPwc[i5]->SetBinContent(i1,hPw[i5]->GetBinContent(1,i1));
    }
  }
  
  
  //cout<<"maxcontent:\t"<<maxcon<<endl;
  out<<"maxcontent:\t"<<maxcon<<endl;
  //cout<<"mincontentLa:\t"<<minconl<<"maxcontentLa:\t"<<maxconl<<endl;
  out<<"mincontentLa:\t"<<minconl<<"maxcontentLa:\t"<<maxconl<<endl;

  TCanvas *c1 = new TCanvas("c1","",1100,1000);
  TCanvas *c2 = new TCanvas("c2","",1000,1000);
  TCanvas *c4 = new TCanvas("c4","",2000,1000);
  TCanvas *c5 = new TCanvas("c5","",3840,2160);
  TCanvas *c6 = new TCanvas("c6","",3840,2160);
  TCanvas *c7 = new TCanvas("c7","",3840,2160);
  TCanvas *c8 = new TCanvas("c8","",3840,2160);
  TCanvas *c9 = new TCanvas("c9","",ndivise*60,2160);
  TCanvas *c10= new TCanvas("c10","",2000,1000);
  TH1F *hMod  = new TH1F("hMod","",240,0,240);
  hMod->SetStats(0);
  hMod->SetTitle("");
  hMod->GetXaxis()->SetTitle("Pwidth(100ns)");
  hMod->GetYaxis()->SetTitle("times(Normolize)");
  hMod->GetYaxis()->SetTitleOffset(1.4);
  TH2F *h2DMod  = new TH2F("h2DMod","",4,0,4,4,0,4);
  h2DMod->SetStats(0);
  h2DMod->SetTitle("");
  h2DMod->GetXaxis()->SetTitle("X");
  h2DMod->GetYaxis()->SetTitle("Y");
  h2DMod->GetZaxis()->SetTitle("Rate[s^{-1}]");
  h2DMod->GetYaxis()->SetTitleOffset(1.4);
  h2DMod->GetZaxis()->SetTitleOffset(0);
  h2DMod->GetXaxis()->SetLabelSize(0.07);
  h2DMod->GetYaxis()->SetLabelSize(0.07);
  h2DMod->SetMarkerSize(3);

  TCanvas *c3 = new TCanvas("c3","",1200*2*2,1000*2*2);
  c3->Divide(2,2); 
  c3->cd(1);
  c3->SetGridx(); 
  TH2F *hXYZMod  = new TH2F("hXYZMod","",16,0,16,180,0,180);
  hXYZMod->GetXaxis()->SetTitle("Channel");
  hXYZMod->GetYaxis()->SetTitle("Pwidth(100ns)");
  hXYZMod->GetZaxis()->SetTitle("Number");
  hXYZMod->GetXaxis()->SetTitleOffset(1.2);
  hXYZMod->GetYaxis()->SetTitleOffset(1.2);
  hXYZMod->GetZaxis()->SetTitleOffset(1.4);
  hXYZMod->SetLineColor(0);
  hXYZMod->SetStats(0); 

  TLegend *L3D1  = new TLegend(0.3,0.91,0.8,1);
  TLegend *L3D3  = new TLegend(0.3,0.91,0.8,1);
  TLegend *L3D5  = new TLegend(0.3,0.91,0.8,1);
  TLegend *L3D4  = new TLegend(0.3,0.91,0.8,1);
  TLegend *L3D[4]= {L3D1,L3D3,L3D5,L3D4};
  for(int i5=0;i5<4;i5++){
    L3D[i5]->SetFillColor(0);
    L3D[i5]->SetLineColor(0);
    L3D[i5]->SetFillColorAlpha (0, 0.00);
    L3D[i5]->AddEntry("hMod",Form("MTB-%d",BD[i5]),"");
  }
  TText *ZeroZero = new TText(3.5,.5,"0");
  ZeroZero->SetTextAlign(22);  ZeroZero->SetTextColor(1); 
  ZeroZero->SetTextSize(0.06);
  TText *ThreThre = new TText(.5,.5,"3");
  ThreThre->SetTextAlign(22);  ThreThre->SetTextColor(1); 
  ThreThre->SetTextSize(0.06);
  
  hPw1c->SetLineColor(1);
  hPw3c->SetLineColor(kBlue);
  hPw5c->SetLineColor(kGreen-2);
  hPw4c->SetLineColor(kOrange+1);
  hPw1c->SetLineWidth(2);
  hPw3c->SetLineWidth(2);
  hPw5c->SetLineWidth(2);
  hPw4c->SetLineWidth(2);
  // for (int i1=0;i1<16;i1++){
    // hMod->GetXaxis()->SetBinLabel(i1+1 ,Form("%d",i1));
  // }
  //cout<<"The during days is"<<(Dayf-Dayi)<<endl;
  out<<"The during days is"<<(Dayf-Dayi)<<endl;
  c1->Divide(2,2); 
  for(int i5=0;i5<4;i5++){
    c1->cd(i5+1)->SetTicks(1,1);
    hXYZMod->Draw();
    hPw[i5]->Draw("colzsame");
    L3D[i5]->Draw();
    hPw[i5]->GetZaxis()->SetRangeUser(0,9500);
  }
  hXYZMod->GetYaxis()->SetRangeUser(0,160);
  hXYZMod->GetXaxis()->SetRangeUser(0,16);
  hXYZMod->GetZaxis()->SetRangeUser(0,9500);

  c1->Print(Form("./Result/PWidthColz_%d_%d_%dto%d_%d_%dcom.png",2021,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),2021,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c1->Print(Form("./Result/PWidthColz_%d_%d_%dto%d_%d_%dcom.pdf",2021,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),2021,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  
  c1->Close();
  c2->cd();
  c2->SetTicks(1,1);
  hMod->Draw();
  for(int i5=0;i5<4;i5++){
    hPwc[i5]->Sumw2();
    hPwc[i5]->Scale(1./hPwc[i5]->Integral(-1,-1));
    hPwc[i5]->Draw("hsame");
  }
  hMod->GetXaxis()->SetRangeUser(0,180);
  hMod->GetYaxis()->SetRangeUser(0,0.08);
  TLegend *LPwc = new TLegend(0.7,0.7,0.85,0.85);
  LPwc->SetFillColor(0);
  LPwc->SetLineColor(0);
  LPwc->SetFillColorAlpha (0, 0.00);
  LPwc->AddEntry("hPw1c","MTB-1","le");
  LPwc->AddEntry("hPw3c","MTB-3","le");
  LPwc->AddEntry("hPw5c","MTB-5","le");
  LPwc->AddEntry("hPw4c","MTB-4","le");
  LPwc->Draw();
  c2->Print(Form("./Result/PWidthCompaCh1boxc180_%d_%d_%dto%d_%d_%dcom.png",2021,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),2021,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  hMod->GetXaxis()->SetRangeUser(0,40);
  c2->Print(Form("./Result/PWidthCompaCh1boxc40_%d_%d_%dto%d_%d_%dcom.png",2021,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),2021,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));;
  c4->Divide(2,1);
  c4->Print(Form("./Result/PwidthCompChBox_%d_%d_%dto%d_%d_%dcom.pdf(",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));


  c4->cd(1);
  hMod->Draw();
  for (int i1=0;i1<BinNumOnCha/4;i1++){
    for(int i5=0;i5<4;i5++){
      for(int i2=1;i2<181;i2++){
        hPwc[i5]->SetBinContent(i2,hPw[i5]->GetBinContent(i1+1,i2));
      }
      hPwc[i5]->Sumw2();
      hPwc[i5]->Scale(1./hPwc[i5]->Integral(-1,-1));
      hPwc[i5]->Draw("hsame");
    }
    LPwc->Draw();
    hMod->GetYaxis()->SetRangeUser(0,0.1);
    hMod->GetXaxis()->SetRangeUser(0,40);
    hMod->SetTitle(Form("PWidthCompare For Channel %d cut on 40",i1));
    TH1F *hMod1 = (TH1F*) hMod->Clone();
    c4->cd(2);
    hMod1->Draw();
    for(int i5=0;i5<4;i5++) hPwc[i5]->Draw("hsame");
    LPwc->Draw();
    hMod1->GetYaxis()->SetRangeUser(0,0.1);
    hMod1->GetXaxis()->SetRangeUser(0,180);
    hMod1->SetTitle(Form("PWidthCompare For Channel %d cut on 180",i1));
    c4->Print(Form("./Result/PwidthCompChBox_%d_%d_%dto%d_%d_%dcom.pdf",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
    for(int i5=0;i5<4;i5++) hPwc[i5]->Reset("ICESM");
  }  
  c4->cd();
  hMod->Reset("ICESM");
  hMod->SetTitle("The End");
  hMod->Draw();
  c4->Print(Form("./Result/PwidthCompChBox_%d_%d_%dto%d_%d_%dcom.pdf)",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  
   
  c9->Divide(1,4);
  
  for(int i5=0;i5<4;i5++){
    c9->cd(i5+1);
    hPWvT[i5]->SetStats(0);
    hPWvT[i5]->SetTitle(Form("PWidth VS time on MTB%d",BD[i5]));
    hPWvT[i5]->GetXaxis()->SetTitle("unixtime(half hour/bin)");
    hPWvT[i5]->GetYaxis()->SetTitle("PWidth(100ns)");
    hPWvT[i5]->GetZaxis()->SetTitle("times");
    // hPWvT[i5]->GetYaxis()->SetTitleOffset(1.4);
    // hPWvT[i5]->GetZaxis()->SetTitleOffset(0);
    // hPWvT[i5]->GetXaxis()->SetLabelSize(0.07);
    // hPWvT[i5]->GetYaxis()->SetLabelSize(0.07);
    hPWvT[i5]->Draw("colz");
  }
  c9->Print(Form("./Result/PWvT160%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c9->Print(Form("./Result/PWvT160%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  hPWvT[0]->GetYaxis()->SetRangeUser(0,80);
  hPWvT[1]->GetYaxis()->SetRangeUser(0,80);
  hPWvT[2]->GetYaxis()->SetRangeUser(0,80);
  hPWvT[3]->GetYaxis()->SetRangeUser(0,80);

  c9->Print(Form("./Result/PWvT080%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c9->Print(Form("./Result/PWvT080%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  
  
  
  ifstream inTT1("/home/yusiang/tunnel/shimen/HK/Data/RVT1dat.txt");
  ifstream inTT3("/home/yusiang/tunnel/shimen/HK/Data/RVT3dat.txt");
  ifstream inTT5("/home/yusiang/tunnel/shimen/HK/Data/RVT5dat.txt");
  ifstream inTT4("/home/yusiang/tunnel/shimen/HK/Data/RVT4dat.txt");
  float b=0,d=0;
  int i3[4] ={0,0,0,0};
  vector<float> tin1v,tin3v,tin5v,tin4v;
  vector<float> temin1v,temin3v,temin5v,temin4v;
   for(;inTT1>>d>>b;i3[0]++){
    tin1v.push_back(d*1.0);
    temin1v.push_back(b);
  }for(;inTT3>>d>>b;i3[1]++){
    tin3v.push_back(d*1.0);
    temin3v.push_back(b);
  }for(;inTT5>>d>>b;i3[2]++){
    tin5v.push_back(d*1.0);
    temin5v.push_back(b);
  }for(;inTT4>>d>>b;i3[3]++){
    tin4v.push_back(d*1.0);
    temin4v.push_back(b);
  }
  float tin1[tin1v.size()],tin3[tin3v.size()],tin5[tin5v.size()],tin4[tin4v.size()];
  float temin1[tin1v.size()],temin3[tin3v.size()],temin5[tin5v.size()],temin4[tin4v.size()];
  float *tin[4]={tin1,tin3,tin5,tin4};
  float *temin[4]={temin1,temin3,temin5,temin4};
  //TH1F *tVT  = new TH1F("tVT","",BinOnTime,unixtimei,unixtimef);

  for(int i4=0;i4<i3[0];i4++){
    tin[0][i4]=tin1v[i4];
    temin[0][i4]=temin1v[i4];///50.;
  }
  for(int i4=0;i4<i3[1];i4++){
    tin[1][i4]=tin3v[i4];
    temin[1][i4]=temin3v[i4];///50.;
  }
  for(int i4=0;i4<i3[2];i4++){
    tin[2][i4]=tin5v[i4];
    temin[2][i4]=temin5v[i4];///50.;
  }
  for(int i4=0;i4<i3[3];i4++){
    tin[3][i4]=tin4v[i4];
    temin[3][i4]=temin4v[i4];///50.;
  }

  TGraph *Gtvt1  = new TGraph(tin1v.size(),tin1,temin1);
  TGraph *Gtvt3  = new TGraph(tin3v.size(),tin3,temin3);
  TGraph *Gtvt5  = new TGraph(tin5v.size(),tin5,temin5);
  TGraph *Gtvt4  = new TGraph(tin4v.size(),tin4,temin4);
  TGraph *Gtvt[4]= {Gtvt1,Gtvt3,Gtvt5,Gtvt4};
  c5->cd();
  for(int i5=0;i5<4;i5++){
    Gtvt[i5]->Draw("Apl");
    Gtvt[i5]->SetTitle(Form("MTB%d",BD[i5]));
    c5->Print(Form("./Result/TempVsTimeTGB%d_%d_%d_%dto%d_%d_%dcom.pdf)",BD[i5],Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  }
  for(int i5=0;i5<3;i5++){
    Gtvt[i5]->Draw("plsame");
  }
  Gtvt4->SetTitle(Form("MTB1354"));
  c5->Print(Form("./Result/TempVsTimeTG_%d_%d_%dto%d_%d_%dcom.pdf)",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  int tinsize[4]= {tin1v.size(),tin3v.size(),tin5v.size(),tin4v.size()};
  TH2F *TPWVTM   = new TH2F("TPWVTM", "",50,0,50,180,0,180);
  TH2F *TPWVT1  = new TH2F("TPWVT1","",50,0,50,180,0,180);
  TH2F *TPWVT3  = new TH2F("TPWVT3","",50,0,50,180,0,180);
  TH2F *TPWVT5  = new TH2F("TPWVT5","",50,0,50,180,0,180);
  TH2F *TPWVT4  = new TH2F("TPWVT4","",50,0,50,180,0,180);
  TH1F *TtVT1   = new TH1F("TtVT1","",50,0,50);
  TH1F *TtVT3   = new TH1F("TtVT3","",50,0,50);
  TH1F *TtVT5   = new TH1F("TtVT5","",50,0,50);
  TH1F *TtVT4   = new TH1F("TtVT4","",50,0,50);
  TH2F *TPWVT[4]={TPWVT1,TPWVT3,TPWVT5,TPWVT4};
  TH1F *TtVT[4]={TtVT1,TtVT3,TtVT5,TtVT4};
  unixtime=0;
  float tmp[4]={0,0,0,0};
  data.GetEntry(0);
  unixtime  = data.GetLong64("unixtime");
  for (int i5=0;i5<4;i5++){
    if (unixtime<tin[i5][0]) tmp[i5]=temin1[0];
  }
  for(int evp=0;evp <evs;evp++){
    data.GetEntry(evp);
    unixtime  = data.GetLong64("unixtime");
    pwidth    = data.GetPtrInt("pwidth");
    nHits     = data.GetInt("nHits");
    board     = data.GetPtrInt("board");
    for(int i5=0;i5<4;i5++){
      for(int i4=0;i4<tinsize[i5]-1;i4++){
        if (unixtime<tin[i5][i4+1]&&unixtime>tin[i5][i4]){
          tmp[i5]=temin[i5][i4];
          ////cout<<BD[i5]<<"\t"<<tmp[i5]<<endl;
          break;
        }
      }
    }
    for(int i1 = 0; i1<nHits ;++i1){
      for(int i5=0;i5<4;i5++){
        if(board[i1]==BD[i5]){
          TPWVT[i5]->Fill(tmp[i5],pwidth[i1]);
          TtVT[i5]->Fill(tmp[i5]);
          break;
        }  ////cout<<tmp[0]<<"\t"<<pwidth[i1]<<endl;
      }
    }
  }
  

  TLegend *TPWVT1Title  = new TLegend(0.25,0.91,0.75,1);
  TLegend *TPWVT3Title  = new TLegend(0.25,0.91,0.75,1);
  TLegend *TPWVT5Title  = new TLegend(0.25,0.91,0.75,1);
  TLegend *TPWVT4Title  = new TLegend(0.25,0.91,0.75,1);
  TLegend *TPWVTTitle[4]={TPWVT1Title,TPWVT3Title,TPWVT5Title,TPWVT4Title};
  for (int i5=0;i5<4;i5++){
    TPWVTTitle[i5]->SetFillColor(0);
    TPWVTTitle[i5]->SetLineColor(0);
    TPWVTTitle[i5]->SetFillColorAlpha (0,0.0);
    TPWVTTitle[i5]->AddEntry("hMod",Form("Temperature Vs PWidth on MTB%d",BD[i5]),"");
  }

  TPWVTM->SetStats(0);
  TPWVTM->GetXaxis()->SetTitle("Temtereture(#circC)");
  TPWVTM->GetYaxis()->SetTitle("Pwidth(100ns)");
  TPWVTM->SetTitle("");
  c5->Clear();
  c5->Divide(2,2);

  for (int i5=0;i5<4;i5++){
    c5->cd(i5+1);
    TPWVTM->Draw("colz");
    TPWVT[i5]->Draw("colzsame");
    TPWVTTitle[i5]->Draw();
    TPWVT[i5]->GetZaxis()->SetRangeUser(0,320000);
  }
  TPWVTM->GetXaxis()->SetRangeUser(10,50);
  c5->Print(Form("./Result/PWVT180Colz%d_%d_%dto%d_%d_%dcom.pdf",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c5->Print(Form("./Result/PWVT180Colz%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c5->cd(1)->SetLogz();
  c5->cd(2)->SetLogz();
  c5->cd(3)->SetLogz();
  c5->cd(4)->SetLogz();
  c5->Print(Form("./Result/PWVTLg180Colz%d_%d_%dto%d_%d_%dcom.pdf",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c5->Print(Form("./Result/PWVTLg180Colz%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));

  
  TH2F *TPWVT1N = (TH2F*) TPWVT1->Clone();
  TH2F *TPWVT3N = (TH2F*) TPWVT3->Clone();
  TH2F *TPWVT5N = (TH2F*) TPWVT5->Clone();
  TH2F *TPWVT4N = (TH2F*) TPWVT4->Clone();
  TProfile *hprof1  = new TProfile("hprof1","",50,0,50,0,180);
  TProfile *hprof3  = new TProfile("hprof3","",50,0,50,0,180);
  TProfile *hprof5  = new TProfile("hprof5","",50,0,50,0,180);
  TProfile *hprof4  = new TProfile("hprof4","",50,0,50,0,180);

  for(int i1=1;i1<181;i1++){
    for(int i2=1;i2<51;i2++){
      if(TPWVT1->GetBinContent(i2,i1)==0){
        TPWVT1N->SetBinContent(i2,i1,0);
        TPWVT3N->SetBinContent(i2,i1,0);
        TPWVT5N->SetBinContent(i2,i1,0);
        TPWVT4N->SetBinContent(i2,i1,0);
      }else{
        TPWVT1N->SetBinContent(i2,i1,TPWVT1->GetBinContent(i2,i1)/TtVT1->GetBinContent(i2));
        TPWVT3N->SetBinContent(i2,i1,TPWVT3->GetBinContent(i2,i1)/TtVT3->GetBinContent(i2));
        TPWVT5N->SetBinContent(i2,i1,TPWVT5->GetBinContent(i2,i1)/TtVT5->GetBinContent(i2));
        TPWVT4N->SetBinContent(i2,i1,TPWVT4->GetBinContent(i2,i1)/TtVT4->GetBinContent(i2));
        hprof1->Fill(i2-1,i1,TPWVT1->GetBinContent(i2,i1)/TtVT1->GetBinContent(i2));
        hprof3->Fill(i2-1,i1,TPWVT3->GetBinContent(i2,i1)/TtVT3->GetBinContent(i2));
        hprof5->Fill(i2-1,i1,TPWVT5->GetBinContent(i2,i1)/TtVT5->GetBinContent(i2));
        hprof4->Fill(i2-1,i1,TPWVT4->GetBinContent(i2,i1)/TtVT4->GetBinContent(i2));
        ////cout<<hprof1->getBinContent(i2,i1)<<endl;
        
      }
    }
  }
  
  TH2F *TPWVTN[4]   = {TPWVT1N,TPWVT3N,TPWVT5N,TPWVT4N};
  TProfile *hprof[4]= {hprof1,hprof3,hprof5,hprof4};

  c6->Clear();
  c6->Divide(2,2);
  for (int i5=0;i5<4;i5++){
    c6->cd(i5+1);
    TPWVTM->Draw("colz");
    TPWVTN[i5]->Draw("colzsame");
    TPWVTTitle[i5]->Draw();
    TPWVTN[i5]->GetZaxis()->SetRangeUser(0,0.1);
    TPWVTN[i5]->GetXaxis()->SetRangeUser(11,48);
  }
  TPWVTM->GetXaxis()->SetRangeUser(10,50);
  c6->Print(Form("./Result/PWVTNor180Colz%d_%d_%dto%d_%d_%dcom.pdf",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c6->Print(Form("./Result/PWVTNor180Colz%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));

  c6->cd(1)->SetLogz();
  c6->cd(2)->SetLogz();
  c6->cd(3)->SetLogz();
  c6->cd(4)->SetLogz();
  c6->Print(Form("./Result/PWVTNorLg180Colz%d_%d_%dto%d_%d_%dcom.pdf",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c6->Print(Form("./Result/PWVTNorLg180Colz%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));

 
  TLegend *Lprof1  = new TLegend(0.75,0.91,1,1);
  TLegend *Lprof3  = new TLegend(0.75,0.91,1,1);
  TLegend *Lprof5  = new TLegend(0.75,0.91,1,1);
  TLegend *Lprof4  = new TLegend(0.75,0.91,1,1);
  TLegend *Lprof[4]={Lprof1,Lprof3,Lprof5,Lprof4};
  for (int i5=0;i5<4;i5++){
    Lprof[i5]->SetFillColor(0);
    Lprof[i5]->SetLineColor(0);
    Lprof[i5]->SetFillColorAlpha (0, 0.0);
    Lprof[i5]->AddEntry("hprof1","Mean& 1#sigma bar","ple");
  }
  
  c6->Clear();
  c6->Divide(2,2);
  for (int i5=0;i5<4;i5++){
    c6->cd(i5+1);
    TPWVTM->Draw("colz");
    TPWVTN[i5]->Draw("colzsame");
    hprof[i5]->Draw("EPsame");
    TPWVTTitle[i5]->Draw();
    Lprof[i5]->Draw();
    TPWVTN[i5]->GetZaxis()->SetRangeUser(0,0.08);
    TPWVTN[i5]->GetXaxis()->SetRangeUser(11,48);
    hprof[i5]->GetYaxis()->SetRangeUser(0,180);
    hprof[i5]->SetMarkerStyle(21);
    hprof[i5]->SetMarkerColor(1);
    hprof[i5]->SetLineColor(1);
    hprof[i5]->SetStats(0);
  }
  TPWVTM->GetXaxis()->SetRangeUser(10,50);
  c6->Print(Form("./Result/PWVTNor180TPFColz%d_%d_%dto%d_%d_%dcom.pdf",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c6->Print(Form("./Result/PWVTNor180TPFColz%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  TPWVTM->GetYaxis()->SetRangeUser(0,120);
  c6->Print(Form("./Result/PWVTNor120TPFColz%d_%d_%dto%d_%d_%dcom.pdf",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c6->Print(Form("./Result/PWVTNor120TPFColz%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  TPWVTM->GetYaxis()->SetRangeUser(0,60);
  c6->Print(Form("./Result/PWVTNor060TPFColz%d_%d_%dto%d_%d_%dcom.pdf",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c6->Print(Form("./Result/PWVTNor060TPFColz%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c6->cd(1)->SetLogz();
  c6->cd(2)->SetLogz();
  c6->cd(3)->SetLogz();
  c6->cd(4)->SetLogz();
  //c6->Print("./Result/PwidthVLogNorTPF.png");
  TPWVTM->GetYaxis()->SetRangeUser(0,180);
  c6->Print(Form("./Result/PWVTNorLg180TPFColz%d_%d_%dto%d_%d_%dcom.pdf",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c6->Print(Form("./Result/PWVTNorLg180TPFColz%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  TPWVTM->GetYaxis()->SetRangeUser(0,120);
  c6->Print(Form("./Result/PWVTNorLg120TPFColz%d_%d_%dto%d_%d_%dcom.pdf",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c6->Print(Form("./Result/PWVTNorLg120TPFColz%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  TPWVTM->GetYaxis()->SetRangeUser(0,60);
  c6->Print(Form("./Result/PWVTNorLg060TPFColz%d_%d_%dto%d_%d_%dcom.pdf",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c6->Print(Form("./Result/PWVTNorLg060TPFColz%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c6->Clear();
  c6->Divide(2,2);
  for (int i5=0;i5<4;i5++){
    c6->cd(i5+1);
    c6->cd(i5+1)->SetPhi(90);
    TPWVTM->Draw("colz");
    TPWVTN[i5]->Draw("Lego2same");
    hprof[i5]->Draw("EPsame");
    TPWVTTitle[i5]->Draw();
    Lprof[i5]->Draw();
    TPWVTN[i5]->GetZaxis()->SetRangeUser(0,0.08);
    TPWVTN[i5]->GetXaxis()->SetRangeUser(11,48);
    hprof[i5]->GetYaxis()->SetRangeUser(0,180);
    hprof[i5]->SetMarkerStyle(21);
    hprof[i5]->SetMarkerColor(1);
    hprof[i5]->SetLineColor(1);
    hprof[i5]->SetStats(0);
  }
  TPWVTM->GetXaxis()->SetRangeUser(10,50);
  c6->Print(Form("./Result/PWVTNor180TPFLego2%d_%d_%dto%d_%d_%dcom.pdf",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c6->Print(Form("./Result/PWVTNor180TPFLego2%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  TPWVTM->GetYaxis()->SetRangeUser(0,120);
  c6->Print(Form("./Result/PWVTNor120TPFLego2%d_%d_%dto%d_%d_%dcom.pdf",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c6->Print(Form("./Result/PWVTNor120TPFLego2%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  TPWVTM->GetYaxis()->SetRangeUser(0,60);
  c6->Print(Form("./Result/PWVTNor060TPFLego2%d_%d_%dto%d_%d_%dcom.pdf",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c6->Print(Form("./Result/PWVTNor060TPFLego2%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c6->cd(1)->SetLogz();
  c6->cd(2)->SetLogz();
  c6->cd(3)->SetLogz();
  c6->cd(4)->SetLogz();
  //c6->Print("./Result/PwidthVLogNorTPF.png");
  TPWVTM->GetYaxis()->SetRangeUser(0,180);
  c6->Print(Form("./Result/PWVTNorLg180TPFLego2%d_%d_%dto%d_%d_%dcom.pdf",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c6->Print(Form("./Result/PWVTNorLg180TPFLego2%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  TPWVTM->GetYaxis()->SetRangeUser(0,120);
  c6->Print(Form("./Result/PWVTNorLg120TPFLego2%d_%d_%dto%d_%d_%dcom.pdf",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c6->Print(Form("./Result/PWVTNorLg120TPFLego2%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  TPWVTM->GetYaxis()->SetRangeUser(0,60);
  c6->Print(Form("./Result/PWVTNorLg060TPFLego2%d_%d_%dto%d_%d_%dcom.pdf",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c6->Print(Form("./Result/PWVTNorLg060TPFLego2%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  
  c6->Clear();
  TH1F *PWVTem201   = new TH1F("PWVTem201","PWVTem201",60,0,60);
  TH1F *PWVTem331   = new TH1F("PWVTem331","PWVTem331",60,0,60);
  TH1F *PWVTem461   = new TH1F("PWVTem461","PWVTem461",60,0,60);
  TH1F *PWVTem203   = new TH1F("PWVTem203","PWVTem203",60,0,60);
  TH1F *PWVTem333   = new TH1F("PWVTem333","PWVTem333",60,0,60);
  TH1F *PWVTem463   = new TH1F("PWVTem463","PWVTem463",60,0,60);
  TH1F *PWVTem205   = new TH1F("PWVTem205","PWVTem205",60,0,60);
  TH1F *PWVTem335   = new TH1F("PWVTem335","PWVTem335",60,0,60);
  TH1F *PWVTem465   = new TH1F("PWVTem465","PWVTem465",60,0,60);
  TH1F *PWVTem204   = new TH1F("PWVTem204","PWVTem204",60,0,60);
  TH1F *PWVTem334   = new TH1F("PWVTem334","PWVTem334",60,0,60);
  TH1F *PWVTem464   = new TH1F("PWVTem464","PWVTem464",60,0,60);
  TH1F *PWVTemN    = new TH1F("PWVTemN" ,"",60,0,60);
  TH1F *PWVTem[4][3]={{PWVTem201,PWVTem331,PWVTem461},{PWVTem203,PWVTem333,PWVTem463},{PWVTem205,PWVTem335,PWVTem465},{PWVTem204,PWVTem334,PWVTem464}};
  int STem[3]={14,30,47};
  PWVTemN->SetStats(0);
  PWVTemN->GetYaxis()->SetRangeUser(0,0.08);
  PWVTemN->GetYaxis()->SetRangeUser(0,0.08);
  PWVTemN->GetXaxis()->SetTitle("PWidth(100ns)");
  PWVTemN->GetYaxis()->SetTitle("Ratio(normolize)");
  TLegend *LPWVTem = new TLegend(0.7,0.8,1,0.99);
  LPWVTem->SetFillColor(0);
  LPWVTem->SetLineColor(1);
  LPWVTem->SetNColumns(1);
  TLegend *SETitle1  = new TLegend(0.25,0.91,0.75,1);
  TLegend *SETitle3  = new TLegend(0.25,0.91,0.75,1);
  TLegend *SETitle5  = new TLegend(0.25,0.91,0.75,1);
  TLegend *SETitle4  = new TLegend(0.25,0.91,0.75,1);
  TLegend *SETitle[4]={SETitle1,SETitle3,SETitle5,SETitle4};
  for (int i5=0;i5<4;i5++){
    SETitle[i5]->SetFillColor(0);
    SETitle[i5]->SetLineColor(0);
    SETitle[i5]->SetFillColorAlpha (0,0.0);
    SETitle[i5]->AddEntry("hpwmod",Form("PWidth distribution on MTB%d",BD[i5]),"");
  }
  c6->Divide(2,2);
  for(int i7=0;i7<3;i7++){
    LPWVTem->AddEntry(PWVTem[0][i7],Form("T=%d#circC",STem[i7]),"l");
    for(int i5=0;i5<4;i5++){
      PWVTem[i5][i7]->SetLineColor(i7+1);
      PWVTem[i5][i7]->SetStats(0);
    }
  }
  for(int i5=0;i5<4;i5++){
    c6->cd(i5+1);
    PWVTemN->Draw("");
    for(int i7=0;i7<3;i7++){
      for(int i6=1;i6<61;i6++){
        PWVTem[i5][i7]->SetBinContent(i6,TPWVTN[i5]->GetBinContent(STem[i7],i6));
      }
      PWVTem[i5][i7]->Draw("hsame");
      SETitle[i5]->Draw();
    }
    LPWVTem->Draw();
  }
  
  c6->Print(Form("./Result/PWVTBbox203346%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c6->Print(Form("./Result/PWVTBbox203346%d_%d_%dto%d_%d_%dcom.pdf",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));


  Int_t    nLayers ;//total number for hit in a frame/event
  Int_t    nH0     ;//total number for hit in a frame/event
  Int_t    nH1     ;//total number for hit in a frame/event
  Int_t    nH2     ;//total number for hit in a frame/event
  Int_t    nH3      ;//total number for hit in a frame/event
  Int_t   pwidth0  ;
  Int_t   pwidth1  ;
  Int_t   pwidth2  ;
  Int_t   pwidth3  ;
  TH1F *hSEpw1      = new TH1F("SelectOfMTB1","",160,0,160);
  TH1F *hSEpw3      = new TH1F("SelectOfMTB3","",160,0,160);
  TH1F *hSEpw5      = new TH1F("SelectOfMTB5","",160,0,160);
  TH1F *hSEpw4      = new TH1F("SelectOfMTB4","",160,0,160);
  TH1F *hpwmod      = new TH1F("hpwmod","",160,0,160);
  TH1F *hSEpw[4]={hSEpw1,hSEpw3,hSEpw5,hSEpw4};
  
  //Printf("vRootFiles found: %i", (int) vRootFiles.size());
  TreeReader data1("/data4/cmkuo/muography/data/shimen/selectedTracks.root");
  //ofstream out(Form("%sSEV2.dat",DirOperate));
  for (Long64_t ev = 0; ev <evs; ++ev) {//evs; ++ev) {
    data1.GetEntry(ev);
    unixtime  = data1.GetLong64("unixtime");
    //frame     = data1.GetInt("frame");//event
    nLayers = data1.GetInt("nLayers");
    nH0     = data1.GetInt("nH0");
    pwidth0  = data1.GetInt("pwidth0");
    pwidth1  = data1.GetInt("pwidth1");
    pwidth2  = data1.GetInt("pwidth2");
    pwidth3  = data1.GetInt("pwidth3");
    //seq       = data1.GetPtrInt("seq");
    if(nLayers == 3 && nH0==0){
      hSEpw4->Fill(pwidth3);
      hSEpw5->Fill(pwidth2);
      hSEpw3->Fill(pwidth1);
    }
    if(nLayers == 4 && nH0==1){
      hSEpw1->Fill(pwidth0);
    }
  }
  TLegend *LhSEpw = new TLegend(0.7,0.8,1,0.99);
  LhSEpw->SetFillColor(0);
  LhSEpw->SetLineColor(1);
  LhSEpw->SetNColumns(1);
  int nH0nLa[2][4]={{1,0,0,0},{4,3,3,3}};
  c7->cd();
  c7->Divide(2,2);
  for(int i4=0;i4<3;i4++){
    c7->cd(i4+1);
    hSEpw[i4+1]->Draw("h");
    LhSEpw->Draw();
  }
  for(int i5=0;i5<4;i5++){
    hSEpw[i5]->GetYaxis()->SetRangeUser(0,120);
    hSEpw[i5]->GetXaxis()->SetTitle("PWidth(100ns)");
    hSEpw[i5]->GetYaxis()->SetTitle("times");
    hSEpw[i5]->SetTitle(Form("PWidth distribution on MTB%d",BD[i5]));
    LhSEpw->AddEntry(hSEpw[i5],Form("pwidth%d:nLayers=%d,nH3=%d",i5,nH0nLa[0][i5],nH0nLa[1][i5]),"l");
  }
  c7->cd(4);
  hSEpw1->Draw("");
  hSEpw3->Draw("hsame");
  hSEpw4->Draw("hsame");
  hSEpw5->Draw("hsame");
  hSEpw1->Draw("hsame");
  LhSEpw->Draw();
  
  TH1F *hSEpwN1       = (TH1F*) hSEpw1->Clone();
  TH1F *hSEpwN3       = (TH1F*) hSEpw3->Clone();
  TH1F *hSEpwN5       = (TH1F*) hSEpw5->Clone();
  TH1F *hSEpwN4       = (TH1F*) hSEpw4->Clone();
  TH1F *hSEpwN[4]={hSEpwN1,hSEpwN3,hSEpwN5,hSEpwN4};
  TLegend *LhSEpwN1 = new TLegend(0.7,0.75,1,0.8);
  TLegend *LhSEpwN3 = new TLegend(0.7,0.75,1,0.8);
  TLegend *LhSEpwN5 = new TLegend(0.7,0.75,1,0.8);
  TLegend *LhSEpwN4 = new TLegend(0.7,0.75,1,0.8);
  TLegend *LhSEpwN[4]={LhSEpwN1,LhSEpwN3,LhSEpwN5,LhSEpwN4};
  TLegend *LSigAll1 = new TLegend(0.7,0.8,1,1);
  TLegend *LSigAll3 = new TLegend(0.7,0.8,1,1);
  TLegend *LSigAll5 = new TLegend(0.7,0.8,1,1);
  TLegend *LSigAll4 = new TLegend(0.7,0.8,1,1);
  TLegend *LSigAll[4]={LSigAll1,LSigAll3,LSigAll5,LSigAll4};


  for(int i5=0;i5<4;i5++){
    hSEpw[i5]->SetLineColor(i5+1);
    hSEpw[i5]->GetXaxis()->SetRangeUser(0,60);
    hSEpwN[i5]->Sumw2();
    hSEpwN[i5]->Scale(1./hSEpw[i5]->Integral(-1,-1));
    hSEpwN[i5]->GetYaxis()->SetRangeUser(0,0.08);
    hSEpwN[i5]->GetXaxis()->SetRangeUser(0,60);
    hSEpwN[i5]->GetXaxis()->SetTitle("PWidth(100ns)");
    hSEpwN[i5]->GetYaxis()->SetTitle("Ratio(Normolize)");
    hSEpwN[i5]->SetLineColor(4);
    hSEpwN[i5]->SetStats(1);
    c6->cd(i5+1);
    //hpwNmod->Draw("");
    hSEpwN[i5]->Draw("hsame");
    SETitle[i5]->Draw();
    LhSEpwN[i5]->SetFillColor(0);
    LhSEpwN[i5]->SetLineColor(1);
    LhSEpwN[i5]->SetNColumns(1);
    LhSEpwN[i5]->AddEntry(hSEpwN[i5],Form("pwidth%d:nLayers=%d,nH3=%d",i5,nH0nLa[0][i5],nH0nLa[1][i5]),"el");
    LhSEpwN[i5]->Draw();
  }  
  c7->Print(Form("./Result/SEwidthbox%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c7->Print(Form("./Result/SEwidthbox%d_%d_%dto%d_%d_%dcom.pdf",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c6->Print(Form("./Result/PWSeTembox%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c6->Print(Form("./Result/PWSeTembox%d_%d_%dto%d_%d_%dcom.pdf",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  
  
  c8->Divide(2,2);
  for(int i5=0;i5<4;i5++){
    hPWB[i5]->SetLineColor(2);
    hPWB[i5]->Sumw2();
    hPWB[i5]->Scale(1./hPWB[i5]->Integral(-1,-1));
    hPWB[i5]->GetYaxis()->SetRangeUser(0,0.08);
    hPWB[i5]->GetXaxis()->SetRangeUser(0,60);
    c8->cd(i5+1);
    hSEpwN[i5]->Draw("h");
    hPWB[i5]->Draw("hsames");
    LhSEpwN[i5]->SetFillColor(0);
    LhSEpwN[i5]->SetLineColor(1);
    LhSEpwN[i5]->SetNColumns(2);
    LSigAll[i5]->AddEntry(hPWB[i5]  ,Form("MTB%d pwidth distribution ",BD[i5]),"el");
    LSigAll[i5]->AddEntry(hSEpwN[i5],Form("pwidth%d:nLayers=%d,nH3=%d",i5,nH0nLa[0][i5],nH0nLa[1][i5]),"el");
    LSigAll[i5]->Draw();
    
  }
  c8->Update();
  TPaveStats *stat1 = (TPaveStats*) hPWB1->FindObject("stats");
  TPaveStats *stat3 = (TPaveStats*) hPWB3->FindObject("stats");
  TPaveStats *stat5 = (TPaveStats*) hPWB5->FindObject("stats");
  TPaveStats *stat4 = (TPaveStats*) hPWB4->FindObject("stats");
  c8->Update();
  stat1->SetX1NDC(0.7);
  stat1->SetX2NDC(1.0);
  stat1->SetY1NDC(0.4);
  stat1->SetY2NDC(0.6);
  c8->Update();
  stat3->SetX1NDC(0.7);
  stat3->SetX2NDC(1.0);
  stat3->SetY1NDC(0.4);
  stat3->SetY2NDC(0.6);
  c8->Update();
  stat5->SetX1NDC(0.7);
  stat5->SetX2NDC(1.0);
  stat5->SetY1NDC(0.4);
  stat5->SetY2NDC(0.6);
  c8->Update();
  stat4->SetX1NDC(0.7);
  stat4->SetX2NDC(1.0);
  stat4->SetY1NDC(0.4);
  stat4->SetY2NDC(0.6);
  
  c8->Print(Form("./Result/AlPWSebox%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c8->Print(Form("./Result/AlPWSebox%d_%d_%dto%d_%d_%dcom.pdf",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  //cout<<"aul"<<endl;
  //TGaxis *ARate = new TGaxis(0,0, 100, 1,0,1.5,205,"+L");
  //cout<<"aul"<<endl;
  //ARate->SetTitle("Ratio");
  //cout<<"aul"<<endl;
  c10->Divide(2,2);
  for(int i0=0;i0<4;i0++){
    c10->cd(i0+1);
    hPWB292[i0]->Scale(1./hPWB292[i0]->Integral(-1,-1));
    hPWB290[i0]->Scale(1./hPWB290[i0]->Integral(-1,-1));
    hPWB288[i0]->Scale(1./hPWB288[i0]->Integral(-1,-1));
    ofstream outChangeVB(Form("/home/yusiang/tunnel/OperateResule/hPWB%d.txt",BD[i0]));
    outChangeVB<<Form("B%d X axis\t288\t290\t292\n",BD[i0]);
    for(int i1=0;i1<180;i1++){
      outChangeVB<<i1<<"\t"<<hPWB288[i0]->GetBinContent(i1)<<"\t"<<hPWB290[i0]->GetBinContent(i1)<<"\t"<<hPWB292[i0]->GetBinContent(i1)<<endl;
    }
    outChangeVB.close();
    // TH1F *hPWB288BD = (TH1F*) hPWB288[i0]->Clone();
    // TH1F *hPWB290BD = (TH1F*) hPWB290[i0]->Clone();
    // hPWB288BD->Divide(hPWB288[i0]);
    // hPWB290BD->Divide(hPWB290[i0]);
    hPWB292[i0]->Draw("");
    hPWB288[i0]->Draw("histsame");
    hPWB292[i0]->Draw("histsame");
    hPWB290[i0]->Draw("histsame");
    //hPWB288BD->Draw("psame");
    //hPWB290BD->Draw("psame");
    //cout<<"aulb"<<endl;
    //ARate->Draw("same");
    //cout<<"aula"<<endl;
    c10->cd(i0+1)->Update();
    //cout<<"aulu"<<endl;
    hPWB288[i0]->SetLineColor(2);
    hPWB290[i0]->SetLineColor(4);
    hPWB292[i0]->SetLineColor(1);
    // hPWB288BD->SetMarkerStyle(2);
    // hPWB290BD->SetMarkerStyle(2);
    // hPWB288BD->SetMarkerColor(2);
    // hPWB290BD->SetMarkerColor(4);
    if(i0==0) hPWB290[i0]->GetXaxis()->SetRangeUser(0,100);
    else      hPWB290[i0]->GetXaxis()->SetRangeUser(0,50);
    hPWB292[i0]->SetStats(0);
    hPWB290[i0]->SetStats(0);
    hPWB288[i0]->SetStats(0);
    hPWB292[i0]->SetTitle(Form("VoltResetCompare On MTB%d",BD[i0]));
    if(i0==0) hPWB292[i0]->GetXaxis()->SetRangeUser(0,100);
    else      hPWB292[i0]->GetXaxis()->SetRangeUser(0,50);
    hPWB292[i0]->GetXaxis()->SetTitle("PWidth(100ns)");
    hPWB292[i0]->GetYaxis()->SetTitle("Ratio of times");
    TLegend *LPWVAB = new TLegend(0.6,0.7,0.9,0.9);
    LPWVAB->SetFillColor(0);
    LPWVAB->SetLineColor(1);
    LPWVAB->SetNColumns(1);
    
    if (i0==0){
      LPWVAB->AddEntry(hPWB288[i0],Form("MTB%d 28.8V",BD[i0]),"l");
      LPWVAB->AddEntry(hPWB290[i0],Form("MTB%d 29.0V",BD[i0]),"l");
    }else{
      LPWVAB->AddEntry(hPWB288[i0],Form("MTB%d 29.2V",BD[i0]),"l");
      LPWVAB->AddEntry(hPWB290[i0],Form("MTB%d 29.2V",BD[i0]),"l");
    }
    LPWVAB->AddEntry(hPWB292[i0],Form("MTB%d 29.2V",BD[i0]),"l");
    //LPWVAB->AddEntry(hPWB288BD,Form("MTB-%d-2.88V/2.92V",BD[i0]),"p");
    //LPWVAB->AddEntry(hPWB290BD,Form("MTB-%d-2.90V/2.92V",BD[i0]),"p");
    LPWVAB->Draw();
  }
  c10->Update();
  c10->Print(Form("./Result/PWBACReVbox%d_%d_%dto%d_%d_%dcom.png",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
  c10->Print(Form("./Result/PWBACReVbox%d_%d_%dto%d_%d_%dcom.pdf",Yeari,UTMGMT2021(unixtimei),UTDGMT2021(unixtimei),Yearf,UTMGMT2021(unixtimef),UTDGMT2021(unixtimef)));
}







*/