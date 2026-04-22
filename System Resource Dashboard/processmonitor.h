#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QHash>

struct ProcessInfo {
    int pid;
    QString name;
    double cpuUsage;
};

class ProcessMonitor : public QObject {
    Q_OBJECT

public:
    explicit ProcessMonitor(QObject *parent = nullptr);
    QVector<ProcessInfo> getTopCPUProcesses(int count = 5);

private:
    long getTotalCPUTime() const;
    long getProcessCPUTime(int pid) const;
    QString getProcessName(int pid) const;
    QVector<int> getAllPIDs() const;

    QHash<int, long> previousProcessTimes;
    long previousTotalTime = 0;
};

#endif // PROCESSMONITOR_H
