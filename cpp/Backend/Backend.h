#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include <QList>
#include <QUrl>
#include <QDir>
// #include <QFile>
// #include <Qfile
#include <QEventLoop>
#include <QTimer>
#include <QStandardPaths>
#include <QtMultimedia/QMediaPlayer> // load song data
#include <QtMultimedia/QMediaMetaData> // load song data

#include <cpp/DebugPrint.h>
#include <cpp/Song/Song.h>
#include <cpp/Personalization/Personalization.h>
#include <cpp/Playlist/Playlist.h>


/*
 * exec path:
 * 1 start main.cpp
 * 2 create Backend
 * 3 create QML
 * 4 QML emits signal to Backend "initializeBackend" (with loading page)
 * 5 in slot (in Backend) "initializeBackend" prepare all stuff, like personalizations and directoryStructure
 * 6 Backend will emit error (like personalizationLoadError) if anything failed or emit backendInitialized if all was fine
 * 7
 */

class Backend : public QObject
{
    typedef QList<Song *> SongList;

    Q_OBJECT
    Q_PROPERTY(Personalization *personalization READ getPersonalization NOTIFY personalizationChanged   FINAL)
    Q_PROPERTY(Playlist *playlist               READ getPlaylist        NOTIFY playlistChanged          FINAL)

public:
    explicit Backend(QObject *parent = nullptr);
    ~Backend();

    Q_INVOKABLE void initializeBackend();

    void savePersonalization();

public slots:
    void reinitializePersonalization();
    void useDefaultPersonalization();

    void loadSongs();

public: // getters
    Personalization *getPersonalization() const;
    Playlist *getPlaylist() const;
    QUrl getRootDirectory() const;
    QString getSongExtensions() const;

public: // setters
    void setRootDirectory(QUrl rootDirectory);
    void setSongExtensions(QString songExtensions);

signals: // something happend
    void backendInitialized();
    void personalizationLoadError();
    void songLoadError(QString desc);
    void loadProtectorLimited(int limit);

signals: // some variable changed
    void personalizationChanged(); // never emited
    void playlistChanged(); // never emited

    void rootDirectoryChanged();
    void songsChanged(SongList songs);
    void songExtensionsChanged();

private:
    Song *createSong(QFileInfo &file, Song *song);

private:
    Personalization *m_personalization;
    Playlist *m_playlist;

    QUrl m_rootDirectory; // stores root path where all songs are located (also in subfolders)
    QString m_songExtensions;
};

#endif // BACKEND_H
