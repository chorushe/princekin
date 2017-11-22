#include "measurereport.h"

MeasureReport::MeasureReport()
{

    for(int i=0;i<4;i++)
    {
        memInfo[i]=0;
        cpuInfo[i]=0;
        batteryInfo[i]=0;
        cpuTempInfo[i]=0;
    }

    wifiTrafficstats=0;
    mobileTrafficstats=0;

    memOverThresTime=0;
    cpuOverThresTime=0;
    batteryOverThresTime=0;
    cpuTempOverThresTime=0;
    wifiOverThresTime=0;
    mobileOverThresTime=0;

}

