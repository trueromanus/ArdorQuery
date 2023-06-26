#ifndef POSTSCRIPTRESPONSEMODEL_H
#define POSTSCRIPTRESPONSEMODEL_H

#include <QObject>
#include <QString>

class PostScriptResponseModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList headers READ headers WRITE setHeaders NOTIFY headersChanged)
    Q_PROPERTY(int statusCode READ statusCode WRITE setStatusCode NOTIFY statusCodeChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage WRITE setErrorMessage NOTIFY errorMessageChanged)
    Q_PROPERTY(int bodySize READ bodySize WRITE setBodySize NOTIFY bodySizeChanged)
    Q_PROPERTY(QString route READ route WRITE setRoute NOTIFY routeChanged)

private:
    QStringList m_headers { QStringList() };
    int m_statusCode { 0 };
    QString m_errorMessage { "" };
    int m_bodySize { 0 };
    QString m_route { "" };

public:
    explicit PostScriptResponseModel(QObject *parent = nullptr);

    QStringList headers() const noexcept { return m_headers; }
    int statusCode() const noexcept { return m_statusCode; }
    QString errorMessage() const noexcept { return m_errorMessage; }
    int bodySize() const noexcept { return m_bodySize; }
    QString route() const noexcept { return m_route; }

    void setHeaders(QStringList headers) noexcept;
    void setStatusCode(int statusCode) noexcept;
    void setErrorMessage(const QString& errorMessage) noexcept;
    void setBodySize(int bodySize) noexcept;
    void setRoute(const QString& route) noexcept;

signals:
    void headersChanged();
    void statusCodeChanged();
    void errorMessageChanged();
    void bodySizeChanged();
    void routeChanged();

};

#endif // POSTSCRIPTRESPONSEMODEL_H
