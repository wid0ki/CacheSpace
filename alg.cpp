#include "alg.h"
#include "ui_alg.h"

alg::alg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::alg)
{
    ui->setupUi(this);
}

alg::~alg()
{
    delete ui;
}
