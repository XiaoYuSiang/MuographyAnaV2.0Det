#include <TF1.h>

#include "./Macro/AnaVariable.h"
using namespace MuographAnaVariable;
using namespace std;



void MainControl(){
  /* const path of Macro compiling */
  gSystem->SetBuildDir(Form("tmpdir"), kTRUE);
  
  
  
  /* Adjustable Path Variable */
  const char path_Raw[50] = "/data4/YuSiang/S4_11F_4lay_5deg/";
  //path of the raw data for muon (*_Mu.txt)
  const char path_Hk [50] = "/data4/YuSiang/S4_11F_4lay_5deg/";
  //path of the raw data for House keeping (*_HK.txt)
  const char path_OpR[50] = "/data4/YuSiang/ExampleTestSpace/Ana/";
  //path of result and operation data
  const char path_Mac[50] = "./Macro/";
  //path of Macros, it is the Macro path your download from GitHub
  
  
  
  /* Data name want to search!! */
  /*Example:   
    1. A directory contain 3 file:
        Name1.txt & Name.txt & N.txt
    2. case for name_Raw = "Name":
        Will analyze the file: Name.txt
    3. case for name_Raw = "Name*":
        Will analyze the file: Name.txt & Name1.txt
    4. case for name_Raw = "*":
        Will analyze the file: Name.txt & Name1.txt % N.txt
  Finish Example */
  const char name_Raw [40] = "2022*_Mu";
  const char name_Hk  [40] = "2022*_HK";
  
  
  
  bool testMode        = false;//pwidth ana. test mode
  bool normalizeMode   = true;
  bool NegHiRatChMode  = true;
  bool lowstatisticMode= false;
  bool rootFileMode    = true;
  bool SETightMode     = true;
  bool PointTextMode   = true;
  bool OperMode[7]={
    testMode,         normalizeMode,    NegHiRatChMode,
    lowstatisticMode, rootFileMode,     SETightMode,
    PointTextMode,
  };
  
  
  
  bool SkipBlock  = true;//default skip the progress = true
  bool ReDSLAna   = true;//default skip the progress = true @@
  /* P.01 */
  /* Copy the AnaVariable.h into path_Mac */
  system(Form("cp AnaVariable.h  %sAnaVariable.h",path_Mac));
  
  
  
  if(SkipBlock==false){
    /* P.02 */
    /* Establish the Compile dir set */
    gROOT->LoadMacro(Form("%sDataBaseLocate.C+",path_Mac));
    DataBaseLocate( path_Raw, path_Hk, path_OpR, path_Mac, name_Raw, name_Hk, '' );
    cout<<"Path of Raw data: "<<path_Raw<<endl;
    cout<<"Path of Root Dir: "<<path_OpR<<endl;
    
    /* P.03 */
    /* Locate the data of detector */
    gROOT->LoadMacro(Form("%sODectImfAna.C+",path_Mac));
    ODectImfAna("V2.0");
    //ODectImfAna(const char *Path_ODetIMFData, const char *DetectorVersion="V2.0")
    
    /* P.04 */
    /* Find Raw .txt data Names and path */
    gROOT->LoadMacro(Form("%sDataNameAna.C+",path_Mac));
    int txtfnum = DataNameAnaTxt_Mu();
    //int DataNameAnaTxt_Mu(const char OPT='P') 
    cout<<"Find Raw .txt data Number:  "<<txtfnum<<endl;
    
    /* P.05 */
    /* Find HK .txt data Names and path */
    //gROOT->LoadMacro(Form("%sDataNameAna.C+",path_Mac));
    int Hktxtfnum = DataNameAnaTxt_Hk();
    //int DataNameAnaTxt_Mu(const char OPT='P') 
    cout<<"Find Hk .txt data Number:  "<<Hktxtfnum<<endl;
    
    /* P.06 */
    /* Convert File from raw .txt to be raw .root */
    gROOT->LoadMacro(Form("%sCaConvertor.C+",path_Mac));
    MuoCaConvertor();
    
    /* P.07 */
    /* Find Raw .root data Names and path */
    //gROOT->LoadMacro(Form("%sDataNameAna.C+",path_Mac));
    int rotfnum = DataNameAnaRoot_Mu('');
    //int DataNameAnaRoot_Mu(const char OPT='P') 
    cout<<"Find Raw .root data Number:  "<<rotfnum<<endl;
  }
  
  if(ReDSLAna){
    /* P.08 */
    /*Analize the Data for time start to end and save to head file*/
    gROOT->LoadMacro(Form("%sDSLAnaV2.C+",path_Mac));
    DSLAnaV2();
  }
  
  if(SkipBlock==false){
    /* P.09 */
    /* Data Collection Efficiency on every day */
    gROOT->LoadMacro(Form("%sDayEAnaV2.C+",path_Mac));
    DayEAnaV2();
    
    /* P.10 */
    /*Convert Raw_Mu.root to be Gap*.root by tcnt cut*/
    gROOT->LoadMacro(Form("%sEventAna.C+",path_Mac));
    EventAna();
    //EventAna(vector<string> files,const char* sapaths,const int indexi, const int indexf )
    
    /* P.11 */
    /*Simpling ID of Mu and Ele to be Events*.root */
    gROOT->LoadMacro(Form("%sMuonElectronID.C+",path_Mac));
    MuonElectronID(OperMode, 28,29);//140
    //MuonElectronID(c.bool*OperMode, c.int indexi=28, c.int indexf=29)
    IDCaseNumAna(OperMode, 28, 29);//140
    //IDCaseNumAna(c.bool*OperMode, c.int indexi=28, c.int indexf=29) 
  
    /* P.12 */
    /* Verticle Straight line Event Ana to be Track*.root */
    gROOT->LoadMacro(Form("%sVerticalTracksAna.C+",path_Mac));
    VerticalTracksAna();
    //VerticalTracksAna(c.int indexi=28, c.int indexf=29 )
    
    /* P.13 */
    /* Find real events in straight line Tracking Ana. */
    gROOT->LoadMacro(Form("%sSTRealEvAna.C+",path_Mac));
    STRealEvAna();
    
    /* P.14 */
    /* Pwidth analyze for function fitting and trigger ana. */
    gROOT->LoadMacro(Form("%sPwidthAna.C+",path_Mac));
    PwidthAnaV2(OperMode);
    //PWidthPeakAna(c.bool*OperMode)
    
    /* P.15 */
    /* Pwidth analyze for function fitting and trigger ana. */
    gROOT->LoadMacro(Form("%sPWidthPeakAna.C+",path_Mac));
    PWidthPeakAna(OperMode);
    //PWidthPeakAna(const bool*OperMode)
    
    /* P.16 */
    /* Straight line Tracking of Event to be ETracks*.txt */
    gROOT->LoadMacro(Form("%sTrackAna.C+",path_Mac));
    TrackAna();
    
    /* P.17 */
    /* Straight line Tracking Result display*.txt */
    gROOT->LoadMacro(Form("%sFitTrackResAna.C+",path_Mac));
    FitTrackResAna();
    
    /* P.18 */
    /* ana. of SE numbers per day and pwidth distribution */
    gROOT->LoadMacro(Form("%sSEAnaV2.C+",path_Mac));
    SEAnaV2();
    
    /* P.19 */
    /* Rate ana for separate board and chID (easy to observe all) */
    gROOT->LoadMacro(Form("%sR4VTAna.C+",path_Mac));
    R4VTAna();
    
    /* P.20 */
    /* Rate ana for general chID (easy to observe all) */
    gROOT->LoadMacro(Form("%sRVTAna.C+",path_Mac));
    RVTAna();
    
    /* P.21 */
    /* Rate Vs Time cp of channel or board for different display way */
    gROOT->LoadMacro(Form("%sRVTCAna.C+",path_Mac));
    RVTCAna();
    
    /* P.22 */
    /* Find HK data and record data to be .dat */
    gROOT->LoadMacro(Form("%sHKProAna.C+",path_Mac));
    HKProAna();
    
    /* P.23 */
    /* draw th2f colz for TVTime or HVTime and save average TVtime and HVTime*/
    //gROOT->LoadMacro(Form("%sHKProAna.C+",path_Mac));
    HKAna();
    
    /* P.24 */
    /* Draw TVTime or HVTime by TGraph */
    //gROOT->LoadMacro(Form("%sHKProAna.C+",path_Mac));
    HKTGAna();
    
    /* P.25 */
    /* Rate Vs house keeping data for Temperature or humidity */
    gROOT->LoadMacro(Form("%sRVHKAna.C+",path_Mac));
    RVHKAna(OperMode);
    
    /* P.26 */
    /* Hit count/sequence skip ana. */
    gROOT->LoadMacro(Form("%sSeqSkipAna.C+",path_Mac));
    SeqSkipAna();
    
  }
    
    
    


}


