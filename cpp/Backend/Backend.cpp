#include "Backend.h"

Backend::Backend(QObject *parent)
    : QObject{parent},
    m_personalization(nullptr),
    m_rootDirectory("")
{
    m_personalization = new Personalization(this);
}

Backend::~Backend()
{
    this->savePersonalization();
}

void Backend::initializeBackend()
{
    if(m_personalization->loadPersonalizationFromJson())
    {
        WR << "Personalization load failed";
        emit this->personalizationLoadError();
        return;
    }

    emit this->personalizationChanged();
    this->setRootDirectory(m_personalization->getRootDirectory());


    emit this->backendInitialized();
}

void Backend::savePersonalization()
{
    m_personalization->setRootDirectory(m_rootDirectory);
    if(m_personalization->savePersonalizationToJson())
    {
        WR << "Personalization save failed";
    }
}

void Backend::reinitializePersonalization()
{
    if(m_personalization != nullptr)
        delete m_personalization;

    m_personalization = new Personalization(this);

    if(m_personalization->loadPersonalizationFromJson())
    {
        WR << "Personalization load failed";
        emit this->personalizationLoadError();
        return;
    }

    emit this->personalizationChanged();
    this->setRootDirectory(m_personalization->getRootDirectory());
}

void Backend::useDefaultPersonalization()
{
    this->m_personalization->setDefaultPersonalizationData();
    emit this->personalizationChanged();
    this->setRootDirectory(m_personalization->getRootDirectory());
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

    // get a valid root directory or an empty "" string
    QString validRootDirectory = this->getValidRootDirectory();

    if(validRootDirectory != "")
        createStructureDirectory(m_rootDirectory, -1);

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
    if (!rootDirectory.isEmpty() && rootDirectory.startsWith("file:///"))
        rootDirectory = rootDirectory.right(8);

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

QString Backend::getValidRootDirectory() const
{
    // at this point root directory won't contains "file:///" prefix
    // test if given from json or from select root path (GUI) is valid
    // if not return "" // or default music location

    if(!m_rootDirectory.isEmpty())
    {
        if(QDir(m_rootDirectory).exists())
            return m_rootDirectory;
        else
            DB << "dir" << m_rootDirectory << "not exist!";
    }
    else
        WR << "dir" << m_rootDirectory << "is empty!";

    // QString newPath = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    return "";
}

void Backend::createStructureDirectory(QString path, int depth)
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
        createStructureDirectory(path + '/' + dir, depth+1);

}

