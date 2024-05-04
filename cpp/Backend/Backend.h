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

#include <cpp/DebugPrint.h>
#include <cpp/Song/Song.h>
#include <cpp/Directory/Directory.h>
#include <cpp/Personalization/Personalization.h>


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

    void loadPersonalization(); // started in constructor
    void savePersonalization();

public slots:
    void personalizationDataChanged();
    void qmlInitialized(); // emited in Component.onConpleated

    void loadSongsStructure();
    // void refreshStructure();
    void loadSongs(); // started in qmlInitialized and emits songs changed
    void tmp();

    void setRootDirectory(QString rootDirectory);

public:
    Personalization *getPersonalization() const;

    QString getRootDirectory() const;
    QDirectoryList getDirectoryStructure() const;
    QSongList getSongs() const;

private:
    void createStructureDirectory(QString path, int depth);

signals:
    void backendLoaded();
    void personalizationLoadError(QString desc);
    void backendLoadError(QString desc);

    void personalizationChanged();
    void songsChanged();
    void rootDirectoryChanged();
    void directoryStructureChanged();

private:
    bool m_personalizationLoaded;
    bool m_qmlInitialized;

    Personalization *m_personalization;
    QString m_rootDirectory; // stores root path where all songs are located (also in subfolders)
    QDirectoryList m_directoryStructure; // stores what subfolders are in rootDirectory // rename to directory (or something) and will contains also bool m_ischoosed
    QSongList m_songs; // stores what songs are in playlist // QStringList m_directoryStructureSongs; // maybe better name
};

#endif // BACKEND_H
