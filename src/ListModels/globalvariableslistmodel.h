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

#ifndef GLOBALVARIABLESLISTMODEL_H
#define GLOBALVARIABLESLISTMODEL_H

#include <QObject>
#include <QMap>
#include <QAbstractListModel>

class GlobalVariablesListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int selected READ selected WRITE setSelected NOTIFY selectedChanged)

private:
    enum GlobaVariablesRoles {
        LineRole = Qt::UserRole + 1,
        IdentifierRole,
        IsActiveRole,
        IsOddRole,
    };

    QString m_savedGlobalVariablesFile { "globalvariables" };
    QMap<QString, QString> m_variables { QMap<QString, QString>() };
    QStringList m_lines { QStringList() };
    int m_selected { 0 };

public:
    explicit GlobalVariablesListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString replaceGlobalVariables(const QString& value);

    int selected() const noexcept { return m_selected; }
    void setSelected(int selected) noexcept;

    void addLine();
    void addVariable(const QString& name, const QString& value);

    Q_INVOKABLE void shortcutHandler(const QString& shortcut) noexcept;
    Q_INVOKABLE void fillLines();
    Q_INVOKABLE void clearLines();
    Q_INVOKABLE void setLine(int identifier, const QString& value);
    Q_INVOKABLE void removeLine(int index);
    Q_INVOKABLE void parseLines();

private:
    void readCache();
    void writeCache();

signals:
    void selectedChanged();
    void closeWindowRequired();

};

#endif // GLOBALVARIABLESLISTMODEL_H
