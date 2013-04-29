#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "auth.h"
#include "alg.h"
#include <time.h>
#include <ctime>
#include <stdlib.h>
#include <QProgressDialog>
#include <QTime>
#include <QDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->indLogOutputText->setReadOnly(true);
    ui->stReport->setReadOnly(true);
    ui->stCacheSize->setRange(0,100);
    ui->stQueueSize->setRange(0,10000);
    ps = new QGraphicsScene;
    ps->setSceneRect(0.0,0.0,400.0,250.0);
    ui->grapgArea->setScene(ps);
    connect (ui->alg, SIGNAL(triggered()), this, SLOT(alg_slot()));
    connect (ui->auth, SIGNAL(triggered()), this, SLOT(auth_slot()));
    connect (ui->exit, SIGNAL(triggered()), this, SLOT(close()));
    connect (ui->stForEachAlg, SIGNAL(clicked(bool)), this, SLOT (all_checked_st()));
    connect (ui->grForEach, SIGNAL(clicked(bool)), this, SLOT (all_checked_gr()));
    connect (ui->stCacheSize, SIGNAL (valueChanged(int)), this, SLOT (output_cache_size()));
    connect (ui->stQueueSize, SIGNAL (valueChanged(int)), this, SLOT (output_queue_size()));
}

void MainWindow::output_cache_size()
{
    ui->stCacheOutput->display(ui->stCacheSize->value());
}

void MainWindow::output_queue_size()
{
    ui->stQueueOutput->display(ui->stQueueSize->value());
}

void MainWindow::all_checked_st()
{
    if (!ui->stForEachAlg->isTristate())
    {
        ui->stLRU->setTristate(true);
        ui->stLRU->setCheckState(Qt::Checked);
        ui->stMRU->setTristate(true);
        ui->stMRU->setCheckState(Qt::Checked);
        ui->stRR->setTristate(true);
        ui->stRR->setCheckState(Qt::Checked);
    }
}

void MainWindow::all_checked_gr()
{
    if (!ui->grForEach->isTristate())
    {
        ui->grLRU->setTristate(true);
        ui->grLRU->setCheckState(Qt::Checked);
        ui->grMRU->setTristate(true);
        ui->grMRU->setCheckState(Qt::Checked);
        ui->grRR->setTristate(true);
        ui->grRR->setCheckState(Qt::Checked);
    }
}

void MainWindow::auth_slot()
{
    auth *s=new auth;
    s->show();
}

void MainWindow::alg_slot()
{
    alg *s=new alg;
    s->show();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ps;
}

bool MainWindow::notfull (cache* c, int cs)
{
    bool t=true;
    for (int i=0;i<cs;i++)
    {
        if (c[i].full==false) t=false;
    }
    return !t;
}

void MainWindow::stPrintLog (cache* ca4, int cs, list<int> q, bool bit)
{
    ui->indLogOutputText->appendPlainText(QString::fromUtf8("\nCache contains:\n"));
    for (int i=0; i<cs; i++)
    {
        ui->indLogOutputText->insertPlainText(QString::fromUtf8("|")+QString::number(ca4[i].el)+QString::fromUtf8("|"));
    }
    if (bit)
    {
        ui->indLogOutputText->appendPlainText(QString::fromUtf8("\n"));
        for (int i=0; i<cs; i++)
        {
            ui->indLogOutputText->insertPlainText(QString::fromUtf8("|")+QString::number(ca4[i].bit_old)+QString::fromUtf8("|"));
        }
    }
    ui->indLogOutputText->appendPlainText(QString::fromUtf8("List contains: \n"));
    list<int>::iterator it=q.begin();
    while (it!=q.end())
    {
        ui->indLogOutputText->insertPlainText(QString::number(*it)+QString::fromUtf8(" "));
        it++;
    }
}

int size_list_cache(list<cache> foo)
{
    int size=0;
    list<cache>::iterator it=foo.begin();
    while (it!=foo.end())
    {
        size++;
        it++;
    }
    return size;
}

void MainWindow::LRU (int cs, int qs, bool print)
{
    float get=0;
    int noget=0;
    bool valid;
    list<int> q;
    for (int i=0; i<qs; i++) q.push_back(rand()%qs);
    list<int>::iterator p=q.begin();
    cache* c;
    c=new cache [cs];
    for (int i=0; i<cs; i++)
    {
        c[i].bit_old=0;
        c[i].el=0;
        c[i].full=false;
    }
    int id=0;
    int j;
    while (!q.empty())
    {
        if (print)
            stPrintLog (c, cs, q,true);
        valid=false;
        p=q.begin();

        for (int i=0; i<cs; i++)
        {
            if (c[i].full==true)
            c[i].bit_old++;
        }

        for (int i=0; i<cs; i++)
        {
            if (c[i].el==(*p))
            {
                valid=true;
                id=i;
            }
        }

        if (valid)
        {
            get++;
            c[id].bit_old=0;
        }
        else
        {
            noget++;
            if (notfull(c,cs))
            {
                j=0;
                while (c[j].full==false) j++;
                c[j-1].el=*p;
                c[j-1].bit_old=0;
                c[j-1].full=true;
            }
            else
            {
                for (int i=0;i<cs;i++)
                {
                    if(c[i].bit_old>c[id].bit_old) id=i;
                }
                c[id].el=*p;
                c[id].bit_old=0;
            }
        }
        q.erase(p);
    }
    if (print)
    {
        if (cs!=0 && qs!=0)
        {
            ui->indLogOutputText->appendPlainText(QString::fromUtf8("Lathency:  "));
            ui->indLogOutputText->insertPlainText(QString::number(get/(noget+get)));
        }
        else
            ui->indLogOutputText->appendPlainText(QString::fromUtf8("Please, input queue size and cache size...I check it!"));
    }
    graphInfo=get/(noget+get);
}

