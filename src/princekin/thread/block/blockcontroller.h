#ifndef BLOCKCONTROLLER_H
#define BLOCKCONTROLLER_H

#include "blockworker.h"

class BlockController : public QObject
{
    Q_OBJECT
public:
    BlockController();
    ~BlockController();

public:
    void startWorkerForTrafficService(const QString &);

private:
    QThread ThreadInstance;

};
#endif // BLOCKCONTROLLER_H
