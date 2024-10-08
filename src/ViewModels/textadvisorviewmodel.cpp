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

#include "textadvisorviewmodel.h"
#include "../globalconstants.h"

TextAdvisorViewModel::TextAdvisorViewModel(QObject *parent)
    : QObject{parent}
{
    fillCompletings();
}

bool TextAdvisorViewModel::isContainsHeader(const QString &text) const noexcept
{
    auto firstTwoCharacter = text.mid(0, 2).toLower();
    auto lowerText = text.toLower();
    if (m_singleCompletings->contains(firstTwoCharacter)) {
        auto value = m_singleCompletings->value(firstTwoCharacter);
        auto lowerValue = value.toLower();
        if (lowerValue.startsWith(lowerText)) return true;
        if (lowerText.startsWith(lowerValue)) return true;
    }
    if (m_multipleCompletings->contains(firstTwoCharacter)) {
        auto completings = m_multipleCompletings->value(firstTwoCharacter);
        foreach (auto completing, completings) {
            auto lowerValue = completing.toLower();
            if (lowerText.startsWith(lowerValue)) return true;
            if (lowerValue.startsWith(lowerText)) return true;
        }
    }

    if (firstTwoCharacter == CustomHeaderPrefix) return true;
    if (lowerText.startsWith(HeaderPrefix)) return true;

    return false;
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
    m_singleCompletings->insert("au", "Authorization");
    m_singleCompletings->insert("ca", "Cache-Control");
    m_singleCompletings->insert("da", "Date");
    m_singleCompletings->insert("ex", "Expect");
    m_singleCompletings->insert("fo", "Forwarded");
    m_singleCompletings->insert("fr", "From");
    m_singleCompletings->insert("ho", "Host");
    m_singleCompletings->insert("ht", "HTTP2-Settings");
    m_singleCompletings->insert("or", "Origin");
    m_singleCompletings->insert("ra", "Range");
    m_singleCompletings->insert("re", "Referer");
    m_singleCompletings->insert("re", "Referer");
    m_singleCompletings->insert("us", "User-Agent");
    m_singleCompletings->insert("up", "Upgrade");
    m_singleCompletings->insert("vi", "Via");
    m_singleCompletings->insert("wa", "Warning");

    QStringList acceptList;
    acceptList.append("Accept");
    acceptList.append("Accept-Charset");
    acceptList.append("Accept-Encoding");
    acceptList.append("Accept-Language");
    acceptList.append("Accept-Datetime");
    acceptList.append("Access-Control-Request-Method");
    acceptList.append("Access-Control-Request-Headers");
    m_multipleCompletings->insert("ac", acceptList);

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

    QStringList maList;
    maList.append("Mandatory");
    maList.append("Max-Forwards");
    m_multipleCompletings->insert("ma", maList);

    QStringList prList;
    prList.append("Pragma");
    prList.append("Prefer");
    prList.append("Proxy-Authorization");
    m_multipleCompletings->insert("pr", prList);

    QStringList trList;
    trList.append("Trailer");
    trList.append("Transfer-Encoding");
    m_multipleCompletings->insert("tr", trList);
}
