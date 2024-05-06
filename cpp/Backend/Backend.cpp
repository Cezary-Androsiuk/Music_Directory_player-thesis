#include "Backend.h"

Backend::Backend(QObject *parent)
    : QObject{parent},
    m_personalization(nullptr),
    m_rootDirectory("")
{
    // create value // to have default data for initialisation
    m_personalization = new Personalization(this);
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

    // now, having personalizations variable initialize directory structure
    this->initializeDirectoryStructure();


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
    // delete value if exist
    if(m_personalization != nullptr)
        delete m_personalization;

    // create value
    m_personalization = new Personalization(this);

    // load data from json
    if(m_personalization->loadPersonalizationFromJson())
    {
        WR << "Personalization load failed";
        emit this->personalizationLoadError();
        return;
    }
    emit this->personalizationChanged();

    // now, having personalizations variable initialize directory structure
    this->initializeDirectoryStructure();

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

    // now, having personalizations variable initialize directory structure
    this->initializeDirectoryStructure();

    // at this point any error that can occur is personalization error (ex: file not found)
    // if this error was solved then just emit backendInitialized, but otherwise solution might be
    //     much more complex
    emit this->backendInitialized();
}

void Backend::initializeDirectoryStructure()
{
    // update local root directory variable
    this->setValidRootDirectory(m_personalization->getRootDirectory());
}


void Backend::loadDirectoryStructure()
{
    // // can convert local file to url and url to local file
    // QUrl url = QUrl::fromLocalFile(m_rootDirectory);
    // DB << url.toLocalFile();
    // DB << url.toString();

    // delete old structure
    for(const auto &i: m_directoryStructure)
        delete i;
    m_directoryStructure.clear();

    // root directory is path or an empty "" string
    if(m_rootDirectory != "")
        createStructureDirectoryRecursive(m_rootDirectory, -1);

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

void Backend::setRootDirectory(QString rootDirectory)
{
    m_rootDirectory = rootDirectory;
    emit this->rootDirectoryChanged();
}

Personalization *Backend::getPersonalization() const
{
    return m_personalization;
}

QString Backend::getRootDirectory() const
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

void Backend::setValidRootDirectory(QString rootDirectory)
{
    if (!rootDirectory.isEmpty() && rootDirectory.startsWith("file:///"))
        rootDirectory = rootDirectory.right(8);

    if(!rootDirectory.isEmpty())
    {
        if(QDir(rootDirectory).exists())
            this->setRootDirectory(rootDirectory);
        else{
            DB << "given directory:" << rootDirectory << "not exist!";
            this->setRootDirectory(DEFAULT_ROOT_DIRECTORY);
        }
    }
    else
    {
        WR << "given directory:" << rootDirectory << "is empty!";
        this->setRootDirectory(DEFAULT_ROOT_DIRECTORY);
    }
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

