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

#ifndef TEXTADVISORVIEWMODEL_H
#define TEXTADVISORVIEWMODEL_H

#include <QObject>
#include <QMap>

class TextAdvisorViewModel : public QObject
{
    Q_OBJECT
private:
    QScopedPointer<QMap<QString, QString>> m_singleCompletings { new QMap<QString, QString>() };
    QScopedPointer<QMap<QString, QStringList>> m_multipleCompletings { new QMap<QString, QStringList>() };

public:
    explicit TextAdvisorViewModel(QObject *parent = nullptr);

    Q_INVOKABLE void makeSuggestions(const QString& text);

private:
    void fillCompletings();

signals:

};

#endif // TEXTADVISORVIEWMODEL_H
