#ifndef ALG_H
#define ALG_H

#include <QDialog>

namespace Ui {
    class alg;
}

class alg : public QDialog
{
    Q_OBJECT

public:
    explicit alg(QWidget *parent = 0);
    ~alg();

private:
    Ui::alg *ui;
};

#endif // ALG_H
