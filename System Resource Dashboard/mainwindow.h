#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QProgressBar>
#include <QTableWidget>
#include <QTimer>
#include <QPushButton>

#include "systemmonitor.h"
#include "processmonitor.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateStats();
    void killProcess(int row);

private:
    QLabel *cpuLabel;
    QProgressBar *cpuBar;

    QLabel *ramLabel;
    QProgressBar *ramBar;

    QLabel *diskLabel;
    QProgressBar *diskBar;

    QTableWidget *processTable;

    QTimer *updateTimer;
    SystemMonitor systemMonitor;
    ProcessMonitor processMonitor;
};

#endif // MAINWINDOW_H
