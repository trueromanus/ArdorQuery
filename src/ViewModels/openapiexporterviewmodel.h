#ifndef OPENAPIEXPORTERVIEWMODEL_H
#define OPENAPIEXPORTERVIEWMODEL_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "../ListModels/openapiaddresseslistmodel.h"

class OpenApiExporterViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(OpenApiAddressesListModel* addresses READ addresses NOTIFY addressesChanged)
    Q_PROPERTY(QString url READ url NOTIFY urlChanged)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)

private:
    OpenApiAddressesListModel* m_addresses { new OpenApiAddressesListModel(this) };
    QString m_url { "" };
    QNetworkAccessManager* m_networkManager { new QNetworkAccessManager(this) };
    bool m_loading { false };

public:
    explicit OpenApiExporterViewModel(QObject *parent = nullptr);

    OpenApiAddressesListModel* addresses() const noexcept { return m_addresses; }

    QString url() const noexcept { return m_url; }

    bool loading() const noexcept { return m_loading; }

    Q_INVOKABLE void loadOpenApiScheme() noexcept;

private:
    void parseJsonSpecification(const QString& json) noexcept;

private slots:
    void requestFinished(QNetworkReply *reply);

signals:
    void addressesChanged();
    void urlChanged();
    void loadingChanged();

};

#endif // OPENAPIEXPORTERVIEWMODEL_H
