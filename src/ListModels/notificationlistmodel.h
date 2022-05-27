#ifndef NOTIFICATIONLISTMODEL_H
#define NOTIFICATIONLISTMODEL_H

#include <QObject>
#include <QObject>
#include <QAbstractListModel>
#include <QQueue>
#include <QDateTime>
#include <QTimer>
#include <QSharedPointer>

class NotificationListModel : public QAbstractListModel
{
    Q_OBJECT
private:
    bool m_currentAnimating { false };
    QQueue<std::tuple<int, QDateTime>> m_currentEvents { QQueue<std::tuple<int, QDateTime>>() };
    QScopedPointer<QTimer> m_timer { new QTimer() };
    QList<std::tuple<QString, QString, QString>> m_messages { QList<std::tuple<QString, QString, QString>>() };

    enum NotificationPoolField {
        IndexField = 0,
        ExpiredField
    };

    enum NotificationPoolRoles {
        TitleRole = Qt::UserRole + 1,
        MessageRole,
        IsErrorRole,
        IsInformationRole,
        IsAnimationRole
    };

public:
    explicit NotificationListModel(QObject *parent = nullptr);
    void pushMessage() noexcept;
    void refresh() noexcept;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE void itemAnimationEnded() noexcept;

    void pushErrorMessage(const QString &title, const QString &message);
    void pushInfoMessage(const QString &title, const QString &message);

signals:
    void startItemAnimation(const int index);

private slots:
    void timerTriggered();

};

#endif // NOTIFICATIONLISTMODEL_H
