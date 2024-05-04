#ifndef SONG_H
#define SONG_H

#include <QObject>
#include <QString>

#include <cpp/DebugPrint.h>

class Song : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ getTitle CONSTANT FINAL)
public:
    explicit Song(QObject *parent = nullptr);

    QString getTitle() const;

    void setTitle(const QString &title);

signals:

private:
    QString m_title;
};

#endif // SONG_H
