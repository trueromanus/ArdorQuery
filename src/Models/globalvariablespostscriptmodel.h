#ifndef GLOBALVARIABLESPOSTSCRIPTMODEL_H
#define GLOBALVARIABLESPOSTSCRIPTMODEL_H

#include <QObject>
#include "../ListModels/globalvariableslistmodel.h"

class GlobalVariablesPostScriptModel : public QObject
{
    Q_OBJECT

private:
    GlobalVariablesListModel* m_globalVariables { nullptr };

public:
    explicit GlobalVariablesPostScriptModel(QObject *parent, GlobalVariablesListModel* globalVariables);

    Q_INVOKABLE bool has(const QString& name);
    Q_INVOKABLE QString get(const QString& name);

signals:
};

#endif // GLOBALVARIABLESPOSTSCRIPTMODEL_H
