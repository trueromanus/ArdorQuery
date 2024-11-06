#ifndef POSTSCRIPTSESSIONMODEL_H
#define POSTSCRIPTSESSIONMODEL_H

#include <QObject>
#include <QVariant>

class PostScriptSessionModel : public QObject
{
    Q_PROPERTY(QVariant shared READ shared WRITE setShared NOTIFY sharedChanged FINAL)
    Q_OBJECT

private:
    QVariant m_shared { QVariant() };
    QMap<QString, QString> m_remapValues { QMap<QString, QString>() };

public:
    explicit PostScriptSessionModel(QObject *parent = nullptr);

    QVariant shared() const noexcept { return m_shared; }
    void setShared(const QVariant& shared);

    QMap<QString, QString>& getRemapValues() noexcept { return m_remapValues; }

    Q_INVOKABLE void setRemapValue(const QString& name, const QString& remapValue);
    Q_INVOKABLE QString getRemapValue(const QString& name);

signals:
    void sharedChanged();

};

#endif // POSTSCRIPTSESSIONMODEL_H
