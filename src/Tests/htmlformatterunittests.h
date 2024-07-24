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

#ifndef HTMLFORMATTERUNITTESTS_H
#define HTMLFORMATTERUNITTESTS_H

#include <QObject>
#include <QtTest/QtTest>

class HtmlFormatterUnitTests : public QObject
{
    Q_OBJECT
public:
    explicit HtmlFormatterUnitTests(QObject *parent = nullptr);

private slots:
    void htmlLowerHeader();
    void htmlLowerHeaderSilent();
    void htmlUpperHeader();
    void htmlUpperHeaderSilent();
    void emptyFullTag();
    void emptyFullTagSilent();
    void attributeWithUrl();
    void attributeWithUrlSilent();
    void commentTag();
    void commentTagSilent();
};

#endif // HTMLFORMATTERUNITTESTS_H
