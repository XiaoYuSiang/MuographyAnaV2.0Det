#include <typeinfo>
#include <algorithm>
#include <TStyle.h>
using namespace MuographAnaVariable;
namespace MuographGobelFuns{
  int BDcheck(int b){
    int z = -1;
    for(int i=0;i<NumBD;i++){
      if( b == BD[i] ){
        z = i;
        break;
      } 
    }if(z==-1){
      cout<<"ERROR:  Please Enter the true value b into the BDcheck(int b)!!!"<<endl;
      cout<<"Now the Enter value b is:  "<< b <<endl;
      throw;
    } 
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
  
  vector<string> HKRootList(const char * DirOperate){
    vector<string> HKFilePathList;
    char TmpFileRoute[200];
    char HKFileList[200];
    sprintf(HKFileList, "%sHKTxtNameAna.dat",DirOperate);
    // cout<<HKFileList<<endl;
    ifstream in(HKFileList);
    for(int i= 0; in>>TmpFileRoute ; i++){
      HKFilePathList.push_back(TmpFileRoute);
      //cout<<HKFilePathList[i]<<endl;
    }
    return HKFilePathList;
  }
  
  void FastStyleStats(float boundaryX,float boundaryY){
    gStyle-> SetStatX(boundaryX);
    gStyle-> SetStatY(boundaryY);
    gStyle-> SetStatW(0.2);
    gStyle-> SetStatH(0.1); 
  }
  
  float MaxSetUserRatio(const float M, const float R, const int ord=1){
    return float(int(M*pow(10,ord)*R)+pow(10,ord-1))/float(pow(10,ord));
  }
  float MinSetUserRatio(const float m, const float R, const int ord=1){
    return float(int(m*pow(10,ord)*R)-pow(10,ord-1))/float(pow(10,ord));
  }
  
  
};