#ifndef PLAYERQUEUE_H
#define PLAYERQUEUE_H

#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaMetaData>

#include <cpp/DebugPrint.h>

class PlayerQueue
{
public:
    PlayerQueue();
    ~PlayerQueue();

    QMediaPlayer *getCurrentSong() const;
    QMediaPlayer *getNextSong() const;

    void popCurrent();
    void pushNext(QMediaPlayer *song);

private:
    void pushCaseXX(QMediaPlayer *song);
    void pushCaseX_(QMediaPlayer *song);
    void pushCase__(QMediaPlayer *song);

private:
    QMediaPlayer *m_currentSong;
    QMediaPlayer *m_nextSong;
};

#endif // PLAYERQUEUE_H
