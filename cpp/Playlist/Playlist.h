#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include <vector>
#include <QRandomGenerator>

#include <cpp/DebugPrint.h>
#include <cpp/Song/Song.h>

class Playlist : public QObject
{
    typedef QList<Song *> SongList;

    Q_OBJECT
    Q_PROPERTY(SongList songs READ getSongs NOTIFY songsChanged FINAL)

public:
    explicit Playlist(QObject *parent = nullptr);

    SongList getSongs() const;

    // void setSongs()
public slots:
    void loadPlaylistSongs(SongList songs);
    void shufflePlaylistSongs();

signals:
    void songsChanged();

private:
    static std::vector<int> getUniqueRandomNumbers(int count);
    static SongList shuffleList(const SongList &songs);

private:
    SongList m_songs;
};

#endif // PLAYLIST_H
