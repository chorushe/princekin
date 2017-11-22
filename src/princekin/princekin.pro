#-------------------------------------------------
#
# Project created by QtCreator 2016-04-25T11:34:21
#
#-------------------------------------------------
QT       += core gui concurrent xml xmlpatterns xlsx axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = princekin
TEMPLATE = app


SOURCES += main.cpp\
        princekin.cpp \
    ui/record/record.cpp \
    utils/qcustomplot.cpp \
    utils/paramclass.cpp \
    ui/setting/monkeysetup.cpp \
    vtdxml/arrayList.cpp \
    vtdxml/autoPilot.cpp \
    vtdxml/binaryExpr.cpp \
    vtdxml/bookMark.cpp \
    vtdxml/cachedExpr.cpp \
    vtdxml/contextBuffer.cpp \
    vtdxml/decoder.cpp \
    vtdxml/elementFragmentNs.cpp \
    vtdxml/fastIntBuffer.cpp \
    vtdxml/fastLongBuffer.cpp \
    vtdxml/filterExpr.cpp \
    vtdxml/funcExpr.cpp \
    vtdxml/indexHandler.cpp \
    vtdxml/intHash.cpp \
    vtdxml/lex.yy.cc \
    vtdxml/literalExpr.cpp \
    vtdxml/locationPathExpr.cpp \
    vtdxml/nodeRecorder.cpp \
    vtdxml/numberExpr.cpp \
    vtdxml/pathExpr.cpp \
    vtdxml/textIter.cpp \
    vtdxml/transcoder.cpp \
    vtdxml/unaryExpr.cpp \
    vtdxml/unionExpr.cpp \
    vtdxml/variableExpr.cpp \
    vtdxml/vtdGen.cpp \
    vtdxml/vtdNav.cpp \
    vtdxml/vtdNav_L5.cpp \
    vtdxml/XMLChar.cpp \
    vtdxml/XMLModifier.cpp \
    vtdxml/xpath_driver.cc \
    vtdxml/xpathParser.tab.cc \
    utils/parsexml.cpp \
    utils/globalvar.cpp \
    utils/filemanager.cpp \
    ui/tree/createproject.cpp \
    ui/tree/deletedialog.cpp \
    ui/tree/renamedialog.cpp \
    ui/tree/treehelper.cpp \
    utils/sohuthread.cpp \
    utils/controller.cpp \
    utils/worker.cpp \
    utils/helper.cpp \
    utils/pushbutton.cpp \
    utils/dataclass.cpp \
    utils/filehandle.cpp \
    ui/replay/replayform.cpp \
    utils/assistmeasure.cpp \
    utils/measurereport.cpp \
    utils/servercontrol.cpp \
    utils/test.cpp \
    ui/setting/allsetting.cpp \
    utils/clickedlabel.cpp \
    utils/xmleditdialog.cpp \
    ui/setting/replaysetup.cpp \
    utils/replaymeasurehelp.cpp \
    ui/record/newwidget.cpp \
    thread/block/blockcontroller.cpp \
    thread/block/blockworker.cpp \
    ui/record/newscriptwidget.cpp \
    ui/setting/performancesetup.cpp \
    ui/behavior/behaviourwidget.cpp \
    ui/performance/performancewidget.cpp \
    ui/monkey/monkeywidget.cpp \
    ui/autotravel/autotravelwidget.cpp \
    ui/setting/autotravelsetup.cpp \
    ui/replay/createreport.cpp \
    ui/replay/initenv.cpp \
    ui/replay/performance.cpp \
    ui/replay/preset.cpp \
    ui/replay/writeresult.cpp \
    thread/battery/batterytempcontroller.cpp \
    thread/battery/batterytempworker.cpp \
    thread/cpu/cpucontroller.cpp \
    thread/cpu/cputempcontroller.cpp \
    thread/cpu/cputempworker.cpp \
    thread/cpu/cpuworker.cpp \
    thread/devices/devicesworker.cpp \
    thread/memory/memcontroller.cpp \
    thread/memory/memworker.cpp \
    thread/netstate/netstatecontroller.cpp \
    thread/netstate/netstateworker.cpp \
    thread/replay/replaycontroller.cpp \
    thread/replay/replayworker.cpp \
    thread/trafficstats/trafficcontroller.cpp \
    thread/trafficstats/trafficworker.cpp \
    ui/autotravel/cmdworker.cpp \
    thread/trafficstats2/trafficcontroller2.cpp \
    thread/trafficstats2/trafficworker2.cpp \
    ui/autotravel/travelworker.cpp \
    ui/performance/trafficstatdetail.cpp \
    ui/performance/performancedetail.cpp \
    ui/performance/tempdetail.cpp \
    ui/performance/chartdetail.cpp \
    thread/chart/chartcontroller.cpp \
    thread/chart/chartworker.cpp \
    thread/chart/datacontroller.cpp \
    thread/chart/datatworker.cpp \
    ui/behavior/urltoxml.cpp \
    ui/setting/behavioursetup.cpp \
    ui/performance/datacomparewidget.cpp \
    ui/performance/listwidget.cpp \
    xlsxinfo/readsource.cpp \
    xlsxinfo/writeinfo.cpp \
    xlsxinfo/xlsxinfo.cpp \
    ui/setting/pulldownwidget.cpp \
    ui/setting/princekinabout.cpp \
    xlsxinfo/generatexlsx.cpp \
    utils/assistbehaviorworker.cpp \
    thread/frames/framescontroller.cpp \
    thread/frames/framesworker.cpp \
    xlsxinfo/appsize.cpp \
    ui/interface/interfacewidget.cpp \
    ui/interface/interdataclass.cpp \
    ui/setting/interfacesetup.cpp \
    ui/behavior/editmultixml.cpp \
    utils/staticticsherper.cpp \
    tool/apkinfoform.cpp \
    tool/toolboxwidget.cpp \
    tool/getlogcatwidget.cpp \
    xlsxinfo/htmlinfo.cpp \
    xlsxinfo/replayhtml.cpp \
    utils/SwitchControl.cpp \
    ui/behavior/replayforbehaviorcontroller.cpp \
    ui/behavior/replayforbehaviorworker.cpp \
    ui/behavior/behaviorworker.cpp \
    thread/adb/adbshellcontroller.cpp \
    thread/adb/adbshellworker.cpp \
    xlsxinfo/excelcontroller.cpp \
    xlsxinfo/excelworker.cpp \
    utils/execmd.cpp \
    ui/replay/form.cpp \
    tool/getlogcatworker.cpp \
    xlsxinfo/replayhtmlcontroller.cpp \
    ui/monkey/monkeyworker.cpp \  
    ui/replay/createcrashreport.cpp \
    ui/record/editscriptfilewidget.cpp
    ui/record/editscriptfilewidget.cpp


