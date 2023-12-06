#include <QString>
#include <QDir>
#include <QStandardPaths>
#include "globalconstants.h"

QString getCachePath(const QString& filename) noexcept;

void createIfNotExistsFile(const QString& path, const QString& defaultContent) noexcept;

QString adjustShortcutsForDisplay(QString& value) noexcept;
