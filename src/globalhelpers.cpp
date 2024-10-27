#include <QUrl>
#include "globalhelpers.h"
#include "globalconstants.h"

QString getCachePath(const QString& filename) noexcept
{
    QString cacheFileName = "/" + filename;
    if (IsPortable) {
        return QDir::currentPath() + cacheFileName;
    } else {
        return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + cacheFileName;
    }
}

void createIfNotExistsFile(const QString& path, const QString& defaultContent) noexcept
{
    if (QFile::exists(path)) return;

    QFile file(path);
    file.open(QFile::WriteOnly | QFile::Text);
    file.write(defaultContent.toUtf8());
    file.close();
}

QString adjustShortcutsForDisplay(QString &value) noexcept
{
    return value
#ifdef Q_OS_MACOS
        .replace("control", "Command")
        .replace("alt", "Option")
#else
        .replace("control", "Control")
        .replace("alt", "Alt")
#endif
        .replace("shift", "Shift")
        .replace("page", "Page")
        .replace("up", "Up")
        .replace("down", "Down")
        .replace("enter", "Enter")
        .replace("insert", "Insert")
        .replace("escape", "Escape")
        .replace("delete", "Delete")
        .replace("tab", "Tab")
        .replace("backspace", "Backspace")
        .replace("plus", "Plus")
        .replace("minus", "Minus")
        .replace("f1", "F1")
        .replace("f2", "F2")
        .replace("f3", "F3")
        .replace("f4", "F4")
        .replace("f5", "F5")
        .replace("f6", "F6")
        .replace("f7", "F7")
        .replace("f8", "F8")
        .replace("f9", "F9")
        .replace("f10", "F10")
        .replace("f11", "F11")
        .replace("f12", "F12");
}

QString removeFileProtocol(QString &value) noexcept
{
#ifdef Q_OS_WIN
    return value.replace("file:///", "");
#else
    return value.replace("file://", "");
#endif
}

QString getFileProtocol() noexcept
{
#ifdef Q_OS_WIN
    return "file:///";
#else
    return "file://";
#endif
}

QString extractFileNameFromContentDisposition(const QString &value) noexcept
{
    auto contentDisposition = value.toLower();
    QString downloadFileName = "";
    if (contentDisposition.isEmpty()) return downloadFileName;

    contentDisposition = contentDisposition.replace("content-disposition: ", "");

    if (!value.contains("attachment", Qt::CaseInsensitive)) return downloadFileName;

    auto parts = contentDisposition.split(";");
    foreach (auto part, parts) {
        auto nameParts = part.split("=");
        if (nameParts.length() != 2) continue;

        auto fileNamePart = nameParts[0].replace("\"","").trimmed().toLower();
        if (fileNamePart != "filename" && fileNamePart != "filename*") continue;
        bool isEncoded = fileNamePart == "filename*";
        auto fileNameValue = nameParts[1].replace("\"","").replace("utf-8''", "");
        downloadFileName = isEncoded ? QUrl::fromPercentEncoding(fileNameValue.toUtf8()) : fileNameValue;
    }

    return downloadFileName;
}
