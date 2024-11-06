#include <QJSValue>
#include "postscriptsessionmodel.h"

PostScriptSessionModel::PostScriptSessionModel(QObject *parent)
    : QObject{parent}
{

}

void PostScriptSessionModel::setShared(const QVariant& shared)
{
    auto type = QString(shared.typeName());
    if (type == "QJSValue") {
        auto jsValue = shared.value<QJSValue>();
        m_shared = jsValue.toVariant(QJSValue::ConvertJSObjects);
    } else {
        if (m_shared == shared) return;

        m_shared = shared;
    }

    emit sharedChanged();
}

void PostScriptSessionModel::setRemapValue(const QString &name, const QString &remapValue)
{
    if (m_remapValues.contains(name)) {
        m_remapValues[name] = remapValue;
    } else {
        m_remapValues.insert(name, remapValue);
    }
}

QString PostScriptSessionModel::getRemapValue(const QString &name)
{
    if (m_remapValues.contains(name)) return m_remapValues.value(name);

    return "";
}


