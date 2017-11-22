#ifndef GLOBALVAR
#define GLOBALVAR

#include <QString>
#include <QHash>
#include <QProcess>


extern int gdLoop;
extern int gdInterval;
extern int gTravelNumber;
extern int gBehaviorNumber;
extern int gInterfaceNumber;
extern int gMonkeyNumber;
extern int gPerformanceNumber;
extern int gReplayNumber;
extern int gInterfaceTimeWarning;
extern int gInterfaceTotalNum;
extern int gInterfaceAveReponseTime;
extern int gInterfaceMaxTime;
extern int gInterfaceMinTime;

extern double memMin;
extern double memMax;
extern double cpuMin;
extern double cpuMax;
extern double batteryMin;
extern double batteryMax;
extern double cpuTempMin;
extern double cpuTempMax;
extern double wifiMin;
extern double wifiMax;
extern double mobileMin;
extern double mobileMax;
extern double gMemAve;
extern double gCpuAve;
extern double gBatteryAve;
extern double gCpuTempAve;
extern double gPerformanceWifi;
extern double gPerformanceMobile;
extern double gInterfaceTimeOutRate;

extern bool gIsStopTraffic;
extern bool gisMemCheck;
extern bool gisCpuCheck;
extern bool gisCpuTempCheck;
extern bool gisBatteryCheck;
extern bool gisWifiCheck;
extern bool gisMobileCheck;
extern bool gIsStopTrafficService;

extern QString gAppDir;
extern QString gConfigDir;
extern QString gtempConfigDir;
extern QString gAndroidJarDir;
extern QString gPackageName;
extern QString gRepositoryName;
extern QString gMyWorkSpace;
extern QString gRepositoryDir;
extern QString gUiautomatorDir;
extern QString gResultDir;
extern QString gScriptDir;
extern QString gModuleDir;
extern QString gVarDir;
extern QString gInvokeDir;
extern QString gSuiteDir;
extern QString gApkDir;
extern QString gDebugDir;
extern QString gCommandDir;
extern QString gTreeRootDir;
extern QString gReportDir;
extern QString gRepositoryDir2;
extern QString gUiautomatorDir2;
extern QString gResultDir2;
extern QString gModuleDir2;
extern QString gVarDir2;
extern QString gInvokeDir2;
extern QString gApkDir2;
extern QString gDebugDir2;
extern QString gCommandDir2;
extern QString gTreeRootDir2;
extern QString gLoop;
extern QString gInterval;
extern QString gTrafficInterval;
extern QString gTrafficSaveDir;
extern QString gNetState;
extern QString gLogDir;
extern QString gProjectDir;
extern QString gEmptyScriptDir;
extern QString gDeviceId;
extern QString gMailName;
extern QString gMailKey;
extern QString gWorkSpace;
extern QString gMemThres;
extern QString gCpuThres;
extern QString gBatteryThres;
extern QString gCpuTempThres;
extern QString gWifiThres;
extern QString gMobileThres;
extern QString gStatisticsConfigDir;
extern QString gStatisticsXmlDir;
extern QString gNewConfigDir;
extern QString gNewWorkSpaceDir;
extern QString gNewScriptDir;
extern QString gNewDisk;
extern QString gOldPackageName;
extern QString gIconPath;
extern QString gStartTextDate;
extern QString gDatePath;
extern QString gPackagePath;
extern QString gDaily_AppVersion;
extern QString gDaily_DeviceModel;
extern QString gDaily_SystemVersion;
extern QString gDaily_ModuleBaseName;
extern QString gCrashModuleList;
extern QString gDetailedPath;
extern QStringList gPvStringList;
extern QStringList gPvStringList2;
extern QStringList gVvStringList;
extern QStringList gBdStringList;
extern QStringList gCatonStringList;
extern QStringList gEventList;
extern QStringList gText;
extern QStringList gContentDesc;
extern QStringList gResourceId;
extern QStringList gClass;
extern QStringList gBounds;
extern QStringList gMemValue_list;
extern QStringList gCpuValue_list;
extern QStringList gCpuTempValue_list;
extern QStringList gBatteryTempValue_list;


extern QHash<QString,bool> gStopMemoryHash;
extern QHash<QString,bool> gHashCrash;

extern QHash<QString,int> gLetterHash;
extern QHash<QString,int> gErrorScriptNum;
extern QHash<QString,int> gAllScriptNum;
extern QHash<QString,int> gRightScriptNum;

extern QHash<QString,int> gMin_Mem_Hash;
extern QHash<QString,int> gMax_Mem_Hash;
extern QHash<QString,int> gMin_Cpu_Hash;
extern QHash<QString,int> gMax_Cpu_Hash;
extern QHash<QString,int> gMin_Batterytemp_Hash;
extern QHash<QString,int> gMax_Batterytemp_Hash;
extern QHash<QString,int> gMin_Cputemp_Hash;
extern QHash<QString,int> gMax_Cputemp_Hash;

extern QHash<QString,QString> gUidHash;
extern QHash<QString,QString> gNetStateHash;
extern QHash<QString,QString> gDeviceDir;
extern QHash<QString,QString> gHashCurrentScriptName;
extern QHash<QString,QString> gHashConfig;
extern QHash<QString,QString> gHashEvent;
extern QHash<QString,QString> gData_Traffic_Hash;

extern QHash<QString,QProcess *> gHashProc;
extern QMap<QString,bool> gmap;

extern QVector<double> memNum;
extern QVector<double> cpuNum;
extern QVector<double> memTimeInt;
extern QVector<double> cpuTimeInt;
extern QVector<double> memThres;
extern QVector<double> cpuThres;
extern QVector<double> batteryNum;
extern QVector<double> cpuTempNum;
extern QVector<double> batteryTimeInt;
extern QVector<double> cpuTempTimeInt;
extern QVector<double> batteryThres;
extern QVector<double> cpuTempThres;
extern QVector<double> wifiNum;
extern QVector<double> mobileNum;
extern QVector<double> wifiTimeInt;
extern QVector<double> mobileTimeInt;
extern QVector<double> wifiThres;
extern QVector<double> mobileThres;
extern QVector<double> gMemValue_vector;
extern QVector<double> gCpuValue_vector;
extern QVector<double> gCpuTempValue_vector;
extern QVector<double> gBatteryTempValue_vector;
extern QVector<QString> memTime;
extern QVector<QString> cpuTime;
extern QVector<QString> batteryTime;
extern QVector<QString> cpuTempTime;
extern QVector<QString> wifiTime;
extern QVector<QString> mobileTime;
extern QVector<QString> gExcelFilePath;

#endif // GLOBALVAR
