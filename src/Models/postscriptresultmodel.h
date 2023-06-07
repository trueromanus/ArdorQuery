#ifndef POSTSCRIPTRESULTMODEL_H
#define POSTSCRIPTRESULTMODEL_H

#include <QObject>

class PostScriptResultModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool hasErrors READ hasErrors WRITE setHasErrors NOTIFY hasErrorsChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage WRITE setErrorMessage NOTIFY errorMessageChanged)

private:
    bool m_hasErrors { false };
    QString m_errorMessage { "" };

public:
    explicit PostScriptResultModel(QObject *parent = nullptr);

    bool hasErrors() const noexcept { return m_hasErrors; }
    void setHasErrors(bool hasErrors) noexcept;

    QString errorMessage() const noexcept { return m_errorMessage; }
    void setErrorMessage(const QString& errorMessage) noexcept;

signals:
    void hasErrorsChanged();
    void errorMessageChanged();

};

#endif // POSTSCRIPTRESULTMODEL_H
