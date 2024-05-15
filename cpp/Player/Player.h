#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaMetaData>

#include <cpp/DebugPrint.h>
#include <cpp/Song/Song.h>
#include <cpp/PlayerQueue/PlayerQueue.h>

class Player : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Song *currentSong                    READ getCurrentSong     NOTIFY currentSongChanged   FINAL)
    Q_PROPERTY(Song *nextSong                       READ getNextSong        NOTIFY nextSongChanged      FINAL)
    // Q_PROPERTY(const PlayerQueue &playerQueue       READ getPlayerQueue     CONSTANT                    FINAL)
public:
    explicit Player(QObject *parent = nullptr);

public slots:
    void initialize(); /// triggered by Playlist::playlistInitialized()

    void playOtherNextSongByID(int songID); /// triggered by qml when user select other next song

public:
    Song *getCurrentSong() const;
    Song *getNextSong() const;

    // const PlayerQueue &getPlayerQueue() const;

public slots:
    void setCurrentSong(Song *song); /// triggered by signal Playlist::newCurrentSongLoaded
    void setNextSong(Song *song); /// triggered by signal Playlist::newNextSongLoaded

signals:
    void currentSongChanged();
    void nextSongChanged();

    void askForSongByPosition(int position, bool forCurrentSongPurpose = true);
    void askForSongByID(int id, bool forCurrentSongPurpose = true);
    void askForNextSongByCurrentSongID(int currentSongID);

private:
    Song *m_currentSong;
    Song *m_nextSong;

    PlayerQueue *m_playerQueue;
};

#endif // PLAYER_H
