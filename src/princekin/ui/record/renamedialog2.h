#ifndef RENAMEDIALOG2_H
#define RENAMEDIALOG2_H

#include <QDialog>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include "globalvar.h"
namespace Ui {
class RenameDialog2;
}

class RenameDialog2 : public QDialog
{
    Q_OBJECT

public:
    explicit RenameDialog2(QWidget *parent = 0);
    ~RenameDialog2();

    void setFilePath(const QString &);

private:
    QString getBaseName(const QString &);
    QString getAbsolutePath(const QString &);
    void removeFile(const QString &);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::RenameDialog2 *ui;

    QString qFilePath;
};

#endif // RENAMEDIALOG2_H
