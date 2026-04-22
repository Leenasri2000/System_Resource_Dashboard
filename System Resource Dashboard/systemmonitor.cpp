#include "systemmonitor.h"
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QProcess>
#include <QDebug>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>

SystemMonitor::SystemMonitor(QObject *parent) : QObject(parent) {}

// Get CPU Usage from /proc/stat (Linux only)
int SystemMonitor::getCPUUsage() {
    static long prevTotal = 0, prevIdle = 0;

    QFile file("/proc/stat");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open /proc/stat";
        return 0;
    }

    QTextStream in(&file);
    QString line = in.readLine();
    file.close();

    if (line.isEmpty()) {
        qWarning() << "Empty /proc/stat";
        return 0;
    }

    QStringList parts = line.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
    if (parts.size() < 5) {
        qWarning() << "Unexpected format in /proc/stat";
        return 0;
    }

    long user = parts[1].toLong();
    long nice = parts[2].toLong();
    long system = parts[3].toLong();
    long idle = parts[4].toLong();
    long total = user + nice + system + idle;

    long totalDiff = total - prevTotal;
    long idleDiff = idle - prevIdle;

    prevTotal = total;
    prevIdle = idle;

    if (totalDiff == 0) return 0;

    return static_cast<int>(100 * (1.0 - (double(idleDiff) / totalDiff)));
}

int SystemMonitor::getRAMUsage() {
    struct sysinfo memInfo;
    sysinfo(&memInfo);

    long totalMem = memInfo.totalram;
    long freeMem = memInfo.freeram;

    long usedMem = totalMem - freeMem;
    return static_cast<int>((100 * usedMem) / totalMem);
}


int SystemMonitor::getDiskUsage(const QString& path) {
    struct statvfs stat;

    if (statvfs(path.toUtf8().constData(), &stat) != 0) {
        // Handle error
        return -1;
    }

    unsigned long totalBlocks = stat.f_blocks;
    unsigned long freeBlocks = stat.f_bfree;
    unsigned long usedBlocks = totalBlocks - freeBlocks;

    double totalSize = totalBlocks * stat.f_frsize;
    double usedSize = usedBlocks * stat.f_frsize;

    int usagePercentage = static_cast<int>((usedSize / totalSize) * 100);
    return usagePercentage;
}
