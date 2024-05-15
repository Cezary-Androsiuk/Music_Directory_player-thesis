#include "Backend.h"

Backend::Backend(QObject *parent)
    : QObject{parent},
    m_cancelLoadingSong(false)
{
    this->initializeParameters();
    this->initializeConnections();
}

Backend::~Backend()
{
    m_personalization->savePersonalizationToJson();
}

void Backend::initializeParameters()
{
    m_personalization = new Personalization(this);
    m_personalization->loadPersonalizationFromJson(); /// if an error occur will be handled in initializeBackendWithQML()

    /// manually assign the initialised values (connection was not created yet)
    this->setRootDirectory(m_personalization->getRootDirectory());
    /// no connection has been set, then loadSongs need to be started manually also
    this->setSongExtensions(m_personalization->getSongExtensions());

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

    /// on personalization's rootDirectory changed, change Backend's rootDirectory
    QObject::connect(m_personalization, &Personalization::rootDirectoryChanged,     this, &Backend::setRootDirectory);

    /// on personalization's songExtensions changed, change Backend's songExtensions
    QObject::connect(m_personalization, &Personalization::songExtensionsChanged,    this, &Backend::setSongExtensions);

    /// on rootDirectory changed, load songs again
    QObject::connect(this, &Backend::rootDirectoryChanged,  this, &Backend::loadSongs);

    /// on songs loaded, pass them to playlist
    QObject::connect(this, &Backend::songsChanged, m_playlist, &Playlist::loadPlaylistSongs);

    /// after the first loading of songs in playlist, start player to initialize with default songs (0 and 1)
    QObject::connect(m_playlist, &Playlist::playlistInitialized, m_player, &Player::initialize);

    /// on ask for Song by player, load song in playlist
    QObject::connect(m_player, &Player::askForSongByPosition,           m_playlist, &Playlist::loadSongByPosition);
    QObject::connect(m_player, &Player::askForSongByID,                 m_playlist, &Playlist::loadSongByID);
    QObject::connect(m_player, &Player::askForNextSongByCurrentSongID,  m_playlist, &Playlist::loadNextSongByCurrentSongID);

    /// on song loaded by playlist, pass it to player
    QObject::connect(m_playlist, &Playlist::newCurrentSongLoaded,   m_player, &Player::setCurrentSong);
    QObject::connect(m_playlist, &Playlist::newNextSongLoaded,      m_player, &Player::setNextSong);

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

void Backend::setRootDirectory(QUrl rootDirectory)
{
    if(m_rootDirectory == rootDirectory) // removes binding loop in qml
        return;

    m_rootDirectory = rootDirectory;
    emit this->rootDirectoryChanged(); /// used to trigger loadSongs
}

void Backend::setSongExtensions(QString songExtensions)
{
    if(m_songExtensions == songExtensions) // removes binding loop in qml
        return;

    m_songExtensions = songExtensions;
}

void Backend::loadSongs()
{
    // Qt not provides recursive directory iterator, so i will use std::filesystem
    // QDir rootDirectory(m_rootDirectory.toLocalFile());
    std::filesystem::path rootDirectory(m_rootDirectory.toLocalFile().toStdString());
    if(!std::filesystem::exists(rootDirectory))
    {
        DB << "rootDirectory not exist! cannot load songs!";
        emit this->songsLoadError("rootDirectory not exist! cannot load songs!");
        return;
    }

    QStringList extensions = m_songExtensions.split(";", Qt::SplitBehavior::enum_type::SkipEmptyParts);
    // for(auto &i : filters)
    //     i.push_front("*."); // convert list of extensions to filter
    // rootDirectory.setNameFilters(filters);













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

    emit this->loadingSongsInProgress(0,0,0);
    QCoreApplication::processEvents();

    try{
        for(const auto &i : std::filesystem::recursive_directory_iterator(rootDirectory))
        {
            ++filesTotal;
            if(filesTotal >= filesLimit)
                break;
        }
    }
    catch(std::exception e)
    {
        WR << "exception occur while loading count of files:" << e.what();
    }

    if(filesTotal > filesLimit)
        filesTotal = filesLimit;

    double refreshProgressPercentage = 0.05; // 5%
    int filesLoadedToRefresh = 0;


    SongList songs;
    try{
        for(const auto &i : std::filesystem::recursive_directory_iterator(rootDirectory))
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
                m_cancelLoadingSong = false; // to not cancel next one
                break;
            }

            // check for load protector
            ++filesLoaded;
            if(filesLoaded >= filesLimit)
            {
                emit this->loadProtectorLimited(filesLoaded);
                break;
            }
        }
    }
    catch(std::exception e)
    {
        WR << "exception occur while loading files:" << e.what();
    }


    DB << "songs loaded!";
    emit this->loadingSongsFinished(); // for qml
    emit this->songsChanged(songs); // for playlist
}

void Backend::cancelLoadingSongs()
{
    m_cancelLoadingSong = true;
}
