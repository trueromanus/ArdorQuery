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

#include "httpperformerviewmodel.h"

HttpPerformerViewModel::HttpPerformerViewModel(QObject *parent)
    : QObject{parent}
{
    connect(m_networkManager.get(), &QNetworkAccessManager::finished, this, &HttpPerformerViewModel::requestFinished);
}

void HttpPerformerViewModel::setHttpRequest(const HttpRequestViewModel *viewModel) noexcept
{
    if (m_httpRequest == viewModel) return;

    m_httpRequest = const_cast<HttpRequestViewModel*>(viewModel);
    emit httpRequestChanged();
}

void HttpPerformerViewModel::performRequest()
{
    //TODO: get info about url and
}

void HttpPerformerViewModel::requestFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::TimeoutError) return;
    if (reply->error() == QNetworkReply::ProtocolFailure) return;
    if (reply->error() == QNetworkReply::HostNotFoundError) return;

    //QString data = reply->readAll();

    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (status_code.isValid()) {
        auto statusCode = status_code.toInt();
        qDebug() << statusCode;
    }

    // auto headers = reply->rawHeaderPairs();

    //TODO: handling response and emit result
}
