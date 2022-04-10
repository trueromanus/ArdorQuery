#ifndef TEXTADVISORVIEWMODEL_H
#define TEXTADVISORVIEWMODEL_H

#include <QObject>
#include <QMap>

class TextAdvisorViewModel : public QObject
{
    Q_OBJECT
private:
    QScopedPointer<QMap<QString, QString>> m_singleCompletings { new QMap<QString, QString>() };
    QScopedPointer<QMap<QString, QStringList>> m_multipleCompletings { new QMap<QString, QStringList>() };

public:
    explicit TextAdvisorViewModel(QObject *parent = nullptr);

    Q_INVOKABLE void makeSuggestions(const QString& text);

private:
    void fillCompletings();

signals:

};

#endif // TEXTADVISORVIEWMODEL_H
