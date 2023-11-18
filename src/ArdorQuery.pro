QT += quick network websockets quickcontrols2 concurrent svg

CONFIG(debug, debug|release) {
    QT += testlib
}

CONFIG += c++17

RC_ICONS = logo.ico

SOURCES += \
        Formatters/formatterfactory.cpp \
        Formatters/htmlformatter.cpp \
        Formatters/jsonformatter.cpp \
        Formatters/outputformatter.cpp \
        Formatters/plaintextformatter.cpp \
        Formatters/xmlformatter.cpp \
        ListModels/addressespalettelistmodel.cpp \
        ListModels/globalvariableslistmodel.cpp \
        ListModels/httprequestslistmodel.cpp \
        ListModels/maintabslistmodel.cpp \
        ListModels/notificationlistmodel.cpp \
        ListModels/openapiaddresseslistmodel.cpp \
        ListModels/openapirouteslistmodel.cpp \
        ListModels/outputformatslistmodel.cpp \
        ListModels/requestscommandpalettelistmodel.cpp \
        ListModels/responsebodylistmodel.cpp \
        ListModels/shortcutslistmodel.cpp \
        Models/httprequestitem.cpp \
        Models/httprequestmodel.cpp \
        Models/openapiaddressmodel.cpp \
        Models/openapiparametermodel.cpp \
        Models/openapiresponsemodel.cpp \
        Models/openapiroutemodel.cpp \
        Models/openapiroutesauthorizationmodel.cpp \
        Models/openapiroutesoptions.cpp \
        Models/postscriptresponsemodel.cpp \
        Models/postscriptresultmodel.cpp \
        Models/shortcutsection.cpp \
        QuickControls/backendimage.cpp \
        ViewModels/backendviewmodel.cpp \
        ViewModels/globaleventhandlermodel.cpp \
        ViewModels/httpperformerviewmodel.cpp \
        ViewModels/httprequestresultviewmodel.cpp \
        ViewModels/httprequestviewmodel.cpp \
        ViewModels/openapiexporterviewmodel.cpp \
        ViewModels/requestexternalviewmodel.cpp \
        ViewModels/textadvisorviewmodel.cpp \
        globalconstants.cpp \
        globalhelpers.cpp \
        main.cpp
# Adding tests for debug
CONFIG(debug, debug|release) {
SOURCES += \
        Tests/jsonformatterunittests.cpp \
        Tests/textadvisorviewmodelunittests.cpp \
        Tests/globalvariablesunittest.cpp \
        Tests/htmlformatterunittests.cpp
}

RESOURCES += qml.qrc

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Formatters/formatterfactory.h \
    Formatters/htmlformatter.h \
    Formatters/jsonformatter.h \
    Formatters/outputformatter.h \
    Formatters/plaintextformatter.h \
    Formatters/xmlformatter.h \
    ListModels/addressespalettelistmodel.h \
    ListModels/globalvariableslistmodel.h \
    ListModels/httprequestslistmodel.h \
    ListModels/maintabslistmodel.h \
    ListModels/notificationlistmodel.h \
    ListModels/openapiaddresseslistmodel.h \
    ListModels/openapirouteslistmodel.h \
    ListModels/outputformatslistmodel.h \
    ListModels/requestscommandpalettelistmodel.h \
    ListModels/responsebodylistmodel.h \
    ListModels/shortcutslistmodel.h \
    Models/httprequestitem.h \
    Models/httprequestmodel.h \
    Models/openapiaddressmodel.h \
    Models/openapiparametermodel.h \
    Models/openapiresponsemodel.h \
    Models/openapiroutemodel.h \
    Models/openapiroutesauthorizationmodel.h \
    Models/openapiroutesoptions.h \
    Models/postscriptresponsemodel.h \
    Models/postscriptresultmodel.h \
    Models/shortcutsection.h \
    QuickControls/backendimage.h \
    ViewModels/backendviewmodel.h \
    ViewModels/globaleventhandlermodel.h \
    ViewModels/httpperformerviewmodel.h \
    ViewModels/httprequestresultviewmodel.h \
    ViewModels/httprequestviewmodel.h \
    ViewModels/openapiexporterviewmodel.h \
    ViewModels/requestexternalviewmodel.h \
    ViewModels/textadvisorviewmodel.h \
    globalconstants.h \
    globalenums.h \
    globalhelpers.h

# Adding tests for debug
CONFIG(debug, debug|release) {
HEADERS += \
    Tests/jsonformatterunittests.h \
    Tests/textadvisorviewmodelunittests.h \
    Tests/globalvariablesunittest.h \
    Tests/htmlformatterunittests.h
}