void MainWindow::MRU (int cs, int qs, bool print)
{
    float get=0;
    int noget=0;
    list<int> q;
    for (int i=0; i<qs; i++) q.push_back(rand()%qs);
    list<int>::iterator p=q.begin();
    cache* c;
    c=new cache [cs];
    for (int i=0; i<cs; i++)
    {
        c[i].bit_old=0;
        c[i].el=0;
        c[i].full=false;
    }

    int id=0;
    int j;
    bool valid;

    while (!q.empty())
    {
        if (print)
            stPrintLog (c, cs, q, true);
        valid=false;
        p=q.begin();

        for (int i=0; i<cs; i++)
        {
            if (c[i].full==true)
            c[i].bit_old++;
        }

        for (int i=0; i<cs; i++)
        {
            if (c[i].el==(*p))
            {
                valid=true;
                id=i;
            }
        }
        if (valid)
        {
            get++;
            c[id].bit_old=0;
        }
        else
        {
            noget++;
            if (notfull(c,cs))
            {
                j=0;
                while (c[j].full==false) {j++;}
                c[j-1].el=*p;
                c[j-1].bit_old=0;
                c[j-1].full=true;
            }
            else
            {
                for (int i=0;i<cs;i++)
                {
                    if(c[i].bit_old<c[id].bit_old) id=i;
                }
                c[id].el=*p;
                c[id].bit_old=0;
                }
            }
        q.erase(p);
    }
    if (print)
    {
        if (cs!=0 && qs!=0)
        {
            ui->indLogOutputText->appendPlainText(QString::fromUtf8("Lathency:  "));
            ui->indLogOutputText->insertPlainText(QString::number(get/(noget+get)));
        }
        else
            ui->indLogOutputText->appendPlainText(QString::fromUtf8("Please, input queue size and cache size...I check it!"));
    }
    graphInfo=get/(noget+get);
}

void MainWindow::RR (int cs, int qs, bool print)
{
        float get=0.0;
        int noget=0;
        list<int> q;
        for (int i=0; i<qs; i++) q.push_back(rand()%qs);
        list<int>::iterator p=q.begin();
        cache* c;
        c=new cache [cs];
        bool valid;
        for (int i=0; i<cs; i++)
        {
            c[i].el=0;
            c[i].full=false;
            c[i].bit_old=0;
        }
        while (!q.empty())
        {
            if (print)
                stPrintLog(c,cs,q, false);
                valid=false;
                p=q.begin();

                for (int i=0; i<cs; i++)
                {
                    if (c[i].full)
                        c[i].bit_old++;
                        if (c[i].el==*p)
                        {
                            valid=true;
                            c[i].bit_old=0;
                        }
                }

                if (valid)
                {
                        get++;
                }
                else
                {
                        noget++;
                        c[rand()%cs].el=*p;

                }
                q.erase(p);
        }
        if (print)
        {
            if (cs!=0 && qs!=0)
            {
                ui->indLogOutputText->appendPlainText(QString::fromUtf8("Lathency:  "));
                ui->indLogOutputText->insertPlainText(QString::number(get/(noget+get)));
            }
            else
                ui->indLogOutputText->appendPlainText(QString::fromUtf8("Please, input queue size and cache size...I check it!"));
        }
        graphInfo=get/(noget+get);
}

void MainWindow::on_stCountStat_clicked()
{
    int n = ui->stCountAtt->value()+1;
    if (n==0) ui->statusBar->showMessage(QString::fromUtf8("Input the number of attempts"),3000);
    else
    {
        int cs=ui->stCacheSize->value();
        int qs=ui->stQueueSize->value();
        QProgressDialog progress(QString::fromUtf8("Count elapsed time"), QString::fromUtf8("Abort"), 0, n*8, this);
        progress.setWindowModality(Qt::WindowModal);
        progress.setWindowTitle(QString::fromUtf8("Processing..."));
        progress.show();

        int progr = 0;
        clock_t time;
        progress.setValue(progr);
        if (ui->stLRU->isChecked())
        {
            ui->stReport->insertPlainText(QString::fromUtf8("LRU:\n"));
            for (int i=1; i<n; i++)
            {
                time = clock();
                LRU (cs,qs,false);
                time = clock() - time;
                ui->stReport->insertPlainText(QString::number(time)+QString::fromUtf8("\t"));
                progr++;
                progress.setValue(progr);
            }
        }
        if (ui->stMRU->isChecked())
        {
            ui->stReport->insertPlainText(QString::fromUtf8("\n\nMRU:\n"));
            for (int i=1; i<n; i++)
            {
                time = clock();
                MRU(cs,qs,false);
                time = clock() - time;
                ui->stReport->insertPlainText(QString::number(time)+QString::fromUtf8("\t"));
                progr++;
                progress.setValue(progr);
            }
        }
        if (ui->stRR->isChecked())
        {
            ui->stReport->insertPlainText(QString::fromUtf8("\n\nRR:\n"));
            for (int i=1; i<n; i++)
            {
                time = clock();
                RR(cs,qs,false);
                time = clock() - time;
                ui->stReport->insertPlainText(QString::number(time)+QString::fromUtf8("\t"));
                progr++;
                progress.setValue(progr);
            }
        }
        progress.close();
    }
}

