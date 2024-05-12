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

#include <cpp/DebugPrint.h>
#include <cpp/Song/Song.h>
#include <cpp/Personalization/Personalization.h>


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
    typedef QList<Song *> QSongList;

    Q_OBJECT
    Q_PROPERTY(Personalization *personalization     READ getPersonalization                                 NOTIFY personalizationChanged       FINAL)
    Q_PROPERTY(QSongList songs                      READ getSongs                                           NOTIFY songsChanged                 FINAL)

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
    QUrl getRootDirectory() const;
    QSongList getSongs() const;
    QString getSongExtensions() const;

public: // setters
    void setRootDirectory(QUrl rootDirectory);
    void setSongExtensions(QString songExtensions);

signals: // something happend
    void backendInitialized();
    void personalizationLoadError();
    void songLoadError(QString desc);

signals: // some variable changed
    void personalizationChanged();
    void rootDirectoryChanged();
    void songsChanged();
    void songExtensionsChanged();

private:
    Personalization *m_personalization;
    QUrl m_rootDirectory; // stores root path where all songs are located (also in subfolders)
    QSongList m_songs; // stores what songs are in playlist // QStringList m_directoryStructureSongs; // maybe better name
    QString m_songExtensions;
};

#endif // BACKEND_H
