#ifndef GLOBALVARIABLESPOSTSCRIPTMODEL_H
#define GLOBALVARIABLESPOSTSCRIPTMODEL_H

#include <QObject>
#include <QSharedPointer>
#include "../ListModels/globalvariableslistmodel.h"
#include "../Models/httprequestmodel.h"

class GlobalVariablesPostScriptModel : public QObject
{
    Q_OBJECT

private:
    GlobalVariablesListModel* m_globalVariables { nullptr };
    QSharedPointer<QList<HttpRequestModel*>> m_requests { nullptr };

public:
    explicit GlobalVariablesPostScriptModel(QObject *parent, GlobalVariablesListModel* globalVariables, QSharedPointer<QList<HttpRequestModel*>> requests);

    Q_INVOKABLE bool has(const QString& name);
    Q_INVOKABLE QString get(const QString& name);
    Q_INVOKABLE QVariantMap getQueryStatus(const QString& unique);
    Q_INVOKABLE QList<QString> getQueryHeaders(const QString& unique);

signals:
};

#endif // GLOBALVARIABLESPOSTSCRIPTMODEL_H
