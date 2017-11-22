#include "replaymeasurehelp.h"

ReplayMeasureHelp::ReplayMeasureHelp()
{
    isMemOverThres=false;
    isCpuOverThres=false;
    isBatteryOverThers=false;
    isCpuTempOverThres=false;
    isWifiOverThres=false;
    isMobileOverThres=false;


}

void ReplayMeasureHelp::RecieveLogcatOutput()
{
    QString tempStr=QString::fromUtf8(pLogcat->readAllStandardOutput());
    emit SendProcessPutput(tempStr,deviceName);
}

ReplayMeasureHelp::~ReplayMeasureHelp()
{

}
