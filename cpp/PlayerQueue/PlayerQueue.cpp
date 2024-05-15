#include "PlayerQueue.h"

PlayerQueue::PlayerQueue() :
    m_currentSong(nullptr),
    m_nextSong(nullptr)
{

}

PlayerQueue::~PlayerQueue()
{
    if(m_currentSong != nullptr)
        delete m_currentSong;

    if(m_nextSong != nullptr)
        delete m_nextSong;
}

QMediaPlayer *PlayerQueue::getCurrentSong() const
{
    if(m_currentSong == nullptr)
        WR << "asked for null!";
    return m_currentSong;
}

QMediaPlayer *PlayerQueue::getNextSong() const
{
    if(m_nextSong == nullptr)
        WR << "asked for null!";
    return m_nextSong;
}

void PlayerQueue::popCurrent()
{
    // possible cases at this point:      [x][y]  [x][_]  [_][_]
    if(m_currentSong == nullptr)
    {
        WR << "player queue is already empty, nothing to pop";
        return;
    }
    // possible cases at this point:      [x][y]  [x][_]  ------
    delete m_currentSong;
    // possible cases at this point:      [?][y]  [?][_]  ------   ? is a trash value
    m_currentSong = m_nextSong;
    // possible cases at this point:      [y][y]  [_][_]  ------
    m_nextSong = nullptr;
    // possible cases at this point:      [y][_]  [_][_]  ------
}

void PlayerQueue::pushNext(QMediaPlayer *song)
{
    // when [x][x]
    if(m_currentSong != nullptr && m_nextSong != nullptr)
        this->pushCaseXX(song);

    // when [x][_]
    else if(m_currentSong != nullptr && m_nextSong == nullptr)
        this->pushCaseXX(song);

    // // case [_][x] - this case will never occur (thanks to the fourth case)
    else if(m_currentSong != nullptr && m_nextSong != nullptr)
        WR << "wtf, that case shoudn't be possible!!!";

    // when [_][_]
    else if(m_currentSong == nullptr && m_nextSong == nullptr)
        this->pushCaseXX(song);

}

void PlayerQueue::pushCaseXX(QMediaPlayer *song){
    // starts with [x][y]
    this->popCurrent();
    // current state [y][_]
    this->pushCaseX_(song);
    // ends with [y][z]
}

void PlayerQueue::pushCaseX_(QMediaPlayer *song){
    // starts with [x][_]
    m_nextSong = song;
    // ends with [x][z]
}

void PlayerQueue::pushCase__(QMediaPlayer *song){
    // starts with [_][_]
    m_currentSong = song;
    // ends with [z][_]
}
