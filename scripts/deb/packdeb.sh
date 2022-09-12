#!/bin/bash

echo "Script for generate deb package"

git pull
cd ../../src
qmake6
sudo make

echo "Application builded"

cd ../scripts/deb/
mkdir tmp
yes | cp -r source/* tmp

echo "Folder for package prepared"

sed -i "/Version: 0.0.5/c\Version: $1" tmp/DEBIAN/control
sed -i "/Version=1.0/c\Version=$1" tmp/usr/share/applications/ArdorQuery.desktop

yes | cp ../../src/ArdorQuery tmp/usr/bin/

echo "Content replaced"

dpkg-deb -b tmp ardorquery_$1_amd64.deb

rm -rf tmp

echo "Package repacked"
