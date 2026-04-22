#ifndef SYSTEMMONITOR_H
#define SYSTEMMONITOR_H

#include <QObject>

class SystemMonitor : public QObject {
    Q_OBJECT

public:
    explicit SystemMonitor(QObject *parent = nullptr);

    int getCPUUsage();
    int getRAMUsage();
     int getDiskUsage(const QString& path = "/");
};

#endif // SYSTEMMONITOR_H
