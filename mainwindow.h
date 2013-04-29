#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <list.h>

namespace Ui
{
    class MainWindow;
}

struct cache
{
    int bit_old;
    int el;
    bool full;
};

struct freq
{
        int el;
        int count;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QGraphicsScene* ps;
    explicit MainWindow(QWidget *parent = 0);
    bool notfull (cache*, int);
    void LRU (int, int, bool);
    void MRU (int, int, bool);
    void RR (int, int, bool);
    void stPrintLog (cache*, int, list<int>, bool);
    void scatterPlot ();
    float graphInfo;
    int x;
    int y;
    ~MainWindow();

public slots:
    void alg_slot();
    void auth_slot();
    void all_checked_st ();
    void all_checked_gr ();
    void output_cache_size();
    void output_queue_size();

private slots:
    void on_stCountStat_clicked();
    void on_grDrawPlots_clicked();
    void on_indDoAlg_clicked();
    void on_indLogOutput_clicked();
    void on_stClearLog_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
