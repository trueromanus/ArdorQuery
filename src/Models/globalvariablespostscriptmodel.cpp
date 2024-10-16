#include "globalvariablespostscriptmodel.h"

GlobalVariablesPostScriptModel::GlobalVariablesPostScriptModel(QObject *parent, GlobalVariablesListModel* globalVariables)
    : QObject{parent}
{
    m_globalVariables = globalVariables;
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
