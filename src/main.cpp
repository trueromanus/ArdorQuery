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
#include "ViewModels/httprequestviewmodel.h"
#include "ViewModels/textadvisorviewmodel.h"
#include "ViewModels/httpperformerviewmodel.h"
#include "ViewModels/backendviewmodel.h"
#include "ViewModels/requestexternalviewmodel.h"
#include "ListModels/shortcutslistmodel.h"

void registerQmlTypes();

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Universal");

    registerQmlTypes();

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
}
