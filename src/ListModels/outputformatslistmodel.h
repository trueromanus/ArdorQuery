#ifndef OUTPUTFORMATSLISTMODEL_H
#define OUTPUTFORMATSLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QStringList>

class OutputFormatsListModel : public QAbstractListModel
{
    Q_OBJECT
private:
    QStringList m_outputFormats { QStringList() };
    QMap<QString, QString> m_outputFormatTitles { QMap<QString, QString>() };
    enum OutputFormatRoles {
        IdentfierRole = Qt::UserRole + 1,
        TitleRole,
        IndexRole,
    };

public:
    explicit OutputFormatsListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:

};

#endif // OUTPUTFORMATSLISTMODEL_H
