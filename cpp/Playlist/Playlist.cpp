#include "Playlist.h"

Playlist::Playlist(QObject *parent)
    : QObject{parent},
    m_playlistInitialized(false)
{
    /// connect sub signals to signal
    QObject::connect(this, &Playlist::songsLoaded, this, &Playlist::songsChanged);
    QObject::connect(this, &Playlist::songsShuffled, this, &Playlist::songsChanged);
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

    // DB << "list of songs:";
    // DB << m_songs;
    // for(const auto &s : m_songs)
    //     DB << s->getID() << " " << s->getTitle();

    if(!m_playlistInitialized)
    {
        m_playlistInitialized = true;
        emit this->playlistInitialized();
    }

    emit this->songsLoaded();
}

void Playlist::shufflePlaylistSongs()
{
    m_songs = this->shuffleList(m_songs);
    emit this->songsShuffled();
}

void Playlist::loadSongByPosition(int position, bool forCurrentSongPurpose)
{
    DB << "start to loading song by position:" << position << "for"
       << (forCurrentSongPurpose ? "current song" : "next song");

    if(m_songs.empty())
    {
        WR << "songs list is empty!";
        return;
    }

    if(position >= m_songs.size())
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
    DB << "start to loading song by id:" << id << "for"
       << (forCurrentSongPurpose ? "current song" : "next song");

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
    DB << "start to loading next song current song id:" << currentSongID;

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
    int songListIndex = 0;

    // read random number from songs and append to the songNewOrder list
    for(const int &randomNumber : shuffleOrderList)
    {
        Song *song = songs[randomNumber];
        song->setListIndex(songListIndex++);
        songsNewOrder.append(song);
    }

    return songsNewOrder;
}