void MainWindow::on_indDoAlg_clicked()
{
    int cs=ui->indCacheSize->value();
    int qs=ui->indQueueSize->value();
    switch (ui->indAlg->currentIndex())
    {
    case 0: LRU (cs,qs,true);
            ui->statusBar->showMessage(QString::fromUtf8("Choosen LRU Algoritm for Caching"),3000);
            break;
    case 1: MRU (cs,qs,true);
            ui->statusBar->showMessage(QString::fromUtf8("Choosen MRU Algoritm for Caching"),3000);
            break;
    case 2: RR (cs,qs,true);
            ui->statusBar->showMessage(QString::fromUtf8("Choosen RR. Respect Random, plz"),6000);
            break;
    }
    if (cs==0) ui->statusBar->showMessage(QString::fromUtf8("I can't illustrate cache policy without cache"),3000);
    if (qs==0) ui->statusBar->showMessage(QString::fromUtf8("I can't work without my own goal"),3000);
}

void MainWindow::on_indLogOutput_clicked()
{
    int n = QMessageBox::warning(this,QString::fromUtf8("Warning"),
                                 QString::fromUtf8("Do you really want to destroy everything that I did?"),
                                 QString::fromUtf8("Yes"), QString::fromUtf8("No"),0,1);
    if (!n)
    {
        ui->indLogOutputText->clear();
        ui->statusBar->showMessage(QString::fromUtf8("I'm stay soo empty..."),3000);
    }
    else
        ui->statusBar->showMessage(QString::fromUtf8("Thanx a lot"),3000);
}

void MainWindow::on_grDrawPlots_clicked()
{
    if ((ui->grLRU->isChecked() || ui->grMRU->isChecked() || ui->grRR->isChecked())==false)
    {
        QMessageBox::critical(this, QString::fromUtf8("Error"), QString::fromUtf8("Please choose some chache policy for drawing"), QMessageBox::Ok);
    }
    else
    {
        ps->clear();
        ps->addLine(0,0,0,270, QPen (Qt::red,3,Qt::SolidLine));
        ps->addLine(0,0,7,10,QPen (Qt::red,3,Qt::SolidLine));
        ps->addLine(0,0,-7,10,QPen (Qt::red,3,Qt::SolidLine));
        ps->addLine(-20,250,400,250, QPen (Qt::red,3,Qt::SolidLine));
        ps->addLine(400,250,390,257,QPen (Qt::red,3,Qt::SolidLine));
        ps->addLine(400,250,390,243,QPen (Qt::red,3,Qt::SolidLine));
        scatterPlot();
    }
}

void MainWindow::scatterPlot () //точечная
{
    ui->statusBar->showMessage(QString::fromUtf8("Choosen Scatter Plot style for drawing"),3000);
    for (int i=1; i<50; i=i+10)
    {
        for (int j=1; j<5; j=j+1)
        {
            int alg_count=0;
            if (ui->grLRU->isChecked())
            {
                LRU(j,i,false);
                ps->addEllipse(i+50*alg_count,graphInfo*100,10,10,QPen (Qt::black,3,Qt::SolidLine),QBrush(Qt::green));
                alg_count++;
            }
            if (ui->grRR->isChecked())
            {
                RR(j,i,false);
                ps->addEllipse(i+50*alg_count,graphInfo*100,10,10,QPen (Qt::black,3,Qt::SolidLine),QBrush(Qt::magenta));
                alg_count++;
            }
            if (ui->grMRU->isChecked())
            {
                MRU(j,i,false);
                ps->addEllipse(i+50*alg_count,graphInfo*100,10,10,QPen (Qt::black,3,Qt::SolidLine),QBrush(Qt::blue));
                alg_count++;
            }
        }
    }
}

void MainWindow::on_stClearLog_clicked()
{
    int n = QMessageBox::warning(this,QString::fromUtf8("Warning"),
                                 QString::fromUtf8("Do you really want to destroy everything that I did?"),
                                 QString::fromUtf8("Yes"), QString::fromUtf8("No"),0,1);
    if (!n)
    {
        ui->stReport->clear();
        ui->statusBar->showMessage(QString::fromUtf8("I'm stay soo empty..."),3000);
    }
    else
        ui->statusBar->showMessage(QString::fromUtf8("Thanx a lot"),3000);
}
