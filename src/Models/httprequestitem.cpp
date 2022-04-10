#include "httprequestitem.h"

HttpRequestItem::HttpRequestItem()
{

}

void HttpRequestItem::setType(int type) noexcept
{
    m_type = type;
}

void HttpRequestItem::setText(QString text) noexcept
{
    m_text = text;
}
