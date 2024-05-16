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

public slots: /// bottom player actions
    void play();
    void pause();

public:
    Song *getCurrentSong() const;

public slots:
    void setCurrentSong(Song *song); /// triggered by signal Playlist::newCurrentSongLoaded

signals:
    void currentSongChanged();

    void askForSongByPosition(int position); /// emitted when player want a new song
    void askForSongByID(int id); /// emitted when player want a new song

private:
    Song *m_currentSong;
};

#endif // PLAYER_H
