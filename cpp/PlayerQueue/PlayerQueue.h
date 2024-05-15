#ifndef PLAYERQUEUE_H
#define PLAYERQUEUE_H

#include <QObject>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaMetaData>

#include <cpp/DebugPrint.h>

class PlayerQueue : public QObject
{
    Q_OBJECT
    // Q_PROPERTY(bool isCurrentSongSet READ isCurrentSongSet NOTIFY currentSongChanged FINAL)
    // Q_PROPERTY(bool isNextSongSet    READ isNextSongSet    NOTIFY nextSongChanged    FINAL)
public:
    explicit PlayerQueue(QObject *parent = nullptr);
    ~PlayerQueue();

    QMediaPlayer *getCurrentSong() const;
    QMediaPlayer *getNextSong() const;

    bool isCurrentSongSet() const;
    bool isNextSongSet() const;

    void popCurrent();
    void pushNext(QMediaPlayer *song);

signals:
    void currentSongChanged();
    void nextSongChanged();

private:
    void pushCaseXX(QMediaPlayer *song);
    void pushCaseX_(QMediaPlayer *song);
    void pushCase__(QMediaPlayer *song);

private:
    QMediaPlayer *m_currentSong;
    QMediaPlayer *m_nextSong;
};

#endif // PLAYERQUEUE_H
