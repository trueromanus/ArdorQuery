#ifndef REQUESTEXTERNALVIEWMODEL_H
#define REQUESTEXTERNALVIEWMODEL_H

#include <QObject>
#include "../globalconstants.h"
#include "../ViewModels/httprequestviewmodel.h"

class RequestExternalViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(HttpRequestViewModel* httpRequest READ httpRequest WRITE setHttpRequest NOTIFY httpRequestChanged)

private:
    HttpRequestViewModel* m_httpRequest { nullptr };

public:
    explicit RequestExternalViewModel(QObject *parent = nullptr);

    QString parseFromString(const QString& input) noexcept;

    HttpRequestViewModel* httpRequest() const noexcept { return m_httpRequest; }
    void setHttpRequest(const HttpRequestViewModel* httpRequest) noexcept;

    Q_INVOKABLE void appendFromClipboard();
    Q_INVOKABLE void replaceFromClipboard();

private:
    QString getTextFromClipboard() const noexcept;

signals:
    void httpRequestChanged();

};

#endif // REQUESTEXTERNALVIEWMODEL_H
