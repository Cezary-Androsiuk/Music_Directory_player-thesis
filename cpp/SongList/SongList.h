#ifndef SONGLIST_H
#define SONGLIST_H

#include <QObject>
#include <QString>

#include <cpp/DebugPrint.h>
#include <cpp/Song/Song.h>

class SongList : public QObject
{
    Q_OBJECT
public:
    explicit SongList(QObject *parent = nullptr);

signals:

private:
    QList<Song*> m_songs;
};

#endif // SONGLIST_H
