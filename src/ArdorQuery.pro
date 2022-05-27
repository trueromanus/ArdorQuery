QT += quick network websockets quickcontrols2

CONFIG += c++17

RC_ICONS = logo.ico

SOURCES += \
        ListModels/httprequestslistmodel.cpp \
        ListModels/maintabslistmodel.cpp \
        ListModels/notificationlistmodel.cpp \
        ListModels/responsebodylistmodel.cpp \
        ListModels/shortcutslistmodel.cpp \
        Models/httprequestitem.cpp \
        Models/httprequestmodel.cpp \
        Models/shortcutsection.cpp \
        ViewModels/backendviewmodel.cpp \
        ViewModels/httpperformerviewmodel.cpp \
        ViewModels/httprequestresultviewmodel.cpp \
        ViewModels/httprequestviewmodel.cpp \
        ViewModels/requestexternalviewmodel.cpp \
        ViewModels/textadvisorviewmodel.cpp \
        globalconstants.cpp \
        main.cpp

RESOURCES += qml.qrc

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ListModels/httprequestslistmodel.h \
    ListModels/maintabslistmodel.h \
    ListModels/notificationlistmodel.h \
    ListModels/responsebodylistmodel.h \
    ListModels/shortcutslistmodel.h \
    Models/httprequestitem.h \
    Models/httprequestmodel.h \
    Models/shortcutsection.h \
    ViewModels/backendviewmodel.h \
    ViewModels/httpperformerviewmodel.h \
    ViewModels/httprequestresultviewmodel.h \
    ViewModels/httprequestviewmodel.h \
    ViewModels/requestexternalviewmodel.h \
    ViewModels/textadvisorviewmodel.h \
    globalconstants.h \
    globalenums.h
