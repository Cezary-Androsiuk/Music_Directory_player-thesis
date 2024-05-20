#include "Player.h"

Player::Player(QObject *parent)
    : QObject{parent},
    m_currentSong(nullptr)
{
    LFL("Player instance               %1 created");
}

Player::~Player()
{
    LFL("Player instance               %1 destroyed");
}

void Player::initialize()
{
    DB << "player start initialize";
    // current song should be nullptr
    emit this->askForSongByPosition(0);
}

void Player::playOtherNextSongByID(int songID)
{
    DB << "choosed song" << songID;
    emit this->askForSongByID(songID);
}

void Player::playNextSong()
{
    if(m_nextSongIsFist)
    {
        emit this->askForSongByPosition(0);
        m_nextSongIsFist = false;
    }
    else
        emit this->askForSongByPosition(m_currentSong->getListIndex()+1);
}

void Player::setNextSongAs0()
{
    m_nextSongIsFist = true;
}

void Player::play()
{

}

void Player::pause()
{

}

Song *Player::getCurrentSong() const
{
    return m_currentSong;
}

void Player::setCurrentSong(Song *song)
{

    DB << "changing current song from" << m_currentSong << "to" << song;
    DB << song->getID() << " " << song->getListIndex() << " " << song->getTitle();
    // song is just a borrowed from Playlist (and will be deleted there)
    m_currentSong = song;
    emit this->currentSongChanged();
}
