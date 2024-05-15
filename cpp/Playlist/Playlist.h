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
    void shufflePlaylistSongs();                    /// triggered by QML button

    void loadSongByPosition(int position, bool forCurrentSongPurpose = true);
    void loadSongByID(int id, bool forCurrentSongPurpose = true);
    void loadNextSongByCurrentSongID(int currentSongID);

signals:
    void playlistInitialized();             /// emitted after fist time the loadPlaylistSong has been executed and triggers Player to initialize
    void songsLoaded();                     /// sub signal for songsChanged emitted after songs has been loaded // mainly used by Player
    void songsShuffled();                   /// sub signal for songsChanged emitted after songs has been shuffled // mainly used by Player
    void songsChanged();                    /// emitted when any change occur in songs list // mainly used by QML
    void newCurrentSongLoaded(Song *song);  /// emitted after
    void newNextSongLoaded(Song *song);

private:
    static std::vector<int> getUniqueRandomNumbers(int count);
    static SongList shuffleList(const SongList &songs);

private:
    bool m_playlistInitialized;
    SongList m_songs;
};

#endif // PLAYLIST_H


/*
 * init:                    player current = getSongByPos(0),       next = nextSong(current.id)
 * songFinished:            player current = next,                  next = nextSong(current.id)
 * clicked on play songID   player current = getSongById(songID),   next = nextSong(current.id)
 * on shuffle:              player current = current,               next = getSongByPos(0)
 */
