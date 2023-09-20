# Instruction for building from source on Ubuntu 22+, Linux Mint 21+, Debian 12+

## Install required dependencies

```bash
sudo apt-get install qmake6 qt6-base-dev libqt6websockets6-dev libqt6svg6-dev qt6-declarative-dev qml6-module-qtquick qml6-module-qtquick-controls qml6-module-qtquick-dialogs qml6-module-qtquick-templates qml6-module-qtquick-window qml6-module-qtquick-layouts qml6-module-qtqml-workerscript qml-qt6 libqt6quick6 libqt6quickcontrols2-6
```

## Prepare repository

```bash
cd ~
git clone https://github.com/trueromanus/ArdorQuery.git
cd anilibria-winmaclinux
```

## Building project

```bash
cd src
qmake
sudo make
```

## Running application

```bash
./ArdorQuery
```
