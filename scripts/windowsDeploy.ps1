#################################################
## Deploy ArdorQuery application for Windows
##
## You need create environment variable QTWINDEPLOYPATH
## and set value to full path of windeployqt.exe
##
#################################################

# Prepare files
Remove-Item -Path ../deploy/output -Recurse -Force -ErrorAction Ignore
New-Item -Path ../deploy -Name "output" -ItemType "directory"
Copy-Item -Path ../releaseBuild/release/ArdorQuery.exe -Destination ../deploy/output

# Build project

& "C:\IDEs\opensourceqt\6.7.1\msvc2019_64\bin\windeployqt.exe" --no-translations --release --no-sql --no-xml --no-positioning --no-system-d3d-compiler --no-opengl-sw --no-3dcore --no-3dquick --no-3dinput --no-3danimation --no-3dextras --qmldir ../src ../deploy/output/ArdorQuery.exe

# Remove redundant thing
Remove-Item -Path ../deploy/output/qmltooling -Recurse -Force
Remove-Item -Path ../deploy/output/qml/QtQuick\Controls\Fusion -Recurse -Force
Remove-Item -Path ../deploy/output/qml/QtQuick\Controls\Imagine -Recurse -Force
Remove-Item -Path ../deploy/output/qml/QtQuick\Controls\Material -Recurse -Force
Remove-Item -Path ../deploy/output/qml/QtQuick\Controls\Windows -Recurse -Force
Remove-Item -Path ../deploy/output/qml/QtQuick\Scene3D -Recurse -Force
Remove-Item -Path ../deploy/output/qml/QtQuick\NativeStyle -Recurse -Force
Remove-Item -Path ../deploy/output/qml/QtQuick\LocalStorage -Recurse -Force

# Copy license
New-Item -Path ../deploy/output -Name "licenses" -ItemType "directory"
Copy-Item -Path ../deploy/licenses/* -Destination ../deploy/output/licenses

# Copy Windows C runtime libraries
Copy-Item -Path ../windowsclibs/*.dll -Destination ../deploy/output

# Compress final build to zip file
Remove-Item -Path ../deploy/release.zip -ErrorAction Ignore
Compress-Archive -Path ../deploy/output/* -DestinationPath ../deploy/windows.amd64-0.0.0.zip -Force

# Copy icon file what need for building installer
Copy-Item -Path ../src/logo.ico -Destination ../deploy/output