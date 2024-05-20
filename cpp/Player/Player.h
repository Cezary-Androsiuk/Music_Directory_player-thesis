#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaMetaData>

#include <cpp/DebugPrint.h>
#include <cpp/Song/Song.h>

class Player : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Song *currentSong                    READ getCurrentSong     NOTIFY currentSongChanged   FINAL)
public:
    explicit Player(QObject *parent = nullptr);
    ~Player();

public slots:
    void initialize(); /// triggered by Playlist::playlistInitialized()


    void playOtherNextSongByID(int songID); /// triggered by qml when user select other next song
    void playNextSong(); /// triggered when current song has ended
    void setNextSongAs0(); /// triggered when songs are shuffled and sets next song to first pos

public slots: /// bottom player actions
    void play();
    void pause();

public:
    Song *getCurrentSong() const;

public slots:
    void setCurrentSong(Song *song); /// triggered by signal Playlist::newCurrentSongLoaded

signals:
    void currentSongChanged();
    void songEnded();

    void askForSongByPosition(int position); /// emitted when player want a new song
    void askForSongByID(int id); /// emitted when player want a new song

private:
    Song *m_currentSong;

    bool m_nextSongIsFist; /// stores info about if next song should be pos 0 or currentsong.pos+1
};

#endif // PLAYER_H
