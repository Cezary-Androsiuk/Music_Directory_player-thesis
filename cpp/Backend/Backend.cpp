#include "Backend.h"

Backend::Backend(QObject *parent)
    : QObject{parent},
    m_personalization(nullptr),
    m_rootDirectory(QUrl())
{
    // create value // to have default data for initialisation
    m_personalization = new Personalization(this);

    // manually assign the initialised values (if personalization's default value is equal
    //     to that which was readed from personalization.json then value won't be formally
    //     changed and signal won't be emitted)
    this->setRootDirectory(m_personalization->getRootDirectory());
    this->setSongExtensions(m_personalization->getSongExtensions());

    // on personalization's rootDirectory changed, change Backend's rootDirectory
    QObject::connect(
        m_personalization, &Personalization::rootDirectoryChanged, this, [this](){
            this->setRootDirectory(this->m_personalization->getRootDirectory());
        });

    // on personalization's songExtensions changed, change Backend's songExtensions
    QObject::connect(
        m_personalization, &Personalization::songExtensionsChanged, this, [this](){
            this->setSongExtensions(m_personalization->getSongExtensions());
        });

    // on rootDirectory changed, change directoryStructure
    QObject::connect(
        this, &Backend::rootDirectoryChanged,
        this, &Backend::loadSongs
        );

    m_playlist = new Playlist(this);

    QObject::connect(
        this, &Backend::songsChanged,
        m_playlist, &Playlist::loadPlaylistSongs
        );

}

Backend::~Backend()
{
    // save personalizations to json
    this->savePersonalization();
}

void Backend::initializeBackend()
{
    // load data from json
    if(m_personalization->loadPersonalizationFromJson())
    {
        WR << "Personalization load failed";
        emit this->personalizationLoadError();
        return;
    }

    emit this->backendInitialized();
}

void Backend::savePersonalization()
{
    // update personalizations variable
    m_personalization->setRootDirectory(m_rootDirectory);

    // save personalizations to json
    if(m_personalization->savePersonalizationToJson())
    {
        WR << "Personalization save failed";
    }
}

void Backend::reinitializePersonalization()
{
    // load data from json
    if(m_personalization->loadPersonalizationFromJson())
    {
        WR << "Personalization load failed";
        emit this->personalizationLoadError();
        return;
    }

    // at this point any error that can occur is personalization error (ex: file not found)
    // if this error was solved then just emit backendInitialized, but otherwise solution might be
    //     much more complex
    emit this->backendInitialized();
}

void Backend::useDefaultPersonalization()
{
    // use default data for personalizations
    m_personalization->setDefaultPersonalizationData();

    // at this point any error that can occur is personalization error (ex: file not found)
    // if this error was solved then just emit backendInitialized, but otherwise solution might be
    //     much more complex
    emit this->backendInitialized();
}

void Backend::loadSongs()
{
    // Qt not provides recursive directory iterator, so i will use std::filesystem
    // QDir rootDirectory(m_rootDirectory.toLocalFile());
    std::filesystem::path rootDirectory(m_rootDirectory.toLocalFile().toStdString());
    if(!std::filesystem::exists(rootDirectory))
    {
        DB << "rootDirectory not exist! cannot load songs!";
        emit this->songLoadError("rootDirectory not exist! cannot load songs!");
        return;
    }

    QStringList extensions = m_songExtensions.split(";", Qt::SplitBehavior::enum_type::SkipEmptyParts);
    // for(auto &i : filters)
    //     i.push_front("*."); // convert list of extensions to filter
    // rootDirectory.setNameFilters(filters);

    SongList songs;

    int files = 0;
    int filesMax = m_personalization->getLoadProtector();







    // get song duration
    QMediaPlayer mp;

    /// wait for the event (load media)
    QEventLoop loop;
    connect(&mp, &QMediaPlayer::mediaStatusChanged, &loop, &QEventLoop::quit);
    mp.setSource(song_path);
    loop.exec();
    if(mp.mediaStatus() != QMediaPlayer::LoadedMedia){
        WR << "error while loading song: " << mp.errorString();
        return;
    }

    QString songTitle(mp.metaData().value(QMediaMetaData::Title).toString());
    if(songTitle.isEmpty()) // title in metadata was not found
    {
        WR << "song file doesn't contains Title metadata, setting file name as the Title";
    }
    else // title can be taken from metadata
    {

    }



    if(lambda_get_value_by_id(2 /*Title field id*/).toString() == ""){
        if(song_title == ""){
            WR << "song file doesn't contains Title metadata, setting file name as the Title";
            song_title = QFileInfo(song_path).baseName();
        }
        DB << "setting own title to" << song_title;
        int index = lambda_get_map_index_by_id(2/*Title field id*/);
        if(index == -1) // Title not exist
        {
            WR << "error, Title field not found!";
            emit this->signalAddSongError("error, Title field not found!");
            return;
        }
        else // Title field found
        {
            auto map = new_song_data[index].toMap();
            map["value"] = song_title;
            // this way because new_song_data[0].toMap() returns constant map
            new_song_data[index] = map;
        }
    }
    auto duration = QString::number(mp.duration());









    for(const auto &i : std::filesystem::recursive_directory_iterator(rootDirectory))
    {
        QFileInfo file(std::filesystem::path(i).string().c_str());

        // check if match one of the extensions
        for(const auto &ext : extensions)
        {
            if(file.isFile() && file.suffix() == ext){
                DB << "song: " << file.absoluteFilePath();
                Song *song = new Song(this);

                songs.append(createSong());
                break;
            }
        }

        ++files;
        if(files > filesMax)
        {
            emit this->loadProtectorLimited(files);
            break;
        }
    }

    DB << "songs loaded!";
    emit this->songsChanged(songs);
}

Personalization *Backend::getPersonalization() const
{
    return m_personalization;
}

Playlist *Backend::getPlaylist() const
{
    return m_playlist;
}

QUrl Backend::getRootDirectory() const
{
    return m_rootDirectory;
}

void Backend::setRootDirectory(QUrl rootDirectory)
{
    if(m_rootDirectory == rootDirectory) // removes binding loop in qml
        return;

    m_rootDirectory = rootDirectory;
    emit this->rootDirectoryChanged();
}

void Backend::setSongExtensions(QString songExtensions)
{
    if(m_songExtensions == songExtensions) // removes binding loop in qml
        return;

    m_songExtensions = songExtensions;
    emit this->songExtensionsChanged();
}

Song *Backend::createSong(QFileInfo &file, Song *song)
{
    song->setTitle(file.fileName());
    return song;
}
