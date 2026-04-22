#include "processmonitor.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QHash>
#include <algorithm>

ProcessMonitor::ProcessMonitor(QObject *parent) : QObject(parent) {}

QVector<ProcessInfo> ProcessMonitor::getTopCPUProcesses(int count) {
    QVector<ProcessInfo> processList;
    long totalCPUTime = getTotalCPUTime();
    QVector<int> pids = getAllPIDs();

    for (int pid : pids) {
        long processTime = getProcessCPUTime(pid);
        if (processTime == 0) continue;

        double cpuUsage = 0;
        if (previousProcessTimes.contains(pid)) {
            long prevProcessTime = previousProcessTimes.value(pid);
            long processTimeDiff = processTime - prevProcessTime;
            long totalTimeDiff = totalCPUTime - previousTotalTime;
            cpuUsage = (totalTimeDiff > 0) ? (100.0 * processTimeDiff / totalTimeDiff) : 0;
        }

        previousProcessTimes[pid] = processTime;

        if (cpuUsage > 0) {
            ProcessInfo info;
            info.pid = pid;
            info.name = getProcessName(pid);
            info.cpuUsage = cpuUsage;
            processList.append(info);
        }
    }

    previousTotalTime = totalCPUTime;

    std::sort(processList.begin(), processList.end(), [](const ProcessInfo &a, const ProcessInfo &b) {
        return b.cpuUsage < a.cpuUsage;
    });

    if (processList.size() > count) {
        processList.resize(count);
    }

    return processList;
}

long ProcessMonitor::getTotalCPUTime() const {
    QFile file("/proc/stat");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return 0;

    QTextStream in(&file);
    QString line = in.readLine();
    file.close();

    QStringList parts = line.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
    if (parts.size() < 5) return 0;

    long user = parts[1].toLong();
    long nice = parts[2].toLong();
    long system = parts[3].toLong();
    long idle = parts[4].toLong();

    return user + nice + system + idle;
}

long ProcessMonitor::getProcessCPUTime(int pid) const {
    QFile file(QString("/proc/%1/stat").arg(pid));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return 0;

    QTextStream in(&file);
    QString line = in.readLine();
    file.close();

    QStringList parts = line.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
    if (parts.size() < 17) return 0;

    long utime = parts[13].toLong();
    long stime = parts[14].toLong();

    return utime + stime;
}

QString ProcessMonitor::getProcessName(int pid) const {
    QFile file(QString("/proc/%1/comm").arg(pid));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return QString();

    QTextStream in(&file);
    QString name = in.readLine().trimmed();
    file.close();

    return name;
}

QVector<int> ProcessMonitor::getAllPIDs() const {
    QVector<int> pids;
    QDir procDir("/proc");
    QStringList entries = procDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QString &entry : entries) {
        bool ok;
        int pid = entry.toInt(&ok);
        if (ok) {
            pids.append(pid);
        }
    }

    return pids;
}
