#include "paramclass.h"

ParamClass::ParamClass()
{
    logLevel=1;
    count=true;
    countNum="100";
    throttle=true;
    throttleNum="100";
    seed=true;
    seedNum="0";
    random=false;
    touch=false;
    touchNum="0";
    motion=false;
    motionNum="0";
    trackBall=false;
    trackBalNum="0";
    syskeys=false;
    syskeysNum="0";
    nav=false;
    navNum="0";
    majorNav=false;
    majorNavNum="0";
    appSwitch=false;
    appSwitchNum="0";
    flip=false;
    flipNum="0";
    pinchZoom=false;
    pinchZoomNum="0";
    anyevent=false;
    anyeventNum="0";

    className="";
    iCrashes=true;
    iTimeouts=true;
    iSecurity=true;
    dbgNoEvent=false;
    killProcess=false;
    monitorNative=true;
    waitDbg=false;
    hprof=false;

    filePath="";

    cmdMonkeyStr="";
}

