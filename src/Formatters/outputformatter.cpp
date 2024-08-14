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

#include "outputformatter.h"

OutputFormatter::OutputFormatter()
{
}

QString OutputFormatter::format(const QString &data)
{
    //In the base class, simple return inputs
    //the actual formatting needs to be done in descendants
    return data;
}

QMap<int, FormatterLine *> OutputFormatter::silentFormat(const QString &data)
{
    Q_UNUSED(data);

    //In the base class, simple return inputs
    //the actual formatting needs to be done in descendants
    QMap<int, FormatterLine *> result;
    return result;
}
