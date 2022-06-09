#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>

namespace MuoAna_path_dir{
  /*  Path of Macros!!!( *.C or *.h ) */
  const char DirMacros       [180] = "/home/yusiang/TestSpace/Macro/";
  /*  Path of Raw Datas (*_Mu.txt) */
  const char DirRawData      [180] = "/data4/YuSiang/S4_11F_4lay_5deg/";
  /*  Path of Hk Datas (*_HK.txt) */
  const char DirHkData       [180] = "/data4/YuSiang/S4_11F_4lay_5deg/";
  /*  Root path of Save Datas  */
  const char DirAnaPath      [180] = "/home/yusiang/TestSpace/A/";
  /*  Path of All type Dir in root path  */
  const char DirIMFdat       [180] = "/home/yusiang/TestSpace/A/DetIMF/";
  const char DirRotfile      [180] = "/home/yusiang/TestSpace/A/rootfile/";
  const char DirOperate      [180] = "/home/yusiang/TestSpace/A/OperateResule/";
  const char DirResult       [180] = "/home/yusiang/TestSpace/A/Result/";
  const char DirHousKep      [180] = "/home/yusiang/TestSpace/A/HouseKeep/";
  /*  Path of Operation result and data  */
  const char DirOperEV       [180] = "/home/yusiang/TestSpace/A/OperateResule/RawEventDat/";
  const char DirOperEVGT     [180] = "/home/yusiang/TestSpace/A/OperateResule/EventGapDat/";
  const char DirOperEVGTST   [180] = "/home/yusiang/TestSpace/A/OperateResule/EvGTrackDat/";
  const char DirOperEVGTLF   [180] = "/home/yusiang/TestSpace/A/OperateResule/EvGTLFitDat/";
  /*  Path of Result and data  */
  const char DirRes_ChEff    [180] = "/home/yusiang/TestSpace/A/Result/BoardChannelEffective/";
  const char DirRes_ChRt     [180] = "/home/yusiang/TestSpace/A/Result/BoardGRtNor/";
  const char DirRes_ChPw     [180] = "/home/yusiang/TestSpace/A/Result/BoardGPwNor/";
  const char DirRes_PwVCase  [180] = "/home/yusiang/TestSpace/A/Result/BoardPwVCase/";
  const char DirRes_LFirTra  [180] = "/home/yusiang/TestSpace/A/Result/Tracking_Result/";
  const char DirRes_RatVT    [180] = "/home/yusiang/TestSpace/A/Result/RateVsTime/";
  const char DirRes_PwidVT   [180] = "/home/yusiang/TestSpace/A/Result/PwidthVsTime/";
  const char DirRes_HousKep  [180] = "/home/yusiang/TestSpace/A/Result/HouseKeeping/";
  const char DirRes_OpEff    [180] = "/home/yusiang/TestSpace/A/Result/OperateVsDate/";
  const char DirRes_TempVT   [180] = "/home/yusiang/TestSpace/A/Result/TemperatureVsTime/";
  const char DirRes_HumiVT   [180] = "/home/yusiang/TestSpace/A/Result/HumidityVsTime/";
  const char DirRes_DayEff   [180] = "/home/yusiang/TestSpace/A/Result/DayEff/";
  const char DirRes_RatVCh   [180] = "/home/yusiang/TestSpace/A/Result/RateVsChannel/";
  const char DirRes_RatVSun  [180] = "/home/yusiang/TestSpace/A/Result/RateVsSunnyDay/";
  const char DirRes_HKVRat   [180] = "/home/yusiang/TestSpace/A/Result/HuoseKeepDatVsRate/";
  /*  Name marker of raw datas  */
  const char name_Raw_txt    [50] = "2022*_Mu.txt";
  const char name_Raw_root   [50] = "2022*_Mu.root";
  const char name_Hk_txt     [50] = "2022*_HK.txt";
};
