#ifndef CREATEPROJECT_H
#define CREATEPROJECT_H

#include <QDialog>


namespace Ui {
class CreateProject;
}

class CreateProject : public QDialog
{
    Q_OBJECT

public:
    explicit CreateProject(QString labelText,QWidget *parent = 0);
    ~CreateProject();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();


private:
    Ui::CreateProject *ui;
public:
    QString inputName;
};

#endif // CREATEPROJECT_H
