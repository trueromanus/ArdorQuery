QT += quick network websockets quickcontrols2

CONFIG += c++17

SOURCES += \
        ListModels/maintabslistmodel.cpp \
        Models/httprequestitem.cpp \
        ViewModels/backendviewmodel.cpp \
        ViewModels/httpperformerviewmodel.cpp \
        ViewModels/httprequestviewmodel.cpp \
        ViewModels/textadvisorviewmodel.cpp \
        main.cpp

resources.files = main.qml 
resources.prefix = /$${TARGET}
RESOURCES += resources \
    qml.qrc

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ListModels/maintabslistmodel.h \
    Models/httprequestitem.h \
    ViewModels/backendviewmodel.h \
    ViewModels/httpperformerviewmodel.h \
    ViewModels/httprequestviewmodel.h \
    ViewModels/textadvisorviewmodel.h

DISTFILES += \
    HttpRequestEditor.qml