HEADERS  += princekin.h \
    ui/record/record.h \
    utils/qcustomplot.h \
    utils/paramclass.h \
    ui/setting/monkeysetup.h \
    vtdxml/arrayList.h \
    vtdxml/autoPilot.h \
    vtdxml/binaryExpr.h \
    vtdxml/bookMark.h \
    vtdxml/cachedExpr.h \
    vtdxml/contextBuffer.h \
    vtdxml/customTypes.h \
    vtdxml/decoder.h \
    vtdxml/elementFragmentNs.h \
    vtdxml/everything.h \
    vtdxml/expr.h \
    vtdxml/fastIntBuffer.h \
    vtdxml/fastLongBuffer.h \
    vtdxml/filterExpr.h \
    vtdxml/FlexLexer.h \
    vtdxml/funcExpr.h \
    vtdxml/indexHandler.h \
    vtdxml/intHash.h \
    vtdxml/literalExpr.h \
    vtdxml/location.h \
    vtdxml/location.hh \
    vtdxml/locationPathExpr.h \
    vtdxml/nodeRecorder.h \
    vtdxml/numberExpr.h \
    vtdxml/pathExpr.h \
    vtdxml/position.hh \
    vtdxml/stack.h \
    vtdxml/stack.hh \
    vtdxml/test.h \
    vtdxml/textIter.h \
    vtdxml/transcoder.h \
    vtdxml/unaryExpr.h \
    vtdxml/unionExpr.h \
    vtdxml/UTF8Char.h \
    vtdxml/variableExpr.h \
    vtdxml/vtdException.h \
    vtdxml/vtdGen.h \
    vtdxml/vtdNav.h \
    vtdxml/vtdNav_L5.h \
    vtdxml/XMLChar.h \
    vtdxml/XMLModifier.h \
    vtdxml/xpath_driver.h \
    vtdxml/xpathParser.tab.hh \
    utils/parsexml.h \
    commonhead/commonhead.h \
    commonhead/componenthead.h \
    utils/globalvar.h \
    utils/filemanager.h \
    ui/tree/createproject.h \
    ui/tree/deletedialog.h \
    ui/tree/renamedialog.h \
    ui/tree/treehelper.h \
    utils/sohuthread.h \
    utils/controller.h \
    utils/worker.h \
    utils/helper.h \
    utils/pushbutton.h \
    utils/dataclass.h \
    utils/filehandle.h \
    ui/replay/replayform.h \
    utils/assistmeasure.h \
    utils/measurereport.h \
    utils/servercontrol.h \
    utils/test.h \
    ui/setting/allsetting.h \
    utils/clickedlabel.h \
    utils/xmleditdialog.h \
    ui/setting/replaysetup.h \
    utils/replaymeasurehelp.h \
    ui/record/newwidget.h \
    thread/block/blockcontroller.h \
    thread/block/blockworker.h \
    ui/record/newscriptwidget.h \
    ui/setting/performancesetup.h \
    ui/behavior/behaviourwidget.h \
    ui/performance/performancewidget.h \
    ui/monkey/monkeywidget.h \
    ui/autotravel/autotravelwidget.h \
    ui/setting/autotravelsetup.h \
    ui/replay/createreport.h \
    ui/replay/initenv.h \
    ui/replay/performance.h \
    ui/replay/preset.h \
    ui/replay/writeresult.h \
    thread/battery/batterytempcontroller.h \
    thread/battery/batterytempworker.h \
    thread/cpu/cpucontroller.h \
    thread/cpu/cputempcontroller.h \
    thread/cpu/cputempworker.h \
    thread/cpu/cpuworker.h \
    thread/devices/devicesworker.h \
    thread/memory/memcontroller.h \
    thread/memory/memworker.h \
    thread/netstate/netstatecontroller.h \
    thread/netstate/netstateworker.h \
    thread/replay/replaycontroller.h \
    thread/replay/replayworker.h \
    thread/trafficstats/trafficcontroller.h \
    thread/trafficstats/trafficworker.h \
    ui/autotravel/cmdworker.h \
    thread/trafficstats2/trafficcontroller2.h \
    thread/trafficstats2/trafficworker2.h \
    ui/autotravel/travelworker.h \
    ui/performance/trafficstatdetail.h \
    ui/performance/performancedetail.h \
    ui/performance/tempdetail.h \
    ui/performance/chartdetail.h \
    thread/chart/chartcontroller.h \
    thread/chart/chartworker.h \
    thread/chart/datacontroller.h \
    thread/chart/dataworker.h \
    ui/behavior/urltoxml.h \
    ui/setting/behavioursetup.h \
    ui/performance/datacomparewidget.h \
    ui/performance/listwidget.h \
    xlsxinfo/readsource.h \
    xlsxinfo/writeinfo.h \
    xlsxinfo/xlsxinfo.h \
    ui/setting/pulldownwidget.h \
    ui/setting/princekinabout.h \
    xlsxinfo/generatexlsx.h \
    utils/assistbehaviorworker.h \
    thread/frames/framescontroller.h \
    thread/frames/framesworker.h \
    xlsxinfo/appsize.h \
    ui/interface/interfacewidget.h \
    ui/interface/interdataclass.h \
    ui/setting/interfacesetup.h \
    ui/behavior/editmultixml.h \
    utils/staticticsherper.h \
    tool/apkinfoform.h \
    tool/toolboxwidget.h \
    tool/getlogcatwidget.h \
    xlsxinfo/htmlinfo.h \
    xlsxinfo/replayhtml.h \
    utils/SwitchControl.h \
    ui/behavior/replayforbehaviorcontroller.h \
    ui/behavior/replayforbehaviorworker.h \
    ui/behavior/behaviorworker.h \
    thread/adb/adbshellcontroller.h \
    thread/adb/adbshellworker.h \
    xlsxinfo/excelcontroller.h \
    xlsxinfo/excelworker.h \
    utils/execmd.h \
    ui/replay/form.h \
    tool/getlogcatworker.h \
    xlsxinfo/replayhtmlcontroller.h \
    ui/monkey/monkeyworker.h \
    ui/replay/createcrashreport.h \
    ui/record/editscriptfilewidget.h
    ui/record/editscriptfilewidget.h

