#include "Backend.h"

Backend::Backend(QObject *parent)
    : QObject{parent},
    m_cancelLoadingSong(false)
{
    LFL("Backend instance              %1 created");
    this->initializeParameters();
    this->initializeConnections();
}

Backend::~Backend()
{
    m_personalization->savePersonalizationToJson();
    LFL("Backend instance              %1 destroyed");
}

void Backend::initializeParameters()
{
    m_personalization = new Personalization(this);
    m_personalization->loadPersonalizationFromJson(); /// if an error occur will be handled in initializeBackendWithQML()

    m_playlist = new Playlist(this);
    m_player = new Player(this);
}

void Backend::initializeConnections()
{
    /// old lambda example:
    // QObject::connect(
    //     m_personalization, &Personalization::rootDirectoryChanged, this, [this](){
    //         this->setRootDirectory(this->m_personalization->getRootDirectory());
    //     });

    /// on rootDirectory changed, load songs again
    QObject::connect(m_personalization, &Personalization::rootDirectoryChanged,  this, &Backend::loadSongs);

    /// on songs loaded, pass them to playlist
    QObject::connect(this, &Backend::loadingSongsFinished, m_playlist, &Playlist::loadPlaylistSongs);

    /// after loading new songs in playlist, start player to initialize with default songs
    QObject::connect(m_playlist, &Playlist::songsLoaded, m_player, &Player::initialize);

    /// on ask for Song by player, load song in playlist
    QObject::connect(m_player, &Player::askForSongByPosition,           m_playlist, &Playlist::loadSongByPosition);
    QObject::connect(m_player, &Player::askForSongByID,                 m_playlist, &Playlist::loadSongByID);

    /// on song loaded by playlist, pass it to player
    QObject::connect(m_playlist, &Playlist::newCurrentSongLoaded,   m_player, &Player::setCurrentSong);

}


void Backend::checkPersonalization()
{
    int errorCode = m_personalization->getErrorCodeIfOccurWhileLoading();
    if(errorCode)
    {
        WR << "Personalization load failed with code:" << errorCode;
        emit this->personalizationLoadError(errorCode);
        return;
    }

    emit this->backendInitialized(); // because it is the last step to check initialization
}

void Backend::reinitializePersonalization()
{
    m_personalization->loadPersonalizationFromJson();

    /* emits */ this->checkPersonalization();
}

void Backend::useDefaultPersonalization()
{
    m_personalization->setDefaultPersonalizationData();

    /* emits */ this->checkPersonalization();
}

Personalization *Backend::getPersonalization() const
{
    return m_personalization;
}

Playlist *Backend::getPlaylist() const
{
    return m_playlist;
}

Player *Backend::getPlayer() const
{
    return m_player;
}

