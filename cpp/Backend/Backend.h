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
#include <cpp/Directory/Directory.h>
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
    typedef QList<Directory *> QDirectoryList;
    typedef QList<Song *> QSongList;

    Q_OBJECT
    Q_PROPERTY(Personalization *personalization     READ getPersonalization                                 NOTIFY personalizationChanged       FINAL)
    Q_PROPERTY(QString rootDirectory                READ getRootDirectory           WRITE setRootDirectory  NOTIFY rootDirectoryChanged         FINAL)
    Q_PROPERTY(QDirectoryList directoryStructure    READ getDirectoryStructure                              NOTIFY directoryStructureChanged    FINAL)
    Q_PROPERTY(QSongList songs                      READ getSongs                                           NOTIFY songsChanged                 FINAL)

public:
    explicit Backend(QObject *parent = nullptr);
    ~Backend();

    Q_INVOKABLE void initializeBackend();

    void savePersonalization();

public slots:
    void reinitializePersonalization();
    void useDefaultPersonalization();
    void loadDirectoryStructure();
    // void refreshStructure();
    void loadSongs(); // started in qmlInitialized and emits songs changed

    void setRootDirectory(QString rootDirectory);

public:
    Personalization *getPersonalization() const;

    QString getRootDirectory() const;
    QDirectoryList getDirectoryStructure() const;
    QSongList getSongs() const;

private:
    QString getValidRootDirectory() const;
    void createStructureDirectory(QString path, int depth);

signals:
    void backendInitialized();
    void personalizationLoadError();


    void personalizationChanged();
    void songsChanged();
    void rootDirectoryChanged();
    void directoryStructureChanged();

private:

    Personalization *m_personalization;
    QString m_rootDirectory; // stores root path where all songs are located (also in subfolders)
    QDirectoryList m_directoryStructure; // stores what subfolders are in rootDirectory // rename to directory (or something) and will contains also bool m_ischoosed
    QSongList m_songs; // stores what songs are in playlist // QStringList m_directoryStructureSongs; // maybe better name
};

#endif // BACKEND_H