FORMS    += princekin.ui \
    ui/record/record.ui \
    ui/tree/createproject.ui \
    ui/tree/renamedialog.ui \
    ui/tree/deletedialog.ui \
    ui/replay/replayform.ui \
    ui/setting/allsetting.ui \
    ui/setting/replaysetup.ui \
    ui/record/newwidget.ui \
    ui/record/newscriptwidget.ui \
    ui/setting/performancesetup.ui \
    ui/behavior/behaviourwidget.ui \
    ui/performance/performancewidget.ui \
    ui/monkey/monkeywidget.ui \
    ui/autotravel/autotravelwidget.ui \
    ui/setting/autotravelsetup.ui \
    ui/replay/settingreplay.ui \
    ui/performance/trafficstatdetail.ui \
    ui/performance/performancedetail.ui \
    ui/performance/tempdetail.ui \
    ui/performance/chartdetail.ui \
    ui/behavior/urltoxml.ui \
    ui/setting/behavioursetup.ui \
    ui/performance/datacomparewidget.ui \
    ui/setting/pulldownwidget.ui \
    ui/setting/princekinabout.ui \
    ui/interface/interfacewidget.ui \
    ui/setting/interfacesetup.ui \
    ui/behavior/editmultixml.ui \
    tool/apkinfoform.ui \
    tool/toolboxwidget.ui \
    tool/getlogcatwidget.ui \
    ui/replay/form.ui \
    ui/record/editscriptfilewidget.ui


RESOURCES += \
    qrc/princekin.qrc \

RC_ICONS += \
    qrc/logo/crown.ico


INCLUDEPATH += ui/run\
    ui/record\
    ui/replay\
    ui/tree\
    ui/setting\
    ui/batch\
    ui/dialog\
    ui/behavior\
    ui/monkey\
    ui/performance\
    ui/setting\
    ui/trafficStats\
    utils\
    commonhead\
    vtdxml\
    core\
    thread/battery \
    thread/cpu \
    thread/memory \
    thread/temperature\
    thread/trafficstats\
    thread/trafficstats2\
    thread/netstate\
    thread/replay\
    thread/chart \
    thread/frames \
    thread/adb \
    xlsxinfo

DISTFILES += \
    utils/filemanager.cpp.bak \
    qrc/images/addexcel.png
