#ifndef OPENAPIROUTEMODEL_H
#define OPENAPIROUTEMODEL_H

#include <QString>

class OpenApiRouteModel
{
private:
    QString m_path { "" };
    QString m_method { "" };
    QString m_summary { "" };

public:
    OpenApiRouteModel();

    QString path() const noexcept { return m_path; }
    QString method() const noexcept { return m_method; }
    QString summary() const noexcept { return m_summary; }

    void setPath(const QString& path) noexcept { m_path = path; }
    void setMethod(const QString& method) noexcept { m_method = method; }
    void setSummary(const QString& summary) noexcept { m_summary = summary; }

};

#endif // OPENAPIROUTEMODEL_H
