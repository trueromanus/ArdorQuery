QT += quick network websockets quickcontrols2

CONFIG(debug, debug|release) {
    QT += testlib
}

CONFIG += c++17

RC_ICONS = logo.ico

SOURCES += \
        Formatters/jsonformatter.cpp \
        Formatters/outputformatter.cpp \
        ListModels/httprequestslistmodel.cpp \
        ListModels/maintabslistmodel.cpp \
        ListModels/notificationlistmodel.cpp \
        ListModels/outputformatslistmodel.cpp \
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
# Adding tests for debug
CONFIG(debug, debug|release) {
SOURCES += Tests/jsonformatterunittests.cpp
}

RESOURCES += qml.qrc

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Formatters/jsonformatter.h \
    Formatters/outputformatter.h \
    ListModels/httprequestslistmodel.h \
    ListModels/maintabslistmodel.h \
    ListModels/notificationlistmodel.h \
    ListModels/outputformatslistmodel.h \
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

# Adding tests for debug
CONFIG(debug, debug|release) {
HEADERS += Tests/jsonformatterunittests.h
}
