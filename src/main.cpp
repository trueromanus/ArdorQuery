/*
    ArdorQuery http tester
    Copyright (C) 2022 Roman Vladimirov
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QDir>
#include <QStandardPaths>
#include "ViewModels/httprequestviewmodel.h"
#include "ViewModels/textadvisorviewmodel.h"
#include "ViewModels/httpperformerviewmodel.h"
#include "ViewModels/backendviewmodel.h"
#include "ViewModels/requestexternalviewmodel.h"
#include "ListModels/shortcutslistmodel.h"
#include "QuickControls/backendimage.h"
#include "ListModels/openapiaddresseslistmodel.h"
#include "ViewModels/openapiexporterviewmodel.h"
#include "ListModels/addressespalettelistmodel.h"
#include "ListModels/globalvariableslistmodel.h"
#include "ListModels/responsebodylistmodel.h"
#include "ViewModels/globalmouseviewmodel.h"
#ifdef QT_DEBUG
#include <QtTest/QtTest>
#include "Tests/jsonformatterunittests.h"
#include "Tests/htmlformatterunittests.h"
#include "Tests/textadvisorviewmodelunittests.h"
#include "Tests/globalvariablesunittest.h"
#endif


void registerQmlTypes();
void adjustmentLocalStorage();
#ifdef QT_DEBUG
void runTest(int argc, char *argv[]);
#endif

int main(int argc, char *argv[])
{
    qputenv("QML_DISABLE_DISK_CACHE", "1");

    QGuiApplication app(argc, argv);

#ifdef QT_DEBUG
    if (argc > 0) {
        auto command = QString(argv[1]);

        if (command == "test") {
            runTest(argc, argv);
            return 0;
        }
    }
#endif

    QQuickStyle::setStyle("Universal");

    registerQmlTypes();

    QCoreApplication::setOrganizationDomain("emptyflowapps.com");
    QCoreApplication::setOrganizationName("EmptyFlow");
    QCoreApplication::setApplicationName("ArdorQuery");

    adjustmentLocalStorage();

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection
    );

    engine.load(url);

    return app.exec();
}

void registerQmlTypes() {
    qmlRegisterType<HttpRequestViewModel>("ArdorQuery.Backend", 1, 0, "HttpRequestViewModel");
    qmlRegisterType<TextAdvisorViewModel>("ArdorQuery.Backend", 1, 0, "TextAdvisorViewModel");
    qmlRegisterType<HttpPerformerViewModel>("ArdorQuery.Backend", 1, 0, "HttpPerformerViewModel");
    qmlRegisterType<BackendViewModel>("ArdorQuery.Backend", 1, 0, "BackendViewModel");
    qmlRegisterType<RequestExternalViewModel>("ArdorQuery.Backend", 1, 0, "RequestExternalViewModel");
    qmlRegisterType<ShortcutsListModel>("ArdorQuery.Backend", 1, 0, "ShortcutsListModel");
    qmlRegisterType<BackendImage>("ArdorQuery.Backend", 1, 0, "BackendImage");
    qmlRegisterType<OpenApiExporterViewModel>("ArdorQuery.Backend", 1, 0, "OpenApiExporterViewModel");
    qmlRegisterType<OpenApiAddressesListModel>("ArdorQuery.Backend", 1, 0, "OpenApiAddressesListModel");
    qmlRegisterType<OpenApiRoutesListModel>("ArdorQuery.Backend", 1, 0, "OpenApiRoutesListModel");
    qmlRegisterType<GlobalVariablesListModel>("ArdorQuery.Backend", 1, 0, "GlobalVariablesListModel");
    qmlRegisterType<ResponseBodyListModel>("ArdorQuery.Backend", 1, 0, "ResponseBodyListModel");
    qmlRegisterType<GlobalMouseViewModel>("ArdorQuery.Backend", 1, 0, "GlobalMouseViewModel");
}

void adjustmentLocalStorage() {
    if (IsPortable) {
        auto cachePath = QDir::currentPath();
        qDebug() << "Portable Cache location: " << cachePath;
    } else {
        QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
        qDebug() << "Cache location: " << QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    }
}

#ifdef QT_DEBUG
void runTest(int argc, char *argv[]) {
    QTest::qExec(new JsonFormatterUnitTests, argc - 1, argv);
    QTest::qExec(new HtmlFormatterUnitTests, argc - 1, argv);
    QTest::qExec(new TextAdvisorViewModelUnitTests, argc - 1, argv);
    QTest::qExec(new GlobalVariablesUnitTest, argc - 1, argv);
}
#endif
