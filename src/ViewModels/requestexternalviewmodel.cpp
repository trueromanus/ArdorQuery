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

#include <QGuiApplication>
#include <QClipboard>
#include "requestexternalviewmodel.h"

RequestExternalViewModel::RequestExternalViewModel(QObject *parent)
    : QObject{parent}
{

}

void RequestExternalViewModel::parseFromString(const QString &input) noexcept
{
    auto lines = input.split("\n");
    bool isBodyType = false;
    QString bodyContent = "";
    auto currentIndex = m_httpRequest->selectedItem();
    auto insertToEnd = currentIndex == m_httpRequest->countItems() - 1;
    if (!insertToEnd) currentIndex += 1;
    foreach (auto line, lines) {
        HttpRequestViewModel::HttpRequestTypes type = HttpRequestViewModel::HttpRequestTypes::UnknownType;
        if (line.startsWith(UrlPrefix)) {
            type = HttpRequestViewModel::HttpRequestTypes::UrlType;
            isBodyType = false;
        }
        if (line.startsWith(MethodPrefix)) {
            type = HttpRequestViewModel::HttpRequestTypes::MethodType;
            isBodyType = false;
        }
        if (line.startsWith(FormPrefix)) {
            type = HttpRequestViewModel::HttpRequestTypes::FormItemType;
            isBodyType = false;
        }
        if (line.startsWith(FilePrefix)) {
            type = HttpRequestViewModel::HttpRequestTypes::FormFileType;
            isBodyType = false;
        }
        if (line.startsWith(ProtocolPrefix)) {
            type = HttpRequestViewModel::HttpRequestTypes::HttpProtocolType;
            isBodyType = false;
        }
        if (line.startsWith(BearerPrefix)) {
            type = HttpRequestViewModel::HttpRequestTypes::BearerType;
            isBodyType = false;
        }
        if (line.startsWith(TitlePrefix)) {
            type = HttpRequestViewModel::HttpRequestTypes::TitleType;
            isBodyType = false;
        }
        if (line.startsWith(ParamPrefix)) {
            type = HttpRequestViewModel::HttpRequestTypes::ParamType;
            isBodyType = false;
        }
        if (line.startsWith(PastryPrefix)) {
            type = HttpRequestViewModel::HttpRequestTypes::PastryType;
            isBodyType = false;
        }
        if (line.startsWith(RoutePrefix)) {
            type = HttpRequestViewModel::HttpRequestTypes::RouteType;
            isBodyType = false;
        }
        if (line.startsWith(OptionsPrefix)) {
            type = HttpRequestViewModel::HttpRequestTypes::OptionsType;
            isBodyType = false;
        }
        if (line.startsWith(PostScriptPrefix)) {
            type = HttpRequestViewModel::HttpRequestTypes::PostScriptType;
            isBodyType = false;
        }
        if (line.startsWith(TimeoutPrefix)) {
            type = HttpRequestViewModel::HttpRequestTypes::TimeoutType;
            isBodyType = false;
        }
        if (line.startsWith(OrderPrefix)) {
            type = HttpRequestViewModel::HttpRequestTypes::OrderType;
            isBodyType = false;
        }
        if (m_textAdvisor->isContainsHeader(line)) {
            type = HttpRequestViewModel::HttpRequestTypes::HeaderType;
            isBodyType = false;
        }
        if (line.startsWith(BodyPrefix) || line.startsWith(JsonPrefix) || line.startsWith(XmlPrefix)) {
            type = HttpRequestViewModel::HttpRequestTypes::BodyType;
            isBodyType = true;
        }

        if (isBodyType) {
            bodyContent.append((!bodyContent.isEmpty() ? "\n" : "") + line);
            continue;
        }

        if (type == HttpRequestViewModel::HttpRequestTypes::UnknownType) continue;

        if (type != HttpRequestViewModel::HttpRequestTypes::UnknownType) {
            m_httpRequest->addItem(insertToEnd ? -1 : currentIndex, type, line);
        }
        currentIndex++;
    }

    if (!bodyContent.isEmpty()) {
        if (bodyContent.last(1) == "\n") bodyContent = bodyContent.mid(0, bodyContent.length() - 1);
        m_httpRequest->addItem(-1, HttpRequestViewModel::HttpRequestTypes::BodyType, bodyContent);
    }
}

void RequestExternalViewModel::setHttpRequest(const HttpRequestViewModel *httpRequest) noexcept
{
    if (m_httpRequest == httpRequest) return;

    m_httpRequest = const_cast<HttpRequestViewModel*>(httpRequest);
    emit httpRequestChanged();
}

void RequestExternalViewModel::setTextAdvisor(const TextAdvisorViewModel *textAdvisor) noexcept
{
    if (m_textAdvisor == textAdvisor) return;

    m_textAdvisor = const_cast<TextAdvisorViewModel*>(textAdvisor);
    emit textAdvisorChanged();
}

void RequestExternalViewModel::appendFromClipboard()
{
    auto text = getTextFromClipboard();
    if (text.isEmpty()) return;

    auto isEmptyList = m_httpRequest->isOnlyEmptyFirstItem();

    parseFromString(text);

    if (isEmptyList && m_httpRequest->countItems() > 1) m_httpRequest->removeFirstItem();
}

void RequestExternalViewModel::replaceFromClipboard()
{
    auto text = getTextFromClipboard();
    if (text.isEmpty()) return;

    m_httpRequest->clearFields();

    parseFromString(text);

    //it need for remove empty first item
    if (m_httpRequest->countItems() > 1) m_httpRequest->removeFirstItem();
}

void RequestExternalViewModel::copyToClipboard()
{
    auto clipboard = QGuiApplication::clipboard();
    auto fields = m_httpRequest->getAllFields();
    clipboard->setText(fields, QClipboard::Clipboard);
}

void RequestExternalViewModel::removeFirstItemIfNeeded()
{
     if (m_httpRequest->countItems() > 1) m_httpRequest->removeFirstItem();
}

QString RequestExternalViewModel::getTextFromClipboard() const noexcept
{
    auto clipboard = QGuiApplication::clipboard();
    return clipboard->text(QClipboard::Clipboard);
}

