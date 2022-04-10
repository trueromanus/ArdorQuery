#include "textadvisorviewmodel.h"

TextAdvisorViewModel::TextAdvisorViewModel(QObject *parent)
    : QObject{parent}
{
    fillCompletings();
}

void TextAdvisorViewModel::makeSuggestions(const QString &text)
{
    if (text.length() == 0) return;
    if (text.length() < 2) return;

    auto firstTwoCharacter = text.mid(0, 2).toLower();
    if (m_singleCompletings->contains(firstTwoCharacter)) {
        if (text != m_singleCompletings->value(firstTwoCharacter)) {
            //TODO: fill suggestion
        }
    }
    if (m_multipleCompletings->contains(firstTwoCharacter)) {
        auto completings = m_multipleCompletings->value(firstTwoCharacter);
        if (!completings.contains(text)) {
            //TODO: fill suggestion
        }
    }
}

void TextAdvisorViewModel::fillCompletings()
{
    m_singleCompletings->insert("Au", "Authorization");

    QStringList acceptList;
    acceptList.append("Accept");
    acceptList.append("Accept-Charset");
    acceptList.append("Accept-Encoding");
    acceptList.append("Accept-Language");
    acceptList.append("Accept-Datetime");
    m_multipleCompletings->insert("accep", acceptList);

    QStringList accessList;
    accessList.append("Access-Control-Request-Method");
    accessList.append("Access-Control-Request-Headers");
    m_multipleCompletings->insert("Acces", accessList);

    m_singleCompletings->insert("ca", "Cache-Control");

    QStringList connectionList;
    connectionList.append("Connection");
    connectionList.append("Content-Encoding");
    connectionList.append("Content-Length");
    connectionList.append("Content-Type");
    connectionList.append("Cookie");
    m_multipleCompletings->insert("co", connectionList);

    QStringList ifList;
    ifList.append("If-Match");
    ifList.append("If-Modified-Since");
    ifList.append("If-None-Match");
    ifList.append("If-Range");
    ifList.append("If-Unmodified-Since");
    m_multipleCompletings->insert("if", ifList);

    m_singleCompletings->insert("da", "Date");

    m_singleCompletings->insert("ex", "Expect");

    m_singleCompletings->insert("fo", "Forwarded");

    m_singleCompletings->insert("fr", "From");

    m_singleCompletings->insert("ho", "Host");

    QStringList maList;
    maList.append("Mandatory");
    maList.append("Max-Forwards");
    m_multipleCompletings->insert("ma", maList);

    m_singleCompletings->insert("ht", "HTTP2-Settings");

    m_singleCompletings->insert("or", "Origin");

    QStringList prList;
    prList.append("Pragma");
    prList.append("Prefer");
    prList.append("Proxy-Authorization");
    m_multipleCompletings->insert("pr", prList);

    m_singleCompletings->insert("ra", "Range");

    m_singleCompletings->insert("re", "Referer");

    //TE only in HTTP2

    QStringList trList;
    trList.append("trailer");
    trList.append("transfer-Encoding");
    m_multipleCompletings->insert("pr", trList);

    m_singleCompletings->insert("re", "Referer");

    m_singleCompletings->insert("us", "User-Agent");

    m_singleCompletings->insert("up", "Upgrade");

    m_singleCompletings->insert("vi", "Via");

    m_singleCompletings->insert("wa", "Warning");
}
