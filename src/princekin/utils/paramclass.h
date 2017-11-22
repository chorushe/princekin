#ifndef PARAMCLASS_H
#define PARAMCLASS_H

#include <QString>

class ParamClass
{
public:
    ParamClass();

    int logLevel;
    bool count;
    QString countNum;
    bool seed;
    QString seedNum;
    bool throttle;
    QString throttleNum;
    bool random;
    //事件百分比
    bool touch;
    QString touchNum;
    bool motion;
    QString motionNum;
    bool trackBall;
    QString trackBalNum;
    bool syskeys;
    QString syskeysNum;
    bool nav;
    QString navNum;
    bool majorNav;
    QString majorNavNum;
    bool appSwitch;
    QString appSwitchNum;
    bool flip;
    QString flipNum;
    bool pinchZoom;
    QString pinchZoomNum;
    bool anyevent;
    QString anyeventNum;
    //类名约束
    QString className;
    //debug
    bool iCrashes;
    bool iTimeouts;
    bool iSecurity;
    bool dbgNoEvent;
    bool killProcess;
    bool monitorNative;
    bool waitDbg;
    bool hprof;
    //path
    QString filePath;
    //cmd
    QString cmdMonkeyStr;
};

#endif // PARAMCLASS_H
