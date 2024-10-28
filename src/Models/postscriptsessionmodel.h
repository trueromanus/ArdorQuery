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

public:
    explicit PostScriptSessionModel(QObject *parent = nullptr);

    QVariant shared() const noexcept { return m_shared; }
    void setShared(const QVariant& shared);

signals:
    void sharedChanged();

};

#endif // POSTSCRIPTSESSIONMODEL_H
