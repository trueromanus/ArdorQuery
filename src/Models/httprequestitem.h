#ifndef HTTPREQUESTITEM_H
#define HTTPREQUESTITEM_H

#include <QString>

class HttpRequestItem
{

private:
    int m_type { 0 };
    QString m_text { "" };

public:
    HttpRequestItem();

    int type() const noexcept { return m_type; }
    void setType(int type) noexcept;

    QString text() const noexcept { return m_text; }
    void setText(QString text) noexcept;

};

#endif // HTTPREQUESTITEM_H
