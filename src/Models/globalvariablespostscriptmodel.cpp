#include "globalvariablespostscriptmodel.h"

GlobalVariablesPostScriptModel::GlobalVariablesPostScriptModel(QObject *parent, GlobalVariablesListModel* globalVariables,
    QSharedPointer<QList<HttpRequestModel*>> requests)
    : QObject{parent}
{
    m_globalVariables = globalVariables;
    m_requests = requests;
}

bool GlobalVariablesPostScriptModel::has(const QString &name)
{
    return m_globalVariables->hasVariableName(name);
}

QString GlobalVariablesPostScriptModel::get(const QString &name)
{
    if (!m_globalVariables->hasVariableName(name)) return "";

    return m_globalVariables->getVariable(name);
}

QVariantMap GlobalVariablesPostScriptModel::getQueryStatus(const QString &unique)
{
    QVariantMap map;

    auto iterator = std::find_if(
        m_requests->begin(),
        m_requests->end(),
        [unique](HttpRequestModel* item) {
            auto id = item->requestModel()->getIdentifier();
            return id == unique;
        }
    );
    if (iterator == m_requests->end()) return map;

    auto item = *iterator;
    map["hasErrors"] = item->resultModel()->hasError();
    map["errorMessage"] = item->resultModel()->networkError();
    map["status"] = item->resultModel()->statusCode();
    map["responseSize"] = item->resultModel()->responseSize();

    return map;
}

QList<QString> GlobalVariablesPostScriptModel::getQueryHeaders(const QString &unique)
{
    QList<QString> result;

    auto iterator = std::find_if(
        m_requests->begin(),
        m_requests->end(),
        [unique](HttpRequestModel* item) {
            auto id = item->requestModel()->getIdentifier();
            return id == unique;
        }
        );
    if (iterator == m_requests->end()) return result;

    auto item = *iterator;

    result.append(item->resultModel()->headers());
    return result;
}
