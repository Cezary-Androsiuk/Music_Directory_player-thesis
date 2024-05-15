#include "Player.h"

Player::Player(QObject *parent)
    : QObject{parent}
{}

void Player::updateCurrentSong(Song *song)
{

}

void Player::updateNextSong(Song *song)
{

}

void Player::playSongByID(int songID)
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

void Player::setCurrentSong(Song *song)
{
    m_currentSong = song;
    emit this->currentSongChanged();
}

void Player::setNextSong(Song *song)
{
    m_nextSong = song;
    emit this->nextSongChanged();
}
