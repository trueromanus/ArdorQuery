QT += quick network websockets quickcontrols2

CONFIG += c++17

SOURCES += \
        Models/httprequestitem.cpp \
        ViewModels/httprequestviewmodel.cpp \
        main.cpp

resources.files = main.qml 
resources.prefix = /$${TARGET}
RESOURCES += resources \
    qml.qrc

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Models/httprequestitem.h \
    ViewModels/httprequestviewmodel.h

DISTFILES += \
    HttpRequestEditor.qml
