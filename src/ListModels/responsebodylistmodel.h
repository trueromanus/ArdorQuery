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

#ifndef RESPONSEBODYLISTMODEL_H
#define RESPONSEBODYLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QImage>
#include <QMap>
#include <QList>
#include <QRegularExpression>
#include <QFont>
#include <QFontMetrics>
#include "../Formatters/formatterfactory.h"
#include "../globalconstants.h"

class ResponseBodyListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QImage bodyImage READ bodyImage NOTIFY bodyImageChanged)
    Q_PROPERTY(int bodyImageWidth READ bodyImageWidth NOTIFY bodyImageWidthChanged)
    Q_PROPERTY(int bodyImageHeight READ bodyImageHeight NOTIFY bodyImageHeightChanged)
    Q_PROPERTY(int countFindedLines READ countFindedLines NOTIFY countFindedLinesChanged)
    Q_PROPERTY(QString countFindedLinesText READ countFindedLinesText NOTIFY countFindedLinesTextChanged)
    Q_PROPERTY(bool lineStarted READ lineStarted NOTIFY lineStartedChanged FINAL)
    Q_PROPERTY(QFontMetrics fontMetrics READ fontMetrics WRITE setFontMetrics NOTIFY fontMetricsChanged FINAL)

private:
    QStringList m_lines { QStringList() };
    QMap<int, FormatterLine *> m_silentLines { QMap<int, FormatterLine *>() };
    int m_silentLinesTab { 0 };
    QByteArray m_originalBody { "" };
    QList<std::tuple<int, int>> m_findedLines { QList<std::tuple<int, int>>() };
    QMap<int, int> m_findedLinesMap { QMap<int, int>() };
    QScopedPointer<FormatterFactory> m_formatterFactory { new FormatterFactory() };
    QImage m_imageSource { QImage() };
    int m_currentFindedIndex { -1 };
    int m_currentFindedLine { -1 };
    QRegularExpression m_fontTagStartRegExp { R"a(<font color=\"\#[A-Za-z0-9]{1,6}\">)a" };
    bool m_notFounded { false };
    QString m_previousFilter { "" };
    int m_typingTimerId { -1 };
    QString m_typingFilter { "" };
    int m_startSelectedLine { -1 };
    int m_endSelectedLine { -1 };
    int m_startSelectedCharacter { -1 };
    int m_endSelectedCharacter { -1 };
    int m_fontHeight { 0 };
    int m_defaultCharacterWidth { 0 };
    int m_shortCharacterWidth { 0 };
    const QString m_shortCharacters { "iIjl" };
    QFontMetrics m_fontMetrics { QFontMetrics(QFont()) };

    enum ResponseBodyRoles {
        CurrentLineRole = Qt::UserRole + 1,
        IndexRole,
        IsFindIndexRole,
        IsSelectedLineRole,
    };

public:
    explicit ResponseBodyListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setBody(const QByteArray& body, const QString& formatter) noexcept;
    void reformatting(const QString& formatter) noexcept;
    void reformatBody(const QString& formatter) noexcept;
    QString getFullBody() const noexcept;
    QByteArray getFullBodyArray() const noexcept;
    bool isHasBody() const noexcept;
    QImage bodyImage() const noexcept { return m_imageSource; }
    int bodyImageWidth() const noexcept { return m_imageSource.width(); }
    int bodyImageHeight() const noexcept { return m_imageSource.height(); }
    int countFindedLines() const noexcept { return m_findedLines.count(); }
    QString countFindedLinesText() const noexcept;
    bool lineStarted() const noexcept { return m_startSelectedLine > -1; }
    int nextFindedResult() noexcept;
    int previousFindedResult() noexcept;
    int getCurrentFindedLine() noexcept;
    void clear() noexcept;

    QFontMetrics fontMetrics() const noexcept { return m_fontMetrics; }
    void setFontMetrics(const QFontMetrics& fontMetrics) noexcept;

    Q_INVOKABLE void searchText(const QString& filter) noexcept;
    Q_INVOKABLE void startSearchText(const QString& filter) noexcept;
    Q_INVOKABLE void selectStartLine(int currentIndex) noexcept;
    Q_INVOKABLE void selectLine(int currentIndex, int width, int height, int x, int y, bool formatting) noexcept;

private:
    QString& cleanLineFromTags(QString& line) noexcept;
    void clearCurrentSelection() noexcept;

protected:
    void timerEvent(QTimerEvent *event) override;

signals:
    void visibleBodyChanged();
    void bodyImageChanged();
    void bodyImageWidthChanged();
    void bodyImageHeightChanged();
    void countFindedLinesChanged();
    void countFindedLinesTextChanged();
    void lineStartedChanged();
    void fontMetricsChanged();

};

#endif // RESPONSEBODYLISTMODEL_H
