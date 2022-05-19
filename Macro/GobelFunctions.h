#include <typeinfo>
#include <algorithm>
using namespace MuographAnaVariable;
namespace MuographGobelFuns{
  int BDcheck(int b){
    int z = 0;
    if     ( b == BD[0] ) z = 0;
    else if( b == BD[1] ) z = 1;
    else if( b == BD[2] ) z = 2;
    else if( b == BD[3] ) z = 3;
    else throw;
    return z;
  }
  char  MonthC[13][5]={
    "-","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
  };
  
  vector<string> RawRootList(const char * DirOperate){
    vector<string> rootFilePathList;
    char TmpFileRoute[200];
    char rootFileList[200];
    sprintf(rootFileList, "%sDataRootNameAna.dat",DirOperate);
    // cout<<rootFileList<<endl;
    ifstream in(rootFileList);
    for(int i= 0; in>>TmpFileRoute ; i++){
      rootFilePathList.push_back(TmpFileRoute);
      //cout<<rootFilePathList[i]<<endl;
    }
    return rootFilePathList;
  }
  

};