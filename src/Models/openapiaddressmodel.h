#ifndef OPENAPIADDRESSMODEL_H
#define OPENAPIADDRESSMODEL_H

#include <QString>
#include <QUuid>

class OpenApiAddressModel
{
private:
    QUuid m_addressId { QUuid::createUuid() };
    QString m_title { "" };
    QString m_address { "" };
    QString m_baseUrl { "" };
    QString m_filter { "" };
    QString m_securities { "" };

public:
    OpenApiAddressModel();

    QUuid addressId() const noexcept { return m_addressId; }
    QString title() const noexcept { return m_title; }
    QString address() const noexcept { return m_address; }
    QString baseUrl() const noexcept { return m_baseUrl; }
    QString filter() const noexcept { return m_filter; }
    QString securities() const noexcept { return m_securities; }

    void setTitle(const QString& title) noexcept { m_title = title; }
    void setAddress(const QString& address) noexcept { m_address = address; }
    void setBaseUrl(const QString& baseUrl) noexcept { m_baseUrl = baseUrl; }
    void setFilter(const QString& filter) noexcept { m_filter = filter; }
    void setSecurities(const QString& securities) noexcept { m_securities = securities; }
    void setAddressId(const QString& id) noexcept { m_addressId = QUuid(id); }
};

#endif // OPENAPIADDRESSMODEL_H
