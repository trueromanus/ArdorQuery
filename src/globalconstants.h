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

#ifndef GLOBALCONSTANT_H
#define GLOBALCONSTANT_H

#include <QString>

extern QString UrlPrefix;
extern QString MethodPrefix;
extern QString FormPrefix;
extern QString BodyPrefix;
extern QString FilePrefix;
extern QString ProtocolPrefix;
extern QString BearerPrefix;
extern QString TitlePrefix;
extern QString ParamPrefix;
extern QString JsonPrefix;
extern QString XmlPrefix;
extern QString HeaderPrefix;
extern QString CustomHeaderPrefix;
extern QString PastryPrefix;
extern QString RoutePrefix;
extern QString OptionsPrefix;
extern QString PostScriptPrefix;
inline QString TimeoutPrefix = "timeout ";
inline QString OrderPrefix = "order ";
inline QString IdentifierPrefix = "unique ";

extern QString NotificationErrorTopic;
extern QString NotificationInfoTopic;

extern QString OutputFormatAuto;
extern QString OutputFormatJson;
extern QString OutputFormatHtml;
extern QString OutputFormatXml;
extern QString OutputFormatImage;
extern QString OutputNeedDownloaded;
extern QString OutputFormatPlainText;
extern QString OutputFormatCss;

extern bool IsPortable;

#endif
