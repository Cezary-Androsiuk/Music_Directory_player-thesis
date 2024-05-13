#include "Playlist.h"

Playlist::Playlist(QObject *parent)
    : QObject{parent}
{

}

Playlist::SongList Playlist::getSongs() const
{
    return m_songs;
}

void Playlist::loadPlaylistSongs(SongList songs)
{
    m_songs = songs;
    m_songs = this->shuffleList(m_songs);
    emit this->songsChanged();
}

void Playlist::shufflePlaylistSongs()
{
    m_songs = this->shuffleList(m_songs);
    emit this->songsChanged();
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

Playlist::SongList Playlist::shuffleList(const SongList &songs)
{
    int songsSize = songs.size();

    auto shuffleOrderList = Playlist::getUniqueRandomNumbers(songsSize);

    SongList songsNewOrder;

    // read random number from songs and append to the songNewOrder list
    for(const int &i : shuffleOrderList)
        songsNewOrder.append(songs[i]);

    return songsNewOrder;
}
