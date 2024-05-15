#include "Playlist.h"

Playlist::Playlist(QObject *parent)
    : QObject{parent}
{

}

SongList Playlist::getSongs() const
{
    return m_songs;
}

void Playlist::loadPlaylistSongs(SongList songs)
{
    for(auto &song : m_songs)
        delete song;

    m_songs = songs;
    m_songs = this->shuffleList(m_songs);
    emit this->songsChanged();
}

void Playlist::shufflePlaylistSongs()
{
    m_songs = this->shuffleList(m_songs);
    emit this->songsChanged();
}

void Playlist::loadSongByPosition(int position, bool forCurrentSongPurpose)
{
    if(m_songs.empty())
    {
        WR << "songs list is empty!";
        return;
    }

    if(m_songs.size() >= position)
    {
        WR << "position:" << position << "goes out of bounds in songs list size:"<< m_songs.size();
        return;
    }

    Song *nextSong = m_songs[position];

    // both signals are connected to Player's slots
    if(forCurrentSongPurpose)
        emit this->newCurrentSongLoaded(nextSong);
    else
        emit this->newNextSongLoaded(nextSong);
}

void Playlist::loadSongByID(int id, bool forCurrentSongPurpose)
{
    if(m_songs.empty())
    {
        WR << "songs list is empty!";
        return;
    }

    int i=0;
    bool foundCurrentSong = false;
    for(const auto &song : m_songs)
    {
        if(song->getID() == id)
        {
            foundCurrentSong = true;
            break;
        }
        ++i;
    }

    // for secure
    if(!foundCurrentSong)
    {
        // current song was not found!
        // in that case just use first song and go cry :<
        WR << "song with id =" << id << "was not found!";
        return;
    }

    Song *nextSong = m_songs[i];

    // both signals are connected to Player's slots
    if(forCurrentSongPurpose)
        emit this->newCurrentSongLoaded(nextSong);
    else
        emit this->newNextSongLoaded(nextSong);
}

void Playlist::loadNextSongByCurrentSongID(int currentSongID)
{
    if(m_songs.empty())
    {
        WR << "songs list is empty!";
        return;
    }
    int i=0;
    bool foundCurrentSong = false;
    for(const auto &song : m_songs)
    {
        ++i;
        if(song->getID() == currentSongID)
        {
            foundCurrentSong = true;
            break;
        }
    }


    // for secure
    if(!foundCurrentSong)
    {
        // current song was not found!
        // in that case just use first song and go cry :<
        WR << "current song with id =" << currentSongID << "was not found!";
        return;
    }

    // now 'i' contains index of the next song
    // if current song is the last one then just loop playlist
    if(m_songs.size() == i) i=0;

    Song *nextSong = m_songs[i];

    emit this->newNextSongLoaded(nextSong);
}

std::vector<int> Playlist::getUniqueRandomNumbers(int count)
{
    // create variables
    std::vector<int> source;
    source.reserve(count);

    std::vector<int> result;
    result.reserve(count);

    // prepare ordered source list [0, count)
    for(int i=0; i<count; ++i)
        source.push_back(i);

    // take random number from source list and add to result (removing number from set)
    for(int i=count; i>0; --i) // from count to 1 (inclusive)
    {
        int random = QRandomGenerator::global()->bounded(i); // range: [0, i)
        result.push_back(source[random]);
        source.erase(source.begin() + random); // remove taken item, range (i variable) will be decremented also
    }

    return result;
}

SongList Playlist::shuffleList(const SongList &songs)
{
    int songsCount = songs.size();

    auto shuffleOrderList = Playlist::getUniqueRandomNumbers(songsCount);

    SongList songsNewOrder;

    // read random number from songs and append to the songNewOrder list
    for(const int &i : shuffleOrderList)
        songsNewOrder.append(songs[i]);

    return songsNewOrder;
}
