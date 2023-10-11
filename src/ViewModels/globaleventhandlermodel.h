#ifndef GLOBALEVENTHANDLERMODEL_H
#define GLOBALEVENTHANDLERMODEL_H

#include <QObject>

class GlobalEventHandlerModel : public QObject
{
    Q_OBJECT

private:
    const QString m_plusKey { "plus" };
    const QString m_minusKey { "minus" };
    const QString m_f1Key { "f1" };
    const QString m_tabKey { "tab" };
    bool m_controlPressed { false };
    bool m_shiftPressed { false };
    bool m_altPressed { false };

public:
    explicit GlobalEventHandlerModel(QObject *parent = nullptr);

    bool eventFilter(QObject* watched, QEvent* event);

signals:
    void backButtonPressed();
    void forwardButtonPressed();
    void keyButtonPressed(const QString& key);
};

#endif // GLOBALEVENTHANDLERMODEL_H
