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

#ifndef HTTPREQUESTVIEWMODEL_H
#define HTTPREQUESTVIEWMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include <QSharedPointer>
#include <QScopedPointer>
#include "../Models/httprequestitem.h"
#include "textadvisorviewmodel.h"

class HttpRequestViewModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int selectedItem READ selectedItem WRITE setSelectedItem NOTIFY selectedItemChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)

public:
    enum class HttpRequestTypes {
        UnknownType = 0,
        UrlType,
        MethodType,
        HeaderType,
        BodyType,
        FormItemType,
        FormFileType,
        HttpProtocolType,
        BearerType,
        TitleType,
        ParamType,
        PastryType,
        RouteType,
        OptionsType,
        PostScriptType
    };

private:
    QScopedPointer<QList<HttpRequestItem*>> m_items { new QList<HttpRequestItem*>() };
    QScopedPointer<QMap<int, int>> m_sortWeight { new QMap<int, int>() };
    int m_selectedItem { -1 };
    QSharedPointer<TextAdvisorViewModel> m_textAdvisor { nullptr };
    const QString m_unnamed { "Unnamed" };
    bool m_visibleAddGlobalVariablePopup { false };

    enum HttpRequestRoles {
        TypeRole = Qt::UserRole + 1,
        TypeColor,
        TextRole,
        IsActiveRole,
        IndexRole,
        IsFocusedRole,
        LastCursorPositionRole
    };

public:
    explicit HttpRequestViewModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    int selectedItem() const noexcept { return m_selectedItem; }
    void setSelectedItem(const int selectedItem) noexcept;

    QString title() const noexcept { return getTitle(); }

    void setTextAdvisor(const QSharedPointer<TextAdvisorViewModel> textAdviser) noexcept;

    Q_INVOKABLE void addItem(const int position, const HttpRequestViewModel::HttpRequestTypes itemType = HttpRequestTypes::UnknownType, const QString initialValue = "", const QString& alias = "");
    Q_INVOKABLE void removeFirstItem();
    Q_INVOKABLE void refreshItem(const int position, const QString& content);
    Q_INVOKABLE void setItemContent(const int position, const QString& content);
    Q_INVOKABLE void setItemCursor(int position, int cursor);
    Q_INVOKABLE void selectUpField();
    Q_INVOKABLE void selectDownField();
    Q_INVOKABLE void selectFirstField();
    Q_INVOKABLE void selectLastField();
    Q_INVOKABLE void clearFields();
    Q_INVOKABLE void clearSelectedField();

    QString getMethod() const noexcept;
    QString getProtocol() const noexcept;
    QString getUrl() const noexcept;
    QString addGetParameters(const QString& url) const noexcept;
    QString getBody() const noexcept;
    QStringList getFormParameters() const noexcept;
    QStringList getFileParameters() const noexcept;
    QStringList getHeaders() const noexcept;
    QStringList getOptions() const noexcept;
    QString getTitle() const noexcept;
    QString getPostScript() const noexcept;
    bool isOnlyEmptyFirstItem() const noexcept;
    int countItems() const noexcept;
    void sortingFields(const bool descending) noexcept;
    QString getAllFields() const noexcept;
    QStringList getAllFieldsAsList() const noexcept;
    int insertGlobalVariableToCursor(const QString& variable) noexcept;

private:
    QString getTypeColor(int type) const;
    QString getTextFromClipboard() const noexcept;
    QString getItemPrefix(const HttpRequestViewModel::HttpRequestTypes itemType, const QString& initialValue) const noexcept;

signals:
    void selectedItemChanged();
    void textAdvisorChanged();
    void titleChanged();

};

#endif // HTTPREQUESTVIEWMODEL_H
