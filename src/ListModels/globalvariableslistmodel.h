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
    Q_PROPERTY(QList<QVariantMap> shortcuts READ shortcuts NOTIFY shortcutsChanged)
    Q_PROPERTY(bool helpVisible READ helpVisible WRITE setHelpVisible NOTIFY helpVisibleChanged)
    Q_PROPERTY(QStringList tabs READ tabs NOTIFY tabsChanged)
    Q_PROPERTY(QString selectedTab READ selectedTab WRITE setSelectedTab NOTIFY selectedTabChanged)
    Q_PROPERTY(bool hasLines READ hasLines NOTIFY hasLinesChanged FINAL)
    Q_PROPERTY(bool needShowSaveNotifications READ needShowSaveNotifications WRITE setNeedShowSaveNotifications NOTIFY needShowSaveNotificationsChanged FINAL)
    Q_PROPERTY(QStringList variableNames READ variableNames NOTIFY variableNamesChanged FINAL)

private:
    enum GlobaVariablesRoles {
        LineRole = Qt::UserRole + 1,
        IdentifierRole,
        IsActiveRole,
        IsOddRole,
    };

    QString m_savedGlobalVariablesFile { "globalvariables" };
    QMap<QString, QString> m_variables { QMap<QString, QString>() };
    QList<QString> m_variablesOrders { QList<QString>() };
    QStringList m_lines { QStringList() };
    int m_selected { 0 };
    QList<QVariantMap> m_shortcuts { QList<QVariantMap>() };
    QMap<QString, QString> m_shortcutCommandMapping { QMap<QString, QString>() };
    QMap<QString, QString> m_shortcutCommands { QMap<QString, QString>() };
    bool m_helpVisible { false };
    QStringList m_tabs { QStringList() };
    QString m_globalVariablesPopupMode { "default" };
    QString m_selectedTab { "" };
    bool m_needShowSaveNotifications { true };
    const QString VariablesTab { "Variables" };
    const QString m_helpCommand { "globalvariableshelp" };
    const QString m_closeWindowCommand { "globalvariablesclosewindow" };
    const QString m_saveGlobalVariablesCommand { "globalvariablessave" };
    const QString m_addNewLineBelowCommand { "globalvariablesaddlinebelow" };
    const QString m_selectLastFieldCommand { "globalvariablesselectlastfield" };
    const QString m_selectNextFieldCommand { "globalvariablesselectnextfield" };
    const QString m_selectTopFieldCommand { "globalvariablesselecttopfield" };
    const QString m_selectPreviousFieldCommand { "globalvariablesselectpreviousfield" };

    const QString m_addLineAboveCommand { "globalvariablesaddlineabovecommand" };
    const QString m_addLineToEndCommand { "globalvariablesaddlinetoendcommand" };
    const QString m_removeSelectedFieldCommand { "globalvariablesremoveselectedfieldcommand" };
    const QString m_removeAllFieldCommand { "globalvariablesremoveallfieldcommand" };

    typedef QString (GlobalVariablesListModel::*globalVariableDelegate)(const QString& content);

    const QString m_dateTimeNowGlobalVariable { "dateTimeNow" };
    const QString m_dateTimeUtcNowGlobalVariable { "dateTimeUtcNow" };
    const QString m_time24HoursNowGlobalVariable { "time24HoursNow" };
    QMap<QString, GlobalVariablesListModel::globalVariableDelegate> m_globalVariableHandlers { QMap<QString, GlobalVariablesListModel::globalVariableDelegate>() };
public:
    explicit GlobalVariablesListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString replaceGlobalVariables(const QString& value, QMap<QString, QString>& remappedValues);

    int selected() const noexcept { return m_selected; }
    void setSelected(int selected) noexcept;

    QList<QVariantMap> shortcuts() const noexcept { return m_shortcuts; }

    bool helpVisible() const noexcept { return m_helpVisible; }
    void setHelpVisible(bool helpVisible) noexcept;

    void setFilterMode(const QString& mode) noexcept;
    void addLine();
    void addLineBefore();
    void addLineToEnd();
    void removeSelectedLine();
    void removeAllLines();
    void addVariable(const QString& name, const QString& value);

    QStringList tabs() const noexcept { return m_tabs; }

    bool hasLines() const noexcept { return !m_lines.isEmpty(); }

    bool needShowSaveNotifications() const noexcept { return m_needShowSaveNotifications; }
    void setNeedShowSaveNotifications(bool needShowSaveNotifications) noexcept;

    QString selectedTab() const noexcept { return m_selectedTab; }
    void setSelectedTab(const QString& selectedTab) noexcept;

    QStringList variableNames() const noexcept;
    void refreshVariableNames() noexcept;

    bool hasVariableNames() const noexcept { return !m_variables.isEmpty(); }
    bool hasVariableName(const QString& name) const noexcept { return m_variables.contains(name); }
    QString getVariable(const QString& name) const noexcept { return m_variables.value(name); }

    Q_INVOKABLE bool shortcutHandler(const QString& shortcut) noexcept;
    Q_INVOKABLE void fillLines();
    Q_INVOKABLE void clearLines();
    Q_INVOKABLE void setLine(int identifier, const QString& value);
    Q_INVOKABLE void removeLine(int index);
    Q_INVOKABLE void parseLines();

private:
    void readCache();
    void writeCache();
    void fillMappings();
    void fillCommands();
    void fillHelpShortcuts();
    QString dateTimeNowVariable(const QString& content);
    QString dateTimeUtcNowVariable(const QString& content);
    QString timeNowVariable(const QString& content);

signals:
    void selectedChanged();
    void closeWindowRequired();
    void shortcutsChanged();
    void helpVisibleChanged();
    void selectedTabChanged();
    void tabsChanged();
    void hasLinesChanged();
    void needShowSaveNotificationsChanged();
    void variableNamesChanged();

};

#endif // GLOBALVARIABLESLISTMODEL_H
