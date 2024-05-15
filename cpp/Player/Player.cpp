#include "Player.h"

Player::Player(QObject *parent)
    : QObject{parent},
    m_currentSong(nullptr),
    m_nextSong(nullptr),
    m_playerQueue(new PlayerQueue(this))
{

}

void Player::initialize()
{
    DB << "player start initialize";
    // current song should be nullptr
    emit this->askForSongByPosition(0, false);
}

void Player::clearPlayer()
{
    m_playerQueue->clearQueue();

    m_currentSong = nullptr; // element was borrowed not created
    m_nextSong = nullptr; // element was borrowed not created
}

void Player::playOtherNextSongByID(int songID)
{
    emit this->askForSongByID(songID, false);
}

void Player::play()
{
    if(m_currentSong == nullptr)
    {

    }
}

void Player::pause()
{

}

Song *Player::getCurrentSong() const
{
    return m_currentSong;
}

Song *Player::getNextSong() const
{
    return m_nextSong;
}

// const PlayerQueue &Player::getPlayerQueue() const
// {
//     return m_playerQueue;
// }

void Player::setCurrentSong(Song *song)
{

    DB << "changing current song from" << m_currentSong << "to" << song;
    DB << song->getID() << " " << song->getListIndex() << " " << song->getTitle();
    // song is just a borrowed from Playlist (and will be deleted there)
    m_currentSong = song;
    emit this->currentSongChanged();
}

void Player::setNextSong(Song *song)
{
    DB << "changing next song from" << m_nextSong << "to" << song;
    DB << song->getID() << " " << song->getListIndex() << " " << song->getTitle();
    // song is just a borrowed from Playlist (and will be deleted there)
    m_nextSong = song;
    emit this->nextSongChanged();
}

void Player::switchSongs()
{
    m_currentSong = m_nextSong;

}
