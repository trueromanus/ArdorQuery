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

#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

flatpak {
    ENV_PREFIX=$$(PREFIX)
    isEmpty(ENV_PREFIX){
        PREFIX=$$[QT_INSTALL_PREFIX]
    }else{
        PREFIX=$$(PREFIX)
    }

    desktop.files += ../org.emptyflow.ArdorQuery.desktop
    desktop.path = $$PREFIX/share/applications
    INSTALLS += desktop

    metadata.path = $$PREFIX/share/metainfo
    metadata.files += ../org.emptyflow.ArdorQuery.appdata.xml
    INSTALLS += metadata

    target.path = $$PREFIX/bin
    INSTALLS += target

    icon_16.path = $$PREFIX/share/icons/hicolor/16x16/apps
    icon_16.files += ../linuxicons/16/ardorquery.png
    icon_16.CONFIG += no_check_exist

    icon_32.path = $$PREFIX/share/icons/hicolor/32x32/apps
    icon_32.files += ../linuxicons/32/ardorquery.png
    icon_32.CONFIG += no_check_exist

    icon_48.path = $$PREFIX/share/icons/hicolor/32x32/apps
    icon_48.files += ../linuxicons/48/ardorquery.png
    icon_48.CONFIG += no_check_exist

    icon_128.path = $$PREFIX/share/icons/hicolor/128x128/apps
    icon_128.files += ../linuxicons/128/ardorquery.png
    icon_128.CONFIG += no_check_exist

    icon_256.path = $$PREFIX/share/icons/hicolor/256x256/apps
    icon_256.files += ../linuxicons/256/ardorquery.png
    icon_256.CONFIG += no_check_exist

    icon_512.path = $$PREFIX/share/icons/hicolor/512x512/apps
    icon_512.files += ../linuxicons/512/ardorquery.png
    icon_512.CONFIG += no_check_exist

    INSTALLS += icon_16 icon_32 icon_48 icon_128 icon_256 icon_512
}

#unix: {

#Setup desktop entry file. https://specifications.freedesktop.org/desktop-entry-spec/latest

#Setup icons for following resolutions: 16x16; 32x32; 128x128; 256x256 and 512x512.
#https://specifications.freedesktop.org/icon-theme-spec/icon-theme-spec-latest.html

#16x16
#    icon_16.path = $$PREFIX/share/icons/hicolor/16x16/apps
#    icon_16.commands = mkdir -p icons/16 && cp ../anilibria.iconset/icon_16x16.png icons/16/anilibria.png
#    icon_16.files += icons/16/anilibria.png
#    icon_16.CONFIG += no_check_exist
#32x32
#    icon_32.path = $$PREFIX/share/icons/hicolor/32x32/apps
#    icon_32.commands = mkdir -p icons/32 && cp ../anilibria.iconset/icon_32x32.png icons/32/anilibria.png
#    icon_32.files += icons/32/anilibria.png
#    icon_32.CONFIG += no_check_exist
#48x48 #TODO: create png icon in 48x48 resolution.
#    icon_48.path = $$PREFIX/share/icons/hicolor/48x48/apps
#    icon_48.commands = mkdir -p icons/48 && cp icon48.png icons/48/anilibria.png
#    icon_48.files += icons/48/anilibria.png
#   icon_48.CONFIG += no_check_exist
#128x128
#    icon_128.path = $$PREFIX/share/icons/hicolor/128x128/apps
#    icon_128.commands = mkdir -p icons/128 && cp ../anilibria.iconset/icon_128x128.png icons/128/anilibria.png
#   icon_128.files += icons/128/anilibria.png
#    icon_128.CONFIG += no_check_exist
#256x256
#    icon_256.path = $$PREFIX/share/icons/hicolor/256x256/apps
#    icon_256.commands = mkdir -p icons/256 && cp ../anilibria.iconset/icon_256x256.png icons/256/anilibria.png
#    icon_256.files += icons/256/anilibria.png
#    icon_256.CONFIG += no_check_exist
#512x512
#    icon_512.path = $$PREFIX/share/icons/hicolor/512x512/apps
#    icon_512.commands = mkdir -p icons/512 && cp ../anilibria.iconset/icon_512x512.png icons/512/anilibria.png
#    icon_512.files += icons/512/anilibria.png
#   icon_512.CONFIG += no_check_exist

#    UNIX_ICONS += icon_16 icon_32 icon_48 icon_128 icon_256 icon_512
#}