void Backend::loadSongs()
{
    emit this->loadingSongsStarted();
    QCoreApplication::processEvents();

    // Qt not provides recursive directory iterator, so i will use std::filesystem
    QUrl rootDirectory = m_personalization->getRootDirectory();
    std::filesystem::path rootPath(rootDirectory.toLocalFile().toStdString());
    if(!std::filesystem::exists(rootPath))
    {
        DB << "rootDirectory not exist! cannot load songs!";
        emit this->loadingSongsError("rootDirectory not exist! cannot load songs!");
        return;
    }

    QString stringExtensions = m_personalization->getSongExtensions();
    QStringList extensions = stringExtensions.split(";", Qt::SplitBehavior::enum_type::SkipEmptyParts);





    // if(lambda_get_value_by_id(2 /*Title field id*/).toString() == ""){
    //     if(song_title == ""){
    //         WR << "song file doesn't contains Title metadata, setting file name as the Title";
    //         song_title = QFileInfo(song_path).baseName();
    //     }
    //     DB << "setting own title to" << song_title;
    //     int index = lambda_get_map_index_by_id(2/*Title field id*/);
    //     if(index == -1) // Title not exist
    //     {
    //         WR << "error, Title field not found!";
    //         emit this->signalAddSongError("error, Title field not found!");
    //         return;
    //     }
    //     else // Title field found
    //     {
    //         auto map = new_song_data[index].toMap();
    //         map["value"] = song_title;
    //         // this way because new_song_data[0].toMap() returns constant map
    //         new_song_data[index] = map;
    //     }
    // }
    // auto duration = QString::number(mp.duration());



    int songsLoaded = 0;
    int filesLoaded = 0;
    int filesTotal = 0;
    int filesLimit = m_personalization->getLoadProtector();
    // fast count songs to tell user how many songs are about to load
    try{ // try is because if directory iterator falls on directory that cannot be oppened \
        then will throw an exception
        for(const auto &i : std::filesystem::recursive_directory_iterator(rootPath))
        {
            ++filesTotal;
            if(filesTotal >= filesLimit)
                break;
            if(m_cancelLoadingSong)
            {
                DB << "loading songs count canceled";
                // do not set m_cancelLoadingSong to false to stop also \
                    loading songs loop
                break;
            }
        }
    }
    catch(std::exception e)
    {
        WR << "exception occur while loading count of files:" << e.what();
        emit this->loadingSongsError(
            QString("exception occur while loading count of files: %1").arg(e.what()));
        return;
    }
    double refreshProgressPercentage = 0.05; // 5%
    int filesLoadedToRefresh = 0;


    SongList songs;
    try{
        for(const auto &i : std::filesystem::recursive_directory_iterator(rootPath))
        {
            QFileInfo file(std::filesystem::path(i).string().c_str());

            // check if match one of the extensions
            for(const auto &ext : extensions)
            {
                if(file.isFile() && file.suffix() == ext){
                    // DB << "song: " << file.absoluteFilePath();

                    Song *song = new Song(this);

                    // // load song media
                    // QMediaPlayer mp;
                    // QEventLoop loop;
                    // connect(&mp, &QMediaPlayer::mediaStatusChanged, &loop, &QEventLoop::quit);
                    // mp.setSource(file.absoluteFilePath());
                    // loop.exec();

                    // // check if loaded correctly (if not use filename as title)
                    // if(mp.mediaStatus() != QMediaPlayer::LoadedMedia){
                    //     WR << "error while loading song: " << mp.errorString() << "setting file name as the Title";
                    //     song->setTitle(file.fileName());
                    // }

                    // // take title from the metadata
                    // QString songTitle(mp.metaData().value(QMediaMetaData::Title).toString());

                    // // check if any title was taken from the metadata (if not use filename as title)
                    // if(songTitle.isEmpty())
                    // {
                    //     // title in metadata was not found
                    //     WR << "song file doesn't contains Title metadata, setting file name as the Title";
                    //     song->setTitle(file.fileName());
                    // }
                    // else
                    //     song->setTitle(songTitle);

                    song->setID(songsLoaded++);
                    song->setTitle(file.fileName());
                    songs.append(song);
                    emit this->loadingSongsInProgress(songsLoaded, filesLoaded, filesTotal);
                    if(filesLoaded >= filesLoadedToRefresh)
                    {
                        filesLoadedToRefresh += filesTotal * refreshProgressPercentage;
                        QCoreApplication::processEvents();
                    }

                    break;
                }
            }

            if(m_cancelLoadingSong)
            {
                DB << "loading songs canceled";
                m_cancelLoadingSong = false; // to not cancel next loading
                break;
            }

            // check for load protector
            ++filesLoaded;
            if(filesLoaded >= filesLimit)
            {
                DB << "loading songs protected";
                emit this->loadingSongsProtected(filesLoaded);
                break;
            }
        }
    }
    catch(std::exception e)
    {
        WR << "exception occur while loading files:" << e.what();
        emit this->loadingSongsError(
            QString("exception occur while loading files: %1").arg(e.what()));
        return;
    }

    DB << "loading songs finished";
    emit this->loadingSongsFinished(songs);
}

void Backend::cancelLoadingSongs()
{
    m_cancelLoadingSong = true;
}
