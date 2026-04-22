#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // CPU
    cpuLabel = new QLabel("CPU Usage:", this);
    cpuBar = new QProgressBar(this);
    layout->addWidget(cpuLabel);
    layout->addWidget(cpuBar);

    // RAM
    ramLabel = new QLabel("RAM Usage:", this);
    ramBar = new QProgressBar(this);
    layout->addWidget(ramLabel);
    layout->addWidget(ramBar);

    // Disk
    diskLabel = new QLabel("Disk Usage:", this);
    diskBar = new QProgressBar(this);
    layout->addWidget(diskLabel);
    layout->addWidget(diskBar);

    // Process Table
    processTable = new QTableWidget(this);
    processTable->setColumnCount(3);
    processTable->setHorizontalHeaderLabels({"Process Name", "CPU Usage (%)", "Action"});
    processTable->horizontalHeader()->setStretchLastSection(true);
    processTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    processTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(processTable);

    setCentralWidget(centralWidget);

    // Timer to update stats every second
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &MainWindow::updateStats);
    updateTimer->start(1000);
}

MainWindow::~MainWindow() {
    delete updateTimer;
}

void MainWindow::updateStats() {
    // Update resource usage bars
    cpuBar->setValue(systemMonitor.getCPUUsage());
    ramBar->setValue(systemMonitor.getRAMUsage());
    diskBar->setValue(systemMonitor.getDiskUsage());

    // Update process table
    QVector<ProcessInfo> topProcesses = processMonitor.getTopCPUProcesses();

    processTable->setRowCount(0); // Clear previous data
    processTable->setRowCount(topProcesses.size());

    for (int i = 0; i < topProcesses.size(); ++i) {
        processTable->setItem(i, 0, new QTableWidgetItem(topProcesses[i].name));
        processTable->setItem(i, 1, new QTableWidgetItem(QString::number(topProcesses[i].cpuUsage, 'f', 2)));

        // Add Kill button
        QPushButton *killButton = new QPushButton("Kill", this);
        connect(killButton, &QPushButton::clicked, [this, i]() { killProcess(i); });
        processTable->setCellWidget(i, 2, killButton);
    }
}

void MainWindow::killProcess(int row) {
    QString processName = processTable->item(row, 0)->text();
    int ret = QProcess::execute("pkill", QStringList() << processName);

    if (ret == 0) {
        QMessageBox::information(this, "Success", QString("Process '%1' terminated successfully.").arg(processName));
    } else {
        QMessageBox::warning(this, "Failure", QString("Failed to terminate process '%1'.").arg(processName));
    }
}
