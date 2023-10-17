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
    const QString m_homeKey { "home" };
    const QString m_insertKey { "insert" };
    const QString m_endKey { "end" };
    const QString m_deleteKey { "delete" };
    const QString m_control { "control" };
    const QString m_shift { "shift" };
    const QString m_alt { "alt" };
    const QString m_aKey { "a" };
    const QString m_bKey { "b" };
    const QString m_cKey { "c" };
    const QString m_dKey { "d" };
    const QString m_eKey { "e" };
    const QString m_fKey { "f" };
    const QString m_gKey { "g" };
    const QString m_hKey { "h" };

    const QString m_iKey { "i" };
    const QString m_jKey { "j" };
    const QString m_kKey { "k" };
    const QString m_lKey { "l" };
    const QString m_mKey { "m" };
    const QString m_nKey { "n" };
    const QString m_oKey { "o" };
    const QString m_pKey { "p" };
    const QString m_qKey { "q" };
    const QString m_rKey { "r" };
    const QString m_sKey { "s" };
    const QString m_tKey { "t" };
    const QString m_uKey { "u" };
    const QString m_vKey { "v" };
    const QString m_wKey { "w" };
    const QString m_xKey { "x" };
    const QString m_yKey { "y" };
    const QString m_zKey { "z" };

    const QString m_mouseXButton1 { "xbutton1" };
    const QString m_mouseXButton2 { "xbutton2" };
    QMap<int, QString> m_keyMapping { QMap<int, QString>() };
    QMap<int, QString> m_mouseKeyMapping { QMap<int, QString>() };
    QSet<QString> m_pressedKeys { QSet<QString>() };
    QSet<QString> m_pressedMouseKeys { QSet<QString>() };
    QMap<QString, int> m_keyOrder { QMap<QString, int>() };

public:
    explicit GlobalEventHandlerModel(QObject *parent = nullptr);

    bool eventFilter(QObject* watched, QEvent* event);

private:
    QString pressedKeysToString();
    QString pressedMouseKeysToString();

signals:
    void backButtonPressed();
    void forwardButtonPressed();
    void keysChanged(const QString& state);
    void mouseChanged(const QString& state);

};

#endif // GLOBALEVENTHANDLERMODEL_H
