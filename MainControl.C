#include <TF1.h>

#include "./Macro/AnaVariable.h"
using namespace MuographAnaVariable;
using namespace std;

void TestSc(){
  gSystem->SetBuildDir(Form("tmpdir"), kTRUE);
  
  
  const char path_Raw[50] = "/data4/YuSiang/S4_11F_4lay_Inv/";
  const char path_OpR[50] = "/home/yusiang/TestSpace/A/";
  const char path_Mac[50] = "/home/yusiang/TestSpace/Macro/";
  const char name_Raw_txt [50] = "*_Mu.txt";
  const char name_Raw_root[50] = "*_Mu.root";
  bool noInitialize = true;
  

  
    /*Establish the Compile dir set */
    gROOT->LoadMacro(Form("%sDataBaseLocate.C+",path_Mac));
    DataBaseLocate( path_Raw, path_OpR, path_Mac, '' );
    cout<<"Path of Raw data: "<<path_Raw<<endl;
    cout<<"Path of Root Dir: "<<path_OpR<<endl;
    
  if(noInitialize!=true){
    /* Locate the data of detector */
    gROOT->LoadMacro(Form("%sODectImfAna.C+",path_Mac));
    ODectImfAna("V2.0");
    //ODectImfAna(const char *Path_ODetIMFData, const char *DetectorVersion="V2.0")
    
    /* Find Raw .txt data Names and path */
    gROOT->LoadMacro(Form("%sDataNameAna.C+",path_Mac));
    int txtfnum = DataNameAnaTxt(name_Raw_txt,'');
    //int DataNameAnaTxt(const char *NameChara,const char OPT='P') 
    cout<<"Find Raw .txt data Number:  "<<txtfnum<<endl;
    
    /* Convert File from raw .txt to be raw .root */
    gROOT->LoadMacro(Form("%sCaConvertor.C+",path_Mac));
    MuoCaConvertor();
    
    /* Find Raw .root data Names and path */
    int rotfnum = DataNameAnaRoot(name_Raw_root,'');
    //int DataNameAnaRoot(const char *NameChara,const char OPT='P') 
    cout<<"Find Raw .root data Number:  "<<rotfnum<<endl;
    
    
    
    /*Analize the Data for time start to end and save to head file*/
    gROOT->LoadMacro(Form("%sDSLAnaV2.C+",path_Mac));
    DSLAnaV2();
    
    /* Data Collection Efficiency on every day */
    gROOT->LoadMacro(Form("%sDayEAnaV2.C+",path_Mac));
    DayEAnaV2();
    
    /*Convert Raw_Mu.root to be Gap*.root by tcnt cut*/
    gROOT->LoadMacro(Form("%sEventAna.C+",path_Mac));
    EventAna();
    //EventAna(vector<string> files,const char* sapaths,const int indexi, const int indexf )
    
    /*Simpling ID of Mu and Ele to be Events*.root */
    gROOT->LoadMacro(Form("%sMuonElectronID.C+",path_Mac));
    MuonElectronID(28,29,false,true);//140
    IDCaseNumAna(28,29,false);//140
    //MuonElectronID(c.int indexi=28, c.int indexf=29,bool testMode=false,bool editMode=false )
    //IDCaseNumAna(c.int indexi=28, c.int indexf=29,bool testMode=false) 
    
    /* Verticle Straight line Event Ana to be Track*.root */
    gROOT->LoadMacro(Form("%sVerticalTracksAna.C+",path_Mac));
    VerticalTracksAna();
    //VerticalTracksAna(c.int indexi=28, c.int indexf=29 )
    
  }
    /* Straight line Tracking of Event to be ETracks*.txt */
    gROOT->LoadMacro(Form("%sTrackAna.C+",path_Mac));
    TrackAna();

    /* Straight line Tracking Result display*.txt */
    gROOT->LoadMacro(Form("%sFitTrackResAna.C+",path_Mac));
    FitTrackResAna();
  
  
  
}



/*

C比較複雜一點
承翰
你Ｃ用ntuplizer就好勒不是嗎
承翰
如果你想用的話
承翰
root 6.20以上
承翰
然後
承翰
#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
承翰
然後把這個放在第一行
承翰
吳承翰
ROOT::EnableImplicitMT();
承翰
吳承翰
#include "ROOT/RDataFrame.hxx"
#include <iostream>

using namespace std;
void multi(){
    ROOT::EnableImplicitMT(4);

    ROOT::RDataFrame df1("ggNtuplizer/EventTree", "/data6/ggNtuples/V10_02_10_05/job_fall17_DYJetsToLL_m50_aMCatNLO/*.root");

    auto pos1 = df1.Filter([](float gen) {return gen > 0.;}, {"genWeight"}).Count();
    auto neg1 = df1.Filter([](float gen) {return gen <= 0.;}, {"genWeight"}).Count();

    cout << *pos1 << endl;
    cout << *neg1 << endl;
}
*/