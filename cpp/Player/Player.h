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
    Q_PROPERTY(Song *currentSong READ getCurrentSong NOTIFY currentSongChanged FINAL)
    Q_PROPERTY(Song *nextSong READ getNextSong NOTIFY nextSongChanged FINAL)
public:
    explicit Player(QObject *parent = nullptr);

public slots:
    void updateCurrentSong(Song *song);
    void updateNextSong(Song *song);

    void playSongByID(int songID); // used by qml when player press play on any song in list

public:

    Song *getCurrentSong() const;
    Song *getNextSong() const;

    void setCurrentSong(Song *song);
    void setNextSong(Song *song);

signals:
    void currentSongChanged();
    void nextSongChanged();

    void askForSongByPosition(int position, bool forCurrentSongPurpose = true);
    void askForSongByID(int id, bool forCurrentSongPurpose = true);
    void askForNextSongByCurrentSongID(int currentSongID);

private:
    Song *m_currentSong;
    Song *m_nextSong;

    PlayerQueue playerQueue;
};

#endif // PLAYER_H
