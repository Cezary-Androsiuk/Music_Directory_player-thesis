#include "Backend.h"

Backend::Backend(QObject *parent)
    : QObject{parent},
    m_personalization(nullptr),
    m_rootDirectory(QUrl())
{
    // create value // to have default data for initialisation
    m_personalization = new Personalization(this);

    // on personalization's rootDirectory changed, change Backend's rootDirectory
    QObject::connect(
        m_personalization, Personalization::rootDirectoryChanged,
        this, [this](){
            this->setRootDirectory(this->m_personalization->getRootDirectory());
        });

    // on personalization's songExtensions changed, change Backend's songExtensions
    QObject::connect(
        m_personalization, Personalization::songExtensionsChanged,
        this, [this](){
            this->setSongExtensions(m_personalization->getSongExtensions());
        });

    // on rootDirectory changed, change directoryStructure
    QObject::connect(
        this, Backend::rootDirectoryChanged,
        this, Backend::loadDirectoryStructure);

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
    emit this->personalizationChanged();


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
    emit this->personalizationChanged();

    // at this point any error that can occur is personalization error (ex: file not found)
    // if this error was solved then just emit backendInitialized, but otherwise solution might be
    //     much more complex
    emit this->backendInitialized();
}

void Backend::useDefaultPersonalization()
{
    // use default data for personalizations
    m_personalization->setDefaultPersonalizationData();
    emit this->personalizationChanged();

    // at this point any error that can occur is personalization error (ex: file not found)
    // if this error was solved then just emit backendInitialized, but otherwise solution might be
    //     much more complex
    emit this->backendInitialized();
}

void Backend::initializeDirectoryStructure()
{
    if(m_rootDirectory.isValid())
    {
        if(QDir(m_rootDirectory.toString()).exists())
        {

        }
        else
            DB << "directory"<< m_rootDirectory <<"not exist";
    }
    else
        DB << "directory"<< m_rootDirectory <<"is not valid!";
}


void Backend::loadDirectoryStructure()
{
    DB << "loading new structure";

    // delete old structure
    for(const auto &i: m_directoryStructure)
        delete i;
    m_directoryStructure.clear();


    // root directory is path or an empty "" string
    if(m_rootDirectory.isValid())
        createStructureDirectoryRecursive(m_rootDirectory.toLocalFile(), -1);

    // empty string will be represented in qml for example as '-'

    emit this->directoryStructureChanged();
}

void Backend::loadSongs()
{
    if(!m_songs.empty())
    {
        for(const auto &song : m_songs)
            delete song;
        m_songs.clear();
    }

    for(int i=0; i<32; i++)
    {
        Song *song = new Song(this);

        song->setTitle(QString("SongSongSongSongSongSongSongSongSongSongSongSongSongSong %1").arg(i));

        m_songs.append(song);
    }

    DB << "songs loaded!";
    emit this->songsChanged();
}

Personalization *Backend::getPersonalization() const
{
    return m_personalization;
}

QUrl Backend::getRootDirectory() const
{
    return m_rootDirectory;
}

Backend::QDirectoryList Backend::getDirectoryStructure() const
{
    return m_directoryStructure;
}

Backend::QSongList Backend::getSongs() const
{
    return m_songs;
}

void Backend::setRootDirectory(QUrl rootDirectory)
{
    DB << "new root directory in Backend:" << rootDirectory;
    m_rootDirectory = rootDirectory;
    emit this->rootDirectoryChanged();
}

void Backend::setSongExtensions(QString songExtensions)
{
    DB << "new song extensions in Backend:" << songExtensions;
    m_songExtensions = songExtensions;
    emit this->songExtensionsChanged();
}

void Backend::createStructureDirectoryRecursive(QString path, int depth)
{
    // remove / if is in the end (but while displayling better be there)
    if (!path.isEmpty() && path.endsWith("/"))
        path.chop(1);

    QDir directory(path);

    if(depth >= 0)
    {
        Directory *dir = new Directory(this);
        dir->setPath(directory.dirName() + "/");
        dir->setDepth(depth);
        m_directoryStructure << dir;
    }

    QStringList dirList = directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(const auto &dir : dirList)
        createStructureDirectoryRecursive(path + '/' + dir, depth+1);

}

