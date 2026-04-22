# System_Resource_Dashboard
A linux system tool that monitors the CPU usage, RAM usage and Disk usage. It also provides the process that are running in the CPU and it also provides option to kill the process.
**

**INSTALLATION AND SETUP**
Before the development the necessary packages are to be installed
Install the qt first
sudo apt update && sudo apt install -y qtbase5-dev qtcharts5-dev qttools5-dev build-essential
Install g++
sudo apt install g++ -y
Verify the versions
qmake --version
# Expected Output: QMake version 3.1, Using Qt version 5.15.13

**PROJECT SETUP**
Create the project directory
mkdir SystemResourceDashboard && cd SystemResourceDashboard
mkdir src include
Create .pro file for qt project
Create systemResourceDashboard.pro file
Add the necessary files, includes and versions in the pro file to build the project


**DEVELOPMENT**
Implement System monitoring
Used sysinfo and /proc/stat for CPU and RAM
Used statvfs for disk usage
Find the running process
Used qprocess to find the running process
Terminate the process
Used pkill to terminate the selected process


**UI DEVELOPMENT**
Implement main window
CPU, RAM and Disk usage are displayed in progress bars
A table listing the active process with disk usage
The kill button allows user to terminate the running process
The UI is updated every second


**DEPLOYMENT**
Create .desktop Entry
[Desktop Entry]
Name=System Resource Dashboard
Exec=/usr/local/bin/SystemResourceDashboard
Icon=/usr/local/share/icons/system-dashboard.png
Type=Application
Categories=Utility;System;
Install system wide
sudo cp SystemResourceDashboard /usr/local/bin/
sudo cp icon.png /usr/local/share/icons/system-dashboard.png
sudo cp SystemResourceDashboard.desktop /usr/share/applications/



**FINAL OUTPUT**
CPU, RAM and Disk usage are implemented
Running process and termination added
System level integration
Runs as a system level linux application

