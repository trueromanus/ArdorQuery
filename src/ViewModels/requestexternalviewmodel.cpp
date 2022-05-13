#include <QGuiApplication>
#include <QClipboard>
#include "requestexternalviewmodel.h"

RequestExternalViewModel::RequestExternalViewModel(QObject *parent)
    : QObject{parent}
{

}

QString RequestExternalViewModel::parseFromString(const QString &input) noexcept
{
    auto lines = input.split("\n");
    foreach (auto line, lines) {
        HttpRequestViewModel::HttpRequestTypes type = HttpRequestViewModel::HttpRequestTypes::UnknownType;
        if (line.startsWith(UrlPrefix)) {
            type = HttpRequestViewModel::HttpRequestTypes::UrlType;
        }
        if (line.startsWith(MethodPrefix)) {
            type = HttpRequestViewModel::HttpRequestTypes::MethodType;
        }
        if (line.startsWith(FormPrefix)) {
            type = HttpRequestViewModel::HttpRequestTypes::FormItemType;
        }
        if (line.startsWith(BodyPrefix)) {
            type = HttpRequestViewModel::HttpRequestTypes::BodyType;
        }

        if (type == HttpRequestViewModel::HttpRequestTypes::UnknownType) continue;

        m_httpRequest->addItem(-1, type, line);
    }

    return "";
}

void RequestExternalViewModel::setHttpRequest(const HttpRequestViewModel *httpRequest) noexcept
{
    if (m_httpRequest == httpRequest) return;

    m_httpRequest = const_cast<HttpRequestViewModel*>(httpRequest);
    emit httpRequestChanged();
}

void RequestExternalViewModel::appendFromClipboard()
{
    auto text = getTextFromClipboard();
    if (text.isEmpty()) return;

    parseFromString(text);
}

void RequestExternalViewModel::replaceFromClipboard()
{
    auto text = getTextFromClipboard();
    if (text.isEmpty()) return;

    m_httpRequest->clearFields();

    parseFromString(text);
}

QString RequestExternalViewModel::getTextFromClipboard() const noexcept
{
    auto clipboard = QGuiApplication::clipboard();
    return clipboard->text(QClipboard::Clipboard);
}

