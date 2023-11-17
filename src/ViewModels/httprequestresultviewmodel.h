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

#ifndef HTTPREQUESTRESULTVIEWMODEL_H
#define HTTPREQUESTRESULTVIEWMODEL_H

#include <QObject>
#include <QDateTime>
#include <QTime>
#include <QElapsedTimer>
#include "../ListModels/responsebodylistmodel.h"
#include "../globalconstants.h"

class HttpRequestResultViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int statusCode READ statusCode WRITE setStatusCode NOTIFY statusCodeChanged)
    Q_PROPERTY(QString displayStatusCode READ displayStatusCode NOTIFY displayStatusCodeChanged)
    Q_PROPERTY(QStringList headers READ headers WRITE setHeaders NOTIFY headersChanged)
    Q_PROPERTY(QString responseSize READ responseSize NOTIFY responseSizeChanged)
    Q_PROPERTY(QString responseReadableSize READ responseReadableSize WRITE setResponseReadableSize NOTIFY responseReadableSizeChanged)
    Q_PROPERTY(QString responseTime READ responseTime NOTIFY responseTimeChanged)
    Q_PROPERTY(QString networkError READ networkError WRITE setNetworkError NOTIFY networkErrorChanged)
    Q_PROPERTY(ResponseBodyListModel* bodyModel READ bodyModel NOTIFY bodyModelChanged)
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)
    Q_PROPERTY(QString displayStatus READ displayStatus NOTIFY displayStatusChanged)
    Q_PROPERTY(QString outputFormat READ outputFormat WRITE setOutputFormat NOTIFY outputFormatChanged)
    Q_PROPERTY(QString actualFormat READ actualFormat NOTIFY actualFormatChanged)
    Q_PROPERTY(bool isFormatting READ isFormatting NOTIFY isFormattingChanged)
    Q_PROPERTY(bool showImage READ showImage NOTIFY showImageChanged)
    Q_PROPERTY(bool hasError READ hasError NOTIFY hasErrorChanged)
    Q_PROPERTY(bool showDownloadFile READ showDownloadFile NOTIFY showDownloadFileChanged)

private:
    int m_statusCode { 0 };
    QStringList m_headers { QStringList() };
    QElapsedTimer* m_elapsedTimer { nullptr };
    bool m_hasResultTime { false };
    uint64_t m_elapsedTime { 0 };
    uint64_t m_responseSize { 0 };
    QString m_networkError;
    QString m_responseReadableSize { "" };
    bool m_isRunning { false };
    QList<QString> m_sizes { QList<QString>() };
    QScopedPointer<ResponseBodyListModel> m_bodyModel { new ResponseBodyListModel() };
    QString m_outputFormat { OutputFormatAuto };
    bool m_isFormatting { false };
    bool m_showImage { false };
    QString m_actualFormat { "" };
    bool m_customErrorResult { false };
    QString m_postScript { "" };
    bool m_showDownloadFile { false };
    QString m_defaultDownloadFile { "" };
    const QString StartHeaderTag { "<font color='#8812a1'>" };
    const QString EndHeaderTag { "</font>" };

public:
    explicit HttpRequestResultViewModel(QObject *parent = nullptr);

    int statusCode() const noexcept { return m_statusCode; }
    void setStatusCode(const int statusCode) noexcept;

    QStringList headers() const noexcept { return m_headers; }
    void setHeaders(const QStringList& headers) noexcept;

    void setBody(const QByteArray& body) noexcept;
    void reformatting() noexcept;

    QString responseSize() const noexcept { return m_responseSize > 0 ? "(" + QString::number(m_responseSize) + " total bytes)" : " - "; }

    QString responseTime() const noexcept;

    QString displayStatusCode() const noexcept { return m_statusCode > 0 ? QString::number(m_statusCode) : (m_networkError.isEmpty() ? " - " : ""); }

    ResponseBodyListModel* bodyModel() const noexcept { return m_bodyModel.get(); }

    QString responseReadableSize() const noexcept { return m_responseReadableSize; }
    void setResponseReadableSize(const QString& responseReadableSize) noexcept;

    QString networkError() const noexcept { return m_networkError; }
    void setNetworkError(const QString& networkError) noexcept;

    void reset() noexcept;
    void trackRequestTime() noexcept;
    void untrackRequestTime() noexcept;

    bool isRunning() const noexcept { return m_isRunning; }

    QString displayStatus() const noexcept;

    QString outputFormat() const noexcept { return m_outputFormat; }
    void setOutputFormat(const QString& outputFormat) noexcept;

    bool isFormatting() const noexcept { return m_isFormatting; }
    bool showImage() const noexcept { return m_showImage; }

    QString actualFormat() const noexcept { return m_actualFormat; }

    void generateImage(const QStringList& fields, const QString& path, bool saveToClipboard) noexcept;

    bool hasError() const noexcept { return m_customErrorResult || !m_networkError.isEmpty(); }

    bool showDownloadFile() const noexcept { return m_showDownloadFile; }

    void setCustomErrorResult(bool hasErrors, const QString& errorMessage);

    uint64_t originResponseSize() const noexcept { return m_responseSize; }

    void setPostScript(const QString& script);
    void clearPostScript();
    QString postScript() const noexcept { return m_postScript; }

    Q_INVOKABLE void copyHeadersToClipboard();
    Q_INVOKABLE void copyBodyToClipboard();
    Q_INVOKABLE void saveBodyToFile(const QString& fileName);
    Q_INVOKABLE void reformatBody();

private:
    QString getReadableSize(uint64_t size) const noexcept;
    QString getFormatFromContentType() noexcept;
    void setBoldTextToPainter(QPainter& painter) noexcept;
    void setNormalTextToPainter(QPainter& painter) noexcept;
    void paintText(QPainter& painter, const QImage& image, int& currentLine, int& lineHeight, const QString& text, bool bold) noexcept;
    QStringList getHeaderLines();

signals:
    void statusCodeChanged();
    void headersChanged();
    void responseSizeChanged();
    void responseTimeChanged();
    void networkErrorChanged();
    void displayStatusCodeChanged();
    void bodyModelChanged();
    void responseReadableSizeChanged();
    void isRunningChanged();
    void displayStatusChanged();
    void outputFormatChanged();
    void isFormattingChanged();
    void showImageChanged();
    void actualFormatChanged();
    void errorSavingGeneratedFile();
    void hasErrorChanged();
    void showDownloadFileChanged();

};

#endif // HTTPREQUESTRESULTVIEWMODEL_H
