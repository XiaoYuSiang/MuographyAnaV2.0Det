//include
#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include "/home/yusiang/personalLib/RootFile/untuplizerv8_YuSiang.h"
#include "path_dir.h"

using namespace std;
using namespace MuoAna_path_dir;

//channel vs rate combine
void DataNameAna(){
  
}

int DataNameAnaRoot(const char *NameChara,const char OPT='P') {
  char path_target[180],path_saved[180];
  sprintf(path_target,"%s%s",DirRotfile,NameChara);
  cout<<"finding raw .root:  "<<NameChara<<" file under dir:  "<<DirRotfile<<endl;
  vector<string> files = find_files(path_target);
  
  
  sprintf(path_saved,"%sDataRootNameAna.dat",DirOperate);
  ofstream out(path_saved);
  cout<<"Save: Raw .txt files:  \""<<NameChara<<"\"  name data in "<< path_saved <<endl;

  //cout<<"FileVectorSize:\t"<<files.size()<<endl;
  int i=0;
  for(;i<int(files.size());i++){
    if(i+1==int(files.size())){
      out<<files[i];
      if(OPT=='P') cout<<files[i]<<endl;
    }else{
      out<<files[i]<<endl;
      if(OPT=='P') cout<<files[i]<<endl;
    }
  }
  if(OPT!='P'){
    cout<<"hint: Skip Print the Information of Directory of All Raw .txt data.(!='P')"<<endl;
  }
  cout<<path_saved << " is saved"<<endl;
  return i;
}


int DataNameAnaTxt(const char *NameChara,const char OPT='P') {
  
  char path_target[180],path_saved[180];
  sprintf(path_target,"%s%s",DirRawData,NameChara);
  cout<<"finding raw .txt:  \""<<NameChara<<"\"  file under dir:  "<<DirRawData<<endl;
  vector<string> files = find_filestxt(path_target);
  
  
  sprintf(path_saved,"%sDataTxtNameAna.dat",DirOperate);
  ofstream out(path_saved);
  cout<<"Save: Raw .txt files:  "<< NameChara <<" name data in "<< path_saved <<endl;

  //cout<<"FileVectorSize:\t"<<files.size()<<endl;
  int i=0;
  for(;i<int(files.size());i++){
    if(i+1==int(files.size())){
      out<<files[i];
      if(OPT=='P') cout<<files[i]<<endl;
    }else{
      out<<files[i]<<endl;
      if(OPT=='P') cout<<files[i]<<endl;
    }
  }
  
  if(OPT!='P'){
    cout<<"hint: Skip Print the Information of Directory of All Raw .txt data.(!='P')"<<endl;
  }
  cout<<path_saved << " is saved"<<endl;
  return i;
}
