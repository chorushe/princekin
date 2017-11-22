#ifndef FORM_H
#define FORM_H

#include <QWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();
public:
    void setScriptText(const QString &);
private:
    Ui::Form *ui;
};

#endif // FORM_H
