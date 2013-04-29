#include "report.h"
#include "ui_report.h"
#include <QTableWidget>

report::report(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::report)
{
    ui->setupUi(this);
}

void report::get_report(double ** mas, int n, int m)
{
    QString name;
    for (int i=0; i<n; i++)
    {
        switch(i)
        {
        case 0: name="RR "; break;
        case 1: name="LRU "; break;
        case 2: name="MRU "; break;
        case 3: name="LFU "; break;
        case 4: name="ARC "; break;
        case 5: name="MQ "; break;
        case 6: name="2Q "; break;
        case 7: name="SLRU "; break;
        }

        if (mas[i][0]==1)
        {
            ui->text->insertPlainText(name);
            for (int j=0; j<m; j++)
            {
                    ui->text->insertPlainText(QString::number(mas[i][j])+QString::fromUtf8("\t"));
            }
        }
    }
ui->text->show();
}

report::~report()
{
    delete ui;
}
