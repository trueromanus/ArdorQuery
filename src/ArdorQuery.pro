QT += quick network websockets quickcontrols2

CONFIG += c++17

RC_ICONS = logo.ico

SOURCES += \
        ListModels/httprequestslistmodel.cpp \
        ListModels/maintabslistmodel.cpp \
        ListModels/responsebodylistmodel.cpp \
        Models/httprequestitem.cpp \
        Models/httprequestmodel.cpp \
        ViewModels/backendviewmodel.cpp \
        ViewModels/httpperformerviewmodel.cpp \
        ViewModels/httprequestresultviewmodel.cpp \
        ViewModels/httprequestviewmodel.cpp \
        ViewModels/textadvisorviewmodel.cpp \
        main.cpp

RESOURCES += qml.qrc

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ListModels/httprequestslistmodel.h \
    ListModels/maintabslistmodel.h \
    ListModels/responsebodylistmodel.h \
    Models/httprequestitem.h \
    Models/httprequestmodel.h \
    ViewModels/backendviewmodel.h \
    ViewModels/httpperformerviewmodel.h \
    ViewModels/httprequestresultviewmodel.h \
    ViewModels/httprequestviewmodel.h \
    ViewModels/textadvisorviewmodel.h
