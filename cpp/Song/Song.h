#ifndef SONG_H
#define SONG_H

#include <QObject>
#include <QString>

#include <cpp/DebugPrint.h>


class Song : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int ID READ getID CONSTANT FINAL)
    Q_PROPERTY(QString title READ getTitle CONSTANT FINAL)
public:
    explicit Song(QObject *parent = nullptr);

    int getID() const;
    QString getTitle() const;

    void setID(int ID);
    void setTitle(const QString &title);

signals:

private:
    int m_ID;
    QString m_title;
};

typedef QList<Song *> SongList;

#endif // SONG_H
