#ifndef RenameDialog_H
#define RenameDialog_H

#include <QDialog>


namespace Ui {
class RenameDialog;
}

class RenameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RenameDialog(QString labelText,QWidget *parent = 0);
    ~RenameDialog();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();


public:
    void set(const QString &);
private:
    Ui::RenameDialog *ui;
public:
    QString inputName;
};

#endif // RenameDialog_H
