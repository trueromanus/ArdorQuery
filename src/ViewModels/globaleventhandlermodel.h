#ifndef GLOBALEVENTHANDLERMODEL_H
#define GLOBALEVENTHANDLERMODEL_H

#include <QObject>
#include <QSet>
#include <QMap>

class GlobalEventHandlerModel : public QObject
{
    Q_OBJECT

private:
    const QString m_plusKey { "plus" };
    const QString m_minusKey { "minus" };
    const QString m_f1Key { "f1" };
    const QString m_f2Key { "f2" };
    const QString m_f3Key { "f3" };
    const QString m_f4Key { "f4" };
    const QString m_f5Key { "f5" };
    const QString m_f6Key { "f6" };
    const QString m_f7Key { "f7" };
    const QString m_f8Key { "f8" };
    const QString m_f9Key { "f9" };
    const QString m_f10Key { "f10" };
    const QString m_f11Key { "f11" };
    const QString m_f12Key { "f12" };
    const QString m_tabKey { "tab" };
    const QString m_escapeKey { "escape" };
    const QString m_control { "control" };
    const QString m_shift { "shift" };
    const QString m_alt { "alt" };
    QMap<int, QString> m_keyMapping { QMap<int, QString>() };
    QSet<QString> m_pressedKeys { QSet<QString>() };

public:
    explicit GlobalEventHandlerModel(QObject *parent = nullptr);

    bool eventFilter(QObject* watched, QEvent* event);

private:
    QString pressedKeysToString();

signals:
    void backButtonPressed();
    void forwardButtonPressed();
    void keysChanged(const QString& state);

};

#endif // GLOBALEVENTHANDLERMODEL_H
