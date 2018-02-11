#include "sendsignalclass.h"

SendSignalClass::SendSignalClass(QObject *parent) : QObject(parent)
{

}

void SendSignalClass::emitCrash()
{
    emit sendCrashSignal();
}
