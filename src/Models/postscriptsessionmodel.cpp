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


