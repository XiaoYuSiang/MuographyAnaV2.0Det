# MuographyAnaV2.0Det
Muography Analysis Code for Detector V2.0

This is the analysis program of Muography for Detector V2.0 made by Yu Siang Xiao.

0.Change the analysis variable in the "AnaVariable.h"

Now, You can change the information in the MainControl.C 

1.Changing the path of floder which "input file" and want to do analysis for:
  path_Raw:   raw datas (ex: XXX_Mu.txt) 
    default:  "/data4/YuSiang/ExampleTestSpace/Data/" (example)
    
  path_Hk :   raw house keeping datas (ex: XXX_Hk.txt)
    default:  "/data4/YuSiang/ExampleTestSpace/Data/" (example)

2.The path you want to save the operation data and the result:
  path_OpR:
    default:  "/data4/YuSiang/ExampleTestSpace/Ana/"

3.The path of the folder which put the analysis "program files"
  path_Mac:   
    default: ./Macro/

4.Change the Data name want to be researched!!
  name_Raw:
    default: "2022*_Mu"

  name_Hk :
    default: "2022*_HK";
  PS: Examples of how to search are put in "MainControl.C"

5.Change the operation mode for you want:
  testMode        : default: false; //use small sample number in some pwidth ana. program.
  normalizeMode   : default: true ; //normalize the difference by the statistic in pwidth ana. program.
  NegHiRatChMode  : default: true ; //Could not use now!
  lowstatisticMode: default: false; //when the data come from large theta, use MuEelID to analyze the pwidth.
  rootFileMode    : default: true ; //re save the data of pwidth ana. want to use.
  SETightMode     : default: true ; //select event tightly for MuEelID in pwidth ana.
  PointTextMode   : default: true ; //draw the data point of HK VS Rate to be text(save time).

6.Set if you want to skip some block of program:
  bool SkipBlock  : default: true ;
      //true : Skip the programs in the range for:  if(SkipBlock==false){}
      //false: enable all of the programs except the "DSLAna.h"
  bool ReDSLAna   : default: true ;
      //rerun the DSLAna.h to save DSLData.h if change something in "AnaVariable.h"

7.Select the program want to be used:
  Put the program block to the outer of range for:  if(SkipBlock==false){}
  
8.Go to the path you save the result, to check the graph.

9.If the program or the graph has something wrong, or the graph has something that needs to improve. Please connect with Yu Siang Xiao.

Thank you for your use.
