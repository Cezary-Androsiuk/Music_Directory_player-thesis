#include "Backend.h"

Backend::Backend(QObject *parent)
    : QObject{parent},
    m_personalization(nullptr),
    m_personalizationLoaded(false),
    m_qmlInitialized(false),
    m_rootDirectory("")
{
    this->loadPersonalization();
}

Backend::~Backend()
{
    this->savePersonalization();
}

void Backend::loadPersonalization()
{
    m_personalization = new Personalization(this);
    if(m_personalization->loadPersonalizationFromJson())
    {
        WR << "Personalization load failed";
    }
    else
        m_personalizationLoaded = true;

    QObject::connect(
        m_personalization, &Personalization::backendPersonalizationDataChanged,
        this, &Backend::personalizationDataChanged);
}

void Backend::savePersonalization()
{
    if(m_personalization->savePersonalizationToJson())
    {
        WR << "Personalization save failed";
    }
}

void Backend::personalizationDataChanged()
{
    m_rootDirectory = m_personalization->getRootDirectory();
}

void Backend::qmlInitialized()
{
    DB << "qml initialized";

    if(!m_personalizationLoaded)
    {
        WR << "personalization load error";
        emit this->personalizationLoadError("");
    }

    // wait 2s

    // QTimer timer;
    // QEventLoop loop;
    // QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);

    // DB << "start pause";
    // timer.start(2000);
    // loop.exec();
    // WR << "end pause";

    this->loadSongsStructure();

    this->loadSongs();

    DB << "backend loaded!";
    emit this->backendLoaded();
}

void Backend::loadSongsStructure()
{
    // // can convert local file to url and url to local file
    // QUrl url = QUrl::fromLocalFile(m_rootDirectory);
    // DB << url.toLocalFile();
    // DB << url.toString();

    if(!m_rootDirectory.isEmpty())
    {
        QDir dir(m_rootDirectory);
        if(dir.exists())
        {
            DB << "dir" << dir.path() << "exist";
            createStructureDirectory(m_rootDirectory, -1);
        }
        else
        {
            WR << "dir" << dir.path() << " not exist!";
        }
    }
    else
    {
        WR << "dir" << m_rootDirectory << "is empty!";
    }

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

void Backend::tmp()
{
    m_rootDirectory = "asdasadasdsaasd";
    emit this->rootDirectoryChanged();
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

