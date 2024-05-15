#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include <vector>
#include <QRandomGenerator>

#include <cpp/DebugPrint.h>
#include <cpp/Song/Song.h>

class Playlist : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SongList songs READ getSongs NOTIFY songsChanged FINAL)

public:
    explicit Playlist(QObject *parent = nullptr);

    SongList getSongs() const;

    // void setSongs()
public slots:
    void loadPlaylistSongs(SongList songs);         /// triggered by Backend::songsChanged(SongList songs)
    void shufflePlaylistSongs();

    void loadSongByPosition(int position, bool forCurrentSongPurpose = true);
    void loadSongByID(int id, bool forCurrentSongPurpose = true);
    void loadNextSongByCurrentSongID(int currentSongID);

signals:
    void songsChanged();
    void newCurrentSongLoaded(Song *song);
    void newNextSongLoaded(Song *song);

private:
    static std::vector<int> getUniqueRandomNumbers(int count);
    static SongList shuffleList(const SongList &songs);

private:
    SongList m_songs;
};

#endif // PLAYLIST_H


/*
 * init:                    player current = getSongByPos(0),       next = nextSong(current.id)
 * songFinished:            player current = next,                  next = nextSong(current.id)
 * clicked on play songID   player current = getSongById(songID),   next = nextSong(current.id)
 * on shuffle:              player current = current,               next = getSongByPos(0)
 */
