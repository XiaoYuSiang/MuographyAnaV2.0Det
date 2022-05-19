#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TF1.h>

using namespace std;
void locatedDirbyPath(const char *path_dir){
  char cmdline[180];
  sprintf(cmdline,"mkdir -p %s",path_dir);
  if (system(cmdline)!=0){
    // cout<<"Don't find dir path:  "<<path_dir<<", and create this directory"<<endl;
    system(cmdline);
  }
}

void DataBaseLocate( const char *path_dirRawData, const char *path_dirAnaData, const char *path_dirMacros="./", const char OPT='P'){
  if(path_dirRawData[0]=='.'){
    cout<<endl;
    cout<<"\"Error\" Path of Raw data need to be \"Full\" Path!!!"<<endl;
    cout<<"      Now, the enter Raw data path is: "<<path_dirRawData<<endl;
    cout<<"      Please check the path of Raw dat !!!"<<path_dirRawData<<endl;
    cout<<endl;
    throw;
  }else if (path_dirAnaData[0]=='.'){
    cout<<endl;
    cout<<"\"Error\" Root path of data base need to be \"Full\" Path!!!"<<endl;
    cout<<"      Now, the enter data base path is: "<<path_dirAnaData<<endl;
    cout<<"      Please check the path of root !!!"<<path_dirAnaData<<endl;
    cout<<endl;
    throw;
  }
  
  char DirMacros[150];
  sprintf(DirMacros ,"%s"       ,path_dirMacros);
  
  char DirIMFdat[150]={}, DirRotfile[150]={}, DirOperate[150]={}, DirResult [150]={};
  sprintf(DirIMFdat ,"%sDetIMF/"       ,path_dirAnaData);
  sprintf(DirRotfile,"%srootfile/"     ,path_dirAnaData);
  sprintf(DirOperate,"%sOperateResule/",path_dirAnaData);
  sprintf(DirResult ,"%sResult/"       ,path_dirAnaData);
  locatedDirbyPath(DirIMFdat);
  locatedDirbyPath(DirRotfile);
  locatedDirbyPath(DirOperate);
  locatedDirbyPath(DirResult);
  
  char DirOperEV[170],      DirOperEVGT[170],      DirOperEVGTST[170],    DirOperEVGTLF[170];
    /*Raw data to Event,  Select by nH and Gap,  straight line tracks,  line fitting tracking*/
  sprintf(DirOperEV     ,"%sRawEventDat/"  ,DirOperate);
  sprintf(DirOperEVGT   ,"%sEventGapDat/"  ,DirOperate);
  sprintf(DirOperEVGTST ,"%sEvGTrackDat/"  ,DirOperate);
  sprintf(DirOperEVGTLF ,"%sEvGTLFitDat/"  ,DirOperate);
  locatedDirbyPath(DirOperEV);
  locatedDirbyPath(DirOperEVGT);
  locatedDirbyPath(DirOperEVGTST);
  locatedDirbyPath(DirOperEVGTLF);
  
  char DirRes_ChEff[170], DirRes_ChRtPw[170], DirRes_PwVCase[170], DirRes_LFirTra[170];
    /*Raw data to Event,  Select by nH and Gap,  straight line tracks,  line fitting tracking*/
  sprintf(DirRes_ChEff  ,"%sBoardChannelEffective/"  ,DirResult);
  sprintf(DirRes_ChRtPw ,"%sBoardGPwaRtNor/"         ,DirResult);
  sprintf(DirRes_PwVCase,"%sBoardPwVCase/"           ,DirResult);
  sprintf(DirRes_LFirTra,"%sTracking_Result/"        ,DirResult);
  locatedDirbyPath(DirRes_ChEff);
  locatedDirbyPath(DirRes_ChRtPw);
  locatedDirbyPath(DirRes_PwVCase);
  locatedDirbyPath(DirRes_LFirTra);
  
  char DirRes_RatVT[170], DirRes_PwidVT[170], DirRes_HousKep[170], DirRes_OpEff[170];
  sprintf(DirRes_RatVT  ,"%sRateVsTime/"        ,DirResult);
  sprintf(DirRes_PwidVT ,"%sPwidthVsTime/"      ,DirResult);
  sprintf(DirRes_HousKep,"%sHouseKeeping/"      ,DirResult);
  sprintf(DirRes_OpEff  ,"%sOperateVsDate/"     ,DirResult);
  locatedDirbyPath(DirRes_RatVT);
  locatedDirbyPath(DirRes_PwidVT);
  locatedDirbyPath(DirRes_HousKep);
  locatedDirbyPath(DirRes_OpEff);
  
  char DirRes_PwiAna[170], DirRes_TempVT[170], DirRes_DayEff[170];
  sprintf(DirRes_PwiAna  ,"%sRateVsTime/"        ,DirResult);
  sprintf(DirRes_TempVT  ,"%sTemperatureVsTime/" ,DirResult);
  sprintf(DirRes_DayEff  ,"%sDayEff/"            ,DirResult);
  locatedDirbyPath(DirRes_PwiAna);
  locatedDirbyPath(DirRes_TempVT);
  locatedDirbyPath(DirRes_DayEff);
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  /* Locate Dir Path data HeadFile */
  ofstream outHfile(Form("%spath_dir.h",path_dirMacros));
  outHfile<<"#include <iostream>"<<endl;
  outHfile<<"#include <vector>"<<endl;
  outHfile<<"#include <fstream>"<<endl;
  outHfile<<"#include <typeinfo>"<<endl;
  outHfile<<"#include <algorithm>"<<endl;
  outHfile<<""<<endl;
  outHfile<<"namespace MuoAna_path_dir{"<<endl;
  outHfile<<"  /*  Path of Macros!!!( *.C or *.h ) */"<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirMacros"     ,path_dirMacros)<<endl;
  outHfile<<"  /*  Path of Raw Datas (*_Mu.txt) */"<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRawData"    ,path_dirRawData)<<endl;
  outHfile<<"  /*  Root path of Save Datas  */"<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirAnaPath"    ,path_dirAnaData)<<endl;
  outHfile<<"  /*  Path of All type Dir in root path  */"<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirIMFdat"     ,DirIMFdat     )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRotfile"    ,DirRotfile    )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirOperate"    ,DirOperate    )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirResult"     ,DirResult     )<<endl;
  outHfile<<"  /*  Path of Operation result and data  */"<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirOperEV"     ,DirOperEV     )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirOperEVGT"   ,DirOperEVGT   )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirOperEVGTST" ,DirOperEVGTST )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirOperEVGTLF" ,DirOperEVGTLF )<<endl;
  outHfile<<"  /*  Path of Result and data  */"<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_ChEff"  ,DirRes_ChEff  )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_ChRtPw" ,DirRes_ChRtPw )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_PwVCase",DirRes_PwVCase)<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_LFirTra",DirRes_LFirTra)<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_RatVT"  ,DirRes_RatVT  )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_PwidVT" ,DirRes_PwidVT )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_HousKep",DirRes_HousKep)<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_OpEff"  ,DirRes_OpEff  )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_PwiAna" ,DirRes_PwiAna )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_TempVT" ,DirRes_TempVT )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_DayEff" ,DirRes_DayEff )<<endl;
  
  
  outHfile<<"};"<<endl;
  outHfile.close();

  if(OPT=='P'){
  
    cout<<"Establishing the Data base!!"<<endl;
    cout<<"  /*  Path of Raw Datas (*_Mu.txt) */"<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRawData"    ,path_dirRawData)<<endl;
    cout<<"  /*  Root path of Save Datas  */"<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirAnaPath"    ,path_dirAnaData)<<endl;
    cout<<"  /*  Path of All type Dir in root path  */"<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirIMFdat"     ,DirIMFdat     )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRotfile"    ,DirRotfile    )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirOperate"    ,DirOperate    )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirResult"     ,DirResult     )<<endl;
    cout<<"  /*  Path of Operation result and data  */"<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirOperEV"     ,DirOperEV     )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirOperEVGT"   ,DirOperEVGT   )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirOperEVGTST" ,DirOperEVGTST )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirOperEVGTLF" ,DirOperEVGTLF )<<endl;
    cout<<"  /*  Path of Result and data  */"<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_ChEff"  ,DirRes_ChEff  )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_ChRtPw" ,DirRes_ChRtPw )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_PwVCase",DirRes_PwVCase)<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_LFirTra",DirRes_LFirTra)<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_RatVT"  ,DirRes_RatVT  )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_PwidVT" ,DirRes_PwidVT )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_HousKep",DirRes_HousKep)<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_OpEff"  ,DirRes_OpEff  )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_PwiAna" ,DirRes_PwiAna )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_TempVT" ,DirRes_TempVT )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_DayEff" ,DirRes_DayEff )<<endl;
    cout<<"Finish to establish the Data base!!"<<endl;
    
  }else{
    cout<<"hint: Skip Print the Information of Directory are establish.(!='P')"<<endl;
  }

}
