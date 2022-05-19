//include
#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TStyle.h>
#include <TPaveStats.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TTree.h>
#include "DSLData.h"
#include "AnaVariable.h"
#include "GobelFunctions.h"
#include "lineFit3D.h"
#include "path_dir.h"
using namespace std;
using namespace DBMMath;
using namespace MuoAna_path_dir;

void ODectImfAna( const char *DetectorVersion="V2.0") {
  /*Set Detector Information which before any rotate the position of scintillator are the coordinate of it own inner position*/
  char Path_ODetIMFData[180];
  sprintf(Path_ODetIMFData,"%s",DirIMFdat);
  // cout<<DirIMFdat<<endl;
  char version[8] = {};
  const char V2OptionName[10][10]={
    "V2.0","v2.0","V2","v2","V2.","v2.","V02.00","v02.00","V2.00","v2.00"
  };
  
  for(int iV2Ns=0 ; iV2Ns<10 ; iV2Ns++){
    if(strcmp(DetectorVersion,V2OptionName[iV2Ns])==0) {
      sprintf(version,"V02.00");
      break;
    }
  }
  
  /*Set Sci size and layer ID*/
  if(strcmp(version,"V02.00")==0){
    int BD[6]={1,3,5,4};
    const double bx=5.,by=5.,bz=1.;
    const double bx_2=bx/2.,by_2=by/2.,bz_2=bz/2.;
    /*Set Sci Array Number*/
    
    /*Data Setting*/

      
    char Path_rotfile[100];
    sprintf(Path_rotfile,"%sSciPosDatV2_00.root",Path_ODetIMFData);
    TFile *rotfile = new TFile(Path_rotfile,"RECREATE");
    
    Int_t     boardID; //MTB ID
    Int_t     channelID; //ID of channel in MTB
    Int_t     GeneralID; //General ID of channel
    Int_t     iX, iY, iZ;
    Float_t   pX, pY, pZ;
    
    TTree *tPos = new TTree("tPos","original Position of scintillator in detector V02.00");
    tPos->Branch("boardID"     ,&boardID      , "boardID/I"  );
    tPos->Branch("channelID"   ,&channelID    , "channelID/I");
    tPos->Branch("GeneralID"   ,&GeneralID    , "GeneralID/I");
    tPos->Branch("iX"          ,&iX           , "iX/I"       );
    tPos->Branch("iY"          ,&iY           , "iY/I"       );
    tPos->Branch("iZ"          ,&iZ           , "iZ/I"       );
    tPos->Branch("pX"          ,&pX           , "pX/F"       );
    tPos->Branch("pY"          ,&pY           , "pY/F"       );
    tPos->Branch("pZ"          ,&pZ           , "pZ/F"       );
    
    //Fill the data
    for (int idZ = 0; idZ <4; ++idZ) {//evs; ++ev) {
      for (int ich = 0; ich <16; ++ich) {//evs; ++ev) {
          iX = ich%4;
          iY = ich/4;
          iZ = idZ;
          boardID = BD[idZ];
          channelID = ich;
          GeneralID = channelID+boardID*16;
          pX = 2.5 + 5.*iX;
          pY = 2.5 + 5.*iY;
          pZ = .5 + 50.*iZ;
          tPos->Fill();
        }
    }

    rotfile->Write();
    cout<<Form("%s is RECREATE.",Path_rotfile)<<endl;
    rotfile->Close();
  }else{
    cout<<endl;
    cout<<"Error: ODectImfAna.C(c.char *spath, c.char *version=\"V2.0\") need a correct version number!!!"<<endl;
    cout<<"version number now is:    "<<DetectorVersion<<"    can not be Identified by program"<<endl;
    cout<<"Please Enter Like this table:    "<<endl;
    for(int iV2Ns=0 ; iV2Ns<10 ; iV2Ns++){
      cout<<Form("%10.10s    =    %10.10s",V2OptionName[iV2Ns],"V02.00")<<endl;
    }
    cout<<"Or connect to the author to add new version detector data"<<endl;
    cout<<endl;
    throw;
  }
}








  
  
  
  
  